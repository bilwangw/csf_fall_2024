/*
 * x86-64 assembly language implementations of functions
 */

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
 *
 *
 */
	.globl imgproc_mirror_h
imgproc_mirror_h:
	/* TODO: implement */
	/* Store the width and height in r12 and r13 registers */
	subq $8, %rsp // allocate stack memory
	//movq 0(%rdi), %r12 // assign width
	//movq 4(%rdi), %r13 // assign height
	mov 0(%rdi), %ecx // assign width
	mov 4(%rdi), %edx // assign height
	movq 8(%rdi), %r14 // assign pixel value

	//--------------------------------------Pseudocode------------------------------------------
	// get pixel value and assign it to the other side		                                   |
	// use leaq to access specific array xy (it automatically does the i + j * width thing)    |
	// loop through x																		   |
	// 	  loop through y																	   |
	// 	      assign (-x,y) to (x,y)														   |
	//------------------------------------------------------------------------------------------

	x_loop_start: // start x loop
	movl $0, %eax // set x iterator to 0

	x_loop:
	movl $0, %ebx // set y iterator to 0 / reset y loop once it goes through one iteration

	y_loop:
	// need to do assignment
	// move rdi pixel at (x + y * width) into rsi pixel at (-x + y * width)

	y_loop_end:
	cmpl %edx, %ebx // compare y iterator and height
	jl y_loop // jump back to start of y_loop if ebx < r13

	x_loop_end:
	cmpl %ecx, %eax // compare x iterator and width
	jl x_loop // jump back to start of x_loop if eax < r12



	addq $8, %rsp // free stack memory
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
