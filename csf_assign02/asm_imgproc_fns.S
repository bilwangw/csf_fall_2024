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
 *	 %eax - x iterator
 *   %ebx - y iterator
 *
 */
	.globl imgproc_mirror_h

imgproc_mirror_h:
	/*
	 * Register use:
	*	%r10d - row index input
	*	%r11d - row index output
 	*   %r12 - pointer to output Image
 	*   %r13 - pointer to source Image
 	*   %r14d - width
 	*   %r15d - height
 	*   %ebx - something else
 	*
	* Memory use:
 	*   -8(%rbp) - pixel value of input
 	*   -16(%rbp) - address output pixel location
 	*   -12(%rbp) - 
 	*   -16(%rbp) - 
 	*   -24(%rbp) - first element in column
 	*   -32(%rbp) - column
 	*/

	/*--------------------------------------Pseudocode------------------------------------------

	indexing math:
	(output pixel array start address + (4 * width - 4 * x) + width * y)
	(input pixel array start address + 4 * x + 4 * width * y)
	allocate stack memory

	get width
	get height

	set x iterator to 0
	start the xy loop
		x loop:
			set y iterator to 0
			put input start address into IN // start doing math for indexing
			add 4 * x to IN
			put output start address into OUT // mirrored x pixel for output
			add 4 * width to OUT
			subtract 4 * x from OUT
			y loop:
				add 4 * width to IN // add the y offset
				add 4 * width to OUT
				get the value at address IN and put that value into the value at address OUT
				increment y iterator
				check if y iterator is out of bounds
				jump back to y loop if in bounds
		x loop end:
			increment x iterator
			check if x iterator is out of bounds
			jump back to x loop if in bounds


									   
	------------------------------------------------------------------------------------------*/
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	//pushq %r8
	//movq %rsp, %rbp
	subq $8, %rsp // allocate stack memory
	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15
	//sub $1, %r14
	//sub $1, %r15
	lea 8(%rdi), %r12 // move address of input image pixel into r12
	lea 8(%rsi), %r13 // move address of output image pixel into r13

	mov $0, %eax // y iterator to 0
	.loop:
		inc %eax
		//mov %r12, %r13
		mov (%r12), %r8d
		mov %r8d, (%r13)
		add $4, %r12
		add $4, %r13
		cmp $10, %eax
		jl .loop
	
	// move output pointer to the end of the width
/*
	mov %r14d, %edx // getting 4 * width
	sub $1, %edx
	shl $2, %edx
	add %edx, %r13d
	add $1, %edx

	.y_loop:
		mov $0, %ebx // x iterator to 0

		.x_loop:
			
			mov (%r12), %r8d
			mov %r8d, (%r13)
			
			add $4, %r12
			//sub $4, %r13
			
			inc %ebx
			cmp %r14d, %ebx // check if x iterator is out of bounds
			jl .x_loop // if not, continue looping

			//add 4 to r12 to get first element of next row
			add $4, %r12
			mov $0, %edx
			mov %r14d, %edx // getting 4 * width
			sub $1, %edx
			shl $3, %edx
			add %edx, %r13d

	.y_loop_end:
		inc %eax // increment y iterator
		cmp %r15d, %eax // check if y iterator is out of bounds
		jl .y_loop // if not, continue looping
*/
	//popq %r8
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	mov $0, %eax
	addq $8, %rsp
	//popq %rbp
	ret
	
	
	
	
	/*----------- Commented out old code

	// Beginnning of xy iterator

	//mov $0, %rax // set x iterator to 0

	sub $1, %r14d
	mov $0, %r10d // assign row ind of input
	mov %r14d, %r11d //assign row ind of output
	add $1, %r14d
	
	//will put this in loop
	lea (%r12, %r11, 4), %rbx //store address of pixel output location
	mov (%r13, %r10, 4), %r9d
	mov %r9, (%rbx)//move the value into target output location


	
	x_loop:
	movl $0, %ebx // set y iterator to 0 / reset y loop once it goes through one iteration
	//mov $0, %r14d // set y coordinate to 0
	add $4, %r13d // move pixel address by desired amount (r13 address += 1 pixel)
	sub $4, %r9d // decrement output address by 1 each time

	y_loop:
	// need to do assignment
	// move rdi pixel at (x + y * width) into rsi pixel at ((width - 1 - x) + y * width)
	lea (%r13d,%r14d,4), %edx // store the pixel address in r15 (r13 address + y * width)
	mov (%r15d), %edx
	mov %edx, %r9d // assign the pixel value at r15 (input pixel) to r9 (output pixel) (r9 = r15)

	inc %ebx // increment y iterator
	cmp %r15d, %ebx // compare y iterator and height
	add %ecx, %r14d // add width to y coordinate, moving down a row each time (calculating y * width)
	jl y_loop // jump back to start of y_loop if ebx < edx


	x_loop_end:
	inc %rax // increment x interator
	cmp %r12d, %eax // compare x iterator and width
	jl x_loop // jump back to start of x_loop if eax < ecx
	
	mov $0, %eax
	addq $8, %rsp // free stack memory
	ret
	*/

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
	/* TODO: implement */
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
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %rsi - pointer to output struct Image
 */
	.globl imgproc_grayscale
imgproc_grayscale:
	/* TODO: implement */
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
