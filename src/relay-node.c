#include "stdio.h"
#include "config.h"
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "sys/log.h"
#include <stdio.h>


#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO

#define RPL_CONF_DAG_ROOT_RANK 2

static struct simple_udp_connection udp_conn;

static uip_ipaddr_t first_end_node;
static uip_ipaddr_t second_end_node;

PROCESS(relay_process, "Relay Node Process");
AUTOSTART_PROCESSES(&relay_process); 


int is_default_address(const uip_ipaddr_t *address) {
	for (int i = 0; i < 4; i++) {
		if (address->u8[i] != 0) {
			return 0;
		}
	}
	return 1;
}


int compare_ip_addresses(const uip_ipaddr_t *addr1, const uip_ipaddr_t *addr2) {
	for (int i = 0; i < sizeof(uip_ipaddr_t); i++) {
		if (addr1->u8[i] != addr2->u8[i]) {
			return 0;
		}
	}
	return 1;
}


static void udp_rx_callback(
	struct simple_udp_connection *conn,
	const uip_ipaddr_t *sender_addr,
	uint16_t sender_port,
	const uip_ipaddr_t *receiver_addr,
	uint16_t receiver_port,
	const uint8_t *data,
	uint16_t datalen
) {
	LOG_INFO("Received request '%.*s'\n", datalen, (char *) data);
	
	if (is_default_address(&first_end_node) && !compare_ip_addresses(&second_end_node, sender_addr)) {
		first_end_node = *sender_addr;
	} else if (is_default_address(&second_end_node) && !compare_ip_addresses(&first_end_node, sender_addr)) {
		second_end_node = *sender_addr;
	}

	if (compare_ip_addresses(&first_end_node, sender_addr)) {
		if (!is_default_address(&second_end_node)) {
			// relay the packet to the second end node
			simple_udp_sendto(&udp_conn, data, datalen, &second_end_node);
		}
	} else {
		if (!is_default_address(&first_end_node)) {
			// relay the packet to the first end node
			simple_udp_sendto(&udp_conn, data, datalen, &first_end_node);
		}
	}

	printf("FIRST ADDR: %d\n", is_default_address(&first_end_node));
	printf("SECOND ADDR: %d\n", is_default_address(&second_end_node));

	LOG_INFO_6ADDR(sender_addr);
	LOG_INFO("\n");
}


PROCESS_THREAD(relay_process, ev, data)
{
  PROCESS_BEGIN();
		apply_config();
		NETSTACK_ROUTING.root_start();
		simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);

		printf("FIRST ADDR: %d\n", is_default_address(&first_end_node));
		printf("SECOND ADDR: %d\n", is_default_address(&second_end_node));
	PROCESS_END();
}
