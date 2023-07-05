#include "config.h"
#include "net/netstack.h"
#include "dev/radio.h"

void set_radio_transmission_power(int power) {
	radio_value_t tx_power = power;
	NETSTACK_RADIO.set_value(RADIO_PARAM_TXPOWER, tx_power);
}

void set_radio_channel(int channel) {
	radio_value_t radio_channel = channel;
	NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, radio_channel);
}

void apply_config() {
	// set_radio_channel(10);
	// set_radio_transmission_power(10);
}

