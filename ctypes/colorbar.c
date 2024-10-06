#include <stdlib.h>
#include "colorbar.h"

/* 関数プロトタイプ宣言 */
static unsigned char *generate_color_bar_image(int, int, int, void *, int, int);

/* マクロ定義 */
#if 0
#define	IMG_WIDTH		640
#define	IMG_HEIGHT		480
#else
#define	IMG_WIDTH		1280
#define	IMG_HEIGHT		720
#endif
#define	IMG_COLOR_FACTOR	3
#if 1
#define	NUM_OF_COLOR_BARS	8
#else
#define	NUM_OF_COLOR_BARS	16
#endif

static unsigned char	colors_of_color_bars[NUM_OF_COLOR_BARS][IMG_COLOR_FACTOR] = {
  #if 1
  {255, 255, 255},
  {255, 255,   0},
  {  0, 255, 255},
  {  0, 255,   0},
  {255,   0, 255},
  {255,   0,   0},
  {  0,   0, 255},
  {  0,   0,   0},
  #else
  {255, 255, 255},
  {255, 255,   0},
  {  0, 255, 255},
  {  0, 255,   0},
  {255,   0, 255},
  {255,   0,   0},
  {  0,   0, 255},
  {  0,   0,   0},
  {128, 128, 128},
  {128, 128,   0},
  {  0, 128, 128},
  {  0, 128,   0},
  {128,   0, 128},
  {128,   0,   0},
  {  0,   0, 128},
  {  0,   0,   0},
  #endif
};  
    
unsigned char *get_color_bar_image(unsigned int *pHeight, unsigned int *pWidth)
{
  *pWidth = IMG_WIDTH;
  *pHeight = IMG_HEIGHT;

  return generate_color_bar_image(IMG_WIDTH, IMG_HEIGHT, NUM_OF_COLOR_BARS, colors_of_color_bars, kFalse, kRGB_Order);
}

unsigned char *get_color_bar_image_with_pixel_interleaved(unsigned int *pHeight, unsigned int *pWidth)
{
  *pWidth = IMG_WIDTH;
  *pHeight = IMG_HEIGHT;

  return generate_color_bar_image(IMG_WIDTH, IMG_HEIGHT, NUM_OF_COLOR_BARS, colors_of_color_bars, kTrue, kRGB_Order);
}

unsigned char *get_color_bar_image_in_pixel_interleaved_bgr_order(unsigned int *pHeight, unsigned int *pWidth)
{
  *pWidth = IMG_WIDTH;
  *pHeight = IMG_HEIGHT;

  return generate_color_bar_image(IMG_WIDTH, IMG_HEIGHT, NUM_OF_COLOR_BARS, colors_of_color_bars, kTrue, kBGR_Order);
}

static unsigned char *generate_color_bar_image(int width, int height, int num_of_color_bars, void *table, int pixel_interleaved, int color_order)
{
  int	i, j, k;
  unsigned char	*red, *green, *blue, *image;
  unsigned char *color_bar_table = (unsigned char *)table;
  
  image = (unsigned char *)malloc(width * height * IMG_COLOR_FACTOR);

  if (!pixel_interleaved) {	// 面順次
    red = image;
    green = red + width * height;
    blue = green + width * height;
  } else if (color_order == kRGB_Order) {	// 点順次
    red = image;
    green = image + 1;
    blue = image + 2;
  } else {
    blue = image;
    green = image + 1;
    red = image + 2;
  }

  for (j = 0; j < height; j++) {
    for (k = 0; k < num_of_color_bars; k++) {
      for (i = 0; i < width / num_of_color_bars; i++) {
	*red = *color_bar_table;
	*green = *(color_bar_table + 1);
	*blue = *(color_bar_table + 2);
	if (!pixel_interleaved) {	// 面順次
	  red++;
	  green++;
	  blue++;
	} else {	// 点順次
	  red += 3;
	  green += 3;
	  blue += 3;
	}
      }
      color_bar_table += IMG_COLOR_FACTOR;
    }
    color_bar_table = (unsigned char *)table;
  }

  return image;
}
