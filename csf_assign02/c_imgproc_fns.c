// C implementations of image processing functions

#include <stdlib.h>
#include <assert.h>
#include "imgproc.h"

#include <stdio.h>

// TODO: define your helper functions here
// helper functions to get rgba values from a pixel, specifying the index
// values are obtained by bit shifting to get rid of data on the right, and using modulus to get rid of data on the left
uint32_t get_r(struct Image *input_img, int i) {
    return (input_img->data[i] >> 24) % (1<<8);
}
uint32_t get_g(struct Image *input_img, int i) {
    return (input_img->data[i] >> 16) % (1<<8);
}
uint32_t get_b(struct Image *input_img, int i) {
    return (input_img->data[i] >> 8) % (1<<8);
}
uint32_t get_a(struct Image *input_img, int i) {
    return (input_img->data[i]) % (1<<8);
}

// Encode a pixel value from rgba values using bit shift
uint32_t make_pixel(uint32_t r,uint32_t g,uint32_t b,uint32_t a) {
  return (r << 24) + (g << 16) + (b << 8) + a;
}

// Convert a pixel in an image to grayscale, and return the encoded pixel value
uint32_t to_grayscale(struct Image *input_img, int i) {
  uint32_t target = (79 * get_r(input_img, i) + 128 * get_g(input_img, i) + 49 * get_b(input_img, i))/256;
  return make_pixel(target, target, target, get_a(input_img, i));
}

// Take a pixel in a foreground and background image, and combine them using the given overlay formula, return the encoded pixel value
uint32_t to_composite(struct Image *fg, struct Image *bg, int i) {
    uint32_t foreground_a = get_a(fg, i);
    uint32_t target_r = (foreground_a * get_r(fg, i) + (255-foreground_a)*get_r(bg,i))/255;
    uint32_t target_g = (foreground_a * get_g(fg, i) + (255-foreground_a)*get_g(bg,i))/255;
    uint32_t target_b = (foreground_a * get_b(fg, i) + (255-foreground_a)*get_b(bg,i))/255;
    return make_pixel(target_r, target_g, target_b, 255);
}

// Mirror input image horizontally.
// This transformation always succeeds.
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
void imgproc_mirror_h( struct Image *input_img, struct Image *output_img ) {
  //copy the width and height
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
  // assign pixels starting from the right edge of the input to the left edge of the output
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      output_img->data[i*width+j] = input_img->data[i*width+(width-j-1)];
    }
  }
}

// Mirror input image vertically.
// This transformation always succeeds.
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
void imgproc_mirror_v( struct Image *input_img, struct Image *output_img ) {
  // copy the width and height
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
  // assign pixels starting from the bottom edge of the input to the top edge of the output
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      output_img->data[i*width+j] = input_img->data[(height-i-1) * width+j];
    }
  }
}

// Transform image by generating a grid of n x n smaller tiles created by
// sampling every n'th pixel from the original image.
//
// Parameters:
//   input_img  - pointer to original struct Image
//   n          - tiling factor (how many rows and columns of tiles to generate)
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
// Returns:
//   1 if successful, or 0 if either
//     - n is less than 1, or
//     - the output can't be generated because at least one tile would
//       be empty (i.e., have 0 width or height)
int imgproc_tile( struct Image *input_img, int n, struct Image *output_img ) {
  // check if there would be empty tiles or if n is less than one
  if (n < 1 || n > input_img->width || n > input_img->height) {
    return 0;
  }
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);

  // get the excess pixels if the n does not divide evenly into the image
  int h_mod = height % n;
  int w_mod = width % n;
  // iterate through the input image and assign the appropriate pixel to the output
  // to scale down by n times, get every nth pixel in the x and y
  for (int i = 0; i < height; i += n) {
    for (int j = 0; j < width; j += n) {
      // define variables to handle the offset of the tiles
      int w_counter = 0;
      int h_counter = 0;
      // k and l iterate to create n * n copies of the image
      for (int k = 0; k < n; k++) {
        // use xoffset and yoffset to offset the sampled pixels once the excess pixels are used up
        int xoffset = 0;
        int yoffset = 0;
        int add = 0;
        // check if this tile will have excess pixels on the x axis
        if (k < w_mod - 1) {
          add = 1;
        } else {
          add = 0;
        }
        h_counter = 0;
        for (int l = 0; l < n; l++) {
          int add2 = 0;
          // check if this tile will have excess pixels on the y axis
          if (l < h_mod - 1) {
            add2 = 1;
          }
          // check if the current tile is outside of the area with excess pixels, adding x and y sampling offsets as necessary
          if(k >= w_mod && j >= n && w_mod != 0) {
            xoffset = 1;
          }
          if(l >= h_mod && i >= n && h_mod != 0) {
            yoffset = 1;
          }
          // assign the correct pixel value to the output image
          output_img->data[(j)/n + (i*width)/n + w_counter + width * (h_counter)] = input_img->data[j-xoffset*n+(i-yoffset*n)*width]; 
          // iterate the height offset to reach the next row
          h_counter += (height/n) + add2;
        }
        // iterate the width offset to reach the next column
        w_counter += (width/n) + add;
      }
    }
  }
  // return 1 if successful
  return 1;
}

// Convert input pixels to grayscale.
// This transformation always succeeds.
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
void imgproc_grayscale( struct Image *input_img, struct Image *output_img ) {
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
  // iterate through all pixels in the image, applying grayscale function and assign the pixel to the output image
  for (int i = 0; i < height*width; i++) {
    output_img->data[i] = to_grayscale(input_img, i);
  }
}

// Overlay a foreground image on a background image, using each foreground
// pixel's alpha value to determine its degree of opacity in order to blend
// it with the corresponding background pixel.
//
// Parameters:
//   base_img - pointer to base (background) image
//   overlay_img - pointer to overlaid (foreground) image
//   output_img - pointer to output Image
//
// Returns:
//   1 if successful, or 0 if the transformation fails because the base
//   and overlay image do not have the same dimensions
int imgproc_composite( struct Image *base_img, struct Image *overlay_img, struct Image *output_img ) {
  // check if the dimensions of the two images are the same
  if (base_img->width != overlay_img->width || base_img->height != overlay_img->height) {
    return 0;
  }
  int width = base_img->width;
  int height = base_img->height;
  img_init(output_img, width, height);
  // iterate through the image, assigning the generated overlayed pixel to output
  for (int i = 0; i < height*width; i++) {
    output_img->data[i] = to_composite(overlay_img, base_img, i);
  }
  return 1;
}