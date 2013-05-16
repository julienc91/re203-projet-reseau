#!/bin/sh
x-terminal-emulator -t "n1" -e "./router n1 ../config/etoile/n1.cfg"&
x-terminal-emulator -t "n2" -e "./router n1 ../config/etoile/n2.cfg"&
x-terminal-emulator -t "n3" -e "./router n1 ../config/etoile/n3.cfg"&
x-terminal-emulator -t "n4" -e "./router n1 ../config/etoile/n4.cfg"&
x-terminal-emulator -t "n5" -e "./router n1 ../config/etoile/n5.cfg"&
