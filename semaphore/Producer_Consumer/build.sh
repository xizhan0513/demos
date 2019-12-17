#!/bin/bash

if [ "$1" = "clean" ]; then
	rm pc_demo
	exit 0
else
	gcc Producer_Consumer.c -lpthread -o pc_demo
	exit 1
fi
