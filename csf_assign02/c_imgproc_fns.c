// C implementations of image processing functions

#include <stdlib.h>
#include <assert.h>
#include "imgproc.h"

// TODO: define your helper functions here
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
// Mirror input image horizontally.
// This transformation always succeeds.
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
void imgproc_mirror_h( struct Image *input_img, struct Image *output_img ) {
  // TODO: implement
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
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
  // TODO: implement
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
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
  // TODO: implement
  if (n < 1 || n > input_img->width || n > input_img->height) {
    return 0;
  }
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
  int index = 0;
  uint32_t *data = (uint32_t * ) malloc((width) * (height) * sizeof(uint32_t));

  for(int j = 0; j < height; j+=n) {
    for (int i = 0; i < width; i+=n) {
      for (int k = 0; k < n; k++) {
        output_img->data[index]  = input_img->data[i + j*width/n];
      }
      // for (int k = 0; k < n*n; k++) { // rounding error probably
      // // 
      //   data[index + k * width/n] = input_img->data[i + j*width/n];
      // }
      index++;
      
    }
  }
  // for(int j = 0; j < width * height/n; j++) {
  //   //output_img->data[j] = data[j];
  //   for (int i = 0; i < n; i++) {
  //     output_img->data[j + i*height] = data[j];
  //   }
  // }
  free(data);


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
  // TODO: implement
  int width = input_img->width;
  int height = input_img->height;
  img_init(output_img, width, height);
  for (int i = 0; i < height*width; i++) {
    // uint32_t original = input_img->data[i];
    // uint32_t mod = 1;
    // mod = mod << 8;
    // uint32_t a = original % mod;
    // original = original >> 8;
    // uint32_t b = original % mod;
    // original = original >> 8;
    // uint32_t g = original % mod;
    // original = original >> 8;
    // uint32_t r = original % mod;
    uint32_t target = (79 * get_r(input_img, i) + 128 * get_g(input_img, i) + 49 * get_b(input_img, i))/256;
    output_img->data[i] = (target << 24) + (target << 16) + (target << 8) + get_a(input_img, i);
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
  // TODO: implement
  if (base_img->width != overlay_img->width || base_img->height != overlay_img->height) {
    return 0;
  }
  int width = base_img->width;
  int height = base_img->height;
  img_init(output_img, width, height);
  for (int i = 0; i < height*width; i++) {
    uint32_t foreground_a = get_a(overlay_img, i);
    uint32_t target_r = (foreground_a * get_r(overlay_img, i) + (255-foreground_a)*get_r(base_img,i))/255;
    uint32_t target_g = (foreground_a * get_g(overlay_img, i) + (255-foreground_a)*get_g(base_img,i))/255;
    uint32_t target_b = (foreground_a * get_b(overlay_img, i) + (255-foreground_a)*get_b(base_img,i))/255;
    output_img->data[i] = (target_r << 24) + (target_g << 16) + (target_b << 8) + 255;
  }
  return 1;
}
