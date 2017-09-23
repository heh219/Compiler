  global main
  extern printf

  segment .bss
  x$1 resq 1

  section .text
main:
  mov rax,10
  push rax
  pop qword[x$1]
L2:
  push qword[x$1]
  mov rax,0
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  jg J1
  mov rax,0
  jmp J2
J1:
  mov rax,1
J2:
  push rax
  pop rax
  cmp rax,0
  je L3
  push qword[x$1]
  mov rax,1
  push rax
  pop rbx
  pop rax
  sub rax,rbx
  push rax
  pop qword[x$1]
  push qword[x$1]
  mov rdi,fmt1
  pop rsi
  mov rax,0
  push rbp
  call printf
  pop rbp
  jmp L2
L3:
  mov rdi,fmt2
  mov rax,0
  push rbp
  call printf
  pop rbp

  section .data
  fmt1: db `%d\n`, 0
  fmt2: db `Sophie is pig`, 0
