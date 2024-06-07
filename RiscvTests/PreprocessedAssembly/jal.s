#*****************************************************************************
# jal.S
#-----------------------------------------------------------------------------

# Test jal instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Test 2: Basic test
  #-------------------------------------------------------------

test_2:

  li gp, 2
  li ra, 0

  jal x4, target_2
linkaddr_2:

  nop
  nop

  j fail

target_2:

  la x2, linkaddr_2
  bne x2, x4, fail

  #-------------------------------------------------------------
  # Test delay slot instructions not executed nor bypassed
  #-------------------------------------------------------------

  test_3:
 li gp, 3
 li ra, 1
 jal x0, func1_3
 addi ra, ra, 1
 addi ra, ra, 1
 addi ra, ra, 1
 addi ra, ra, 1
 func1_3:
 addi ra, ra, 1
 addi ra, ra, 1
 li x7, 3
 bne ra, x7, fail

  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 


