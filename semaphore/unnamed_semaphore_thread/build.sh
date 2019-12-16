#!/bin/bash

if [ "$1" = "clean" ]; then
	rm uname_sema
	exit 0
else
	gcc unnamed_semaphore_thread.c -lpthread -o uname_sema
	exit 1
fi
