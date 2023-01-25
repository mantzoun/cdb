# CDB

## What is it?
CDB (originally C++ Discord Bot) is both a Discord Bot and an MQTT client.

## What does it do?
CDB is intended to be a smart home assistant, as well as a generic Discord bot. Currently, the user can configure
  * Connection information of an MQTT broker, to which are also connected the devices we are interested in interacting with
  * The MQTT device topics that the application will monitor and publish in

When started, the application will initialize the Discord bot, which will create control buttons for the configured devices, and post in a syslog channel when device status changes are detected.

There is no handling at the moment for multiple Discord Guilds, it is assumed the bot lives only on one private Guild.

For these purposes, the application uses the [DPP](https://dpp.dev/) and [Eclipse mosquitto](https://mosquitto.org/) libraries.

## I/O
CDB also listens on a named FIFO for additional instructions (not implemented)

## More information
information on the project can be found [here](https://www.mantzouneas.gr/projects/cdb/) (page in Greek).
