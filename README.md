# Biking Wireless Sensor Nodes

Project with the objective of determining the performance of packet transmission using Zolertia Remote Revision-B Sensor Nodes's effectiveness under different conditions while biking.

## Experimental Setup:

To conduct our experiments we used three bikes. Two of the bikes are equipped with end nodes, and the bike in the middle with a relay node.
The end nodes should send messages to each other in a configurable periodic fashion. The messages should go through the relay node.

The configurable parameters are:

- Inter Packet Interval.
- Packet size.
- Amount of packets.
- Channel.
- Transmission Power.

```
< End Node > ----- ( m ) ----- < Relay Node > ----- ( m ) ----- < End Node > 
```

## Sensor Nodes Used:

Zolertia Remote Rev-B

## TODO:

- [X] Setting up the dev environment with Raspberries + Sensor Nodes
- [X] One node sending messages to another node
- [X] Messages need to be of specific sizes, the size of messages needs to be configurable.
- [X] Three nodes sending messages, two end nodes sending messages to each other periodically through a relay node
- [X] Writing csv files to the SD cards with relevant information
- [X] Data processing scripts
- [X] Report 

## Compilation and running:

- In order for the Makefile to work you need to set an environment variable `CNG_PATH` with the path to your contiki-ng installation.
- In order to compile the project run: `make TARGET=zoul BOARD=remote-revb NODE_SELECT=<end | relay>`
- To flash a node, connect it to the computer through the **correct port** (important if you don't wish to lose 2 hours of your afternoon like me). Afterwards, in order to flash it with the desired file, use: `make TARGET=zoul BOARD=remote-revb <filename>.upload NODE_SELECT=<end | relay>`
- If you wish to see the output of the node, make sure it's still connected and run: `make TARGET=zoul BOARD=remote-revb login`. This will connect to the node through the serial port.

## Node types

- Relay Nodes: To start this you only need to press the reset button, they do not have any type of different mode.
- End Nodes: For these nodes there are two processes, the normal message sending and logging process, and a file transfer process that sends the data via the serial interface. To change to the second process you need to press the user button on the Zolertia Remote Rev-B nodes.

**Note:** When running make commands, you need to specify the `NODE_SELECT`, which is specific to this project, and is meant to specify if you're compiling the software for one of the end nodes or for the relay node.

## Configuration

In order to change the configuration parameters, such as the `TX_POWER`, `TX_CHANNEL`, `IPI`, `Packet Size`, you can change the `src/project-conf.h` header file.
