import 'dart:typed_data';

int rangeIdx(int value, List<int> ranges) {
  for (var i = 0; i < ranges.length - 1; i++) {
    if (value < ranges[i + 1]) {
      return i;
    }
  }
  return ranges.length - 2;
}

Uint8List applyAverageDithering(
    Uint8List subpixels, int width, int height, int channels) {
  List<int> colorLevels = [];
  for (var i = 0; i < channels; i++) {
    colorLevels.add((i * 255 / (channels - 1)).round());
  }

  var sumR = List.filled(channels - 1, 0);
  var countR = List.filled(channels - 1, 0);
  var sumG = List.filled(channels - 1, 0);
  var countG = List.filled(channels - 1, 0);
  var sumB = List.filled(channels - 1, 0);
  var countB = List.filled(channels - 1, 0);

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    sumR[rangeIdx(subpixels[i], colorLevels)] += subpixels[i];
    countR[rangeIdx(subpixels[i], colorLevels)]++;
    sumG[rangeIdx(subpixels[i + 1], colorLevels)] += subpixels[i + 1];
    countG[rangeIdx(subpixels[i + 1], colorLevels)]++;
    sumB[rangeIdx(subpixels[i + 2], colorLevels)] += subpixels[i + 2];
    countB[rangeIdx(subpixels[i + 2], colorLevels)]++;
  }

  var thresholdR = List.filled(channels - 1, 0);
  var thresholdG = List.filled(channels - 1, 0);
  var thresholdB = List.filled(channels - 1, 0);
  for (var i = 0; i < channels - 1; i++) {
    thresholdR[i] = countR[i] > 0 ? (sumR[i] / countR[i]).round() : 0;
    thresholdG[i] = countG[i] > 0 ? (sumG[i] / countG[i]).round() : 0;
    thresholdB[i] = countB[i] > 0 ? (sumB[i] / countB[i]).round() : 0;
  }

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    var ri = rangeIdx(subpixels[i], colorLevels);
    subpixels[i] =
        subpixels[i] < thresholdR[ri] ? colorLevels[ri] : colorLevels[ri + 1];
    var gi = rangeIdx(subpixels[i + 1], colorLevels);
    subpixels[i + 1] = subpixels[i + 1] < thresholdG[gi]
        ? colorLevels[gi]
        : colorLevels[gi + 1];
    var bi = rangeIdx(subpixels[i + 2], colorLevels);
    subpixels[i + 2] = subpixels[i + 2] < thresholdB[bi]
        ? colorLevels[bi]
        : colorLevels[bi + 1];
  }

  return subpixels;
}

Uint8List applyAverageDitheringYCbCr(
    Uint8List subpixels, int width, int height, int channels) {
  List<int> colorLevels = [];
  for (var i = 0; i < channels; i++) {
    colorLevels.add((i * 255 / (channels - 1)).round());
  }

  var sumY = List.filled(channels - 1, 0);
  var countY = List.filled(channels - 1, 0);
  var sumCb = List.filled(channels - 1, 0);
  var countCb = List.filled(channels - 1, 0);
  var sumCr = List.filled(channels - 1, 0);
  var countCr = List.filled(channels - 1, 0);

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    var Y = (subpixels[i] * 0.299 +
            subpixels[i + 1] * 0.587 +
            subpixels[i + 2] * 0.114)
        .clamp(0, 255)
        .round();
    var Cb = (subpixels[i] * -0.168935 +
            subpixels[i + 1] * -0.331665 +
            subpixels[i + 2] * 0.50059 +
            128)
        .clamp(0, 255)
        .round();
    var Cr = (subpixels[i] * 0.499813 +
            subpixels[i + 1] * -0.418531 +
            subpixels[i + 2] * -0.081282 +
            128)
        .clamp(0, 255)
        .round();

    sumY[rangeIdx(Y, colorLevels)] += Y;
    countY[rangeIdx(Y, colorLevels)]++;
    sumCb[rangeIdx(Cb, colorLevels)] += Cb;
    countCb[rangeIdx(Cb, colorLevels)]++;
    sumCr[rangeIdx(Cr, colorLevels)] += Cr;
    countCr[rangeIdx(Cr, colorLevels)]++;

    subpixels[i] = Y;
    subpixels[i + 1] = Cb;
    subpixels[i + 2] = Cr;
  }

  var thresholdY = List.filled(channels - 1, 0);
  var thresholdCb = List.filled(channels - 1, 0);
  var thresholdCr = List.filled(channels - 1, 0);
  for (var i = 0; i < channels - 1; i++) {
    thresholdY[i] = countY[i] > 0 ? (sumY[i] / countY[i]).round() : 0;
    thresholdCb[i] = countCb[i] > 0 ? (sumCb[i] / countCb[i]).round() : 0;
    thresholdCr[i] = countCr[i] > 0 ? (sumCr[i] / countCr[i]).round() : 0;
  }

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    var Yi = rangeIdx(subpixels[i], colorLevels);
    var Y =
        subpixels[i] < thresholdY[Yi] ? colorLevels[Yi] : colorLevels[Yi + 1];
    var Cbi = rangeIdx(subpixels[i + 1], colorLevels);
    var Cb = subpixels[i + 1] < thresholdCb[Cbi]
        ? colorLevels[Cbi]
        : colorLevels[Cbi + 1];
    var Cri = rangeIdx(subpixels[i + 2], colorLevels);
    var Cr = subpixels[i + 2] < thresholdCr[Cri]
        ? colorLevels[Cri]
        : colorLevels[Cri + 1];

    subpixels[i] = (Y + 1.402 * (Cr - 128)).clamp(0, 255).round();
    subpixels[i + 1] =
        (Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)).clamp(0, 255).round();
    subpixels[i + 2] = (Y + 1.772 * (Cb - 128)).clamp(0, 255).round();
  }

  return subpixels;
}