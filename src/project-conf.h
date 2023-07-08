#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#define INTERVAL_BETWEEN_MESSAGES_SECONDS 1
#define MESSAGE_SIZE 64

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

// RPL Lite configuration parameters
#define RPL_CONF_PARENT_SELECTION 0

// Netstack layer
#define NETSTACK_CONF_MAC csma_driver

void apply_config();
void set_radio_channel(int channel);
void set_radio_transmission_power(int power);

#endif /* PROJECT_CONF_H_ */
