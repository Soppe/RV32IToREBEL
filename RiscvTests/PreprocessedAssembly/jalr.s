#*****************************************************************************
# jalr.S
#-----------------------------------------------------------------------------

# Test jalr instruction.


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
  li t0, 0
  la t1, target_2

  jalr t0, t1, 0
linkaddr_2:

  j fail

target_2:

  la t1, linkaddr_2
  bne t0, t1, fail

  #-------------------------------------------------------------
  # Test 3: Basic test2, rs = rd
  #-------------------------------------------------------------

test_3:

  li gp, 3
  la t0, target_3

  jalr t0, t0, 0
linkaddr_3:

  j fail

target_3:

  la t1, linkaddr_3
  bne t0, t1, fail

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_4:
 li gp, 4
 li x4, 0
 func1_4:
 la x6, func2_4
 jalr x13, x6, 0
 bne x0, gp, fail
 func2_4:
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_4


  test_5:
 li gp, 5
 li x4, 0
 func1_5:
 la x6, func2_5
 nop
 jalr x13, x6, 0
 bne x0, gp, fail
 func2_5:
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_5


  test_6:
 li gp, 6
 li x4, 0
 func1_6:
 la x6, func2_6
 nop
 nop
 jalr x13, x6, 0
 bne x0, gp, fail
 func2_6:
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_6



  #-------------------------------------------------------------
  # Test delay slot instructions not executed nor bypassed
  #-------------------------------------------------------------

  .option push
  .align 2
  .option norvc
  test_7:
 li gp, 7
 li t0, 1
 la t1, func1_7
 jr t1, -4
 addi t0, t0, 1
 addi t0, t0, 1
 addi t0, t0, 1
 addi t0, t0, 1
 func1_7:
 addi t0, t0, 1
 addi t0, t0, 1
 li x7, 4
 bne t0, x7, fail

  .option pop

  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 


