#!/bin/sh
x-terminal-emulator -e "./router n1 conf/n1.cfg"&
x-terminal-emulator -e "./router n2 conf/n2.cfg"&
x-terminal-emulator -e "./router n3 conf/n3.cfg"&
x-terminal-emulator -e "./router n4 conf/n4.cfg"&
