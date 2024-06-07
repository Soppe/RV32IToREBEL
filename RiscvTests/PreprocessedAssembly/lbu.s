#*****************************************************************************
# lbu.S
#-----------------------------------------------------------------------------

# Test lbu instruction.


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
 li x15, 0x000000ff
 la x1, tdat
 lbu x14, 0(x1)
 li x7, 0x000000ff
 bne x14, x7, fail


  test_3:
 li gp, 3
 li x15, 0x00000000
 la x1, tdat
 lbu x14, 1(x1)
 li x7, 0x00000000
 bne x14, x7, fail


  test_4:
 li gp, 4
 li x15, 0x000000f0
 la x1, tdat
 lbu x14, 2(x1)
 li x7, 0x000000f0
 bne x14, x7, fail


  test_5:
 li gp, 5
 li x15, 0x0000000f
 la x1, tdat
 lbu x14, 3(x1)
 li x7, 0x0000000f
 bne x14, x7, fail



  # Test with negative offset

  test_6:
 li gp, 6
 li x15, 0x000000ff
 la x1, tdat4
 lbu x14, -3(x1)
 li x7, 0x000000ff
 bne x14, x7, fail


  test_7:
 li gp, 7
 li x15, 0x00000000
 la x1, tdat4
 lbu x14, -2(x1)
 li x7, 0x00000000
 bne x14, x7, fail


  test_8:
 li gp, 8
 li x15, 0x000000f0
 la x1, tdat4
 lbu x14, -1(x1)
 li x7, 0x000000f0
 bne x14, x7, fail


  test_9:
 li gp, 9
 li x15, 0x0000000f
 la x1, tdat4
 lbu x14, 0(x1)
 li x7, 0x0000000f
 bne x14, x7, fail



  # Test with a negative base

  test_10:
 li gp, 10
 la x1, tdat
 addi x1, x1, -32
 lbu x5, 32(x1)
 li x7, 0x000000ff
 bne x5, x7, fail






  # Test with unaligned base

  test_11:
 li gp, 11
 la x1, tdat
 addi x1, x1, -6
 lbu x5, 7(x1)
 li x7, 0x00000000
 bne x5, x7, fail






  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 la x1, tdat2
 lbu x14, 1(x1)
 addi x6, x14, 0
 li x7, 0x000000f0
 bne x6, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 la x1, tdat3
 lbu x14, 1(x1)
 nop
 addi x6, x14, 0
 li x7, 0x0000000f
 bne x6, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 la x1, tdat1
 lbu x14, 1(x1)
 nop
 nop
 addi x6, x14, 0
 li x7, 0x00000000
 bne x6, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14



  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 la x1, tdat2
 lbu x14, 1(x1)
 li x7, 0x000000f0
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 la x1, tdat3
 nop
 lbu x14, 1(x1)
 li x7, 0x0000000f
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 la x1, tdat1
 nop
 nop
 lbu x14, 1(x1)
 li x7, 0x00000000
 bne x14, x7, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17



  #-------------------------------------------------------------
  # Test write-after-write hazard
  #-------------------------------------------------------------

  test_18:
 li gp, 18
 la x5, tdat
 lbu x2, 0(x5)
 li x2, 2
 li x7, 2
 bne x2, x7, fail






  test_19:
 li gp, 19
 la x5, tdat
 lbu x2, 0(x5)
 nop
 li x2, 2
 li x7, 2
 bne x2, x7, fail







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
 .byte 0xff
tdat2:
 .byte 0x00
tdat3:
 .byte 0xf0
tdat4:
 .byte 0x0f


