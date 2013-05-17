#!/bin/sh
cd controller && ./exec.sh&
sleep 1
cd router && ./exec.sh
