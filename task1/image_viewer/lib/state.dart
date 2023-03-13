import 'dart:io';
import 'dart:typed_data';
import 'dart:ui' as ui;

import 'package:cross_file/cross_file.dart';
import 'package:flutter/material.dart';

import 'filters/filter.dart';
import 'filters/linearFilter.dart';
import 'filters/convolutionFilter.dart';

class MyAppState extends ChangeNotifier {
  ui.Image? originalImage;
  ui.Image? currentImage;

  List<LinearFilter> linearFilters = [
    LinearFilter("Inversion", {0: 255, 255: 0}),
    LinearFilter("Brightness Correction", {0: 20, 235: 255, 255: 255}),
    LinearFilter("Contrast Enhancement", {0: 0, 20: 0, 235: 255, 255: 255}),
  ];

  List<Filter> functionalFilters = [
    GammaCorrection(0.5),
  ];

  List<Filter> convolutionFilters = [
    ConvolutionFilter("Blur", [1, 1, 1, 1, 1, 1, 1, 1, 1], 9),
    ConvolutionFilter("Sharpen", [0, -1, 0, -1, 5, -1, 0, -1, 0]),
    ConvolutionFilter("Edge Detection (all)", [0, -1, 0, -1, 4, -1, 0, -1, 0]),
    ConvolutionFilter("Gaussian Blur", [0, 1, 0, 1, 4, 1, 0, 1, 0], 8),
    ConvolutionFilter("Emboss (south-east)", [-1, -1, 0, -1, 1, 1, 0, 1, 1]),
  ];

  loadImage(String path) async {
    var imageFile = XFile(path);
    var fileRaw = await imageFile.readAsBytes();
    currentImage = await decodeImageFromList(fileRaw);
    originalImage = currentImage;
    notifyListeners();
  }

  saveImage(String path) async {
    if (currentImage == null) return;

    var fileRaw =
        await currentImage!.toByteData(format: ui.ImageByteFormat.png);
    await File(path).writeAsBytes(fileRaw!.buffer.asUint8List());
  }

  restoreOriginalImage() async {
    if (originalImage == null) return;

    currentImage = originalImage;
    notifyListeners();
  }

  applyFilter(Function(Uint8List pixels, int width, int height) filter) async {
    if (currentImage == null) return;

    var fileRaw =
        await currentImage!.toByteData(format: ui.ImageByteFormat.rawRgba);
    var pixels = fileRaw!.buffer.asUint8List();
    pixels = filter(pixels, currentImage!.width, currentImage!.height);
    ui.decodeImageFromPixels(
      pixels,
      currentImage!.width,
      currentImage!.height,
      ui.PixelFormat.rgba8888,
      (image) {
        currentImage = image;
        notifyListeners();
      },
    );
  }

  updateLinearFilter(List<LinearFilter> newLinearFilters) {
    linearFilters = newLinearFilters;
    notifyListeners();
  }

  convertToGrayscale() async {
    if (currentImage == null) return;

    var fileRaw =
        await currentImage!.toByteData(format: ui.ImageByteFormat.rawRgba);
    var pixels = fileRaw!.buffer.asUint8List();
    for (var i = 0; i < pixels.lengthInBytes; i += 4) {
      pixels[i] = pixels[i + 1] = pixels[i + 2] =
          (0.3 * pixels[i] + 0.59 * pixels[i + 1] + 0.11 * pixels[i + 2])
              .round();
    }

    ui.decodeImageFromPixels(
      pixels,
      currentImage!.width,
      currentImage!.height,
      ui.PixelFormat.rgba8888,
      (image) {
        currentImage = image;
        notifyListeners();
      },
    );
  }
}