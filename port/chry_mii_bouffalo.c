/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bflb_emac.h"

static struct bflb_device_s *emac_dev = NULL;

void chry_mdio_write(uint8_t busid, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    if (emac_dev == NULL) {
        emac_dev = bflb_device_get_by_name("emac0");
    }

    bflb_emac_feature_control(emac_dev, EMAC_CMD_SET_PHY_ADDRESS, phy_addr);
    bflb_emac_phy_reg_write(emac_dev, regnum, val);
}

uint16_t chry_mdio_read(uint8_t busid, uint16_t phy_addr, uint16_t regnum)
{
    uint16_t regval;

    if (emac_dev == NULL) {
        emac_dev = bflb_device_get_by_name("emac0");
    }

    bflb_emac_feature_control(emac_dev, EMAC_CMD_SET_PHY_ADDRESS, phy_addr);
    bflb_emac_phy_reg_read(emac_dev, regnum, &regval);
    return regval;
}