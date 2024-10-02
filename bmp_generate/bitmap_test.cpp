#include "bitmap_image.hpp"

int main() {
  bitmap_image image(7, 7);

  // set background to orange
  image.set_all_channels(255, 255, 255);

  image_drawer draw(image);

  draw.pen_width(1);
  draw.pen_color(0, 0, 0);
  //   draw.circle(image.width() / 2, image.height() / 2, 2);
  //   draw.rectangle(1, 1, 5, 5);

  //   draw.triangle(0, 4, 3, 1, 6, 4);
  draw.line_segment(0, 4, 3, 1);
  draw.line_segment(3, 1, 6, 4);
  draw.line_segment(0, 5, 6, 5);

  //   draw.pen_width(1);
  //   draw.pen_color(255, 0, 0);
  //   draw.rectangle(50, 50, 150, 150);

  image.save_image("triangle.bmp");

  return 0;
}