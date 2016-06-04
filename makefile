all: format run-test

format:
	@make exec-format SRC=src/vgscpu.c 
	@make exec-format SRC=src/vgscpu.h
	@make exec-format SRC=src/vgscpu_internal.h
	@make exec-format SRC=test/tp_ldst_a.c
	@make exec-format SRC=test/tp_stack_a.c
	@make exec-format SRC=test/tp_inc.c
	@make exec-format SRC=test/tp_dec.c

exec-format: $(SRC)
	clang-format -assume-filename=.clang-format < $(SRC) >.work; cat .work > $(SRC)

run-test:
	gcc -I./src src/vgscpu.c test/tp_stack_a.c -o tp_stack_a && ./tp_stack_a
	gcc -I./src src/vgscpu.c test/tp_ldst_a.c -o tp_ldst_a && ./tp_ldst_a
	gcc -I./src src/vgscpu.c test/tp_inc.c -o tp_inc && ./tp_inc
	gcc -I./src src/vgscpu.c test/tp_dec.c -o tp_dec && ./tp_dec
