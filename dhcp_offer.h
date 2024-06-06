#ifndef DHCP_OFFER_H
#define DHCP_OFFER_H

#include <vector>
#include <cstdint>

std::vector<uint8_t> createDHCPOffer(const std::vector<uint8_t>& mac);

#endif // DHCP_OFFER_H
