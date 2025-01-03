#*****************************************************************************
# sw.S
#-----------------------------------------------------------------------------

# Test sw instruction.


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
 li x2, 0x00aa00aa
 la x15, func7_2
 sw x2, 0(x1)
 lw x14, 0(x1)
 j func8_2
 func7_2:
 mv x14, x2
 func8_2:
 li x7, 0x00aa00aa
 bne x14, x7, fail


  test_3:
 li gp, 3
 la x1, tdat
 li x2, 0xaa00aa00
 la x15, func7_3
 sw x2, 4(x1)
 lw x14, 4(x1)
 j func8_3
 func7_3:
 mv x14, x2
 func8_3:
 li x7, 0xaa00aa00
 bne x14, x7, fail


  test_4:
 li gp, 4
 la x1, tdat
 li x2, 0x0aa00aa0
 la x15, func7_4
 sw x2, 8(x1)
 lw x14, 8(x1)
 j func8_4
 func7_4:
 mv x14, x2
 func8_4:
 li x7, 0x0aa00aa0
 bne x14, x7, fail


  test_5:
 li gp, 5
 la x1, tdat
 li x2, 0xa00aa00a
 la x15, func7_5
 sw x2, 12(x1)
 lw x14, 12(x1)
 j func8_5
 func7_5:
 mv x14, x2
 func8_5:
 li x7, 0xa00aa00a
 bne x14, x7, fail



  # Test with negative offset

  test_6:
 li gp, 6
 la x1, tdat8
 li x2, 0x00aa00aa
 la x15, func7_6
 sw x2, -12(x1)
 lw x14, -12(x1)
 j func8_6
 func7_6:
 mv x14, x2
 func8_6:
 li x7, 0x00aa00aa
 bne x14, x7, fail


  test_7:
 li gp, 7
 la x1, tdat8
 li x2, 0xaa00aa00
 la x15, func7_7
 sw x2, -8(x1)
 lw x14, -8(x1)
 j func8_7
 func7_7:
 mv x14, x2
 func8_7:
 li x7, 0xaa00aa00
 bne x14, x7, fail


  test_8:
 li gp, 8
 la x1, tdat8
 li x2, 0x0aa00aa0
 la x15, func7_8
 sw x2, -4(x1)
 lw x14, -4(x1)
 j func8_8
 func7_8:
 mv x14, x2
 func8_8:
 li x7, 0x0aa00aa0
 bne x14, x7, fail


  test_9:
 li gp, 9
 la x1, tdat8
 li x2, 0xa00aa00a
 la x15, func7_9
 sw x2, 0(x1)
 lw x14, 0(x1)
 j func8_9
 func7_9:
 mv x14, x2
 func8_9:
 li x7, 0xa00aa00a
 bne x14, x7, fail



  # Test with a negative base

  test_10:
 li gp, 10
 la x1, tdat9
 li x2, 0x12345678
 addi x4, x1, -32
 sw x2, 32(x4)
 lw x5, 0(x1)
 li x7, 0x12345678
 bne x5, x7, fail








  # Test with unaligned base

  test_11:
 li gp, 11
 la x1, tdat9
 li x2, 0x58213098
 addi x1, x1, -3
 sw x2, 7(x1)
 la x4, tdat10
 lw x5, 0(x4)
 li x7, 0x58213098
 bne x5, x7, fail

# 55 "sw.S"
  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xaabbccdd
 la x2, tdat
 sw x1, 0(x2)
 lw x14, 0(x2)
 li x7, 0xaabbccdd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 li x1, 0xdaabbccd
 la x2, tdat
 nop
 sw x1, 4(x2)
 lw x14, 4(x2)
 li x7, 0xdaabbccd
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 li x1, 0xddaabbcc
 la x2, tdat
 nop
 nop
 sw x1, 8(x2)
 lw x14, 8(x2)
 li x7, 0xddaabbcc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14


  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 li x1, 0xcddaabbc
 nop
 la x2, tdat
 sw x1, 12(x2)
 lw x14, 12(x2)
 li x7, 0xcddaabbc
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 li x1, 0xccddaabb
 nop
 la x2, tdat
 nop
 sw x1, 16(x2)
 lw x14, 16(x2)
 li x7, 0xccddaabb
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 li x1, 0xbccddaab
 nop
 nop
 la x2, tdat
 sw x1, 20(x2)
 lw x14, 20(x2)
 li x7, 0xbccddaab
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17



  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 la x2, tdat
 li x1, 0x00112233
 sw x1, 0(x2)
 lw x14, 0(x2)
 li x7, 0x00112233
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 la x2, tdat
 li x1, 0x30011223
 nop
 sw x1, 4(x2)
 lw x14, 4(x2)
 li x7, 0x30011223
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 la x2, tdat
 li x1, 0x33001122
 nop
 nop
 sw x1, 8(x2)
 lw x14, 8(x2)
 li x7, 0x33001122
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
 li x1, 0x23300112
 sw x1, 12(x2)
 lw x14, 12(x2)
 li x7, 0x23300112
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
 li x1, 0x22330011
 nop
 sw x1, 16(x2)
 lw x14, 16(x2)
 li x7, 0x22330011
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
 li x1, 0x12233001
 sw x1, 20(x2)
 lw x14, 20(x2)
 li x7, 0x12233001
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23



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
 .word 0xdeadbeef
tdat2:
 .word 0xdeadbeef
tdat3:
 .word 0xdeadbeef
tdat4:
 .word 0xdeadbeef
tdat5:
 .word 0xdeadbeef
tdat6:
 .word 0xdeadbeef
tdat7:
 .word 0xdeadbeef
tdat8:
 .word 0xdeadbeef
tdat9:
 .word 0xdeadbeef
tdat10:
 .word 0xdeadbeef


