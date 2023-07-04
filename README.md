# wsn-hiking

Wireless Sensor Networks

## Sensor Nodes Used:

Zolertia Remote Rev-B

## TODO:

- [X] Setting up the dev environment with Raspberries + Sensor Nodes
- [ ] Data processing scripts
- [ ] Report 

## Compilation and running:

- In order for the Makefile to work you need to set an environment variable `CNG_PATH` with the path to your contiki-ng installation.
- In order to compile the project run: `make TARGET=zoul BOARD=remote-revb`
- To flash a node, connect it to the computer through the **correct port** (important if you don't wish to lose 2 hours of your afternoon like me). Afterwards, in order to flash it with the desired file, use: `make TARGET=zoul BOARD=remote-revb {filename}.upload`
- If you wish to see the output of the node, make sure it's still connected and run: `make TARGET=zoul BOARD=remote-revb login`. This will connect to the node through the serial port.
