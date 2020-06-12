#!/bin/bash

# This script updates a metadata file periodically from the ARAS log file and
# notifies it to all IceS 2 processes.

LOGFILE=/var/log/aras/aras.log
METAFILE=/tmp/metadata

while true; do

        # Get current block and file from log
        BLOCK=$(cat $LOGFILE | egrep 'Regular block: |Default block: ' | tail -n 1 | sed -e 's/.*: //' | tr -d \" | tr '_' ' ')
        FILE=$(cat $LOGFILE | grep -v ' block: ' | tail -n 1 | sed -e 's/.*\///' | sed -e 's/\.[^.]*$//')
        FILE=$(echo $FILE | sed -e 's/%\([0-9A-Fa-f][0-9A-Fa-f]\)/\\\\\x\1/g' | xargs echo -e)
        ENCODER="ARAS 4.6"
        COMMENT="The Radio Station"

        # Print artist and title in metadata file
        echo "artist=The Radio Station | $BLOCK" > $METAFILE
        echo "title=$FILE" >> $METAFILE
        echo "encoder=$ENCODER" >> $METAFILE
        echo "comment=$COMMENT" >> $METAFILE

        # Send SIGUSR1 signal to make ices2 processes update metadata
        PID=$(pidof ices2)
        if [ $PID ]
        then
                kill -SIGUSR1 $PID
        fi

        sleep 5
done
