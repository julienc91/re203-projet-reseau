#!/bin/sh
cd controller && x-terminal-emulator -t "n1" -e "./controller ../config/etoile/etoile.dot"&
sleep 1
cd router && ./etoile.sh
