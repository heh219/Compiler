  global main
  extern printf

  segment .bss
  x$1 resq 1

  section .text
main:
  mov rax,10
  push rax
  pop qword[x$1]
  push qword[x$1]
  mov rax,10
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  je J1
  mov rax,0
  jmp J2
J1:
  mov rax,1
J2:
  push rax
  pop rax
  cmp rax,0
  je L2
  mov rax,1
  push rax
  pop qword[x$1]
  mov rdi,fmt1
  mov rax,0
  push rbp
  call printf
  pop rbp
L2:

  section .data
  fmt1: db `Hi \n`, 0
