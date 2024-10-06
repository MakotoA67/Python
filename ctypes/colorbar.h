/* 関数プロトタイプ宣言 */
unsigned char *get_color_bar_image(unsigned int *, unsigned int *);
unsigned char *get_color_bar_image_with_pixel_interleaved(unsigned int *, unsigned int *);
unsigned char *get_color_bar_image_in_pixel_interleaved_bgr_order(unsigned int *, unsigned int *);

/* 変数の型宣言 */
enum {
  kFalse = 0, 
  kTrue = 1, 
};

enum {
  kRGB_Order = 0,
  kBGR_Order = 1,
};
