import 'dart:typed_data';

Uint8List convertToGrayscale(Uint8List subpixels, int width, int height) {
  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    subpixels[i] = subpixels[i + 1] = subpixels[i + 2] =
        (0.3 * subpixels[i] + 0.59 * subpixels[i + 1] + 0.11 * subpixels[i + 2])
            .round();
  }

  return subpixels;
}