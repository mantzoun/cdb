#!/bin/sh

export LD_LIBRARY_PATH=./DPP/library

if test -p /tmp/mqtt_disc_cpp.fifo
then
    echo fifo present
else
    echo fifo not present, creating
    mkfifo /tmp/mqtt_disc_cpp.fifo
fi

./bin/cdb ./config.txt.0
