//userspace does not need these, because it has access to SSE registers and GCC knows how to use atomics there
//the kernel currently only seems to use these two

.global __atomic_load_8
.global __atomic_fetch_add_8

//cmpxchg8b is the only ia-32 8-byte atomic that's not xmm-related
__atomic_fetch_add_8:
pushl %ebx
pushl %esi
movl 12(%esp), %esi //target address
xorl %eax, %eax
xorl %edx, %edx
1:
movl %eax, %ebx
addl 16(%esp), %ebx
movl %edx, %ecx
addl 20(%esp), %ecx
cmpxchg8b (%esi)
jnz 1b
popl %esi
popl %ebx
ret

//same overall idea for load
__atomic_load_8:
pushl %ebx
pushl %esi
movl 12(%esp), %esi //target address
xorl %eax, %eax
xorl %edx, %edx
1:
movl %eax, %ebx
movl %edx, %ecx
cmpxchg8b (%esi)
jnz 1b
popl %esi
popl %ebx
ret
