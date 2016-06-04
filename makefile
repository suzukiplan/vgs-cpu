all: format run-test

format:
	@sh tools/format.sh src/vgscpu.c 
	@sh tools/format.sh src/vgscpu.h
	@sh tools/format.sh src/vgscpu_internal.h
	@sh tools/format.sh test/tp_ldst_a.c
	@sh tools/format.sh test/tp_ldst_b.c
	@sh tools/format.sh test/tp_ldst_c.c
	@sh tools/format.sh test/tp_ldst_d.c
	@sh tools/format.sh test/tp_stack_a.c
	@sh tools/format.sh test/tp_stack_b.c
	@sh tools/format.sh test/tp_stack_c.c
	@sh tools/format.sh test/tp_inc.c
	@sh tools/format.sh test/tp_dec.c
	@sh tools/format.sh test/tp_not.c

run-test: vgscpu.o
	gcc -I./src vgscpu.o test/tp_stack_a.c -o tp_stack_a && ./tp_stack_a
	gcc -I./src vgscpu.o test/tp_stack_b.c -o tp_stack_b && ./tp_stack_b
	gcc -I./src vgscpu.o test/tp_stack_c.c -o tp_stack_c && ./tp_stack_c
	gcc -I./src vgscpu.o test/tp_ldst_a.c -o tp_ldst_a && ./tp_ldst_a
	gcc -I./src vgscpu.o test/tp_ldst_b.c -o tp_ldst_b && ./tp_ldst_b
	gcc -I./src vgscpu.o test/tp_ldst_c.c -o tp_ldst_c && ./tp_ldst_c
	gcc -I./src vgscpu.o test/tp_ldst_d.c -o tp_ldst_d && ./tp_ldst_d
	gcc -I./src vgscpu.o test/tp_inc.c -o tp_inc && ./tp_inc
	gcc -I./src vgscpu.o test/tp_dec.c -o tp_dec && ./tp_dec
	gcc -I./src vgscpu.o test/tp_not.c -o tp_not && ./tp_not

vgscpu.o: src/vgscpu.c src/vgscpu.h src/vgscpu_internal.h
	gcc -I./src src/vgscpu.c -c -o vgscpu.o
