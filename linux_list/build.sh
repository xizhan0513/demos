#!/bin/bash

if [ "$1" = "clean" ]; then
	rm demo_list
	exit 0
else
	gcc list_demo.c -o demo_list
	exit 1
fi
