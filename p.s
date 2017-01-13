
  pushl %eax
  pushl $output
  call printf
  popl %eax
  popl %eax

  movl $1, %eax
  movl $0, %ebx
  int $0x80
