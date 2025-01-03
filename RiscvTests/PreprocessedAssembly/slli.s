#*****************************************************************************
# slli.S
#-----------------------------------------------------------------------------

# Test slli instruction.


.text
 .align 6
 .globl main
 main:
 .align 2
 li gp, 0


  #-------------------------------------------------------------
  # Arithmetic tests
  #-------------------------------------------------------------

  test_2:
 li gp, 2
 li x1, 0x00000001
 slli x14, x1, 0
 li x7, 0x00000001
 bne x14, x7, fail


  test_3:
 li gp, 3
 li x1, 0x00000001
 slli x14, x1, 1
 li x7, 0x00000002
 bne x14, x7, fail


  test_4:
 li gp, 4
 li x1, 0x00000001
 slli x14, x1, 7
 li x7, 0x00000080
 bne x14, x7, fail


  test_5:
 li gp, 5
 li x1, 0x00000001
 slli x14, x1, 14
 li x7, 0x00004000
 bne x14, x7, fail


  test_6:
 li gp, 6
 li x1, 0x00000001
 slli x14, x1, 31
 li x7, 0x80000000
 bne x14, x7, fail



  test_7:
 li gp, 7
 li x1, 0xffffffff
 slli x14, x1, 0
 li x7, 0xffffffff
 bne x14, x7, fail


  test_8:
 li gp, 8
 li x1, 0xffffffff
 slli x14, x1, 1
 li x7, 0xfffffffe
 bne x14, x7, fail


  test_9:
 li gp, 9
 li x1, 0xffffffff
 slli x14, x1, 7
 li x7, 0xffffff80
 bne x14, x7, fail


  test_10:
 li gp, 10
 li x1, 0xffffffff
 slli x14, x1, 14
 li x7, 0xffffc000
 bne x14, x7, fail


  test_11:
 li gp, 11
 li x1, 0xffffffff
 slli x14, x1, 31
 li x7, 0x80000000
 bne x14, x7, fail



  test_12:
 li gp, 12
 li x1, 0x21212121
 slli x14, x1, 0
 li x7, 0x21212121
 bne x14, x7, fail


  test_13:
 li gp, 13
 li x1, 0x21212121
 slli x14, x1, 1
 li x7, 0x42424242
 bne x14, x7, fail


  test_14:
 li gp, 14
 li x1, 0x21212121
 slli x14, x1, 7
 li x7, 0x90909080
 bne x14, x7, fail


  test_15:
 li gp, 15
 li x1, 0x21212121
 slli x14, x1, 14
 li x7, 0x48484000
 bne x14, x7, fail


  test_16:
 li gp, 16
 li x1, 0x21212121
 slli x14, x1, 31
 li x7, 0x80000000
 bne x14, x7, fail



  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  test_17:
 li gp, 17
 li x1, 0x00000001
 slli x1, x1, 7
 li x7, 0x00000080
 bne x1, x7, fail



  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  test_18:
 li gp, 18
 li x4, 0
 func1_18:
 li x1, 0x00000001
 slli x14, x1, 7
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_18
 li x7, 0x00000080
 bne x6, x7, fail


  test_19:
 li gp, 19
 li x4, 0
 func1_19:
 li x1, 0x00000001
 slli x14, x1, 14
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_19
 li x7, 0x00004000
 bne x6, x7, fail


  test_20:
 li gp, 20
 li x4, 0
 func1_20:
 li x1, 0x00000001
 slli x14, x1, 31
 nop
 nop
 addi x6, x14, 0
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_20
 li x7, 0x80000000
 bne x6, x7, fail



  test_21:
 li gp, 21
 li x4, 0
 func1_21:
 li x1, 0x00000001
 slli x14, x1, 7
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_21
 li x7, 0x00000080
 bne x14, x7, fail


  test_22:
 li gp, 22
 li x4, 0
 func1_22:
 li x1, 0x00000001
 nop
 slli x14, x1, 14
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_22
 li x7, 0x00004000
 bne x14, x7, fail


  test_23:
 li gp, 23
 li x4, 0
 func1_23:
 li x1, 0x00000001
 nop
 nop
 slli x14, x1, 31
 addi x4, x4, 1
 li x5, 2
 bne x4, x5, func1_23
 li x7, 0x80000000
 bne x14, x7, fail



  test_24:
 li gp, 24
 slli x1, x0, 31
 li x7, 0
 bne x1, x7, fail


  test_25:
 li gp, 25
 li x1, 33
 slli x0, x1, 20
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


 


