#*****************************************************************************
# sb.S
#-----------------------------------------------------------------------------

# Test sb instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Basic tests
  #-------------------------------------------------------------

  test_2:
 li gp, 2
 la x1, tdat
 li x2, 0xffffffaa
 la x15, func7_2
 sb x2, 0(x1)
 lb x14, 0(x1)
 j func8_2
 func7_2:
 mv x14, x2
 func8_2:
 li x7, 0xffffffaa
 bne x14, x7, fail


  test_3:
 li gp, 3
 la x1, tdat
 li x2, 0x00000000
 la x15, func7_3
 sb x2, 1(x1)
 lb x14, 1(x1)
 j func8_3
 func7_3:
 mv x14, x2
 func8_3:
 li x7, 0x00000000
 bne x14, x7, fail


  test_4:
 li gp, 4
 la x1, tdat
 li x2, 0xffffefa0
 la x15, func7_4
 sb x2, 2(x1)
 lh x14, 2(x1)
 j func8_4
 func7_4:
 mv x14, x2
 func8_4:
 li x7, 0xffffefa0
 bne x14, x7, fail


  test_5:
 li gp, 5
 la x1, tdat
 li x2, 0x0000000a
 la x15, func7_5
 sb x2, 3(x1)
 lb x14, 3(x1)
 j func8_5
 func7_5:
 mv x14, x2
 func8_5:
 li x7, 0x0000000a
 bne x14, x7, fail



  # Test with negative offset

  test_6:
 li gp, 6
 la x1, tdat8
 li x2, 0xffffffaa
 la x15, func7_6
 sb x2, -3(x1)
 lb x14, -3(x1)
 j func8_6
 func7_6:
 mv x14, x2
 func8_6:
 li x7, 0xffffffaa
 bne x14, x7, fail


  test_7:
 li gp, 7
 la x1, tdat8
 li x2, 0x00000000
 la x15, func7_7
 sb x2, -2(x1)
 lb x14, -2(x1)
 j func8_7
 func7_7:
 mv x14, x2
 func8_7:
 li x7, 0x00000000
 bne x14, x7, fail


  test_8:
 li gp, 8
 la x1, tdat8
 li x2, 0xffffffa0
 la x15, func7_8
 sb x2, -1(x1)
 lb x14, -1(x1)
 j func8_8
 func7_8:
 mv x14, x2
 func8_8:
 li x7, 0xffffffa0
 bne x14, x7, fail


  test_9:
 li gp, 9
 la x1, tdat8
 li x2, 0x0000000a
 la x15, func7_9
 sb x2, 0(x1)
 lb x14, 0(x1)
 j func8_9
 func7_9:
 mv x14, x2
 func8_9:
 li x7, 0x0000000a
 bne x14, x7, fail



  # Test with a negative base

  test_10:
 li gp, 10
 la x1, tdat9
 li x2, 0x12345678
 addi x4, x1, -32
 sb x2, 32(x4)
 lb x5, 0(x1)
 li x7, 0x00000078
 bne x5, x7, fail








  # Test with unaligned base

  test_11:
 li gp, 11
 la x1, tdat9
 li x2, 0x00003098
 addi x1, x1, -6
 sb x2, 7(x1)
 la x4, tdat10
 lb x5, 0(x4)
 li x7, 0xffffff98
 bne x5, x7, fail

# 55 "sb.S"
  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xffffffdd
 la x2, tdat
 sb x1, 0(x2)
 lb x14, 0(x2)
 li x7, 0xffffffdd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 li x1, 0xffffffcd
 la x2, tdat
 nop
 sb x1, 1(x2)
 lb x14, 1(x2)
 li x7, 0xffffffcd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 li x1, 0xffffffcc
 la x2, tdat
 nop
 nop
 sb x1, 2(x2)
 lb x14, 2(x2)
 li x7, 0xffffffcc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14


  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 li x1, 0xffffffbc
 nop
 la x2, tdat
 sb x1, 3(x2)
 lb x14, 3(x2)
 li x7, 0xffffffbc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 li x1, 0xffffffbb
 nop
 la x2, tdat
 nop
 sb x1, 4(x2)
 lb x14, 4(x2)
 li x7, 0xffffffbb
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 li x1, 0xffffffab
 nop
 nop
 la x2, tdat
 sb x1, 5(x2)
 lb x14, 5(x2)
 li x7, 0xffffffab
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17



  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 la x2, tdat
 li x1, 0x00000033
 sb x1, 0(x2)
 lb x14, 0(x2)
 li x7, 0x00000033
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 la x2, tdat
 li x1, 0x00000023
 nop
 sb x1, 1(x2)
 lb x14, 1(x2)
 li x7, 0x00000023
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 la x2, tdat
 li x1, 0x00000022
 nop
 nop
 sb x1, 2(x2)
 lb x14, 2(x2)
 li x7, 0x00000022
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_20


  test_21:
 li gp, 21
 li x4, 0
 func1_21:
 la x2, tdat
 nop
 li x1, 0x00000012
 sb x1, 3(x2)
 lb x14, 3(x2)
 li x7, 0x00000012
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_21


  test_22:
 li gp, 22
 li x4, 0
 func1_22:
 la x2, tdat
 nop
 li x1, 0x00000011
 nop
 sb x1, 4(x2)
 lb x14, 4(x2)
 li x7, 0x00000011
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_22


  test_23:
 li gp, 23
 li x4, 0
 func1_23:
 la x2, tdat
 nop
 nop
 li x1, 0x00000001
 sb x1, 5(x2)
 lb x14, 5(x2)
 li x7, 0x00000001
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23



  li a0, 0xef
  la a1, tdat
  sb a0, 3(a1)

  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 

tdat:

tdat1:
 .byte 0xef
tdat2:
 .byte 0xef
tdat3:
 .byte 0xef
tdat4:
 .byte 0xef
tdat5:
 .byte 0xef
tdat6:
 .byte 0xef
tdat7:
 .byte 0xef
tdat8:
 .byte 0xef
tdat9:
 .byte 0xef
tdat10:
 .byte 0xef


