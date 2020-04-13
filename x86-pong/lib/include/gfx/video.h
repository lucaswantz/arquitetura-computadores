#ifndef VIDEO_H
#define VIDEO_H

struct image {
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char pixel_data[];
};

extern void draw_square(int x, int y, int w, int h, int c);
extern void clear_img(int x, int y, int c, void *image);
extern void draw_img(int x, int y, void *image);

#endif /* VIDEO_H */

