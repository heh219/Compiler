  global main
  extern printf

  segment .bss
  i$1 resq 1
  n$2 resq 1
  i$3 resq 1
  factorial$4 resq 1

  section .text
main:
  mov rax,1
  push rax
  pop qword[i$1]
  mov rdi,fmt1
  mov rax,0
  push rbp
  call printf
  pop rbp
L2:
  push qword[i$1]
  mov rax,20
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  jle J1
  mov rax,0
  jmp J2
J1:
  mov rax,1
J2:
  push rax
  pop rax
  cmp rax,0
  je L3
  push qword[i$1]
  push qword[i$1]
  call fact
  push rax
  mov rdi,fmt2
  pop rdx
  pop rsi
  mov rax,0
  push rbp
  call printf
  pop rbp
  push qword[i$1]
  mov rax,1
  push rax
  pop rbx
  pop rax
  add rax,rbx
  push rax
  pop qword[i$1]
  jmp L2
L3:
  mov rax,0
  push rax
  pop rax
  ret
fact:
  pop r15
  pop qword[n$2]
  mov rax,1
  push rax
  pop qword[i$3]
  mov rax,1
  push rax
  pop qword[factorial$4]
L4:
  push qword[i$3]
  push qword[n$2]
  pop rbx
  pop rax
  cmp rax,rbx
  jle J3
  mov rax,0
  jmp J4
J3:
  mov rax,1
J4:
  push rax
  pop rax
  cmp rax,0
  je L5
  push qword[factorial$4]
  push qword[i$3]
  pop rbx
  pop rax
  imul rbx
  push rax
  pop qword[factorial$4]
  push qword[i$3]
  mov rax,1
  push rax
  pop rbx
  pop rax
  add rax,rbx
  push rax
  pop qword[i$3]
  jmp L4
L5:
  push qword[factorial$4]
  pop rax
  push r15
  ret

  section .data
  fmt1: db ` n    n!\n`, 0
  fmt2: db `%2d    %d\n`, 0
