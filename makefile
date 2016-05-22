all: run-test

run-test:
	gcc -I./src src/vgscpu.c test/tp_stack_a.c -o tp_stack_a
	./tp_stack_a

