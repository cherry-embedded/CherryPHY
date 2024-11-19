#include "chry_phy.h"
#include "board.h"

#define PHY_ADDR 2

extern void xxx_mdio_write(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum, uint16_t val);
extern uint16_t xxx_mdio_read(struct chry_phy_device *phydev, uint16_t phy_addr, uint16_t regnum);

struct chry_phy_device phydev1;

struct chry_phy_status current_status;

void phy_monitor(void)
{
    struct chry_phy_status status;

    chry_phy_get_status(&phydev1, &status);

    if (memcmp(&current_status, &status, sizeof(struct chry_phy_status)) != 0) {
        memcpy(&current_status, &status, sizeof(struct chry_phy_status));

        if (status.link) {
            printf("Link Status: Up\n");
            printf("Link Speed:  %s\n", status.speed == 10 ? "10Mbps" : status.speed == 100 ? "100Mbps" :
                                                                                              "1000Mbps");
            printf("Link Duplex: %s\n", status.duplex ? "Full Duplex" : "Half Duplex");
        } else {
            printf("Link Status: Down\n");
        }
    }
}

int main(void)
{
    /* init enet */
    //......

    /* init phy */
    struct chry_phy_config config;

    config.auto_negotiation = true;
    config.loopback = false;

    phydev1.mdio_write = xxx_mdio_write;
    phydev1.mdio_read = xxx_mdio_read;
    phydev1.user_data = xxx;
    phydev1.phy_addr = PHY_ADDR;
    chry_phy_init(&phydev1, &config);

    board_timer_create(500, phy_monitor);

    printf("hello cherryphy\n");
    while (1) {
    }
    return 0;
}