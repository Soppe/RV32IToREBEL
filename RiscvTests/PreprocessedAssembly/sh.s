#*****************************************************************************
# sh.S
#-----------------------------------------------------------------------------

# Test sh instruction.

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
 li x2, 0x00000000000000aa
 la x15, func7_2
 sh x2, 0(x1)
 lh x14, 0(x1)
 j func8_2
 func7_2:
 mv x14, x2
 func8_2:
 li x7, 0x00000000000000aa
 bne x14, x7, fail


  test_3:
 li gp, 3
 la x1, tdat
 li x2, 0xffffffffffffaa00
 la x15, func7_3
 sh x2, 2(x1)
 lh x14, 2(x1)
 j func8_3
 func7_3:
 mv x14, x2
 func8_3:
 li x7, 0xffffffffffffaa00
 bne x14, x7, fail


  test_4:
 li gp, 4
 la x1, tdat
 li x2, 0xffffffffbeef0aa0
 la x15, func7_4
 sh x2, 4(x1)
 lw x14, 4(x1)
 j func8_4
 func7_4:
 mv x14, x2
 func8_4:
 li x7, 0xffffffffbeef0aa0
 bne x14, x7, fail


  test_5:
 li gp, 5
 la x1, tdat
 li x2, 0xffffffffffffa00a
 la x15, func7_5
 sh x2, 6(x1)
 lh x14, 6(x1)
 j func8_5
 func7_5:
 mv x14, x2
 func8_5:
 li x7, 0xffffffffffffa00a
 bne x14, x7, fail



  # Test with negative offset

  test_6:
 li gp, 6
 la x1, tdat8
 li x2, 0x00000000000000aa
 la x15, func7_6
 sh x2, -6(x1)
 lh x14, -6(x1)
 j func8_6
 func7_6:
 mv x14, x2
 func8_6:
 li x7, 0x00000000000000aa
 bne x14, x7, fail


  test_7:
 li gp, 7
 la x1, tdat8
 li x2, 0xffffffffffffaa00
 la x15, func7_7
 sh x2, -4(x1)
 lh x14, -4(x1)
 j func8_7
 func7_7:
 mv x14, x2
 func8_7:
 li x7, 0xffffffffffffaa00
 bne x14, x7, fail


  test_8:
 li gp, 8
 la x1, tdat8
 li x2, 0x0000000000000aa0
 la x15, func7_8
 sh x2, -2(x1)
 lh x14, -2(x1)
 j func8_8
 func7_8:
 mv x14, x2
 func8_8:
 li x7, 0x0000000000000aa0
 bne x14, x7, fail


  test_9:
 li gp, 9
 la x1, tdat8
 li x2, 0xffffffffffffa00a
 la x15, func7_9
 sh x2, 0(x1)
 lh x14, 0(x1)
 j func8_9
 func7_9:
 mv x14, x2
 func8_9:
 li x7, 0xffffffffffffa00a
 bne x14, x7, fail



  # Test with a negative base

  test_10:
 li gp, 10
 la x1, tdat9
 li x2, 0x12345678
 addi x4, x1, -32
 sh x2, 32(x4)
 lh x5, 0(x1)
 li x7, 0x5678
 bne x5, x7, fail








  # Test with unaligned base

  test_11:
 li gp, 11
 la x1, tdat9
 li x2, 0x00003098
 addi x1, x1, -5
 sh x2, 7(x1)
 la x4, tdat10
 lh x5, 0(x4)
 li x7, 0x3098
 bne x5, x7, fail

# 55 "sh.S"
  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xffffccdd
 la x2, tdat
 sh x1, 0(x2)
 lh x14, 0(x2)
 li x7, 0xffffccdd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 li x1, 0xffffbccd
 la x2, tdat
 nop
 sh x1, 2(x2)
 lh x14, 2(x2)
 li x7, 0xffffbccd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 li x1, 0xffffbbcc
 la x2, tdat
 nop
 nop
 sh x1, 4(x2)
 lh x14, 4(x2)
 li x7, 0xffffbbcc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14


  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 li x1, 0xffffabbc
 nop
 la x2, tdat
 sh x1, 6(x2)
 lh x14, 6(x2)
 li x7, 0xffffabbc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 li x1, 0xffffaabb
 nop
 la x2, tdat
 nop
 sh x1, 8(x2)
 lh x14, 8(x2)
 li x7, 0xffffaabb
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 li x1, 0xffffdaab
 nop
 nop
 la x2, tdat
 sh x1, 10(x2)
 lh x14, 10(x2)
 li x7, 0xffffdaab
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17



  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 la x2, tdat
 li x1, 0x00002233
 sh x1, 0(x2)
 lh x14, 0(x2)
 li x7, 0x00002233
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 la x2, tdat
 li x1, 0x00001223
 nop
 sh x1, 2(x2)
 lh x14, 2(x2)
 li x7, 0x00001223
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 la x2, tdat
 li x1, 0x00001122
 nop
 nop
 sh x1, 4(x2)
 lh x14, 4(x2)
 li x7, 0x00001122
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
 li x1, 0x00000112
 sh x1, 6(x2)
 lh x14, 6(x2)
 li x7, 0x00000112
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
 sh x1, 8(x2)
 lh x14, 8(x2)
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
 li x1, 0x00003001
 sh x1, 10(x2)
 lh x14, 10(x2)
 li x7, 0x00003001
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23



  li a0, 0xbeef
  la a1, tdat
  sh a0, 6(a1)

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
 .half 0xbeef
tdat2:
 .half 0xbeef
tdat3:
 .half 0xbeef
tdat4:
 .half 0xbeef
tdat5:
 .half 0xbeef
tdat6:
 .half 0xbeef
tdat7:
 .half 0xbeef
tdat8:
 .half 0xbeef
tdat9:
 .half 0xbeef
tdat10:
 .half 0xbeef


