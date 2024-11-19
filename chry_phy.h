/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef CHRY_PHY_H
#define CHRY_PHY_H

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "chry_mii.h"

#define CONFIG_MAX_BUS     2

struct chry_phy_config {
    bool loopback;
    bool auto_negotiation;
    bool duplex;
    uint16_t speed;
};

struct chry_phy_status {
    bool link;
    bool duplex;
    uint16_t speed;
};

struct chry_phy_driver {
    char *phy_name;
    char *phy_desc;
    uint32_t phy_id;
    uint32_t phy_id_mask;
    void (*phy_init)(uint8_t busid, uint16_t phy_addr, struct chry_phy_config *config);
    void (*phy_get_status)(uint8_t busid, uint16_t phy_addr, struct chry_phy_status *status);
};

struct chry_phy_support {
    uint32_t support_100base_t4       : 1;
    uint32_t support_1000base_tx_full : 1;
    uint32_t support_1000base_tx_half : 1;
    uint32_t support_100base_tx_full  : 1;
    uint32_t support_100base_tx_half  : 1;
    uint32_t support_10base_tx_full   : 1;
    uint32_t support_10base_tx_half   : 1;
    uint32_t support_asym_pause       : 1;
    uint32_t support_pause            : 1;
    uint32_t support_autoeng          : 1;

    uint32_t reserved : 22;
};

struct chry_phy_device {
    uint16_t phy_addr;
    uint32_t phy_id;
    struct chry_phy_support support;
    const struct chry_phy_driver *driver;
    void *user_data;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct chry_phy_device g_phy_device[CONFIG_MAX_BUS];

int chry_phy_init(uint8_t busid, uint16_t phy_addr, struct chry_phy_config *config, void *user_data);
void chry_phy_get_status(uint8_t busid, struct chry_phy_status *status);

#ifdef __cplusplus
}
#endif

#endif