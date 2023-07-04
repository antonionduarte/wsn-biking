#include "stdio.h"
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"

#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;


PROCESS(intermediate_process, "Intermediate Node Process");
AUTOSTART_PROCESSES(&intermediate_process); 


static void udp_rx_callback(
	struct simple_udp_connection *conn,
	const uip_ipaddr_t *sender_addr,
	uint16_t sender_port,
	const uip_ipaddr_t *receiver_addr,
	uint16_t receiver_port,
	const uint8_t *data,
	uint16_t datalen
) {
	// TODO: Do stuff within the rx_callback
	
	// In this case, we need to check the IP_ADDR of the person who sent the message,
	// We need to get the IP_ADDR of the other node
	// And then we need to forward the message to the other node
	
	LOG_INFO("Received request '%.*s'\n", datalen, (char *) data);
}


PROCESS_THREAD(intermediate_process, ev, data)
{
  PROCESS_BEGIN();
		NETSTACK_ROUTING.root_start();

		simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);

		/* Setup a periodic timer that expires after 10 seconds. */
		// etimer_set(&timer, CLOCK_SECOND * 10);

		

		// while(1) {
		// 	printf("Hello, world\n");

		// 	/* Wait for the periodic timer to expire and then restart the timer. */
		// 	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

		// 	etimer_reset(&timer);
		// }

  PROCESS_END();
}
