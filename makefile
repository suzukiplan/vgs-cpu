TESTCASE=\
	tp_load_program\
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
	tp_jz_jnz\
	tp_sh\
	tp_add_a\
	tp_sub_a\
	tp_mul_a\
	tp_div_a\
	tp_mod_a\
	tp_and_a\
	tp_or_a\
	tp_xor_a\
	tp_cmp_a\
	tp_cmp2_a\
	tp_add_b\
	tp_sub_b\
	tp_mul_b\
	tp_div_b\
	tp_mod_b\
	tp_and_b\
	tp_or_b\
	tp_xor_b\
	tp_cmp_b\
	tp_cmp2_b\
	tp_add_c\
	tp_sub_c\
	tp_mul_c\
	tp_div_c\
	tp_mod_c\
	tp_and_c\
	tp_or_c\
	tp_xor_c\
	tp_add_d\
	tp_sub_d\
	tp_mul_d\
	tp_div_d\
	tp_mod_d\
	tp_and_d\
	tp_or_d

all:
	@echo usage:
	@echo $$ make format : executes clang-format
	@echo $$ make test : runs test

clean:
	-@rm -f vgscpu.o
	-@rm -f $(TESTCASE)

format:
	sh tools/format.sh src/cpu/vgscpu.c
	sh tools/format.sh src/cpu/vgscpu.h
	sh tools/format.sh src/cpu/vgscpu_internal.h 
	sh tools/format.sh src/cpu/vgscpu_op.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_b.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_c.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_d.h 
	sh tools/format.sh src/test/tp.h 
	for TP in $(TESTCASE); do make format-test-src TP=$$TP; done

test: vgscpu.o
	@for TP in $(TESTCASE); do make run-test-exec TP=$$TP; done
	@rm vgscpu.o

vgscpu.o: src/cpu/vgscpu.c src/cpu/vgscpu.h src/cpu/vgscpu_internal.h
	@gcc -I./src/cpu src/cpu/vgscpu.c -c -o vgscpu.o

format-test-src:
	@sh tools/format.sh src/test/$(TP).c 

run-test-exec:
	@echo testing: $(TP)
	@gcc -I./src/cpu vgscpu.o src/test/$(TP).c -o $(TP)
	@./$(TP)
	@rm ./$(TP)
