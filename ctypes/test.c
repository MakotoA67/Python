/*
 * test.c: カラーバーを例に画像ファイルを生成するC言語によるサンプルプログラム
 *         (Python - C 連携のPythonサンプルプログラムとの比較用)
 */
#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include <jpeglib.h>

#include "colorbar.h"

/* 関数プロトタイプ宣言 */
static int writeBMP(char *fname, unsigned char *img, int width, int height);
#ifdef USE_LIBTIFF
static int writeTIFF(char *fname, unsigned char *img, int width, int height, int color_mode);
#endif
#ifdef USE_JPEGLIB
static int writeJPEG(char *fname, unsigned char *img, int width, int height, int ch, int quality);
#endif

int main(int argc, char **argv)
{
  unsigned char	*ptr2colorBar;
  unsigned int width, height;

#if !defined(USE_LIBTIFF) && !defined(USE_JPEGLIB)
  ptr2colorBar = get_color_bar_image_in_pixel_interleaved_bgr_order(&height, &width);
  writeBMP("./test.bmp", ptr2colorBar, width, height);
#elif defined(USE_LIBTIFF)
  ptr2colorBar = get_color_bar_image(&height, &width);
  writeTIFF("./test.tiff", ptr2colorBar, width, height, 1);
#elif defined(USE_JPEGLIB)
  ptr2colorBar = get_color_bar_image_with_pixel_interleaved(&height, &width);
  writeJPEG("./test.jpg", ptr2colorBar, width, height, 3, 90);
#endif

  return 0;
}

#ifdef USE_LIBTIFF
/*
 * writeTIFF()
 * TIFFファイル出力
 * 画像データの実体は面順次フォーマットのまま格納
 * ※libtiff利用
 */
static int writeTIFF(char *fname, unsigned char *img, int width, int height, int color_mode)
{
  TIFF	*tiff;
  tsize_t	size;

  tiff = TIFFOpen(fname, "w");

  TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);

  TIFFSetField(tiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 1.0);
  TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 1.0);
  TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_NONE);
  TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
  TIFFSetField(tiff, TIFFTAG_SOFTWARE, "libtiff");

  if (color_mode == 1) {
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, height);
    size = TIFFWriteEncodedStrip(tiff, 0, img, width*height);
    size = TIFFWriteEncodedStrip(tiff, 1, img + width*height, width*height);
    size = TIFFWriteEncodedStrip(tiff, 2, img + width*height*2, width*height);
  } else {
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, height);
    size = TIFFWriteEncodedStrip(tiff, 0, img, width*height);
  }
  printf("TIFF file output (%s, %s, %p, %ld)\n",
	 fname, (color_mode == 1) ? "COLOR" : "MONOCHROME", (void *)img, size);

  TIFFClose(tiff);

  return size;
}
#endif

#ifdef USE_JPEGLIB
int writeJPEG(char *fname, unsigned char *img, int width, int height, int ch, int quality)
{
  struct jpeg_compress_struct jpeg;
  struct jpeg_error_mgr err;
  FILE *fo;
  JSAMPLE *address;
  int j;


  jpeg.err = jpeg_std_error(&err);
  jpeg_create_compress(&jpeg);

  fo = fopen(fname, "wb");
  if(fo == NULL){
    fprintf(stderr, "%sは開けません\n", fname);
    jpeg_destroy_compress(&jpeg);
    return -1;
  }

  jpeg_stdio_dest(&jpeg, fo);

  jpeg.image_width = width;
  jpeg.image_height = height;
  jpeg.input_components = ch;
  jpeg.in_color_space = JCS_RGB;
  jpeg_set_defaults(&jpeg);

  //jpeg_set_quality(&jpeg, 50, TRUE);
  jpeg_set_quality(&jpeg, quality, TRUE);

  jpeg_start_compress(&jpeg, TRUE);

  for (j = 0; j < jpeg.image_height; j++ ) {
    address = img + (j * width * ch);
    jpeg_write_scanlines(&jpeg, &address, 1);
  }

  jpeg_finish_compress(&jpeg);

  jpeg_destroy_compress(&jpeg);

  printf("JPEG file output (%s)\n", fname);

  return 0;
}
#endif

#define		BMP_HEADER_OFFSET_BIWIDTH 	18
#define		BMP_HEADER_OFFSET_BIHEIGHT 	22
#define		BMP_HEADER_OFFSET_BFSIZE 	2
#define		BMP_HEADER_OFFSET_BISIZEIMAGE 	34
    
static char bmp_header[] = {
  0x42, 0x4D, // bfType
  0x36, 0x00, 0x2C, 0x01, // bfSize
  0x00, 0x00, // bfReserved1
  0x00, 0x00, // bfReserved2
  0x36, 0x00, 0x00, 0x00, // bfOffBits
  0x28, 0x00, 0x00, 0x00, // biSize
  0x00, 0x0a, 0x00, 0x00, // biWidth
  0x30, 0xfd, 0xff, 0xff, // biHeight (minus means top-down)
  0x01, 0x00, // biPlanes
  0x18, 0x00, // biBitCount
  0x00, 0x00, 0x00, 0x00, // biCompression
  0x00, 0x30, 0x1a, 0x00, // biSizeImage
  0x13, 0x0b, 0x00, 0x00, // biXPixPerMeter
  0x13, 0x0b, 0x00, 0x00, // biYPixPerMeter
  0x00, 0x00, 0x00, 0x00, // biClrUsed
  0x00, 0x00, 0x00, 0x00 // biCirImportant
};

/*
 * writeBMP()
 * BMPファイル出力
 * 画像データの実体は点順次フォーマット
 * ※自前でヘッダを構成
 */
static int writeBMP(char *fname, unsigned char *img, int width, int height)
{
  FILE		*fp;
  unsigned int	imageSize;

  /* BMPファイルのヘッダbiWidthに水平サイズを設定する */
  *(unsigned short *)(bmp_header + BMP_HEADER_OFFSET_BIWIDTH) = width;

  /* BMPファイルのヘッダbiHeightに垂直サイズを設定する
     (BMPはマイナスで逆にトップダウンを示す) */
  *(unsigned short *)(bmp_header + BMP_HEADER_OFFSET_BIHEIGHT) = -height;

  /* BMPファイルのヘッダbfSizeにファイルサイズ、biSizeImageにBMPヘッダを除いたイメージサイズを、
     それぞれ設定する */
  imageSize= width*height*3;

  *(unsigned int *)(bmp_header + BMP_HEADER_OFFSET_BISIZEIMAGE) = imageSize;
  *(unsigned int *)(bmp_header + BMP_HEADER_OFFSET_BFSIZE) = imageSize + sizeof(bmp_header);
  
  fp = fopen(fname, "w");

  fwrite(bmp_header, 1, sizeof(bmp_header), fp);
  fwrite(img, 1, width*height*3, fp);
  fclose(fp);

  printf("BMP file output (%s)\n", fname);

  return 0;
}
