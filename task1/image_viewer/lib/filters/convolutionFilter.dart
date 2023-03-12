import 'dart:math';
import 'dart:typed_data';

import 'filter.dart';

class ConvolutionFilter extends Filter {
  List<int> kernel;
  int get dim => sqrt(kernel.length).toInt();
  int offset;
  double divisor;

  ConvolutionFilter(String name, this.kernel,
      [this.divisor = 1, this.offset = 0])
      : super(name) {
    if (kernel.isEmpty) {
      throw Exception("Kernel must not be empty");
    }

    if (kernel.length != dim * dim) {
      throw Exception("Kernel must be a square matrix");
    }

    if (dim % 2 == 0) {
      throw Exception("Kernel must have an odd size");
    }
  }

  @override
  apply(Uint8List pixels, int width, int height) {
    var result = Uint8List.fromList(pixels);
    int index(int x, int y) {
      if (x < 0) {
        x = 0;
      } else if (x >= width) {
        x = width - 1;
      }

      if (y < 0) {
        y = 0;
      } else if (y >= height) {
        y = height - 1;
      }

      return (y * width + x) * 4;
    }

    double convolution(int x, int y, int channel) {
      var sum = 0.0;
      for (var j = 0; j < dim; j++) {
        for (var i = 0; i < dim; i++) {
          var xn = x + i - dim ~/ 2;
          var yn = y + j - dim ~/ 2;
          sum += pixels[index(xn, yn) + channel] * kernel[j * dim + i];
        }
      }
      return sum / divisor + offset;
    }

    for (var y = 0; y < height; y++) {
      for (var x = 0; x < width; x++) {
        result[index(x, y)] = convolution(x, y, 0).clamp(0, 255).round();
        result[index(x, y) + 1] = convolution(x, y, 1).clamp(0, 255).round();
        result[index(x, y) + 2] = convolution(x, y, 2).clamp(0, 255).round();
      }
    }

    return result;
  }
}