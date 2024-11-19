/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "chry_phy.h"
#include "board.h" /* we cannot get common chip header file, so we use board.h instead */

void chry_mdio_write(uint8_t busid, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    ETH_HandleTypeDef *handle;

    handle = g_phy_device[busid].user_data;
    HAL_ETH_WritePHYRegister(handle, phy_addr, regnum, val);
}

uint16_t chry_mdio_read(uint8_t busid, uint16_t phy_addr, uint16_t regnum)
{
    uint16_t regval;
    ETH_HandleTypeDef *handle;

    handle = g_phy_device[busid].user_data;
    HAL_ETH_ReadPHYRegister(handle, phy_addr, regnum, &regval);
    return regval;
}