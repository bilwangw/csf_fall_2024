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
get_r:
/* this gets the r value out of the pixel
 * input is just the 32 bit encoded pixel value
 *
 *
 * Parameters:
 *
 * %rdi - input
 * %rsi - output
 *
*/
	push %rdx
	//get r by right shift 24
	mov %rdi, %edx // move pixel val into edx
	shr %24, %edx // right shift 24 bits
	mov %edx, %rsi // move value into rsi for output
	pop %rdx
	ret



get_b:
/* this gets the b value out of the pixel
 *
 *
*/

get_g:
/* this gets the g value out of the pixel
 *
 *
*/

get_a:
/* this gets the a value out of the pixel
 *
 *
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

	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	//pushq %r8
	//movq %rsp, %rbp
	subq $8, %rsp // allocate stack memory
	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15
	//sub $1, %r14 // subtract 1 to prevent overflow
	//sub $1, %r15 // subtract 1 to prevent overflow
	mov 8(%rdi), %r12 // move address of input image pixel into r12
	mov 8(%rsi), %r13 // move address of output image pixel into r13

	// move output pointer to the end of the width

	mov %r14d, %edx // getting 4 * width
	sub $1, %edx
	shl $2, %edx
	add %edx, %r13d
	add $1, %edx
	mov $0, %eax // y iterator to 0

	.y_loop_mir_h:
		mov $0, %ebx // x iterator to 0

		.x_loop_mir_h:
			
			mov (%r12), %r8d
			mov %r8d, (%r13)
			
			add $4, %r12 // increment input address
			sub $4, %r13 // decrement output address
			
			inc %ebx
			cmp %r14d, %ebx // check if x iterator is out of bounds
			jl .x_loop_mir_h // if not, continue looping
			//if reached end of row:
			//add 4 to r12 to get first element of next row
			add $4, %r12
			mov $0, %edx
			// add 2 * 4 * width to move the output to the end of the next row
			mov %r14d, %edx // edx = width
			shl $3, %edx // edx * 2^3
			sub $4, %edx // edx -= 4
			add %edx, %r13d

	.y_loop_end_mir_h:
		inc %eax // increment y iterator
		cmp %r15d, %eax // check if y iterator is out of bounds
		jl .y_loop_mir_h // if not, continue looping

	//popq %r8
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	mov $0, %eax
	addq $8, %rsp
	//popq %rbp
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
	pushq %r14
	pushq %r15
	pushq %r12
	pushq %r13
	
	//subq $8, %rsp // allocate stack memory
	mov 0(%rdi), %r14d // assign width to %r14
	mov 4(%rdi), %r15d // assign height %r15
	mov 8(%rdi), %r12 // move address of input image pixel into r12
	mov 8(%rsi), %r13 // move address of output image pixel into r13

	mov $0, %eax
	.loop:
		mov (%r12), %r8d // store value at %r12 into %r8
		// compute the grayscale transformation
		//y=(79*r+128*g+49*b)/256
		//get r by right shift 24
		mov %r8d, %edx // move pixel val into edx
		shr $24, %edx // right shift edx by 24 to get r
		mul $79, %edx
		mov %r8d, (%r13)
		add $4, %r12
		add $4, %r13

		inc %eax
		mov %r15d, %edx
		imul %r14d, %edx
		sub $1, %edx
		cmp %edx, %eax
		jl .loop
	
	popq %r13
	popq %r12
	popq %r15
	popq %r14
	mov $0, %eax
	//addq $8, %rsp
	//popq %rbp
	
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
