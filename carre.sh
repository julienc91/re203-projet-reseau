#!/bin/sh
cd controller && x-terminal-emulator -t "n1" -e "./controller ../config/carre/carre.dot"&
sleep 1
cd router && ./carre.sh
