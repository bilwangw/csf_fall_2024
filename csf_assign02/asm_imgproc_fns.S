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
	pushq %rbp
	movq %rsp, %rbp
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
	popq %rbp
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
	* Memory use:
 	*   -8(%rbp) - pixel value of input
 	*   -16(%rbp) - address output pixel location
 	*   -12(%rbp) - 
 	*   -16(%rbp) - 
 	*   -24(%rbp) - first element in column
 	*   -32(%rbp) - column
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
			
			inc % // increment x iterator
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
	* Memory use:
 	*   -8(%rbp) - pixel value of input
 	*   -16(%rbp) - address output pixel location
 	*   -12(%rbp) - 
 	*   -16(%rbp) - 
 	*   -24(%rbp) - first element in column
 	*   -32(%rbp) - column
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
 * Memory use:
 *   -8(%rbp) - pixel value of input
 *   -16(%rbp) - address output pixel location
 *   -12(%rbp) - 
 *   -16(%rbp) - 
 *   -24(%rbp) - first element in column
 *   -32(%rbp) - column
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
	/* TODO: implement (this stub implementation just returns 0) */
	xorl %eax, %eax
	ret

/*
vim:ft=gas:
*/
