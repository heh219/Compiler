  global main
  extern printf

  segment .bss
  year$3 resq 1
  month$2 resq 1
  lastday$1 resq 1

  section .text
main:
  mov rax,2017
  push rax
  pop qword[year$3]
  mov rax,4
  push rax
  pop qword[month$2]
  push qword[month$2]
  mov rax,4
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
  push qword[month$2]
  mov rax,6
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  je J3
  mov rax,0
  jmp J4
J3:
  mov rax,1
J4:
  push rax
  pop rbx
  pop rax
  or rax,rbx
  push rax
  push qword[month$2]
  mov rax,9
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  je J5
  mov rax,0
  jmp J6
J5:
  mov rax,1
J6:
  push rax
  pop rbx
  pop rax
  or rax,rbx
  push rax
  push qword[month$2]
  mov rax,11
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  je J7
  mov rax,0
  jmp J8
J7:
  mov rax,1
J8:
  push rax
  pop rbx
  pop rax
  or rax,rbx
  push rax
  pop rax
  cmp rax,0
  je L3
  mov rax,30
  push rax
  pop qword[lastday$1]
  jmp L2
L3:
  push qword[month$2]
  mov rax,2
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  jne J9
  mov rax,0
  jmp J10
J9:
  mov rax,1
J10:
  push rax
  pop rax
  cmp rax,0
  je L5
  mov rax,31
  push rax
  pop qword[lastday$1]
  jmp L4
L5:
  push qword[year$3]
  call leapyear
  push rax
  pop rax
  cmp rax,0
  je L7
  mov rax,29
  push rax
  pop qword[lastday$1]
  jmp L6
L7:
  mov rax,28
  push rax
  pop qword[lastday$1]
L6:
L4:
L2:

  section .data
