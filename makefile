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
	tp_je_jne\
	tp_jn_jnn\
	tp_jp_jnp\
	tp_cal_ret\
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
	tp_cmp_c\
	tp_cmp2_c\
	tp_add_d\
	tp_sub_d\
	tp_mul_d\
	tp_div_d\
	tp_mod_d\
	tp_and_d\
	tp_or_d\
	tp_xor_d\
	tp_cmp_d\
	tp_cmp2_d\
	tp_int

VGSASM_SRC=\
	src/asm/vgsasm.c\
	src/asm/vgsasm_parse.c\
	src/asm/vgsasm_parse_push.c\
	src/asm/vgsasm_parse_pop.c\
	src/asm/vgsasm_parse_ld.c\
	src/asm/vgsasm_parse_st.c\
	src/asm/vgsasm_parse_inc.c\
	src/asm/vgsasm_parse_dec.c\
	src/asm/vgsasm_parse_not.c\
	src/asm/vgsasm_parse_shift.c\
	src/asm/vgsasm_parse_acu.c\
	src/asm/vgsasm_parse_arl.c\
	src/asm/vgsasm_parse_branch.c\
	src/asm/vgsasm_parse_int.c\
	src/asm/vgsasm_util.c

all:
	@echo usage:
	@echo $$ make format : executes clang-format
	@echo $$ make build : builds vgscpu.a
	@echo $$ make test : runs test
	@echo $$ make clean : clean modules

clean:
	-@rm -f vgscpu.a
	-@rm -f vgsapi.o
	-@rm -f vgscpu.o
	-@rm -f $(TESTCASE)

format:
	sh tools/format.sh src/asm/vgsasm.h
	sh tools/format.sh src/cpu/vgsapi.c
	sh tools/format.sh src/cpu/vgscpu.c
	sh tools/format.sh src/cpu/vgscpu.h
	sh tools/format.sh src/cpu/vgscpu_internal.h 
	sh tools/format.sh src/cpu/vgscpu_op.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_b.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_c.h 
	sh tools/format.sh src/cpu/vgscpu_op_acu_d.h 
	sh tools/format.sh src/test/tp.h 
	sh tools/format.sh src/test/asmtest.c 
	for AS in $(VGSASM_SRC); do make format-src SRC=$$AS; done
	for TP in $(TESTCASE); do make format-test-src TP=$$TP; done

format-src:
	@sh tools/format.sh $(SRC) 

build: vgscpu.a vgsasm

vgscpu.a: vgscpu.o
	ar ruv vgscpu.a vgscpu.o

vgscpu.o: src/cpu/vgscpu.c src/cpu/vgscpu.h src/cpu/vgscpu_internal.h
	gcc -O2 -I./src/cpu src/cpu/vgscpu.c -c -o vgscpu.o

vgsasm: src/asm/vgsasm.h src/cpu/vgscpu_internal.h $(VGSASM_SRC)
	gcc -O2 -I./src/asm -I./src/cpu $(VGSASM_SRC) -o vgsasm

test: build
	for TP in $(TESTCASE); do make run-test-exec TP=$$TP; done
	make asm-test

format-test-src:
	@sh tools/format.sh src/test/$(TP).c 

run-test-exec:
	@echo testing: $(TP)
	gcc -I./src/cpu src/test/$(TP).c -o $(TP) vgscpu.a
	./$(TP)
	@rm ./$(TP)

asm-test:
	gcc -I./src/cpu src/test/asmtest.c -o asmtest vgscpu.a
	./asmtest src/test/asm_push_pop_a.asm
	./asmtest src/test/asm_push_pop_b.asm
	./asmtest src/test/asm_push_pop_c.asm
	./asmtest src/test/asm_push_pop_d.asm
	./asmtest src/test/asm_ld_st_a.asm
	./asmtest src/test/asm_ld_st_b.asm
	./asmtest src/test/asm_ld_st_c.asm
	./asmtest src/test/asm_ld_st_d.asm
	@rm asmtest
	@rm tp.bin
