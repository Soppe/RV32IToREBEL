#*****************************************************************************
# xor.S
#-----------------------------------------------------------------------------

# Test xor instruction.


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
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 li x7, 0xf00ff00f
 bne x14, x7, fail


  test_3:
 li gp, 3
 li x1, 0x0ff00ff0
 li x2, 0xf0f0f0f0
 xor x14, x1, x2
 li x7, 0xff00ff00
 bne x14, x7, fail


  test_4:
 li gp, 4
 li x1, 0x00ff00ff
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 li x7, 0x0ff00ff0
 bne x14, x7, fail


  test_5:
 li gp, 5
 li x1, 0xf00ff00f
 li x2, 0xf0f0f0f0
 xor x14, x1, x2
 li x7, 0x00ff00ff
 bne x14, x7, fail



  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  test_6:
 li gp, 6
 li x1, 0xff00ff00
 li x2, 0x0f0f0f0f
 xor x1, x1, x2
 li x7, 0xf00ff00f
 bne x1, x7, fail


  test_7:
 li gp, 7
 li x1, 0xff00ff00
 li x2, 0x0f0f0f0f
 xor x2, x1, x2
 li x7, 0xf00ff00f
 bne x2, x7, fail


  test_8:
 li gp, 8
 li x1, 0xff00ff00
 xor x1, x1, x1
 li x7, 0x00000000
 bne x1, x7, fail



  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_9:
 li gp, 9
 li x4, 0
 func1_9:
 li x1, 0xff00ff00
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_9
 li x7, 0xf00ff00f
 bne x6, x7, fail


  test_10:
 li gp, 10
 li x4, 0
 func1_10:
 li x1, 0x0ff00ff0
 li x2, 0xf0f0f0f0
 xor x14, x1, x2
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_10
 li x7, 0xff00ff00
 bne x6, x7, fail


  test_11:
 li gp, 11
 li x4, 0
 func1_11:
 li x1, 0x00ff00ff
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 nop
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_11
 li x7, 0x0ff00ff0
 bne x6, x7, fail



  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xff00ff00
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12
 li x7, 0xf00ff00f
 bne x14, x7, fail


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 li x1, 0x0ff00ff0
 li x2, 0xf0f0f0f0
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13
 li x7, 0xff00ff00
 bne x14, x7, fail


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 li x1, 0x00ff00ff
 li x2, 0x0f0f0f0f
 nop
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14
 li x7, 0x0ff00ff0
 bne x14, x7, fail


  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 li x1, 0xff00ff00
 nop
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15
 li x7, 0xf00ff00f
 bne x14, x7, fail


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 li x1, 0x0ff00ff0
 nop
 li x2, 0xf0f0f0f0
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16
 li x7, 0xff00ff00
 bne x14, x7, fail


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 li x1, 0x00ff00ff
 nop
 nop
 li x2, 0x0f0f0f0f
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17
 li x7, 0x0ff00ff0
 bne x14, x7, fail



  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 li x2, 0x0f0f0f0f
 li x1, 0xff00ff00
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18
 li x7, 0xf00ff00f
 bne x14, x7, fail


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 li x2, 0xf0f0f0f0
 li x1, 0x0ff00ff0
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19
 li x7, 0xff00ff00
 bne x14, x7, fail


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 li x2, 0x0f0f0f0f
 li x1, 0x00ff00ff
 nop
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_20
 li x7, 0x0ff00ff0
 bne x14, x7, fail


  test_21:
 li gp, 21
 li x4, 0
 func1_21:
 li x2, 0x0f0f0f0f
 nop
 li x1, 0xff00ff00
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_21
 li x7, 0xf00ff00f
 bne x14, x7, fail


  test_22:
 li gp, 22
 li x4, 0
 func1_22:
 li x2, 0xf0f0f0f0
 nop
 li x1, 0x0ff00ff0
 nop
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_22
 li x7, 0xff00ff00
 bne x14, x7, fail


  test_23:
 li gp, 23
 li x4, 0
 func1_23:
 li x2, 0x0f0f0f0f
 nop
 nop
 li x1, 0x00ff00ff
 xor x14, x1, x2
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23
 li x7, 0x0ff00ff0
 bne x14, x7, fail



  test_24:
 li gp, 24
 li x1, 0xff00ff00
 xor x2, x0, x1
 li x7, 0xff00ff00
 bne x2, x7, fail


  test_25:
 li gp, 25
 li x1, 0x00ff00ff
 xor x2, x1, x0
 li x7, 0x00ff00ff
 bne x2, x7, fail


  test_26:
 li gp, 26
 xor x1, x0, x0
 li x7, 0
 bne x1, x7, fail


  test_27:
 li gp, 27
 li x1, 0x11111111
 li x2, 0x22222222
 xor x0, x1, x2
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


 


