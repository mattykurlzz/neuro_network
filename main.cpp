#include "./bmp/bitmap_parse.hpp"
#include "neuronet.h"
#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
  double error = 0;
  double minErr = 0.06;
  double mostErr = 0;

  std::vector<unsigned> layers{49, 70, 70, 3};
  std::vector<std::vector<double>> vvRightAnswers{
      {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
  std::vector<std::vector<double>> inputs{};

  {
    std::vector<double> tmp;
    tmp = parse("./files/circle.bmp");
    if (tmp.empty()) {
      return -1;
    }
    std::cout << tmp[5] << std::endl;
    inputs.push_back(tmp);
    // std::cout << inputs[0][5] << std::endl;
    // std::system("pause");

    tmp = parse("./files/triangle.bmp");
    if (tmp.empty()) {
      return -1;
    }
    inputs.push_back(tmp);

    tmp = parse("./files/rect.bmp");
    if (tmp.empty()) {
      return -1;
    }
    inputs.push_back(tmp);
  }
  // inputs.push_back(std::vector<double>{1, 1});
  // inputs.push_back(std::vector<double>{0, 1});
  // inputs.push_back(std::vector<double>{1, 0});
  // // inputs.push_back(std::vector<double>{0, 0});

  Neuromodel model(layers);

  do {
    mostErr = 0;
    for (auto input = inputs.begin(); input < inputs.end(); ++input) {
      model.setRightAnswer(vvRightAnswers[input - inputs.begin()]);
      do {
        model.dropNet();
        model.setInput(*input);
        model.calculate();
        error = model.getErr();
        // model.print();
        std::cout << error << std::endl;
        if (error >= minErr) {
          model.teach();
        }
      } while (error >= minErr);
    }
    std::cout << "gen trained. test: " << std::endl;
    mostErr = 0;
    for (auto input = inputs.begin(); input < inputs.end(); ++input) {
      model.setRightAnswer(vvRightAnswers[input - inputs.begin()]);
      model.dropNet();
      model.setInput(*input);
      model.calculate();
      error = model.getErr();
      model.print();
      std::cout << "error = " << error << std::endl;
      mostErr = (error > mostErr) ? error : mostErr;
    }
  } while (mostErr > minErr);

  // for (int i = 0; i < 1000; ++i) {
  //   model.dropNet();
  //   // model.setInput(input);
  //   model.calculate();
  //   model.getErr();
  //   model.teach();
  // }
  std::cout << "\nmodel trained. running tests... " << std::endl;
  for (auto input = inputs.begin(); input < inputs.end(); ++input) {
    model.setRightAnswer(vvRightAnswers[input - inputs.begin()]);
    model.dropNet();
    model.setInput(*input);
    model.calculate();
    model.print();
    error = model.getErr();
    std::cout << std::endl << error << std::endl;
  }
  std::cout << std::endl;

  return 0;
}