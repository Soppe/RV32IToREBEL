#*****************************************************************************
# ori.S
#-----------------------------------------------------------------------------

# Test ori instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  test_2:
 li gp, 2
 li x1, 0xff00ff00
 ori x14, x1, 0xffffff0f
 li x7, 0xffffff0f
 bne x14, x7, fail


  test_3:
 li gp, 3
 li x1, 0x0ff00ff0
 ori x14, x1, 0x000000f0
 li x7, 0x0ff00ff0
 bne x14, x7, fail


  test_4:
 li gp, 4
 li x1, 0x00ff00ff
 ori x14, x1, 0x0000070f
 li x7, 0x00ff07ff
 bne x14, x7, fail


  test_5:
 li gp, 5
 li x1, 0xf00ff00f
 ori x14, x1, 0x000000f0
 li x7, 0xf00ff0ff
 bne x14, x7, fail



  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  test_6:
 li gp, 6
 li x1, 0xff00ff00
 ori x1, x1, 0x000000f0
 li x7, 0xff00fff0
 bne x1, x7, fail



  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_7:
 li gp, 7
 li x4, 0
 func1_7:
 li x1, 0x0ff00ff0
 ori x14, x1, 0x000000f0
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_7
 li x7, 0x0ff00ff0
 bne x6, x7, fail


  test_8:
 li gp, 8
 li x4, 0
 func1_8:
 li x1, 0x00ff00ff
 ori x14, x1, 0x0000070f
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_8
 li x7, 0x00ff07ff
 bne x6, x7, fail


  test_9:
 li gp, 9
 li x4, 0
 func1_9:
 li x1, 0xf00ff00f
 ori x14, x1, 0x000000f0
 nop
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_9
 li x7, 0xf00ff0ff
 bne x6, x7, fail



  test_10:
 li gp, 10
 li x4, 0
 func1_10:
 li x1, 0x0ff00ff0
 ori x14, x1, 0x000000f0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_10
 li x7, 0x0ff00ff0
 bne x14, x7, fail


  test_11:
 li gp, 11
 li x4, 0
 func1_11:
 li x1, 0x00ff00ff
 nop
 ori x14, x1, 0xffffff0f
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_11
 li x7, 0xffffffff
 bne x14, x7, fail


  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xf00ff00f
 nop
 nop
 ori x14, x1, 0x000000f0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12
 li x7, 0xf00ff0ff
 bne x14, x7, fail



  test_13:
 li gp, 13
 ori x1, x0, 0x000000f0
 li x7, 0x000000f0
 bne x1, x7, fail


  test_14:
 li gp, 14
 li x1, 0x00ff00ff
 ori x0, x1, 0x0000070f
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


 

