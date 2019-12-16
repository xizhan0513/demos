#!/bin/bash

if [ "$1" = "clean" ]; then
	rm server client
	exit 0
else
	gcc server.c -lpthread -o server
	gcc client.c -lpthread -o client
	exit 1
fi
