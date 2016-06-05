all: format run-test

format:
	@sh tools/format.sh src/vgscpu.c
	@sh tools/format.sh src/vgscpu.h
	@sh tools/format.sh src/vgscpu_internal.h 

run-test: vgscpu.o
	@make run-test-exec TP=tp_stack_a
	@make run-test-exec TP=tp_stack_b
	@make run-test-exec TP=tp_stack_c
	@make run-test-exec TP=tp_stack_d
	@make run-test-exec TP=tp_ldst_a
	@make run-test-exec TP=tp_ldst_b
	@make run-test-exec TP=tp_ldst_c
	@make run-test-exec TP=tp_ldst_d
	@make run-test-exec TP=tp_inc
	@make run-test-exec TP=tp_dec
	@make run-test-exec TP=tp_not
	@make run-test-exec TP=tp_jmp
	@make run-test-exec TP=tp_jz_jnz

vgscpu.o: src/vgscpu.c src/vgscpu.h src/vgscpu_internal.h
	@gcc -I./src src/vgscpu.c -c -o vgscpu.o

run-test-exec:
	@echo testing: $(TP)
	@sh tools/format.sh test/$(TP).c 
	@gcc -I./src vgscpu.o test/$(TP).c -o $(TP) && ./$(TP)
