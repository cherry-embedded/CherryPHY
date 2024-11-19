/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bflb_emac.h"

void bl_mdio_write(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    struct bflb_device_s *emac_dev = phydev->user_data;
    bflb_emac_feature_control(emac_dev, EMAC_CMD_SET_PHY_ADDRESS, phy_addr);
    bflb_emac_phy_reg_write(emac_dev, regnum, val);
}

uint16_t bl_mdio_read(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum)
{
    uint16_t regval;

    struct bflb_device_s *emac_dev = phydev->user_data;
    bflb_emac_feature_control(emac_dev, EMAC_CMD_SET_PHY_ADDRESS, phy_addr);
    bflb_emac_phy_reg_read(emac_dev, regnum, &regval);
    return regval;
}