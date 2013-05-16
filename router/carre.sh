#!/bin/sh
x-terminal-emulator -t "n1" -e "bash -c \"./router n1 ../config/carre/n1.cfg; exec bash\""&
x-terminal-emulator -t "n2" -e "bash -c \"./router n2 ../config/carre/n2.cfg; exec bash\""&
x-terminal-emulator -t "n3" -e "bash -c \"./router n3 ../config/carre/n3.cfg; exec bash\""&
x-terminal-emulator -t "n4" -e "bash -c \"./router n4 ../config/carre/n4.cfg; exec bash\""&
x-terminal-emulator -t "n5" -e "bash -c \"./router n5 ../config/carre/n5.cfg; exec bash\""&
x-terminal-emulator -t "n6" -e "bash -c \"./router n6 ../config/carre/n6.cfg; exec bash\""&
x-terminal-emulator -t "n7" -e "bash -c \"./router n7 ../config/carre/n7.cfg; exec bash\""&
x-terminal-emulator -t "n8" -e "bash -c \"./router n8 ../config/carre/n8.cfg; exec bash\""&
x-terminal-emulator -t "n9" -e "bash -c \"./router n9 ../config/carre/n9.cfg; exec bash\""&
