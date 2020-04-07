irmovq	$422, %rax
irmovq	$0xabcd, %rdx
rmmovq	%rdi, (%rax)
rmmovq	%rdi, 15(%rax)
mrmovq  15(%rax), %rdi
rmmovq	%rdi, 0x15(%rax)
mrmovq  0x15(%rax), %rdi
mrmovq  (%rax), %rdi
