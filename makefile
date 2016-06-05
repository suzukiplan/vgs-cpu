TESTCASE=\
	tp_stack_a\
	tp_stack_b\
	tp_stack_c\
	tp_stack_d\
	tp_ldst_a\
	tp_ldst_b\
	tp_ldst_c\
	tp_ldst_d\
	tp_inc\
	tp_dec\
	tp_not\
	tp_jmp\
	tp_jz_jnz

all: format run-test

clean:
	-@rm -f vgscpu.o
	-@rm -f $(TESTCASE)

format:
	@sh tools/format.sh src/vgscpu.c
	@sh tools/format.sh src/vgscpu.h
	@sh tools/format.sh src/vgscpu_internal.h 
	@sh tools/format.sh test/tp.h 

run-test: vgscpu.o
	@for TP in $(TESTCASE); do make run-test-exec TP=$$TP; done

foo:
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

