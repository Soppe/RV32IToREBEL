#*****************************************************************************
# bgeu.S
#-----------------------------------------------------------------------------

# Test bgeu instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Branch tests
  #-------------------------------------------------------------

  # Each test checks both forward and backward branches

  test_2:
 li gp, 2
 li x1, 0x00000000
 li x2, 0x00000000
 bgeu x1, x2, func2_2
 bne x0, gp, fail
 func1_2:
 bne x0, gp, func3_2
 func2_2:
 bgeu x1, x2, func1_2
 bne x0, gp, fail
 func3_2:


  test_3:
 li gp, 3
 li x1, 0x00000001
 li x2, 0x00000001
 bgeu x1, x2, func2_3
 bne x0, gp, fail
 func1_3:
 bne x0, gp, func3_3
 func2_3:
 bgeu x1, x2, func1_3
 bne x0, gp, fail
 func3_3:


  test_4:
 li gp, 4
 li x1, 0xffffffff
 li x2, 0xffffffff
 bgeu x1, x2, func2_4
 bne x0, gp, fail
 func1_4:
 bne x0, gp, func3_4
 func2_4:
 bgeu x1, x2, func1_4
 bne x0, gp, fail
 func3_4:


  test_5:
 li gp, 5
 li x1, 0x00000001
 li x2, 0x00000000
 bgeu x1, x2, func2_5
 bne x0, gp, fail
 func1_5:
 bne x0, gp, func3_5
 func2_5:
 bgeu x1, x2, func1_5
 bne x0, gp, fail
 func3_5:


  test_6:
 li gp, 6
 li x1, 0xffffffff
 li x2, 0xfffffffe
 bgeu x1, x2, func2_6
 bne x0, gp, fail
 func1_6:
 bne x0, gp, func3_6
 func2_6:
 bgeu x1, x2, func1_6
 bne x0, gp, fail
 func3_6:


  test_7:
 li gp, 7
 li x1, 0xffffffff
 li x2, 0x00000000
 bgeu x1, x2, func2_7
 bne x0, gp, fail
 func1_7:
 bne x0, gp, func3_7
 func2_7:
 bgeu x1, x2, func1_7
 bne x0, gp, fail
 func3_7:



  test_8:
 li gp, 8
 li x1, 0x00000000
 li x2, 0x00000001
 bgeu x1, x2, func1_8
 bne x0, gp, func2_8
 func1_8:
 bne x0, gp, fail
 func2_8:
 bgeu x1, x2, func1_8
 func3_8:


  test_9:
 li gp, 9
 li x1, 0xfffffffe
 li x2, 0xffffffff
 bgeu x1, x2, func1_9
 bne x0, gp, func2_9
 func1_9:
 bne x0, gp, fail
 func2_9:
 bgeu x1, x2, func1_9
 func3_9:


  test_10:
 li gp, 10
 li x1, 0x00000000
 li x2, 0xffffffff
 bgeu x1, x2, func1_10
 bne x0, gp, func2_10
 func1_10:
 bne x0, gp, fail
 func2_10:
 bgeu x1, x2, func1_10
 func3_10:


  test_11:
 li gp, 11
 li x1, 0x7fffffff
 li x2, 0x80000000
 bgeu x1, x2, func1_11
 bne x0, gp, func2_11
 func1_11:
 bne x0, gp, fail
 func2_11:
 bgeu x1, x2, func1_11
 func3_11:



  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_12:
 li gp, 12
 li x4, 0
 func1_12:
 li x1, 0xefffffff
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_12


  test_13:
 li gp, 13
 li x4, 0
 func1_13:
 li x1, 0xefffffff
 li x2, 0xf0000000
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_13


  test_14:
 li gp, 14
 li x4, 0
 func1_14:
 li x1, 0xefffffff
 li x2, 0xf0000000
 nop
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_14


  test_15:
 li gp, 15
 li x4, 0
 func1_15:
 li x1, 0xefffffff
 nop
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_15


  test_16:
 li gp, 16
 li x4, 0
 func1_16:
 li x1, 0xefffffff
 nop
 li x2, 0xf0000000
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_16


  test_17:
 li gp, 17
 li x4, 0
 func1_17:
 li x1, 0xefffffff
 nop
 nop
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_17



  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 li x1, 0xefffffff
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 li x1, 0xefffffff
 li x2, 0xf0000000
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 li x1, 0xefffffff
 li x2, 0xf0000000
 nop
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_20


  test_21:
 li gp, 21
 li x4, 0
 func1_21:
 li x1, 0xefffffff
 nop
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_21


  test_22:
 li gp, 22
 li x4, 0
 func1_22:
 li x1, 0xefffffff
 nop
 li x2, 0xf0000000
 nop
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_22


  test_23:
 li gp, 23
 li x4, 0
 func1_23:
 li x1, 0xefffffff
 nop
 nop
 li x2, 0xf0000000
 bgeu x1, x2, fail
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23



  #-------------------------------------------------------------
  # Test delay slot instructions not executed nor bypassed
  #-------------------------------------------------------------

  test_24:
 li gp, 24
 li x1, 1
 bgeu x1, x0, func1_24
 addi x1, x1, 1
 addi x1, x1, 1
 addi x1, x1, 1
 addi x1, x1, 1
 func1_24:
 addi x1, x1, 1
 addi x1, x1, 1
 li x7, 3
 bne x1, x7, fail

# 69 "bgeu.S"
  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 


