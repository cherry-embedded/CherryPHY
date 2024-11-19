/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "chry_phy.h"

#include "chry_phy_dp83848.c"
#include "chry_phy_dp83847.c"
#include "chry_phy_lan8720.c"
#include "chry_phy_rtl8201.c"
#include "chry_phy_rtl8211.c"
#include "chry_phy_yt8522.c"
#include "chry_phy_ksz8081.c"

const struct chry_phy_driver *g_phy_driver_table[] = {
    &dp83848_driver,
    &dp83847_driver,
    &lan8720_driver,
    &rtl8201_driver,
    &rtl8211_driver,
    &yt8522_driver,
    &ksz8081_driver,
};

struct chry_phy_device g_phy_device[CONFIG_MAX_BUS];

int chry_phy_init(uint8_t busid, uint16_t phy_addr, struct chry_phy_config *config, void *user_data)
{
    const struct chry_phy_driver *phydrv = NULL;
    uint16_t phy_id1;
    uint16_t phy_id2;
    uint16_t regval = 0;
    uint32_t phy_id;
    bool extended_status = false;

    /* PHY reset */
    chry_mdio_write(busid, phy_addr, MII_BMCR, BMCR_RESET);
    while (chry_mdio_read(busid, phy_addr, MII_BMCR) & BMCR_RESET) {
    }

    phy_id1 = chry_mdio_read(busid, phy_addr, MII_PHYSID1);
    phy_id2 = chry_mdio_read(busid, phy_addr, MII_PHYSID2);

    //printf("phy_id1: 0x%04x, phy_id2: 0x%04x\r\n", phy_id1, phy_id2);

    phy_id = (phy_id1 << 16) | phy_id2;
    for (uint32_t i = 0; i < sizeof(g_phy_driver_table) / sizeof(g_phy_driver_table[0]); i++) {
        if (g_phy_driver_table[i]->phy_id == (phy_id & g_phy_driver_table[i]->phy_id_mask)) {
            phydrv = g_phy_driver_table[i];
            goto phydrv_found;
        }
    }

    if (phydrv == NULL) {
        printf("Do not match phy driver for phy_id: 0x%08x\r\n", phy_id);
        return -1;
    }

phydrv_found:
    memset(&g_phy_device[busid], 0, sizeof(g_phy_device[busid]));

    g_phy_device[busid].phy_id = phy_id;
    g_phy_device[busid].phy_addr = phy_addr;
    g_phy_device[busid].driver = phydrv;
    g_phy_device[busid].user_data = user_data;

    printf("phy_id: 0x%08x, phy_name: %s, phy_desc: %s\r\n", phy_id, phydrv->phy_name, phydrv->phy_desc);

    regval = chry_mdio_read(busid, phy_addr, MII_BMSR);

    g_phy_device[busid].support.support_pause = 1;

    if (regval & BMSR_100T4) {
        g_phy_device[busid].support.support_100base_t4 = 1;
    }
    if (regval & BMSR_100FULL) {
        g_phy_device[busid].support.support_100base_tx_full = 1;
    }
    if (regval & BMSR_100HALF) {
        g_phy_device[busid].support.support_1000base_tx_half = 1;
    }
    if (regval & BMSR_10FULL) {
        g_phy_device[busid].support.support_10base_tx_full = 1;
    }
    if (regval & BMSR_10HALF) {
        g_phy_device[busid].support.support_10base_tx_half = 1;
    }
    if (regval & BMSR_ANEGCAPABLE) {
        g_phy_device[busid].support.support_autoeng = 1;
    }

    if (regval & BMSR_ESTATEN) {
        regval = chry_mdio_read(busid, phy_addr, MII_GBESR);
        if (regval & GBESR_1000_TFULL) {
            g_phy_device[busid].support.support_1000base_tx_full = 1;
        }
        if (regval & GBESR_1000_THALF) {
            g_phy_device[busid].support.support_1000base_tx_half = 1;
        }
        extended_status = true;
    }

    regval = chry_mdio_read(busid, phy_addr, MII_ANAR);
    regval &= ~(ANAR_SPEED_ALL | ANAR_SLCT | ANAR_PAUSE | ANAR_ASYM_PAUSE);
    regval |= g_phy_device[busid].support.support_100base_t4 ? ANAR_100T4 : 0;
    regval |= g_phy_device[busid].support.support_100base_tx_full ? ANAR_100FULL : 0;
    regval |= g_phy_device[busid].support.support_100base_tx_half ? ANAR_100HALF : 0;
    regval |= g_phy_device[busid].support.support_10base_tx_full ? ANAR_10FULL : 0;
    regval |= g_phy_device[busid].support.support_10base_tx_half ? ANAR_10HALF : 0;
    regval |= g_phy_device[busid].support.support_pause ? ANAR_PAUSE : 0;
    regval |= g_phy_device[busid].support.support_asym_pause ? ANAR_ASYM_PAUSE : 0;
    regval |= ANAR_CSMA;
    chry_mdio_write(busid, phy_addr, MII_ANAR, regval);

    if (extended_status) {
        regval = chry_mdio_read(busid, phy_addr, MII_GBCR);
        regval &= ~(GBCR_1000FULL | GBCR_1000HALF);
        regval |= g_phy_device[busid].support.support_1000base_tx_full ? GBCR_1000FULL : 0;
        regval |= g_phy_device[busid].support.support_1000base_tx_half ? GBCR_1000HALF : 0;
        chry_mdio_write(busid, phy_addr, MII_GBCR, regval);
    }

    regval = 0;
    if (config->loopback) {
        regval |= BMCR_LOOPBACK;
    }

    if (config->auto_negotiation) {
        regval |= BMCR_ANENABLE;
        regval |= BMCR_ANRESTART;
    } else {
        if (config->speed == 100) {
            regval |= BMCR_SPEED100;
        } else if (config->speed == 1000) {
            regval |= BMCR_SPEED100;
            regval |= BMCR_SPEED1000;
        } else {
            regval &= ~BMCR_SPEED100;
        }
    }
    chry_mdio_write(busid, phy_addr, MII_BMCR, regval);

    if (g_phy_device[busid].driver && g_phy_device[busid].driver->phy_init) {
        g_phy_device[busid].driver->phy_init(busid, phy_addr, config);
    }
    return 0;
}

void chry_phy_get_status(uint8_t busid, struct chry_phy_status *status)
{
    if (g_phy_device[busid].driver && g_phy_device[busid].driver->phy_get_status) {
        g_phy_device[busid].driver->phy_get_status(busid, g_phy_device[busid].phy_addr, status);
    }

    if (status->link == 0) {
        memset(status, 0, sizeof(struct chry_phy_status));
    }
}