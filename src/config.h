#ifndef CONFIG_NODES_H
#define CONFIG_NODES_H


#define INTERVAL_BETWEEN_MESSAGES_SECONDS 1
#define MESSAGE_SIZE 64

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

void apply_config();
void set_radio_channel(int channel);
void set_radio_transmission_power(int power);

#endif
