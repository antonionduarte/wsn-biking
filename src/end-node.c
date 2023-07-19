#include <stdint.h>
#include <stdio.h>

#include "os/storage/cfs/cfs.h"
#include "project-conf.h"
#include "stdio.h"
#include "contiki.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "packetbuf.h"
#include "dev/serial-line.h"
#include "dev/leds.h"
#include "sys/log.h"
#include "cpu/arm/common/SD-card/sdcard.h"
#include "arch/cpu/arm/common/SD-card/sdcard.h"
#include "lib/sensors.h"
#include "dev/button-sensor.h"
#include "dev/button-hal.h"
#include "sys/rtimer.h"

#ifndef NEED_FORMATTING
#define NEED_FORMATTING 0
#endif

#define MAX_FILENAME_LEN 64
#define MAX_READ_SIZE 512

#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define RPL_CONF_DAG_ROOT_RANK 1

static struct simple_udp_connection udp_conn;
static process_event_t message_received_event;
static struct rtimer my_rtimer;

PROCESS(end_process, "End Node Process");
PROCESS(file_transfer, "File Transfer Process");

AUTOSTART_PROCESSES(&end_process);

void generate_message(int32_t message_id, size_t buffer_size, char payload_value, char *buffer) {
	if (buffer_size < sizeof(message_id) + MESSAGE_SIZE) {
		fprintf(stderr, "Buffer is too small for the message\n");
		return;
	}
	
	memcpy(buffer, &message_id, sizeof(message_id));
	memset(buffer + sizeof(message_id), payload_value, buffer_size - sizeof(message_id));
}

static int32_t extract_message_id(const char *data) {
	int32_t message_id;
	memcpy(&message_id, data, sizeof(message_id));
	return message_id;
}

// rtimer callback function
void rtimer_callback(struct rtimer *rt, void *ptr)
{
	process_post(&end_process, PROCESS_EVENT_CONTINUE, NULL);
	rtimer_set(&my_rtimer, RTIMER_NOW() + RTIMER_PERIOD, 1, rtimer_callback, NULL);
}

static void udp_rx_callback(
	struct simple_udp_connection *conn,
	const uip_ipaddr_t *sender_addr,
	uint16_t sender_port,
	const uip_ipaddr_t *receiver_addr,
	uint16_t receiver_port,
	const uint8_t *data,
	uint16_t datalen
) 
{
	leds_off(LEDS_ALL);
	leds_on(LEDS_YELLOW);
	clock_delay(40000);
	leds_off(LEDS_YELLOW);

	int16_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	int16_t lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
	long message_id = extract_message_id((char *) data);

	clock_time_t curr_time_in_ticks = clock_time();
	unsigned long curr_time_in_seconds = (unsigned long)(curr_time_in_ticks / CLOCK_SECOND);

	char buffer[128];  // allocate a buffer, ensure it is large enough
	sprintf(buffer, "%ld, %lu, %d, %d\n", message_id, curr_time_in_seconds, rssi, lqi);

	LOG_INFO("%s\n", buffer);

	char filename[100];
	sprintf(filename, "%d-%d-%d-%d.txt", INTERVAL_BETWEEN_MESSAGES_SECONDS, MESSAGE_SIZE, RADIO_TX_POWER, RADIO_CHANNEL);
	printf("FILENAME:%s\n", filename); // Adding this line
	
	// static int file;
	// file = cfs_open(filename, CFS_WRITE | CFS_APPEND);

	// cfs_write(file, (void*) buffer, strlen(buffer));
	// cfs_close(file);

	process_post(&end_process, message_received_event, NULL);
}

PROCESS_THREAD(end_process, ev, data)
{
	static long curr_message = 0;

	uip_ipaddr_t relay_ipaddr; 

	PROCESS_BEGIN();
		
		#if NEED_FORMATTING
			cfs_coffee_format();
		#endif

		simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

		apply_config();

		rtimer_set(&my_rtimer, RTIMER_NOW() + RTIMER_PERIOD, 1, rtimer_callback, NULL);

		while (1) {
			PROCESS_WAIT_EVENT();

			if (ev == message_received_event) {
				leds_off(LEDS_ALL);
				if (NETSTACK_ROUTING.node_is_reachable()) {
					leds_on(LEDS_GREEN);
				} else {
					leds_on(LEDS_RED);
				}
			}

			if (ev == button_hal_press_event) {
				process_start(&file_transfer, NULL);
				PROCESS_EXIT();
			}

			if (ev == PROCESS_EVENT_CONTINUE) {
				if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&relay_ipaddr)) {
					leds_off(LEDS_ALL);
					leds_on(LEDS_GREEN);
					
					char payload_value = 'A';
					char buffer[sizeof(curr_message) + MESSAGE_SIZE];
					generate_message(curr_message, sizeof(curr_message) + MESSAGE_SIZE, payload_value, buffer);
					simple_udp_sendto(&udp_conn, buffer, sizeof(buffer), &relay_ipaddr);
					curr_message++;
				} else {
					leds_off(LEDS_ALL);
					leds_on(LEDS_RED);
				}
			}
		}

	PROCESS_END();
}

PROCESS_THREAD(file_transfer, ev, data)
{
	PROCESS_BEGIN();
	
		struct cfs_dir dir;
		struct cfs_dirent dirent;
		int fd;

		if (cfs_opendir(&dir, "/") == -1) {
			printf("Failed to open directory\n");
			PROCESS_EXIT();
		}

		char buffer[MAX_READ_SIZE];

		while (cfs_readdir(&dir, &dirent) != -1) {
			printf("FILENAME:%s\n", dirent.name);
			printf("FILESTART:\n");

			fd = cfs_open(dirent.name, CFS_READ);
			if (fd != -1) {
				int n;
				while ((n = cfs_read(fd, buffer, MAX_READ_SIZE)) > 0) {
					buffer[n] = '\0';  // Ensure null termination
					printf("%s", buffer);
				}
				printf("\nFILEEND:\n");
				cfs_close(fd);
			} else {
				printf("Failed to open file %s\n", dirent.name);
			}
		}

		printf("Finished.");

		cfs_closedir(&dir);

		PROCESS_EXIT();

	PROCESS_END();
}
