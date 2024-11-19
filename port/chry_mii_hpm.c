#include "hpm_enet_drv.h"

const ENET_Type *enet_base[] = {
    HPM_ENET0,
#ifdef HPM_ENET1
    HPM_ENET1,
#endif
};

void chry_mdio_write(uint8_t busid, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    ENET_Type *ptr = (ENET_Type *)enet_base[busid];
    enet_write_phy(ptr, phy_addr, regnum, val);
}

uint16_t chry_mdio_read(uint8_t busid, uint16_t phy_addr, uint16_t regnum)
{
    ENET_Type *ptr = (ENET_Type *)enet_base[busid];
    return enet_read_phy(ptr, phy_addr, regnum);
}