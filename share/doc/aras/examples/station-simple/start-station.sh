#!/bin/bash

# This script runs the neccessary tools to set up an internet radio

# Run jackd
/usr/bin/jackd -ddummy -r48000 -p1024 1> /dev/null 2> /dev/null &

# Run aras-daemon
aras-daemon /etc/aras/aras.conf > /dev/null &

# Run ffmpeg and ices2
ffmpeg -f jack -i ffmpeg -y -f s16le - 2>/dev/null | ices2 ices-stdinpcm.xml

# Run metadata manager
./update-metadata.sh

sleep 1

# Run jack-plumbing
jack-plumbing -q -u 110000 jack-plumbing-rules 2> /dev/null &
jack-plumbing -q -u 440000 jack-plumbing-rules 2> /dev/null &

