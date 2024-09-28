#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tctest.h"
#include "imgproc.h"

#include <stdio.h>
// An expected color identified by a (non-zero) character code.
// Used in the "Picture" data type.
typedef struct {
  char c;
  uint32_t color;
} ExpectedColor;

// Type representing a "picture" of an expected image.
// Useful for creating a very simple Image to be accessed
// by test functions.
typedef struct {
  ExpectedColor colors[20];
  int width, height;
  const char *data;
} Picture;

// Some "basic" colors to use in test Pictures
#define TEST_COLORS \
    { \
      { ' ', 0x000000FF }, \
      { 'r', 0xFF0000FF }, \
      { 'g', 0x00FF00FF }, \
      { 'b', 0x0000FFFF }, \
      { 'c', 0x00FFFFFF }, \
      { 'm', 0xFF00FFFF }, \
    }

// Expected "basic" colors after grayscale transformation
#define TEST_COLORS_GRAYSCALE \
    { \
      { ' ', 0x000000FF }, \
      { 'r', 0x4E4E4EFF }, \
      { 'g', 0x7F7F7FFF }, \
      { 'b', 0x303030FF }, \
      { 'c', 0xB0B0B0FF }, \
      { 'm', 0x7F7F7FFF }, \
    }

// Colors for test overlay image (for testing the composite
// transformation). Has some fully-opaque colors,
// some partially-transparent colors, and a complete
// transparent color.
#define OVERLAY_COLORS \
  { \
    { 'r', 0xFF0000FF }, \
    { 'R', 0xFF000080 }, \
    { 'g', 0x00FF00FF }, \
    { 'G', 0x00FF0080 }, \
    { 'b', 0x0000FFFF }, \
    { 'B', 0x0000FF80 }, \
    { ' ', 0x00000000 }, \
  }

// Data type for the test fixture object.
// This contains data (including Image objects) that
// can be accessed by test functions. This is useful
// because multiple test functions can access the same
// data (so you don't need to create/initialize that
// data multiple times in different test functions.)
typedef struct {
  // smiley-face picture
  Picture smiley_pic;

  // original smiley-face Image object
  struct Image *smiley;

  // empty Image object to use for output of
  // transformation on smiley-face image
  struct Image *smiley_out;

  // Picture for overlay image (for basic imgproc_composite test)
  Picture overlay_pic;

  // overlay image object
  struct Image *overlay;
} TestObjs;

// Functions to create and clean up a test fixture object
TestObjs *setup( void );
void cleanup( TestObjs *objs );

// Helper functions used by the test code
struct Image *picture_to_img( const Picture *pic );
uint32_t lookup_color(char c, const ExpectedColor *colors);
bool images_equal( struct Image *a, struct Image *b );
void destroy_img( struct Image *img );

// Test functions
void test_mirror_h_basic( TestObjs *objs );
void test_mirror_v_basic( TestObjs *objs );
void test_tile_basic( TestObjs *objs );
void test_grayscale_basic( TestObjs *objs );
void test_composite_basic( TestObjs *objs );

// TODO: add prototypes for additional test functions
void test_get_red();
void test_get_green();
void test_get_blue();
void test_get_a();
void test_composite();
void test_grayscale();
void test_make_pixel();

int main( int argc, char **argv ) {
  // allow the specific test to execute to be specified as the
  // first command line argument
  if ( argc > 1 )
    tctest_testname_to_execute = argv[1];

  TEST_INIT();

  // Run tests.
  // Make sure you add additional TEST() macro invocations
  // for any additional test functions you add.
  TEST( test_mirror_h_basic );
  TEST( test_mirror_v_basic );
  //TEST( test_tile_basic );
  TEST( test_grayscale_basic );
  //TEST( test_composite_basic );
  TEST( test_get_red );
  TEST( test_get_green );
  TEST( test_get_blue );
  TEST( test_get_a );
  //TEST( test_grayscale );
  //TEST( test_make_pixel );
  // TEST( test_composite );

  TEST_FINI();
}

////////////////////////////////////////////////////////////////////////
// Test fixture setup/cleanup functions
////////////////////////////////////////////////////////////////////////

TestObjs *setup( void ) {
  TestObjs *objs = (TestObjs *) malloc( sizeof(TestObjs) );

  Picture smiley_pic = {
    TEST_COLORS,
    16, // width
    10, // height
    "    mrrrggbc    "
    "   c        b   "
    "  r   r  b   c  "
    " b            b "
    " b            r "
    " g   b    c   r "
    "  c   ggrb   b  "
    "   m        c   "
    "    gggrrbmc    "
    "                "
  };
  objs->smiley_pic = smiley_pic;
  objs->smiley = picture_to_img( &smiley_pic );

  objs->smiley_out = (struct Image *) malloc( sizeof( struct Image ) );
  img_init( objs->smiley_out, objs->smiley->width, objs->smiley->height );

  Picture overlay_pic = {
    OVERLAY_COLORS,
    16, 10,
   "                "
   "                "
   "                "
   "                "
   "                "
   "  rRgGbB        "
   "                "
   "                "
   "                "
   "                "
  };
  objs->overlay_pic = overlay_pic;
  objs->overlay = picture_to_img( &overlay_pic );

  return objs;
}

void cleanup( TestObjs *objs ) {
  destroy_img( objs->smiley );
  destroy_img( objs->smiley_out );
  destroy_img( objs->overlay );

  free( objs );
}

////////////////////////////////////////////////////////////////////////
// Test code helper functions
////////////////////////////////////////////////////////////////////////

struct Image *picture_to_img( const Picture *pic ) {
  struct Image *img;

  img = (struct Image *) malloc( sizeof(struct Image) );
  img_init( img, pic->width, pic->height );

  for ( int i = 0; i < pic->height; ++i ) {
    for ( int j = 0; j < pic->width; ++j ) {
      int index = i * img->width + j;
      uint32_t color = lookup_color( pic->data[index], pic->colors );
      img->data[index] = color;
    }
  }

  return img;
}

uint32_t lookup_color(char c, const ExpectedColor *colors) {
  for (int i = 0; ; i++) {
    assert(colors[i].c != 0);
    if (colors[i].c == c) {
      return colors[i].color;
    }
  }
}

// Returns true IFF both Image objects are identical
bool images_equal( struct Image *a, struct Image *b ) {
  if ( a->width != b->width || a->height != b->height )
    return false;

  int num_pixels = a->width * a->height;
  for ( int i = 0; i < num_pixels; ++i ) {
    if ( a->data[i] != b->data[i] )
      return false;
  }

  return true;
}

void destroy_img( struct Image *img ) {
  if ( img != NULL )
    img_cleanup( img );
  free( img );
}

////////////////////////////////////////////////////////////////////////
// Test functions
////////////////////////////////////////////////////////////////////////

void test_mirror_h_basic( TestObjs *objs ) {
  Picture smiley_mirror_h_pic = {
    TEST_COLORS,
    16, 10,
    "    cbggrrrm    "
    "   b        c   "
    "  c   b  r   r  "
    " b            b "
    " r            b "
    " r   c    b   g "
    "  b   brgg   c  "
    "   c        m   "
    "    cmbrrggg    "
    "                "
  };
  struct Image *smiley_mirror_h_expected = picture_to_img( &smiley_mirror_h_pic );

  imgproc_mirror_h( objs->smiley, objs->smiley_out );

  ASSERT( images_equal( smiley_mirror_h_expected, objs->smiley_out ) );

  destroy_img( smiley_mirror_h_expected );
}

void test_mirror_v_basic( TestObjs *objs ) {
  Picture smiley_mirror_v_pic = {
    TEST_COLORS,
    16, 10,
    "                "
    "    gggrrbmc    "
    "   m        c   "
    "  c   ggrb   b  "
    " g   b    c   r "
    " b            r "
    " b            b "
    "  r   r  b   c  "
    "   c        b   "
    "    mrrrggbc    "
  };
  struct Image *smiley_mirror_v_expected = picture_to_img( &smiley_mirror_v_pic );

  imgproc_mirror_v( objs->smiley, objs->smiley_out );

  ASSERT( images_equal( smiley_mirror_v_expected, objs->smiley_out ) );

  destroy_img( smiley_mirror_v_expected );
}

void test_tile_basic( TestObjs *objs ) {
  Picture smiley_tile_3_pic = {
    TEST_COLORS,
    16, 10,
    "  rg    rg   rg "
    "                "
    "  gb    gb   gb "
    "                "
    "  rg    rg   rg "
    "                "
    "  gb    gb   gb "
    "  rg    rg   rg "
    "                "
    "  gb    gb   gb "
  };
  struct Image *smiley_tile_3_expected = picture_to_img( &smiley_tile_3_pic );

  int success = imgproc_tile( objs->smiley, 3, objs->smiley_out );
  ASSERT( success );
  ASSERT( images_equal( smiley_tile_3_expected, objs->smiley_out ) );

  destroy_img( smiley_tile_3_expected );
}

void test_grayscale_basic( TestObjs *objs ) {
  Picture smiley_grayscale_pic = {
    TEST_COLORS_GRAYSCALE,
    16, // width
    10, // height
    "    mrrrggbc    "
    "   c        b   "
    "  r   r  b   c  "
    " b            b "
    " b            r "
    " g   b    c   r "
    "  c   ggrb   b  "
    "   m        c   "
    "    gggrrbmc    "
    "                "
  };

  struct Image *smiley_grayscale_expected = picture_to_img( &smiley_grayscale_pic );

  imgproc_grayscale( objs->smiley, objs->smiley_out );

  ASSERT( images_equal( smiley_grayscale_expected, objs->smiley_out ) );

  destroy_img( smiley_grayscale_expected );
}

void test_composite_basic( TestObjs *objs ) {
  imgproc_composite( objs->smiley, objs->overlay, objs->smiley_out );

  // for all of the fully-transparent pixels in the overlay image,
  // the result image should have a pixel identical to the corresponding
  // pixel in the base image
  for ( int i = 0; i < 160; ++i ) {
    if ( objs->overlay->data[i] == 0x00000000 )
      ASSERT( objs->smiley->data[i] == objs->smiley_out->data[i] );
  }

  // check the computed colors for the partially transparent or
  // fully opaque colors in the overlay image
  ASSERT( 0xFF0000FF == objs->smiley_out->data[82] );
  ASSERT( 0x800000FF == objs->smiley_out->data[83] );
  ASSERT( 0x00FF00FF == objs->smiley_out->data[84] );
  ASSERT( 0x00807FFF == objs->smiley_out->data[85] );
  ASSERT( 0x0000FFFF == objs->smiley_out->data[86] );
  ASSERT( 0x000080FF == objs->smiley_out->data[87] );
}

// //custom tests for helper functions
void test_get_red() {
  Picture test_pic = {
    TEST_COLORS,
    9, // width
    2, // height
    "rgmrrrrrr"
    "cmrrrbbgg"
  };
  struct Image *test_img = picture_to_img( &test_pic );
  img_init(test_img, 9, 2);
  // make sure all r values start out as 0
  for (int i = 0; i < 18; i++) {
    ASSERT(0x0 == get_r(test_img->data[i]));
  }
  //assign rgb values to pixels and check if get_r works
  test_img->data[0] = 0xFF0000FF;
  test_img->data[1] = 0x15001234;
  test_img->data[12] = 0xFFFF00FF;
  test_img->data[6] = 0xC50000FF;

  ASSERT(0xFF == get_r(test_img->data[0]));
  ASSERT(0x15 == get_r(test_img->data[1]));
  ASSERT(0xFF == get_r(test_img->data[12]));
  ASSERT(0xC5 == get_r(test_img->data[6]));
  //ensuring that changing other pixels won't affect existing ones
  ASSERT(0x0 == get_r(test_img->data[3]));
}

void test_get_green() {
  Picture test_pic = {
    TEST_COLORS,
    4, // width
    4, // height
    "rgmr"
    "cmrr"
    "gggg"
    "rrrr"
  };
  struct Image *test_img = picture_to_img( &test_pic );
  img_init(test_img, 4, 4);
  // make sure all r values start out as 0
  for (int i = 0; i < 16; i++) {
    ASSERT(0x0 == get_g(test_img->data[i]));
  }
  //assign rgb values to pixels and check if get_r works
  test_img->data[0] = 0xFFFF00FF;
  test_img->data[1] = 0x15211234;
  test_img->data[12] = 0xFF1400FF;
  test_img->data[6] = 0xC50000FF;

  ASSERT(0xFF == get_g(test_img->data[0]));
  ASSERT(0x21 == get_g(test_img->data[1]));
  ASSERT(0x14 == get_g(test_img->data[12]));
  ASSERT(0x00 == get_g(test_img->data[6]));
  //ensuring that changing other pixels won't affect existing ones
  ASSERT(0x0 == get_g(test_img->data[3]));

}

void test_get_blue() {
  Picture test_pic = {
    TEST_COLORS,
    4, // width
    4, // height
    "rgmr"
    "cmrr"
    "gggg"
    "rrrr"
  };
  struct Image *test_img = picture_to_img( &test_pic );
  img_init(test_img, 4, 4);
  // make sure all r values start out as 0
  for (int i = 0; i < 16; i++) {
    ASSERT(0x0 == get_b(test_img->data[i]));
  }
  //assign rgb values to pixels and check if get_b works
  test_img->data[0] = 0xFFFF00FF;
  test_img->data[1] = 0x15211234;
  test_img->data[12] = 0xFF14FFFF;
  test_img->data[6] = 0xC500C2FF;

  ASSERT(0x00 == get_b(test_img->data[0]));
  ASSERT(0x12 == get_b(test_img->data[1]));
  ASSERT(0xFF == get_b(test_img->data[12]));
  ASSERT(0xC2 == get_b(test_img->data[6]));
  //ensuring that changing other pixels won't affect existing ones
  ASSERT(0x0 == get_b(test_img->data[3]));

}

void test_get_a() {
  Picture test_pic = {
    TEST_COLORS,
    4, // width
    4, // height
    "rgmr"
    "cmrr"
    "gggg"
    "rrrr"
  };
  struct Image *test_img = picture_to_img( &test_pic );
  img_init(test_img, 4, 4);
  // make sure all a values start out as FF
  for (int i = 0; i < 16; i++) {
    ASSERT(0xFF == get_a(test_img->data[i]));
  }
  //assign rgb values to pixels and check if get_r works
  test_img->data[0] = 0xFFFF0000;
  test_img->data[1] = 0x152112CD;
  test_img->data[12] = 0xFF140021;
  test_img->data[6] = 0xC50000FF;

  ASSERT(0x00 == get_a(test_img->data[0]));
  ASSERT(0xCD == get_a(test_img->data[1]));
  ASSERT(0x21 == get_a(test_img->data[12]));
  ASSERT(0xFF == get_a(test_img->data[6]));
  //ensuring that changing other pixels won't affect existing ones
  ASSERT(0xFF == get_a(test_img->data[3]));

}

// void test_composite( TestObjs *objs ) {
//   imgproc_composite( objs->smiley, objs->overlay, objs->smiley_out );

//   // for all of the fully-transparent pixels in the overlay image,
//   // the result image should have a pixel identical to the corresponding
//   // pixel in the base image
  

//   // check the computed colors for the partially transparent or
//   // fully opaque colors in the overlay image
//   ASSERT( 0xFF0000FF == to_composite(objs->overlay, objs->smiley, 82));
//   ASSERT( 0x800000FF == to_composite(objs->overlay, objs->smiley, 83));
//   ASSERT( 0x00FF00FF == to_composite(objs->overlay, objs->smiley, 84));
//   ASSERT( 0x00807FFF == to_composite(objs->overlay, objs->smiley, 85));
//   ASSERT( 0x0000FFFF == to_composite(objs->overlay, objs->smiley, 86));
//   ASSERT( 0x000080FF == to_composite(objs->overlay, objs->smiley, 87));
// }


// void test_grayscale() {
//   Picture smiley_grayscale_pic = {
//       TEST_COLORS_GRAYSCALE,
//       16, // width
//       10, // height
//       "    mrrrggbc    "
//       "   c        b   "
//       "  r   r  b   c  "
//       " b            b "
//       " b            r "
//       " g   b    c   r "
//       "  c   ggrb   b  "
//       "   m        c   "
//       "    gggrrbmc    "
//       "                "
//     };

//     struct Image *smiley_grayscale_expected = picture_to_img( &smiley_grayscale_pic );

//     uint32_t grayed = to_grayscale(smiley_grayscale_expected, 3);
//     ASSERT( grayed == 0x000000FF );
//     grayed = to_grayscale(smiley_grayscale_expected, 4);
//     ASSERT( grayed == 0x7F7F7FFF );

//     destroy_img( smiley_grayscale_expected );
// }

void test_make_pixel() {
  uint32_t pixel = make_pixel(0x34,0x73,0x81,0xFF);
  ASSERT(pixel == 0x347381FF);

  pixel = make_pixel(0xFF,0xFF,0xFF,0xFF);
  ASSERT(pixel == 0xFFFFFFFF);

  pixel = make_pixel(0x00,0x00,0x00,0x00);
  ASSERT(pixel == 0x00000000);

  pixel = make_pixel(0x1,0x5,0x3,0x9);
  ASSERT(pixel == 0x01050309);
}

