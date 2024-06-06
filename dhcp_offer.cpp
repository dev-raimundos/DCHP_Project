#include "dhcp_offer.h"
#include "config.h"
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <string>

std::vector<uint8_t> createDHCPOffer(const std::vector<uint8_t>& mac) {
    if (ipPool.empty()) {
        return {}; // Pool de IP esgotado
    }

    std::string ip = ipPool.back();
    ipPool.pop_back();

    std::vector<uint8_t> offer(240, 0); // Cabeçalho DHCP básico
    offer[0] = 2; // Mensagem tipo: BOOTREPLY
    std::copy(mac.begin(), mac.end(), offer.begin() + 28); // MAC Address

    // Endereço IP oferecido
    in_addr ipAddr;
    inet_pton(AF_INET, ip.c_str(), &ipAddr);
    std::memcpy(&offer[16], &ipAddr, 4);

    // Endereço IP do servidor
    inet_pton(AF_INET, dhcpServerIP.c_str(), &ipAddr);
    std::memcpy(&offer[20], &ipAddr, 4);

    return offer;
}
