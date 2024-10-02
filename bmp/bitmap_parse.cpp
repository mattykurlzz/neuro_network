#include "bitmap_parse.hpp"

#include "bitmap_image.hpp"
// #include <string>
// #include <vector>

std::vector<double> parse(const std::string &filename) {
  std::vector<double> retVector{};
  bitmap_image image(filename);

  if (!image) {
    printf("Error - Failed to open: input bmp\n");
    return retVector;
  }

  const unsigned int height = image.height();
  const unsigned int width = image.width();
  double push;

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      rgb_t colour;

      image.get_pixel(x, y, colour);

      push = 1.0 - (colour.red + colour.blue + colour.green) / 765.001f;
      std::cout << push << std::endl;
      retVector.push_back(push);
    }
  }
  std::cout << retVector[3] << std::endl;
  system("pause");
  return retVector;
}
