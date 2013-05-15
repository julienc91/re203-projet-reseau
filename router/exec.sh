#!/bin/sh
x-terminal-emulator -t "n1" -e "./router n1 conf/n1.cfg"&
x-terminal-emulator -t "n2" -e "./router n2 conf/n2.cfg"&
x-terminal-emulator -t "n3" -e "./router n3 conf/n3.cfg"&
x-terminal-emulator -t "n4" -e "./router n4 conf/n4.cfg"&
