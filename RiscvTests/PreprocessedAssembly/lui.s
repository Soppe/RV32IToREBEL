#*****************************************************************************
# lui.S
#-----------------------------------------------------------------------------

# Test lui instruction.


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
 lui x1, 0x00000
 li x7, 0x00000000
 bne x1, x7, fail


  test_3:
 li gp, 3
 lui x1, 0xfffff
 srai x1,x1,1
 li x7, 0xfffff800
 bne x1, x7, fail


  test_4:
 li gp, 4
 lui x1, 0x7ffff
 srai x1,x1,20
 li x7, 0x000007ff
 bne x1, x7, fail


  test_5:
 li gp, 5
 lui x1, 0x80000
 srai x1,x1,20
 li x7, 0xfffff800
 bne x1, x7, fail



  test_6:
 li gp, 6
 lui x0, 0x80000
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


 


