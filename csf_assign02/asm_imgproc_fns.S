/*
 * x86-64 assembly language implementations of functions
 */
	.section .rodata

	dimFormat:
		.string "%u\n"

	.section .text

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/*
 * TODO: define your helper functions here.
 * Don't forget to use the .globl directive to make
 * them visible to the test code.
 */
 .globl get_r
get_r:
/* this gets the r value out of the pixel
 * input is just the 32 bit encoded pixel value
 *
 *
 * Parameters:
 *
 * %rdi - input
 * 
 * Returns:
 * %rax
*/
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	push %rdx
	//get r by right shift 24
	mov %edi, %edx // move pixel val into edx
	shr $24, %edx // right shift 24 bits
	mov %edx, %eax // move value into rax for output
	// epilogue
	pop %rdx
	addq $8, %rsp
	popq %rbp
	ret
	
	
.globl get_g
get_g:
/* this gets the g value out of the pixel
 * input is just the 32 bit encoded pixel value
 *
 *
 * Parameters:
 *
 * %rdi - input
 * 
 * Returns:
 * %rax
*/
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	push %rdx
	//get b by left shift 8, right shift 24
	mov %edi, %edx // move pixel val into edx
	shl $8, %edx // left shift 8
	shr $24, %edx // right shift 24 bits
	mov %edx, %eax // move value into rax for output
	// epilogue
	pop %rdx
	addq $8, %rsp
	popq %rbp
	ret


.globl get_b
get_b:
/* this gets the b value out of the pixel
 * input is just the 32 bit encoded pixel value
 *
 *
 * Parameters:
 *
 * %rdi - input
 * 
 * Returns:
 * %rax
*/
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	push %rdx
	//get b by left shift 16, right shift 24
	mov %edi, %edx // move pixel val into edx
	shl $16, %edx // left shift 16
	shr $24, %edx // right shift 24 bits
	mov %edx, %eax // move value into rax for output
	// epilogue
	pop %rdx
	addq $8, %rsp
	popq %rbp
	ret

.global get_a
get_a:
/* this gets the a value out of the pixel
 * input is just the 32 bit encoded pixel value
 *
 * Parameters:
 *
 * %rdi - input
 * 
 * Returns:
 * %rax
*/
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	push %rdx
	//get a by left shift 24, right shift 24
	mov %edi, %edx // move pixel val into edx
	shl $24, %edx // left shift 24 bits
	shr $24, %edx // right shift 24 bits
	mov %edx, %eax // move value into rax for output
	// epilogue
	pop %rdx
	addq $8, %rsp
	popq %rbp
	ret
/*
.global blend_components_alt
blend_components_alt:
	// %rdi pixel value of foreground, %rsi pixel value of background, %rax will be the blended pixel value output
	// allocate registers to store pixel values
	pushq %r13 // store r
	pushq %r14 // store g
	pushq %r15 // other use 
	pushq %r12 // store background 
	push %r10

	xor %rax, %rax
	xor %rdx, %rdx
	movl %edi, %ebx // store pixel foreground pixel in ebx
	movl %esi, %r12d // store background pixel in r12d

	mov %ebx, %edi
	call get_a
	mov %eax, %r15d // store foreground value of a in %rdx argument

	mov %ebx, %edi // get r value of foreground, store in r14
	call get_r
	mov %eax, %r13d
	
	mov %r12d, %edi // get r value of background, store in esi arg for blend rgb
	call get_r
	mov %eax, %esi
	mov %r13d, %edi

	mov %r15d, %edx
	call blend_rgb // blend rgb for r values
	mov %eax, %r13d // store blended r value in r13

	mov %ebx, %edi // get g value of foreground, store in r10
	call get_g
	mov %eax, %r14d
	
	mov %r12d, %edi // get g value of background, store in esi arg for blend rgb
	call get_g
	mov %eax, %esi
	mov %r14d, %edi

	mov %r15d, %edx
	call blend_rgb // blend rgb for g values
	mov %eax, %r14d // store blended g value in r14

	mov %ebx, %edi // get b value of foreground, store in r10
	call get_b
	mov %eax, %ebx
	
	mov %r12d, %edi // get b value of background, store in esi arg for blend rgb
	call get_b
	mov %eax, %esi
	mov %ebx, %edi

	mov %r15d, %edx
	call blend_rgb // blend rgb for b values
	mov %eax, %r9d // store blended b value in r9d

	mov %r13d, %edi // r
	mov %r14d, %esi // g
	mov %r9d, %edx // b
	mov $255, %ecx // a
	
	call make_pixel

	pop %r10
	popq %r12
	popq %r15
	popq %r14
	popq %r13
	ret
*/

.global blend_components
blend_components:
/*
 * Parameters:
 * %rdi - pixel value of foreground 
 * %rsi - pixel value of background
 *
 * Returns:
 * %rax - blended pixel value output
*/
	// allocate registers to store pixel values
	pushq %r13 // store r
	pushq %r14 // store g
	pushq %r15 // store b
	pushq %r12 // store a
	push %r10

	xor %rax, %rax
	xor %rdx, %rdx
	movl %edi, %r8d // store pixel foreground pixel in r8
	movl %esi, %r9d // store background pixel in r9

	mov %r8d, %edi
	call get_a
	mov %eax, %r12d // store foreground value of a in %rdx argument

	mov %r8d, %edi // get r value of foreground, store in r10
	call get_r
	mov %eax, %r10d
	
	mov %r9d, %edi // get r value of background, store in esi arg for blend rgb
	call get_r
	mov %eax, %esi
	mov %r10d, %edi

	movl %r12d, %edx
	call blend_rgb // blend rgb for r values
	mov %eax, %r13d // store blended r value in r13

	mov %r8d, %edi // get g value of foreground, store in r10
	call get_g
	mov %eax, %r10d
	
	mov %r9d, %edi // get g value of background, store in esi arg for blend rgb
	call get_g
	mov %eax, %esi
	mov %r10d, %edi

	movl %r12d, %edx
	call blend_rgb // blend rgb for g values
	mov %eax, %r14d // store blended g value in r14

	mov %r8d, %edi // get b value of foreground, store in r10
	call get_b
	mov %eax, %r10d
	
	mov %r9d, %edi // get b value of background, store in esi arg for blend rgb
	call get_b
	mov %eax, %esi
	mov %r10d, %edi

	movl %r12d, %edx
	call blend_rgb // blend rgb for b values
	mov %eax, %r15d // store blended b value in r15

	mov %r13d, %edi // r
	mov %r14d, %esi // g
	mov %r15d, %edx // b
	mov $255, %ecx // a
	
	call make_pixel

	pop %r10
	popq %r12
	popq %r15
	popq %r14
	popq %r13
	ret


.global blend_rgb
blend_rgb:
/*
 * Parameters:
 * %rdi - (r, g, or b) value of foreground, 
 * %rsi - (r, g, or b) value of background, 
 * %rdx - alpha value of foreground
 * 
 * Returns:
 * %rax - blended pixel value
 * 
 * 
*/
	// push callee-saved
	pushq %r12
	pushq %r13
	// apply the formula (af+(255−a)b)/255
	imulq %rdx, %rdi // multiply a * f
	movq $255, %r12 // allocate 255 to r12
	subq %rdx, %r12 // 255 - a
	imulq %r12, %rsi // (255 - a) * b
	addq %rsi, %rdi // af + (255 - a)b
	movq %rdi, %rax // move value to dividend
	movq $255, %rbx // move 255 into divisor
	cqo
	idivq %rbx // (af + (255 - a)b)/255

	// pop callee-saved
	popq %r13
	popq %r12
	ret


.global make_pixel
make_pixel:
/* this encodes a pixel value given rgba values
 *
 * Parameters:
 *
 * %rdi - r
 * %rsi - g
 * %rdx - b
 * %rcx - a
 *
 *
 * Returns:
 * %rax
*/
	// prologue
	
	subq $8, %rsp
	mov $0, %eax // initialize output to 0
	shl $24, %rdi // left shift r to be in the 8 most significant bits
	shl $16, %rsi // left shift g to be in the second group of 8 most significant bits
	shl $8, %rdx // left shift b to be in the third group of 8 most significant bits
	add %edi, %eax // add left shifted r to output
	add %esi, %eax // add left shifted g to output
	add %edx, %eax // add left shifted b to output
	add %ecx, %eax // add a to output
	// epilogue
	addq $8, %rsp
	ret


.global grayscale
grayscale:
/* this encodes a pixel value given rgba values
 *
 * Parameters:
 *
 * %rdi - pixel value
 *
 *
 * Returns:
 * %rax
*/
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	// get the rgba values and move to registers to be input args for make pixel
	call get_g
	mov %eax, %esi
	call get_b
	mov %eax, %edx
	call get_a
	mov %eax, %ecx
	call get_r
	mov %eax, %edi // do r last so rdi does not interfere with inputs of gba
	// grayscale formula y=(79*r+128*g+49*b)/256
	imul $79, %edi
	imul $128, %esi
	imul $49, %edx
	// add to ebx for calculations
	mov $0, %ebx
	add %edi, %ebx
	add %esi, %ebx
	add %edx, %ebx
	shr $8, %ebx // divide by 256
	mov %ebx, %eax // move the calculated value to ebx
	mov %eax, %edi // set the r pixel to the calculated value
	mov %eax, %esi // set the g pixel to the calculated value
	mov %eax, %edx // set the b pixel to the calculated value
	call make_pixel // encode into pixel
	// epilogue
	addq $8, %rsp
	popq %rbp
	ret

/*
 * Implementations of API functions
 */

/*
 * void imgproc_mirror_h( struct Image *input_img, struct Image *output_img );
 *
 * Transform image by mirroring its pixels horizontally.
 * This transformation always succeeds.
 *
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %rsi - pointer to output struct Image
 *
 * Usage:
 *   %r12 - width
 *   %r13 - height
 *	 %ebx - x iterator
 *   %ecx - y iterator
 *
 */
	.globl imgproc_mirror_h

imgproc_mirror_h:
	/*
	 * Register use:
 	*   %r12 - pointer to output Image
 	*   %r13 - pointer to source Image
 	*   %r14 - width
 	*   %r15 - height
 	*   %rdx - calculations
	*	%rbx - x iterator
	*	%rcx - y iterator
 	*
 	*/
	// push used callee registers
	pushq %rbp
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13

	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15

	mov $0, %rdx // set rdx to 0
	add %r14d, %edx // add r14d to 0
	mov %rdx, %r14 // move edx to r14, making r14d 64 bit
	mov $0, %rdx // set rdx to 0
	add %r15d, %edx // add r15d to 0
	mov %rdx, %r15 // move edx to r15, making r15d 64 bit
	mov 8(%rdi), %r12 // move address of input image pixel into r12
	mov 8(%rsi), %r13 // move address of output image pixel into r13

	// move output pointer to the end of the row
	mov $0, %rdx // set rdx to 0
	add %r14, %rdx // add width to rdx
	sub $1, %rdx // subtract one from width to prevent off by one error
	shl $2, %rdx // multiply by 4 for byte size
	add %rdx, %r13 // add rdx offset onto output pointer address, output pointer now points to end of first row
	
	mov $0, %ecx // y iterator to 0

	// start y loop
	.y_loop_mir_h:
		mov $0, %ebx // x iterator to 0
		.x_loop_mir_h: // start x loop
			
			mov (%r12), %r8d // move the value at the input pixel to r8d
			mov %r8d, (%r13) // move the value of the input pixel to the address of the output pixel
			
			add $4, %r12 // increment input address
			sub $4, %r13 // decrement output address
			
			inc %ebx // increment x iterator
			cmp %r14d, %ebx // check if x iterator is out of bounds
			jl .x_loop_mir_h // if not, continue looping
			//if reached end of row:
			mov $0, %rdx // set rdx to 0 for calculations
			// add 2 * 4 * width to the current output address to move the pointer to the end of the next row
			mov %r14, %rdx // set rdx to width
			shl $3, %rdx // width * 8 to adjust for 4 byte size and 2 times width to offset the pointer properly
			add %rdx, %r13 // add the offset onto the address of the output pointer

	.y_loop_end_mir_h:
		inc %ecx // increment y iterator
		cmp %r15d, %ecx // check if y iterator is out of bounds
		jl .y_loop_mir_h // if not, continue looping

	// pop used registers
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	// reset ecx
	mov $0, %ecx
	popq %rbp
	ret


/*
 * void imgproc_mirror_v( struct Image *input_img, struct Image *output_img );
 *
 * Transform image by mirroring its pixels vertically.
 * This transformation always succeeds.
 *
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %rsi - pointer to output struct Image
 */
	.globl imgproc_mirror_v
imgproc_mirror_v:
	/*
	 * Register use:
	*	%r10d - row index input
	*	%r11d - row index output
 	*   %r12 - pointer to output Image
 	*   %r13 - pointer to source Image
 	*   %r14 - width
 	*   %r15 - height
 	*   %rdx - calculations
	*	%rcx - y iterator
	*	%rbx - x iterator
 	*
 	*/

	subq $8, %rsp
	// push used callee registers
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	pushq %r11
	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15
	mov $0, %rdx // set rdx to 0
	add %r14d, %edx // add r14d to 0
	mov %rdx, %r14 // move edx to r14, making r14d 64 bit
	mov $0, %rdx // set rdx to 0
	add %r15d, %edx // add r15d to 0
	mov %rdx, %r15 // move edx to r15, making r15d 64 bit
	mov 8(%rdi), %r12 // move address of input image pixel into r12
	mov 8(%rsi), %r13 // move address of output image pixel into r13

	// set output pointer to the last row
	mov %r14, %rdx // set rdx to width
	imul %r15, %rdx // multiply width by height
	sub %r14, %rdx // subtract one width, so offset is now to the last row
	shl $2, %rdx // multiply by 4 to adjust for 4 byte size
	add %rdx, %r13 // add offset to output pointer address

	mov $0, %rcx // y iterator to 0

	// y loop start
	.y_loop_mir_v:
		mov $0, %rbx // x iterator to 0
		// x loop start
		.x_loop_mir_v:

			movl (%r12), %r8d // move the value at the input pixel to r8d
			movl %r8d, (%r13) // move the value of the input pixel to the address of the output pixel
			
			add $4, %r12 // increment input address
			add $4, %r13 // increment output offset
			
			inc %ebx // increment x iterator
			cmp %r14d, %ebx // check if x iterator is out of bounds
			jl .x_loop_mir_v // if not, continue looping
			//if reached end of row:
			// decrement output pointer by one pixel
			sub $4, %r13
			mov $0, %rdx // set rdx to zero to start calculations
			// subtact 2 * 4 * width + 4 to go up a row for the output
			mov %r14, %rdx // set width to rdx
			shl $3, %rdx // multiple width by 2 * 4 bytes size
			sub $4, %rdx // subtract 1 pixel
			sub %rdx, %r13 // subtract this offset from output pointer, moving up a row and starting from the left side

	.y_loop_end_mir_v:
		inc %ecx // increment y iterator
		cmp %r15d, %ecx // check if y iterator is out of bounds
		jl .y_loop_mir_v // if not, continue looping

	// pop used callee registers
	popq %r11
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	mov $0, %ecx
	addq $8, %rsp

	
	ret


/*
 * int imgproc_tile( struct Image *input_img, int n, struct Image *output_img );
 *
 * Transform image by generating a grid of n x n smaller tiles created by
 * sampling every n'th pixel from the original image.
 *
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %esi - tiling factor (how many rows and columns of tiles to generate)
 *   %rdx - pointer to the output Image (in which the transformed
 *          pixels should be stored)
 *
 * Returns (in %eax):
 *   1 if successful, or 0 if either
 *     - n is less than 1, or
 *     - the output can't be generated because at least one tile would
 *       be empty (i.e., have 0 width or height)
 */
	.globl imgproc_tile
imgproc_tile:
	/* TODO: implement (this stub implementation just returns 0) */
	xorl %eax, %eax
	ret

/*
 * void imgproc_grayscale( struct Image *input_img, struct Image *output_img );
 *
 * Transform image by converting each pixel to grayscale.
 * This transformation always succeeds.
 *
 * Register use:
 *  %r12 - pointer to output Image
 *  %r13 - pointer to source Image
 *  %r14 - width
 *  %r15 - height
 *  %rdx - calculations
 *	%r11 - loop iterator
 *
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %rsi - pointer to output struct Image
 */
	.globl imgproc_grayscale
imgproc_grayscale:
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	// push callee registers
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	
	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15
	mov 8(%rdi), %r12 // move address of input image pixel into r12
	mov 8(%rsi), %r13 // move address of output image pixel into r13

	mov $0, %r11d
	.loop:
		mov (%r12), %r8d // store value at %r12 into %r8
		// compute the grayscale transformation using grayscale function
		mov %r8d, %edi // move pixel val into function input arg
		call grayscale
		mov %eax, (%r13) // move grayscale output to value of output pixel
		add $4, %r12 // increment input pixel
		add $4, %r13 // increment output pixel

		inc %r11d // increment loop counter
		// check if loop within bounds
		mov %r15d, %edx // set edx to width
		imul %r14d, %edx // multiply width * height
		cmp %edx, %r11d // check if loop counter is less than the amount of pixels in the image (width * height)
		jl .loop // continue looping if so
	
	// pop callee registers
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	mov $0, %eax
	// epilogue
	addq $8, %rsp
	popq %rbp
	
	ret

/*
 * int imgproc_composite( struct Image *base_img, struct Image *overlay_img );
 *
 * Overlay a foreground image on a background image, using each foreground
 * pixel's alpha value to determine its degree of opacity in order to blend
 * it with the corresponding background pixel.
 *
 * Parameters:
 *   %rdi - pointer to base (background) image
 *   %rsi - pointer to overlaid (foreground) image
 *   %rdx - pointer to output Image
 *
 * Returns:
 *   1 if successful, or 0 if the transformation fails because the base
 *   and overlay image do not have the same dimensions
 */
	.globl imgproc_composite
imgproc_composite:
	// prologue
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	// push callee registers
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	pushq %r11
	pushq %r10
	
	xor %r14, %r14
	xor %r15, %r15
	mov 0(%rdi), %r14d // assign foreground width to %r14
	mov 4(%rdi), %r15d // assign foreground height %r15
	mov 0(%rsi), %r12d // temporarily assign background width to test
	mov 4(%rsi), %r13d // temporarily assign background height to test

	cmp %r14d, %r12d // check if widths are equal for both images
	jne .composite_fail
	cmp %r15d, %r13d // check if heights are equal for both images
	jne .composite_fail

	xor %r13, %r13 // clear r13
	xor %r12, %r12 // clear r12
	mov 8(%rsi), %r12 // move address of foreground image pixel into r12
	mov 8(%rdi), %r13 // move address of background image pixel into r13
	mov 8(%rdx), %r11 // move address of output image pixel into r11

	mov $0, %r10d
	.composite:
		mov (%r12), %r8d // store value of foreground image pixel into %r8
		mov (%r13), %r9d // store value of background image pixel into %r9
		// compute the composite transformation using composite function
		mov %r8d, %edi // move foreground pixel val into function input arg
		mov %r9d, %esi // move background pixel val into function input arg
		call blend_components // run blend components
		mov %eax, (%r11) // move composite output to value of output pixel
		add $4, %r12 // increment foreground pixel
		add $4, %r13 // increment background pixel
		add $4, %r11 // increment output image

		inc %r10d // increment loop counter
		// check if loop within bounds
		mov %r15d, %edx // set edx to width
		imul %r14d, %edx // multiply width * height
		cmp %edx, %r10d // check if loop counter is less than the amount of pixels in the image (width * height)
		jl .composite // continue looping if so
	
	mov $1, %rax // return 1 if successful (this should run when loop exits normally)

	jmp .composite_end // jump over fail state if running normally

	.composite_fail: // this should only run if dimensions are unequal
	mov $0, %rax // return 0 if unsuccessful

	.composite_end:
	popq %r10
	popq %r11
	// pop callee registers
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	// epilogue
	addq $8, %rsp
	popq %rbp
	ret

/*
vim:ft=gas:
*/
