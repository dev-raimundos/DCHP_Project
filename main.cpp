#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "dhcp_offer.h"
#include "config.h"

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    uint8_t buffer[1024];

    // Cria um socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Erro ao criar o socket: " << strerror(errno) << std::endl;
        return -1;
    }

    // Configura o endereço do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(67); // Porta DHCP

    // Associa o socket ao endereço
    if (bind(sockfd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erro ao associar o socket: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Servidor DHCP iniciado..." << std::endl;

    while (true) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (n < 0) {
            std::cerr << "Erro ao ler dados: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << "Recebido pedido DHCP de " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        std::vector<uint8_t> mac(buffer + 28, buffer + 34);
        std::cout << "MAC Address: ";
        for (uint8_t byte : mac) {
            printf("%02x:", byte);
        }
        std::cout << std::endl;

        std::vector<uint8_t> offer = createDHCPOffer(mac);
        if (offer.empty()) {
            std::cerr << "Nenhum IP disponível para alocação" << std::endl;
            continue;
        }

        if (sendto(sockfd, offer.data(), offer.size(), 0, (const struct sockaddr*)&clientAddr, addrLen) < 0) {
            std::cerr << "Erro ao enviar DHCP Offer: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << "DHCP Offer enviado para " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    }

    close(sockfd);
    return 0;
}
