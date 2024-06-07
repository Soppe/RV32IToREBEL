#*****************************************************************************
# auipc.S
#-----------------------------------------------------------------------------

# Test auipc instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  test_2:
 li gp, 2
 .align 3
 lla a0, func1_2
 li t0, 10000
 add a0, a0, t0;
 jal a1, func1_2
 func1_2:
 sub a0, a0, a1
 li x7, 10000
 bne a0, x7, fail



  test_3:
 li gp, 3
 .align 3
 lla a0, func1_3
 li t0, -10000
 add a0, a0, t0;
 jal a1, func1_3
 func1_3:
 sub a0, a0, a1
 li x7, -10000
 bne a0, x7, fail








  bne x0, gp, pass
 fail:
 li a0, 1
 ecall

 pass:
 li a0, 0
 ecall




  .data


 


