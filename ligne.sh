#!/bin/sh
cd controller && x-terminal-emulator -t "n1" -e "./controller ../config/ligne/ligne.dot"&
sleep 1
cd router && ./ligne.sh
