/* hello.S */

	.section .rodata
sHelloMsg: .string "Hello, world\n"
numFormat: .string "%u\n"
	.section .text

	.globl main
main:
	subq $8, %rsp
    mov $1, %r12
    .forloop:
	    mov %r12, %rsi
        mov $numFormat, %rdi
        call printf
        cmp $10, %r12
        
        jge .end
        inc %r12
        jmp .forloop
    .end:
	    movl $0, %eax
	    addq $8, %rsp
	    ret
             