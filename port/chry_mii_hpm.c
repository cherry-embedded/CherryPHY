#include "chry_phy.h"
#include "hpm_enet_drv.h"

void hpm_mdio_write(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    ENET_Type *ptr = phydev->user_data;
    enet_write_phy(ptr, phy_addr, regnum, val);
}

uint16_t hpm_mdio_read(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum)
{
    ENET_Type *ptr = phydev->user_data;
    return enet_read_phy(ptr, phy_addr, regnum);
}

#ifdef HPMSOC_HAS_HPMSDK_ESC
#include "hpm_esc_drv.h"

void hpm_esc_mdio_write(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum, uint16_t val)
{
    ESC_Type *ptr = phydev->user_data;
    esc_mdio_write(ptr, phy_addr, regnum, val);
}

uint16_t hpm_esc_mdio_read(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum)
{
    uint16_t regval;

    ESC_Type *ptr = phydev->user_data;
    esc_mdio_read(ptr, phy_addr, regnum, &regval);
    return regval;
}
#endif