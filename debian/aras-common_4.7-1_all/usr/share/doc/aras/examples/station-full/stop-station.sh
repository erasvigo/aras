#!/bin/bash

# This script stops the neccessary tools to set up an internet radio

# Stop jackd
killall jackd

# Stop aras-player and aras-recorder
killall aras-player
killall aras-recorder

# Stop calfjackhost
killall calfjackhost

# Stop ffmpeg and ices2
killall ffmpeg
killall ices2

# Stop metadata manager
killall update-metadata.sh

# Stop jack-plumbing
killall jack-plumbing

