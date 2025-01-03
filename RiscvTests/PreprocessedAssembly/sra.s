#*****************************************************************************
# sra.S
#-----------------------------------------------------------------------------

# Test sra instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Arithmetic tests
  #-------------------------------------------------------------

  test_2:
 li gp, 2
 li x1, 0x80000000
 li x2, 0
 sra x14, x1, x2
 li x7, 0x80000000
 bne x14, x7, fail


  test_3:
 li gp, 3
 li x1, 0x80000000
 li x2, 1
 sra x14, x1, x2
 li x7, 0xc0000000
 bne x14, x7, fail


  test_4:
 li gp, 4
 li x1, 0x80000000
 li x2, 7
 sra x14, x1, x2
 li x7, 0xff000000
 bne x14, x7, fail


  test_5:
 li gp, 5
 li x1, 0x80000000
 li x2, 14
 sra x14, x1, x2
 li x7, 0xfffe0000
 bne x14, x7, fail


  test_6:
 li gp, 6
 li x1, 0x80000001
 li x2, 31
 sra x14, x1, x2
 li x7, 0xffffffff
 bne x14, x7, fail



  test_7:
 li gp, 7
 li x1, 0x7fffffff
 li x2, 0
 sra x14, x1, x2
 li x7, 0x7fffffff
 bne x14, x7, fail


  test_8:
 li gp, 8
 li x1, 0x7fffffff
 li x2, 1
 sra x14, x1, x2
 li x7, 0x3fffffff
 bne x14, x7, fail


  test_9:
 li gp, 9
 li x1, 0x7fffffff
 li x2, 7
 sra x14, x1, x2
 li x7, 0x00ffffff
 bne x14, x7, fail


  test_10:
 li gp, 10
 li x1, 0x7fffffff
 li x2, 14
 sra x14, x1, x2
 li x7, 0x0001ffff
 bne x14, x7, fail


  test_11:
 li gp, 11
 li x1, 0x7fffffff
 li x2, 31
 sra x14, x1, x2
 li x7, 0x00000000
 bne x14, x7, fail



  test_12:
 li gp, 12
 li x1, 0x81818181
 li x2, 0
 sra x14, x1, x2
 li x7, 0x81818181
 bne x14, x7, fail


  test_13:
 li gp, 13
 li x1, 0x81818181
 li x2, 1
 sra x14, x1, x2
 li x7, 0xc0c0c0c0
 bne x14, x7, fail


  test_14:
 li gp, 14
 li x1, 0x81818181
 li x2, 7
 sra x14, x1, x2
 li x7, 0xff030303
 bne x14, x7, fail


  test_15:
 li gp, 15
 li x1, 0x81818181
 li x2, 14
 sra x14, x1, x2
 li x7, 0xfffe0606
 bne x14, x7, fail


  test_16:
 li gp, 16
 li x1, 0x81818181
 li x2, 31
 sra x14, x1, x2
 li x7, 0xffffffff
 bne x14, x7, fail



  # Verify that shifts only use bottom six(rv64) or five(rv32) bits

  test_17:
 li gp, 17
 li x1, 0x81818181
 li x2, 0xffffffc0
 sra x14, x1, x2
 li x7, 0x81818181
 bne x14, x7, fail


  test_18:
 li gp, 18
 li x1, 0x81818181
 li x2, 0xffffffc1
 sra x14, x1, x2
 li x7, 0xc0c0c0c0
 bne x14, x7, fail


  test_19:
 li gp, 19
 li x1, 0x81818181
 li x2, 0xffffffc7
 sra x14, x1, x2
 li x7, 0xff030303
 bne x14, x7, fail


  test_20:
 li gp, 20
 li x1, 0x81818181
 li x2, 0xffffffce
 sra x14, x1, x2
 li x7, 0xfffe0606
 bne x14, x7, fail


  test_21:
 li gp, 21
 li x1, 0x81818181
 li x2, 0xffffffff
 sra x14, x1, x2
 li x7, 0xffffffff
 bne x14, x7, fail
 
 
 
  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  test_22:
 li gp, 22
 li x1, 0x80000000
 li x2, 7
 sra x1, x1, x2
 li x7, 0xff000000
 bne x1, x7, fail


  test_23:
 li gp, 23
 li x1, 0x80000000
 li x2, 14
 sra x2, x1, x2
 li x7, 0xfffe0000
 bne x2, x7, fail


  test_24:
 li gp, 24
 li x1, 7
 sra x1, x1, x1
 li x7, 0
 bne x1, x7, fail



  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_25:
 li gp, 25
 li x4, 0
 func1_25:
 li x1, 0x80000000
 li x2, 7
 sra x14, x1, x2
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_25
 li x7, 0xff000000
 bne x6, x7, fail


  test_26:
 li gp, 26
 li x4, 0
 func1_26:
 li x1, 0x80000000
 li x2, 14
 sra x14, x1, x2
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_26
 li x7, 0xfffe0000
 bne x6, x7, fail


  test_27:
 li gp, 27
 li x4, 0
 func1_27:
 li x1, 0x80000000
 li x2, 31
 sra x14, x1, x2
 nop
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_27
 li x7, 0xffffffff
 bne x6, x7, fail



  test_28:
 li gp, 28
 li x4, 0
 func1_28:
 li x1, 0x80000000
 li x2, 7
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_28
 li x7, 0xff000000
 bne x14, x7, fail


  test_29:
 li gp, 29
 li x4, 0
 func1_29:
 li x1, 0x80000000
 li x2, 14
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_29
 li x7, 0xfffe0000
 bne x14, x7, fail


  test_30:
 li gp, 30
 li x4, 0
 func1_30:
 li x1, 0x80000000
 li x2, 31
 nop
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_30
 li x7, 0xffffffff
 bne x14, x7, fail


  test_31:
 li gp, 31
 li x4, 0
 func1_31:
 li x1, 0x80000000
 nop
 li x2, 7
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_31
 li x7, 0xff000000
 bne x14, x7, fail


  test_32:
 li gp, 32
 li x4, 0
 func1_32:
 li x1, 0x80000000
 nop
 li x2, 14
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_32
 li x7, 0xfffe0000
 bne x14, x7, fail


  test_33:
 li gp, 33
 li x4, 0
 func1_33:
 li x1, 0x80000000
 nop
 nop
 li x2, 31
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_33
 li x7, 0xffffffff
 bne x14, x7, fail



  test_34:
 li gp, 34
 li x4, 0
 func1_34:
 li x2, 7
 li x1, 0x80000000
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_34
 li x7, 0xff000000
 bne x14, x7, fail


  test_35:
 li gp, 35
 li x4, 0
 func1_35:
 li x2, 14
 li x1, 0x80000000
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_35
 li x7, 0xfffe0000
 bne x14, x7, fail


  test_36:
 li gp, 36
 li x4, 0
 func1_36:
 li x2, 31
 li x1, 0x80000000
 nop
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_36
 li x7, 0xffffffff
 bne x14, x7, fail


  test_37:
 li gp, 37
 li x4, 0
 func1_37:
 li x2, 7
 nop
 li x1, 0x80000000
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_37
 li x7, 0xff000000
 bne x14, x7, fail


  test_38:
 li gp, 38
 li x4, 0
 func1_38:
 li x2, 14
 nop
 li x1, 0x80000000
 nop
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_38
 li x7, 0xfffe0000
 bne x14, x7, fail


  test_39:
 li gp, 39
 li x4, 0
 func1_39:
 li x2, 31
 nop
 nop
 li x1, 0x80000000
 sra x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_39
 li x7, 0xffffffff
 bne x14, x7, fail



  test_40:
 li gp, 40
 li x1, 15
 sra x2, x0, x1
 li x7, 0
 bne x2, x7, fail


  test_41:
 li gp, 41
 li x1, 32
 sra x2, x1, x0
 li x7, 32
 bne x2, x7, fail


  test_42:
 li gp, 42
 sra x1, x0, x0
 li x7, 0
 bne x1, x7, fail


  test_43:
 li gp, 43
 li x1, 1024
 li x2, 2048
 sra x0, x1, x2
 li x7, 0
 bne x0, x7, fail




  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 


