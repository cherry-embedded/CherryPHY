/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "chry_phy.h"

#define LAN8720_PSCSR               31 /* PHY Special Control/Status Register */

#define LAN8720_PSCSR_DUPLEX_STATUS (0x10U)
/*
 * SPEED (RO)
 *
 * HCDSPEED value:
 * 01 = 10BASE-T
 * 10 = 100BASE-TX
 */
#define LAN8720_PSCSR_SPEED_STATUS  (0xCU)

void lan8720_phy_init(uint8_t busid, uint16_t phy_addr, struct chry_phy_config *config)
{
}

void lan8720_phy_get_status(uint8_t busid, uint16_t phy_addr, struct chry_phy_status *status)
{
    uint16_t regval;

    regval = chry_mdio_read(busid, phy_addr, MII_BMSR);
    status->link = regval & BMSR_LINKSTATUS;

    regval = chry_mdio_read(busid, phy_addr, LAN8720_PSCSR);
    status->duplex = regval & LAN8720_PSCSR_DUPLEX_STATUS;

    switch ((regval & LAN8720_PSCSR_SPEED_STATUS) >> 2) {
        case 1:
            status->speed = 10;
            break;
        case 2:
            status->speed = 100;
            break;

        default:
            break;
    }
}

const struct chry_phy_driver lan8720_driver = {
    .phy_id = 0x0007C0F0,
    .phy_id_mask = 0xFFFFFFF0,
    .phy_name = "LAN8720",
    .phy_desc = "MICROCHIP LAN8720 Ethernet PHY",
    .phy_init = lan8720_phy_init,
    .phy_get_status = lan8720_phy_get_status,
};