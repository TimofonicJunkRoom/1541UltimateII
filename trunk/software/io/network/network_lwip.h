#ifndef NETWORK_LWIP_H
#define NETWORK_LWIP_H

//#include "ipv4/lwip/ip_addr.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/ip_frag.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/inet.h"
#include "lwip/autoip.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
extern "C" {
#include "small_printf.h"
#include "arch/sys_arch.h"
}

#include "network_interface.h"
#include "fifo.h" // my oh so cool fifo! :)

class NetworkLWIP : public NetworkInterface
{
	struct pbuf_custom pbuf_array[64];
	Fifo<struct pbuf_custom *> pbuf_fifo;

	void lwip_poll();
public:
    struct netif   my_net_if;

    // fields that are filled in by the configuration
    struct ip_addr my_ip;
    struct ip_addr my_netmask;
    struct ip_addr my_gateway;
    int dhcp_enable;

    bool   if_up;

    void *driver;
    void (*driver_free_function)(void *driver, void *buffer);
    BYTE (*driver_output_function)(void *driver, void *buffer, int pkt_len);

    NetworkLWIP(void *driver,
				driver_output_function_t out,
				driver_free_function_t free);
    virtual ~NetworkLWIP();

    const char *identify() { return "NetworkLWIP"; }

    bool start();
    void stop();
    void poll();
    void link_up();
    void link_down();
    void set_mac_address(BYTE *mac);
    bool input(BYTE *raw_buffer, BYTE *payload, int pkt_size);

    virtual void init_callback();
    virtual BYTE output_callback(struct netif *, struct pbuf *) {
        printf("Network Interface: Output Callback - Base\n");
    }

    void free_pbuf(struct pbuf_custom *pbuf);

    // from ConfigurableObject
    void effectuate_settings(void);
};

NetworkInterface *getNetworkStack(void *driver,
								  driver_output_function_t out,
								  driver_free_function_t free);

void releaseNetworkStack(void *netstack);

#endif
