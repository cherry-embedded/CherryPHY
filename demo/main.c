#include "chry_phy.h"
#include "board.h"

#define PHY_ADDR 2
#define BUSID    0

struct chry_phy_status current_status;

void phy_monitor(void)
{
    struct chry_phy_status status;

    chry_phy_get_status(BUSID, &status);

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
    config.duplex = true;
    chry_phy_init(BUSID, PHY_ADDR, &config, NULL);

    board_timer_create(1000, phy_monitor);

    printf("hello cherryphy\n");
    while (1) {
    }
    return 0;
}