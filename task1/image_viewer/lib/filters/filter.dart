import 'dart:math';
import 'dart:typed_data';

abstract class Filter {
  String name;

  Filter(this.name);
  Uint8List apply(Uint8List pixels, int width, int height);
}

class GammaCorrection extends Filter {
  final double gamma;

  GammaCorrection(this.gamma) : super("Gamma Correction");

  correction(int value) => (255 * pow(value / 255, gamma)).toInt();

  @override
  Uint8List apply(Uint8List pixels, int width, int height) {
    for (var i = 0; i < pixels.lengthInBytes; i += 4) {
      pixels[i] = correction(pixels[i]);
      pixels[i + 1] = correction(pixels[i + 1]);
      pixels[i + 2] = correction(pixels[i + 2]);
    }
    return pixels;
  }
}