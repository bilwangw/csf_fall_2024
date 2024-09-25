/* hello.S */

	.section .rodata
sHelloMsg: .string "Hello, world\n"

	.section .text

	.globl main
main:
	subq $8, %rsp
    mov $1, %r12
    .forloop:
        inc %r12
	    //mov %r12, %rdi
        mov $sHelloMsg, %rdi
        call printf
        cmp $10, %r12
        jg .end
        jmp .forloop
    .end:
	    movl $0, %eax
	    addq $8, %rsp
	    ret
             