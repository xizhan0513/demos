#!/bin/bash

if [ "$1" = "clean" ]; then
	rm demo_tailq
	exit 0
else
	gcc tailq.c -o demo_tailq
	exit 1
fi
