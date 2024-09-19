	.file	"dhry.c"
	.option nopic
	.attribute arch, "rv32i2p1"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	__udivmodsi4
	.type	__udivmodsi4, @function
__udivmodsi4:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	li	a5,1
	sw	a5,-20(s0)
	sw	zero,-24(s0)
	j	.L2
.L4:
	lw	a5,-40(s0)
	slli	a5,a5,1
	sw	a5,-40(s0)
	lw	a5,-20(s0)
	slli	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a4,-40(s0)
	lw	a5,-36(s0)
	bgeu	a4,a5,.L5
	lw	a5,-20(s0)
	beq	a5,zero,.L5
	lw	a5,-40(s0)
	bge	a5,zero,.L4
	j	.L5
.L7:
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	bltu	a4,a5,.L6
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	sub	a5,a4,a5
	sw	a5,-36(s0)
	lw	a4,-24(s0)
	lw	a5,-20(s0)
	or	a5,a4,a5
	sw	a5,-24(s0)
.L6:
	lw	a5,-20(s0)
	srli	a5,a5,1
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	srli	a5,a5,1
	sw	a5,-40(s0)
.L5:
	lw	a5,-20(s0)
	bne	a5,zero,.L7
	lw	a5,-44(s0)
	beq	a5,zero,.L8
	lw	a5,-36(s0)
	j	.L9
.L8:
	lw	a5,-24(s0)
.L9:
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	__udivmodsi4, .-__udivmodsi4
	.align	2
	.globl	__divsi3
	.type	__divsi3, @function
__divsi3:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	zero,-20(s0)
	lw	a5,-36(s0)
	bge	a5,zero,.L11
	lw	a5,-36(s0)
	neg	a5,a5
	sw	a5,-36(s0)
	lw	a5,-20(s0)
	seqz	a5,a5
	andi	a5,a5,0xff
	sw	a5,-20(s0)
.L11:
	lw	a5,-40(s0)
	bge	a5,zero,.L12
	lw	a5,-40(s0)
	neg	a5,a5
	sw	a5,-40(s0)
	lw	a5,-20(s0)
	seqz	a5,a5
	andi	a5,a5,0xff
	sw	a5,-20(s0)
.L12:
	lw	a5,-36(s0)
	lw	a4,-40(s0)
	li	a2,0
	mv	a1,a4
	mv	a0,a5
	call	__udivmodsi4
	mv	a5,a0
	sw	a5,-24(s0)
	lw	a5,-20(s0)
	beq	a5,zero,.L13
	lw	a5,-24(s0)
	neg	a5,a5
	sw	a5,-24(s0)
.L13:
	lw	a5,-24(s0)
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	__divsi3, .-__divsi3
	.align	2
	.globl	__mulsi3
	.type	__mulsi3, @function
__mulsi3:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	zero,-20(s0)
	j	.L16
.L18:
	lw	a5,-36(s0)
	andi	a5,a5,1
	beq	a5,zero,.L17
	lw	a4,-20(s0)
	lw	a5,-40(s0)
	add	a5,a4,a5
	sw	a5,-20(s0)
.L17:
	lw	a5,-36(s0)
	srli	a5,a5,1
	sw	a5,-36(s0)
	lw	a5,-40(s0)
	slli	a5,a5,1
	sw	a5,-40(s0)
.L16:
	lw	a5,-36(s0)
	bne	a5,zero,.L18
	lw	a5,-20(s0)
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	__mulsi3, .-__mulsi3
	.align	2
	.globl	strcpy
	.type	strcpy, @function
strcpy:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	lw	a5,-36(s0)
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	sw	a5,-24(s0)
	nop
.L21:
	lw	a4,-24(s0)
	addi	a5,a4,1
	sw	a5,-24(s0)
	lw	a5,-20(s0)
	addi	a3,a5,1
	sw	a3,-20(s0)
	lbu	a4,0(a4)
	sb	a4,0(a5)
	lbu	a5,0(a5)
	bne	a5,zero,.L21
	lw	a5,-36(s0)
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	strcpy, .-strcpy
	.align	2
	.globl	strcmp
	.type	strcmp, @function
strcmp:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	j	.L24
.L26:
	lw	a5,-20(s0)
	addi	a4,a5,1
	sw	a4,-20(s0)
	lbu	a5,0(a5)
	bne	a5,zero,.L24
	li	a5,0
	j	.L25
.L24:
	lw	a5,-20(s0)
	lbu	a4,0(a5)
	lw	a5,-24(s0)
	addi	a3,a5,1
	sw	a3,-24(s0)
	lbu	a5,0(a5)
	beq	a4,a5,.L26
	lw	a5,-20(s0)
	lbu	a5,0(a5)
	mv	a4,a5
	lw	a5,-24(s0)
	addi	a5,a5,-1
	sw	a5,-24(s0)
	lw	a5,-24(s0)
	lbu	a5,0(a5)
	sub	a5,a4,a5
.L25:
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	strcmp, .-strcmp
	.align	2
	.globl	printf
	.type	printf, @function
printf:
	addi	sp,sp,-64
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,4(s0)
	sw	a2,8(s0)
	sw	a3,12(s0)
	sw	a4,16(s0)
	sw	a5,20(s0)
	sw	a6,24(s0)
	sw	a7,28(s0)
	li	a5,1
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,64
	jr	ra
	.size	printf, .-printf
	.align	2
	.globl	time
	.type	time, @function
time:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,1
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	time, .-time
	.globl	Ptr_Glob_Buffer
	.bss
	.align	2
	.type	Ptr_Glob_Buffer, @object
	.size	Ptr_Glob_Buffer, 80
Ptr_Glob_Buffer:
	.zero	80
	.globl	Next_Ptr_Glob_Buffer
	.align	2
	.type	Next_Ptr_Glob_Buffer, @object
	.size	Next_Ptr_Glob_Buffer, 80
Next_Ptr_Glob_Buffer:
	.zero	80
	.globl	Ptr_Glob
	.section	.sbss,"aw",@nobits
	.align	2
	.type	Ptr_Glob, @object
	.size	Ptr_Glob, 4
Ptr_Glob:
	.zero	4
	.globl	Next_Ptr_Glob
	.align	2
	.type	Next_Ptr_Glob, @object
	.size	Next_Ptr_Glob, 4
Next_Ptr_Glob:
	.zero	4
	.globl	Int_Glob
	.align	2
	.type	Int_Glob, @object
	.size	Int_Glob, 4
Int_Glob:
	.zero	4
	.globl	Bool_Glob
	.align	2
	.type	Bool_Glob, @object
	.size	Bool_Glob, 4
Bool_Glob:
	.zero	4
	.globl	Ch_1_Glob
	.type	Ch_1_Glob, @object
	.size	Ch_1_Glob, 1
Ch_1_Glob:
	.zero	1
	.globl	Ch_2_Glob
	.type	Ch_2_Glob, @object
	.size	Ch_2_Glob, 1
Ch_2_Glob:
	.zero	1
	.globl	Arr_1_Glob
	.bss
	.align	2
	.type	Arr_1_Glob, @object
	.size	Arr_1_Glob, 200
Arr_1_Glob:
	.zero	200
	.globl	Arr_2_Glob
	.align	2
	.type	Arr_2_Glob, @object
	.size	Arr_2_Glob, 10000
Arr_2_Glob:
	.zero	10000
	.globl	Reg
	.section	.sbss
	.align	2
	.type	Reg, @object
	.size	Reg, 4
Reg:
	.zero	4
	.globl	Begin_Time
	.align	2
	.type	Begin_Time, @object
	.size	Begin_Time, 4
Begin_Time:
	.zero	4
	.globl	End_Time
	.align	2
	.type	End_Time, @object
	.size	End_Time, 4
End_Time:
	.zero	4
	.globl	User_Time
	.align	2
	.type	User_Time, @object
	.size	User_Time, 4
User_Time:
	.zero	4
	.section	.rodata
	.align	2
.LC0:
	.string	"DHRYSTONE PROGRAM, SOME STRING"
	.align	2
.LC1:
	.string	"DHRYSTONE PROGRAM, 1'ST STRING"
	.align	2
.LC2:
	.string	"\n"
	.align	2
.LC3:
	.string	"Dhrystone Benchmark, Version 2.1 (Language: C)\n"
	.align	2
.LC4:
	.string	"Program compiled with 'register' attribute\n"
	.align	2
.LC5:
	.string	"Program compiled without 'register' attribute\n"
	.align	2
.LC6:
	.string	"Execution starts, %d runs through Dhrystone\n"
	.align	2
.LC7:
	.string	"DHRYSTONE PROGRAM, 2'ND STRING"
	.align	2
.LC8:
	.string	"DHRYSTONE PROGRAM, 3'RD STRING"
	.globl	__mulsi3
	.globl	__divsi3
	.align	2
.LC9:
	.string	"Execution ends\n"
	.align	2
.LC10:
	.string	"Final values of the variables used in the benchmark:\n"
	.align	2
.LC11:
	.string	"Int_Glob:            %d\n"
	.align	2
.LC12:
	.string	"        should be:   %d\n"
	.align	2
.LC13:
	.string	"Bool_Glob:           %d\n"
	.align	2
.LC14:
	.string	"Ch_1_Glob:           %c\n"
	.align	2
.LC15:
	.string	"        should be:   %c\n"
	.align	2
.LC16:
	.string	"Ch_2_Glob:           %c\n"
	.align	2
.LC17:
	.string	"Arr_1_Glob[8]:       %d\n"
	.align	2
.LC18:
	.string	"Arr_2_Glob[8][7]:    %d\n"
	.align	2
.LC19:
	.string	"        should be:   Number_Of_Runs + 10\n"
	.align	2
.LC20:
	.string	"Ptr_Glob->\n"
	.align	2
.LC21:
	.string	"  Ptr_Comp:          %d\n"
	.align	2
.LC22:
	.string	"        should be:   (implementation-dependent)\n"
	.align	2
.LC23:
	.string	"  Discr:             %d\n"
	.align	2
.LC24:
	.string	"  Enum_Comp:         %d\n"
	.align	2
.LC25:
	.string	"  Int_Comp:          %d\n"
	.align	2
.LC26:
	.string	"  Str_Comp:          %s\n"
	.align	2
.LC27:
	.string	"        should be:   DHRYSTONE PROGRAM, SOME STRING\n"
	.align	2
.LC28:
	.string	"Next_Ptr_Glob->\n"
	.align	2
.LC29:
	.string	"        should be:   (implementation-dependent), same as above\n"
	.align	2
.LC30:
	.string	"Int_1_Loc:           %d\n"
	.align	2
.LC31:
	.string	"Int_2_Loc:           %d\n"
	.align	2
.LC32:
	.string	"Int_3_Loc:           %d\n"
	.align	2
.LC33:
	.string	"Enum_Loc:            %d\n"
	.align	2
.LC34:
	.string	"Str_1_Loc:           %s\n"
	.align	2
.LC35:
	.string	"        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n"
	.align	2
.LC36:
	.string	"Str_2_Loc:           %s\n"
	.align	2
.LC37:
	.string	"        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n"
	.align	2
.LC38:
	.string	"Measured time too small to obtain meaningful results\n"
	.align	2
.LC39:
	.string	"Please increase number of runs\n"
	.align	2
.LC40:
	.string	"Microseconds for one run through Dhrystone: "
	.align	2
.LC41:
	.string	"Dhrystones per Second:                      "
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-112
	sw	ra,108(sp)
	sw	s0,104(sp)
	addi	s0,sp,112
	lui	a5,%hi(Next_Ptr_Glob)
	lui	a4,%hi(Next_Ptr_Glob_Buffer)
	addi	a4,a4,%lo(Next_Ptr_Glob_Buffer)
	sw	a4,%lo(Next_Ptr_Glob)(a5)
	lui	a5,%hi(Ptr_Glob)
	lui	a4,%hi(Ptr_Glob_Buffer)
	addi	a4,a4,%lo(Ptr_Glob_Buffer)
	sw	a4,%lo(Ptr_Glob)(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lui	a4,%hi(Next_Ptr_Glob)
	lw	a4,%lo(Next_Ptr_Glob)(a4)
	sw	a4,0(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	sw	zero,4(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	li	a4,2
	sw	a4,8(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	li	a4,40
	sw	a4,12(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	addi	a4,a5,16
	lui	a5,%hi(.LC0)
	addi	a1,a5,%lo(.LC0)
	mv	a0,a4
	call	strcpy
	addi	a4,s0,-76
	lui	a5,%hi(.LC1)
	addi	a1,a5,%lo(.LC1)
	mv	a0,a4
	call	strcpy
	lui	a5,%hi(Arr_2_Glob)
	addi	a5,a5,%lo(Arr_2_Glob)
	li	a4,10
	sw	a4,1628(a5)
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(Reg)
	lw	a5,%lo(Reg)(a5)
	beq	a5,zero,.L32
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	j	.L33
.L32:
	lui	a5,%hi(.LC5)
	addi	a0,a5,%lo(.LC5)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
.L33:
	li	a5,2000
	sw	a5,-32(s0)
	lw	a1,-32(s0)
	lui	a5,%hi(.LC6)
	addi	a0,a5,%lo(.LC6)
	call	printf
	li	a0,0
	call	time
	mv	a4,a0
	lui	a5,%hi(Begin_Time)
	sw	a4,%lo(Begin_Time)(a5)
	li	a5,1
	sw	a5,-28(s0)
	j	.L34
.L40:
	call	Proc_5
	call	Proc_4
	li	a5,2
	sw	a5,-36(s0)
	li	a5,3
	sw	a5,-20(s0)
	addi	a4,s0,-108
	lui	a5,%hi(.LC7)
	addi	a1,a5,%lo(.LC7)
	mv	a0,a4
	call	strcpy
	li	a5,1
	sw	a5,-44(s0)
	addi	a4,s0,-108
	addi	a5,s0,-76
	mv	a1,a4
	mv	a0,a5
	call	Func_2
	mv	a5,a0
	seqz	a5,a5
	andi	a5,a5,0xff
	mv	a4,a5
	lui	a5,%hi(Bool_Glob)
	sw	a4,%lo(Bool_Glob)(a5)
	j	.L35
.L36:
	lw	a4,-36(s0)
	mv	a5,a4
	slli	a5,a5,2
	add	a4,a5,a4
	lw	a5,-20(s0)
	sub	a5,a4,a5
	sw	a5,-40(s0)
	lw	a5,-36(s0)
	addi	a4,s0,-40
	mv	a2,a4
	lw	a1,-20(s0)
	mv	a0,a5
	call	Proc_7
	lw	a5,-36(s0)
	addi	a5,a5,1
	sw	a5,-36(s0)
.L35:
	lw	a5,-36(s0)
	lw	a4,-20(s0)
	bgt	a4,a5,.L36
	lw	a5,-36(s0)
	lw	a4,-40(s0)
	mv	a3,a4
	mv	a2,a5
	lui	a5,%hi(Arr_2_Glob)
	addi	a1,a5,%lo(Arr_2_Glob)
	lui	a5,%hi(Arr_1_Glob)
	addi	a0,a5,%lo(Arr_1_Glob)
	call	Proc_8
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	mv	a0,a5
	call	Proc_1
	li	a5,65
	sb	a5,-21(s0)
	j	.L37
.L39:
	lbu	a5,-21(s0)
	li	a1,67
	mv	a0,a5
	call	Func_1
	mv	a4,a0
	lw	a5,-44(s0)
	bne	a4,a5,.L38
	addi	a5,s0,-44
	mv	a1,a5
	li	a0,0
	call	Proc_6
	addi	a4,s0,-108
	lui	a5,%hi(.LC8)
	addi	a1,a5,%lo(.LC8)
	mv	a0,a4
	call	strcpy
	lw	a5,-28(s0)
	sw	a5,-20(s0)
	lui	a5,%hi(Int_Glob)
	lw	a4,-28(s0)
	sw	a4,%lo(Int_Glob)(a5)
.L38:
	lbu	a5,-21(s0)
	addi	a5,a5,1
	sb	a5,-21(s0)
.L37:
	lui	a5,%hi(Ch_2_Glob)
	lbu	a5,%lo(Ch_2_Glob)(a5)
	lbu	a4,-21(s0)
	bleu	a4,a5,.L39
	lw	a5,-36(s0)
	mv	a1,a5
	lw	a0,-20(s0)
	call	__mulsi3
	mv	a5,a0
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	mv	a1,a5
	lw	a0,-20(s0)
	call	__divsi3
	mv	a5,a0
	sw	a5,-36(s0)
	lw	a5,-40(s0)
	lw	a4,-20(s0)
	sub	a4,a4,a5
	mv	a5,a4
	slli	a5,a5,3
	sub	a4,a5,a4
	lw	a5,-36(s0)
	sub	a5,a4,a5
	sw	a5,-20(s0)
	addi	a5,s0,-36
	mv	a0,a5
	call	Proc_2
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
.L34:
	lw	a4,-28(s0)
	lw	a5,-32(s0)
	ble	a4,a5,.L40
	li	a0,0
	call	time
	mv	a4,a0
	lui	a5,%hi(End_Time)
	sw	a4,%lo(End_Time)(a5)
	lui	a5,%hi(.LC9)
	addi	a0,a5,%lo(.LC9)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(.LC10)
	addi	a0,a5,%lo(.LC10)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(Int_Glob)
	lw	a5,%lo(Int_Glob)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC11)
	addi	a0,a5,%lo(.LC11)
	call	printf
	li	a1,5
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Bool_Glob)
	lw	a5,%lo(Bool_Glob)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC13)
	addi	a0,a5,%lo(.LC13)
	call	printf
	li	a1,1
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Ch_1_Glob)
	lbu	a5,%lo(Ch_1_Glob)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC14)
	addi	a0,a5,%lo(.LC14)
	call	printf
	li	a1,65
	lui	a5,%hi(.LC15)
	addi	a0,a5,%lo(.LC15)
	call	printf
	lui	a5,%hi(Ch_2_Glob)
	lbu	a5,%lo(Ch_2_Glob)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC16)
	addi	a0,a5,%lo(.LC16)
	call	printf
	li	a1,66
	lui	a5,%hi(.LC15)
	addi	a0,a5,%lo(.LC15)
	call	printf
	lui	a5,%hi(Arr_1_Glob)
	addi	a5,a5,%lo(Arr_1_Glob)
	lw	a5,32(a5)
	mv	a1,a5
	lui	a5,%hi(.LC17)
	addi	a0,a5,%lo(.LC17)
	call	printf
	li	a1,7
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Arr_2_Glob)
	addi	a5,a5,%lo(Arr_2_Glob)
	lw	a5,1628(a5)
	mv	a1,a5
	lui	a5,%hi(.LC18)
	addi	a0,a5,%lo(.LC18)
	call	printf
	lui	a5,%hi(.LC19)
	addi	a0,a5,%lo(.LC19)
	call	printf
	lui	a5,%hi(.LC20)
	addi	a0,a5,%lo(.LC20)
	call	printf
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a5,0(a5)
	mv	a1,a5
	lui	a5,%hi(.LC21)
	addi	a0,a5,%lo(.LC21)
	call	printf
	lui	a5,%hi(.LC22)
	addi	a0,a5,%lo(.LC22)
	call	printf
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a5,4(a5)
	mv	a1,a5
	lui	a5,%hi(.LC23)
	addi	a0,a5,%lo(.LC23)
	call	printf
	li	a1,0
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a5,8(a5)
	mv	a1,a5
	lui	a5,%hi(.LC24)
	addi	a0,a5,%lo(.LC24)
	call	printf
	li	a1,2
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a5,12(a5)
	mv	a1,a5
	lui	a5,%hi(.LC25)
	addi	a0,a5,%lo(.LC25)
	call	printf
	li	a1,17
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	addi	a5,a5,16
	mv	a1,a5
	lui	a5,%hi(.LC26)
	addi	a0,a5,%lo(.LC26)
	call	printf
	lui	a5,%hi(.LC27)
	addi	a0,a5,%lo(.LC27)
	call	printf
	lui	a5,%hi(.LC28)
	addi	a0,a5,%lo(.LC28)
	call	printf
	lui	a5,%hi(Next_Ptr_Glob)
	lw	a5,%lo(Next_Ptr_Glob)(a5)
	lw	a5,0(a5)
	mv	a1,a5
	lui	a5,%hi(.LC21)
	addi	a0,a5,%lo(.LC21)
	call	printf
	lui	a5,%hi(.LC29)
	addi	a0,a5,%lo(.LC29)
	call	printf
	lui	a5,%hi(Next_Ptr_Glob)
	lw	a5,%lo(Next_Ptr_Glob)(a5)
	lw	a5,4(a5)
	mv	a1,a5
	lui	a5,%hi(.LC23)
	addi	a0,a5,%lo(.LC23)
	call	printf
	li	a1,0
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Next_Ptr_Glob)
	lw	a5,%lo(Next_Ptr_Glob)(a5)
	lw	a5,8(a5)
	mv	a1,a5
	lui	a5,%hi(.LC24)
	addi	a0,a5,%lo(.LC24)
	call	printf
	li	a1,1
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Next_Ptr_Glob)
	lw	a5,%lo(Next_Ptr_Glob)(a5)
	lw	a5,12(a5)
	mv	a1,a5
	lui	a5,%hi(.LC25)
	addi	a0,a5,%lo(.LC25)
	call	printf
	li	a1,18
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lui	a5,%hi(Next_Ptr_Glob)
	lw	a5,%lo(Next_Ptr_Glob)(a5)
	addi	a5,a5,16
	mv	a1,a5
	lui	a5,%hi(.LC26)
	addi	a0,a5,%lo(.LC26)
	call	printf
	lui	a5,%hi(.LC27)
	addi	a0,a5,%lo(.LC27)
	call	printf
	lw	a5,-36(s0)
	mv	a1,a5
	lui	a5,%hi(.LC30)
	addi	a0,a5,%lo(.LC30)
	call	printf
	li	a1,5
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lw	a1,-20(s0)
	lui	a5,%hi(.LC31)
	addi	a0,a5,%lo(.LC31)
	call	printf
	li	a1,13
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lw	a5,-40(s0)
	mv	a1,a5
	lui	a5,%hi(.LC32)
	addi	a0,a5,%lo(.LC32)
	call	printf
	li	a1,7
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	lw	a5,-44(s0)
	mv	a1,a5
	lui	a5,%hi(.LC33)
	addi	a0,a5,%lo(.LC33)
	call	printf
	li	a1,1
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	printf
	addi	a5,s0,-76
	mv	a1,a5
	lui	a5,%hi(.LC34)
	addi	a0,a5,%lo(.LC34)
	call	printf
	lui	a5,%hi(.LC35)
	addi	a0,a5,%lo(.LC35)
	call	printf
	addi	a5,s0,-108
	mv	a1,a5
	lui	a5,%hi(.LC36)
	addi	a0,a5,%lo(.LC36)
	call	printf
	lui	a5,%hi(.LC37)
	addi	a0,a5,%lo(.LC37)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(End_Time)
	lw	a4,%lo(End_Time)(a5)
	lui	a5,%hi(Begin_Time)
	lw	a5,%lo(Begin_Time)(a5)
	sub	a4,a4,a5
	lui	a5,%hi(User_Time)
	sw	a4,%lo(User_Time)(a5)
	lui	a5,%hi(User_Time)
	lw	a4,%lo(User_Time)(a5)
	li	a5,1
	bgt	a4,a5,.L41
	lui	a5,%hi(.LC38)
	addi	a0,a5,%lo(.LC38)
	call	printf
	lui	a5,%hi(.LC39)
	addi	a0,a5,%lo(.LC39)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	j	.L42
.L41:
	lui	a5,%hi(.LC40)
	addi	a0,a5,%lo(.LC40)
	call	printf
	lui	a5,%hi(.LC41)
	addi	a0,a5,%lo(.LC41)
	call	printf
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
.L42:
	li	a5,0
	mv	a0,a5
	lw	ra,108(sp)
	lw	s0,104(sp)
	addi	sp,sp,112
	jr	ra
	.size	main, .-main
	.align	2
	.globl	Proc_1
	.type	Proc_1, @function
Proc_1:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	lw	a5,-36(s0)
	lw	a5,0(a5)
	sw	a5,-20(s0)
	lui	a5,%hi(Ptr_Glob)
	lw	a4,%lo(Ptr_Glob)(a5)
	lw	a5,-36(s0)
	lw	a5,0(a5)
	lw	t6,0(a4)
	lw	t5,4(a4)
	lw	t4,8(a4)
	lw	t3,12(a4)
	lw	t1,16(a4)
	lw	a7,20(a4)
	lw	a6,24(a4)
	lw	a0,28(a4)
	lw	a1,32(a4)
	lw	a2,36(a4)
	lw	a3,40(a4)
	lw	a4,44(a4)
	sw	t6,0(a5)
	sw	t5,4(a5)
	sw	t4,8(a5)
	sw	t3,12(a5)
	sw	t1,16(a5)
	sw	a7,20(a5)
	sw	a6,24(a5)
	sw	a0,28(a5)
	sw	a1,32(a5)
	sw	a2,36(a5)
	sw	a3,40(a5)
	sw	a4,44(a5)
	lw	a5,-36(s0)
	li	a4,5
	sw	a4,12(a5)
	lw	a5,-36(s0)
	lw	a4,12(a5)
	lw	a5,-20(s0)
	sw	a4,12(a5)
	lw	a5,-36(s0)
	lw	a4,0(a5)
	lw	a5,-20(s0)
	sw	a4,0(a5)
	lw	a5,-20(s0)
	mv	a0,a5
	call	Proc_3
	lw	a5,-20(s0)
	lw	a5,4(a5)
	bne	a5,zero,.L45
	lw	a5,-20(s0)
	li	a4,6
	sw	a4,12(a5)
	lw	a5,-36(s0)
	lw	a4,8(a5)
	lw	a5,-20(s0)
	addi	a5,a5,8
	mv	a1,a5
	mv	a0,a4
	call	Proc_6
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a4,0(a5)
	lw	a5,-20(s0)
	sw	a4,0(a5)
	lw	a5,-20(s0)
	lw	a4,12(a5)
	lw	a5,-20(s0)
	addi	a5,a5,12
	mv	a2,a5
	li	a1,10
	mv	a0,a4
	call	Proc_7
	j	.L47
.L45:
	lw	a5,-36(s0)
	lw	a4,0(a5)
	lw	a5,-36(s0)
	lw	t6,0(a4)
	lw	t5,4(a4)
	lw	t4,8(a4)
	lw	t3,12(a4)
	lw	t1,16(a4)
	lw	a7,20(a4)
	lw	a6,24(a4)
	lw	a0,28(a4)
	lw	a1,32(a4)
	lw	a2,36(a4)
	lw	a3,40(a4)
	lw	a4,44(a4)
	sw	t6,0(a5)
	sw	t5,4(a5)
	sw	t4,8(a5)
	sw	t3,12(a5)
	sw	t1,16(a5)
	sw	a7,20(a5)
	sw	a6,24(a5)
	sw	a0,28(a5)
	sw	a1,32(a5)
	sw	a2,36(a5)
	sw	a3,40(a5)
	sw	a4,44(a5)
.L47:
	nop
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	Proc_1, .-Proc_1
	.align	2
	.globl	Proc_2
	.type	Proc_2, @function
Proc_2:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	lw	a5,-36(s0)
	lw	a5,0(a5)
	addi	a5,a5,10
	sw	a5,-20(s0)
.L50:
	lui	a5,%hi(Ch_1_Glob)
	lbu	a4,%lo(Ch_1_Glob)(a5)
	li	a5,65
	bne	a4,a5,.L49
	lw	a5,-20(s0)
	addi	a5,a5,-1
	sw	a5,-20(s0)
	lui	a5,%hi(Int_Glob)
	lw	a5,%lo(Int_Glob)(a5)
	lw	a4,-20(s0)
	sub	a4,a4,a5
	lw	a5,-36(s0)
	sw	a4,0(a5)
	sw	zero,-24(s0)
.L49:
	lw	a5,-24(s0)
	bne	a5,zero,.L50
	nop
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	Proc_2, .-Proc_2
	.align	2
	.globl	Proc_3
	.type	Proc_3, @function
Proc_3:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	beq	a5,zero,.L52
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	lw	a4,0(a5)
	lw	a5,-20(s0)
	sw	a4,0(a5)
.L52:
	lui	a5,%hi(Int_Glob)
	lw	a4,%lo(Int_Glob)(a5)
	lui	a5,%hi(Ptr_Glob)
	lw	a5,%lo(Ptr_Glob)(a5)
	addi	a5,a5,12
	mv	a2,a5
	mv	a1,a4
	li	a0,10
	call	Proc_7
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	Proc_3, .-Proc_3
	.align	2
	.globl	Proc_4
	.type	Proc_4, @function
Proc_4:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	lui	a5,%hi(Ch_1_Glob)
	lbu	a5,%lo(Ch_1_Glob)(a5)
	addi	a5,a5,-65
	seqz	a5,a5
	andi	a5,a5,0xff
	sw	a5,-20(s0)
	lui	a5,%hi(Bool_Glob)
	lw	a4,%lo(Bool_Glob)(a5)
	lw	a5,-20(s0)
	or	a4,a4,a5
	lui	a5,%hi(Bool_Glob)
	sw	a4,%lo(Bool_Glob)(a5)
	lui	a5,%hi(Ch_2_Glob)
	li	a4,66
	sb	a4,%lo(Ch_2_Glob)(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	Proc_4, .-Proc_4
	.align	2
	.globl	Proc_5
	.type	Proc_5, @function
Proc_5:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	lui	a5,%hi(Ch_1_Glob)
	li	a4,65
	sb	a4,%lo(Ch_1_Glob)(a5)
	lui	a5,%hi(Bool_Glob)
	sw	zero,%lo(Bool_Glob)(a5)
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	Proc_5, .-Proc_5
	.align	2
	.globl	Proc_6
	.type	Proc_6, @function
Proc_6:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	lw	a5,-24(s0)
	lw	a4,-20(s0)
	sw	a4,0(a5)
	lw	a0,-20(s0)
	call	Func_3
	mv	a5,a0
	bne	a5,zero,.L56
	lw	a5,-24(s0)
	li	a4,3
	sw	a4,0(a5)
.L56:
	lw	a4,-20(s0)
	li	a5,4
	bgtu	a4,a5,.L66
	lw	a5,-20(s0)
	slli	a4,a5,2
	lui	a5,%hi(.L59)
	addi	a5,a5,%lo(.L59)
	add	a5,a4,a5
	lw	a5,0(a5)
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L59:
	.word	.L63
	.word	.L62
	.word	.L61
	.word	.L67
	.word	.L58
	.text
.L63:
	lw	a5,-24(s0)
	sw	zero,0(a5)
	j	.L57
.L62:
	lui	a5,%hi(Int_Glob)
	lw	a4,%lo(Int_Glob)(a5)
	li	a5,100
	ble	a4,a5,.L64
	lw	a5,-24(s0)
	sw	zero,0(a5)
	j	.L57
.L64:
	lw	a5,-24(s0)
	li	a4,3
	sw	a4,0(a5)
	j	.L57
.L61:
	lw	a5,-24(s0)
	li	a4,1
	sw	a4,0(a5)
	j	.L57
.L58:
	lw	a5,-24(s0)
	li	a4,2
	sw	a4,0(a5)
	j	.L57
.L67:
	nop
.L57:
.L66:
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	Proc_6, .-Proc_6
	.align	2
	.globl	Proc_7
	.type	Proc_7, @function
Proc_7:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	lw	a5,-36(s0)
	addi	a5,a5,2
	sw	a5,-20(s0)
	lw	a4,-40(s0)
	lw	a5,-20(s0)
	add	a4,a4,a5
	lw	a5,-44(s0)
	sw	a4,0(a5)
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	Proc_7, .-Proc_7
	.align	2
	.globl	Proc_8
	.type	Proc_8, @function
Proc_8:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	sw	a3,-48(s0)
	lw	a5,-44(s0)
	addi	a5,a5,5
	sw	a5,-24(s0)
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,-48(s0)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a4,a4,a5
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a4,0(a4)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	addi	a5,a5,30
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,-24(s0)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	sw	a5,-20(s0)
	j	.L70
.L71:
	lw	a4,-24(s0)
	mv	a5,a4
	slli	a5,a5,1
	add	a5,a5,a4
	slli	a5,a5,3
	add	a5,a5,a4
	slli	a5,a5,3
	mv	a4,a5
	lw	a5,-40(s0)
	add	a4,a5,a4
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,-24(s0)
	sw	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L70:
	lw	a5,-24(s0)
	addi	a5,a5,1
	lw	a4,-20(s0)
	ble	a4,a5,.L71
	lw	a4,-24(s0)
	mv	a5,a4
	slli	a5,a5,1
	add	a5,a5,a4
	slli	a5,a5,3
	add	a5,a5,a4
	slli	a5,a5,3
	mv	a4,a5
	lw	a5,-40(s0)
	add	a4,a5,a4
	lw	a5,-24(s0)
	addi	a5,a5,-1
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a2,0(a5)
	lw	a4,-24(s0)
	mv	a5,a4
	slli	a5,a5,1
	add	a5,a5,a4
	slli	a5,a5,3
	add	a5,a5,a4
	slli	a5,a5,3
	mv	a4,a5
	lw	a5,-40(s0)
	add	a3,a5,a4
	lw	a5,-24(s0)
	addi	a5,a5,-1
	addi	a4,a2,1
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a2,a4,a5
	lw	a4,-24(s0)
	mv	a5,a4
	slli	a5,a5,1
	add	a5,a5,a4
	slli	a5,a5,3
	add	a5,a5,a4
	slli	a5,a5,3
	mv	a4,a5
	li	a5,4096
	addi	a5,a5,-96
	add	a5,a4,a5
	lw	a4,-40(s0)
	add	a3,a4,a5
	lw	a4,0(a2)
	lw	a5,-24(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	lui	a5,%hi(Int_Glob)
	li	a4,5
	sw	a4,%lo(Int_Glob)(a5)
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	Proc_8, .-Proc_8
	.align	2
	.globl	Func_1
	.type	Func_1, @function
Func_1:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	mv	a5,a0
	mv	a4,a1
	sb	a5,-36(s0)
	mv	a5,a4
	sb	a5,-40(s0)
	lbu	a5,-36(s0)
	sb	a5,-17(s0)
	lbu	a5,-17(s0)
	sb	a5,-18(s0)
	lbu	a4,-18(s0)
	lbu	a5,-40(s0)
	beq	a4,a5,.L73
	li	a5,0
	j	.L74
.L73:
	lui	a5,%hi(Ch_1_Glob)
	lbu	a4,-17(s0)
	sb	a4,%lo(Ch_1_Glob)(a5)
	li	a5,1
.L74:
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	Func_1, .-Func_1
	.align	2
	.globl	Func_2
	.type	Func_2, @function
Func_2:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	li	a5,2
	sw	a5,-20(s0)
	j	.L76
.L77:
	lw	a5,-20(s0)
	lw	a4,-36(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	mv	a3,a5
	lw	a5,-20(s0)
	addi	a5,a5,1
	lw	a4,-40(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	mv	a1,a5
	mv	a0,a3
	call	Func_1
	mv	a5,a0
	bne	a5,zero,.L76
	li	a5,65
	sb	a5,-21(s0)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L76:
	lw	a4,-20(s0)
	li	a5,2
	ble	a4,a5,.L77
	lbu	a4,-21(s0)
	li	a5,86
	bleu	a4,a5,.L78
	lbu	a4,-21(s0)
	li	a5,89
	bgtu	a4,a5,.L78
	li	a5,7
	sw	a5,-20(s0)
.L78:
	lbu	a4,-21(s0)
	li	a5,82
	bne	a4,a5,.L79
	li	a5,1
	j	.L80
.L79:
	lw	a1,-40(s0)
	lw	a0,-36(s0)
	call	strcmp
	mv	a5,a0
	ble	a5,zero,.L81
	lw	a5,-20(s0)
	addi	a5,a5,7
	sw	a5,-20(s0)
	lui	a5,%hi(Int_Glob)
	lw	a4,-20(s0)
	sw	a4,%lo(Int_Glob)(a5)
	li	a5,1
	j	.L80
.L81:
	li	a5,0
.L80:
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	Func_2, .-Func_2
	.align	2
	.globl	Func_3
	.type	Func_3, @function
Func_3:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	lw	a5,-36(s0)
	sw	a5,-20(s0)
	lw	a4,-20(s0)
	li	a5,2
	bne	a4,a5,.L83
	li	a5,1
	j	.L84
.L83:
	li	a5,0
.L84:
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	Func_3, .-Func_3
	.ident	"GCC: () 13.2.0"
