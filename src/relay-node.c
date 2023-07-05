#include "stdio.h"
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "sys/log.h"
#include "config.h"


#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO


static struct simple_udp_connection udp_conn;


PROCESS(relay_process, "Relay Node Process");
AUTOSTART_PROCESSES(&relay_process); 


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

}


PROCESS_THREAD(relay_process, ev, data)
{
  PROCESS_BEGIN();
		apply_config();
		NETSTACK_ROUTING.root_start();
		simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);
  
	PROCESS_END();
}
