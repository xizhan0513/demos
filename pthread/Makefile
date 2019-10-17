all:
	make demo_pthread
	make priority_pthread
demo_pthread: pthread.c
	@gcc -o demo_pthread pthread.c -lpthread
priority_pthread: pthread_priority.c 
	@gcc -o priority_pthread pthread_priority.c -lpthread
clean:
	@rm demo_pthread
	@rm priority_pthread 
