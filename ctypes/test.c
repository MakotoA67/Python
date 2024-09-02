#include <stdio.h>
#include <stdlib.h>

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

static unsigned char *generate_color_bar_image(int, int, int, void *);
unsigned char *get_color_bar_image(unsigned int *, unsigned int *);

int main(int argc, char **argv)
{
  int	i, j;
  unsigned char	*ptr2colorBar;
  unsigned int width, height;

  ptr2colorBar = get_color_bar_image(&height, &width);

  printf("1st color: (%d, %d)\n", width, height);
  for (j = 0; j < height; j++) {
    printf("%d: ", j);
    for (i = 0; i < width; i++) {
      printf("0x%02x ", *(ptr2colorBar + j * width + i));
    }
    printf("\n");
  }

  printf("2nd color: (%d, %d)\n", width, height);
  for (j = 0; j < height; j++) {
    printf("%d: ", j);
    for (i = 0; i < width; i++) {
      printf("0x%02x ", *(ptr2colorBar + width*height + j * width + i));
    }
    printf("\n");
  }
  
  printf("3rd color: (%d, %d)\n", width, height);
  for (j = 0; j < height; j++) {
    printf("%d: ", j);
    for (i = 0; i < width; i++) {
      printf("0x%02x ", *(ptr2colorBar + width*height*2 + j * width + i));
    }
    printf("\n");
  }
  
  return 0;
}

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

  return generate_color_bar_image(IMG_WIDTH, IMG_HEIGHT, NUM_OF_COLOR_BARS, colors_of_color_bars);
}

static unsigned char *generate_color_bar_image(int width, int height, int num_of_color_bars, void *table)
{
  int	i, j, k;
  unsigned char	*red, *green, *blue, *image;
  unsigned char *color_bar_table = (unsigned char *)table;
  
  image = (unsigned char *)malloc(width * height * IMG_COLOR_FACTOR);

  red = image;
  green = red + width * height;
  blue = green + width * height;

  for (j = 0; j < height; j++) {
    for (k = 0; k < num_of_color_bars; k++) {
      for (i = 0; i < width / num_of_color_bars; i++) {
	*red++ = *color_bar_table;
	*green++ = *(color_bar_table + 1);
	*blue++ = *(color_bar_table + 2);
      }
      color_bar_table += IMG_COLOR_FACTOR;
    }
    color_bar_table = (unsigned char *)table;
  }
      
  return image;
}
