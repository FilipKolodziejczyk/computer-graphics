import 'dart:typed_data';

import 'filter.dart';

class LinearFilter extends Filter {
  final Map<int, int> _mapping = {};
  late Map<int, int> points;

  LinearFilter(String name, this.points) : super(name) {
    if (points.length < 2) {
      throw Exception("At least 2 points must be specified");
    }

    if (points.keys.any((element) => element < 0 || element > 255)) {
      throw Exception("X coordinates must be in range [0, 255]");
    }

    if (points.values.any((element) => element < 0 || element > 255)) {
      throw Exception("Y coordinates must be in range [0, 255]");
    }

    regenerate();
  }

  @override
  apply(Uint8List pixels, int width, int height) {
    for (var i = 0; i < pixels.lengthInBytes; i += 4) {
      pixels[i] = _mapping[pixels[i]]!;
      pixels[i + 1] = _mapping[pixels[i + 1]]!;
      pixels[i + 2] = _mapping[pixels[i + 2]]!;
    }
    return pixels;
  }

  regenerate() {
    points = Map.fromEntries(
        points.entries.toList()..sort((e1, e2) => e1.key.compareTo(e2.key)));

    var x = points.keys.toList();
    for (var i = 0; i < x.length - 1; i++) {
      var x1 = x[i];
      var x2 = x[i + 1];
      var y1 = points[x1]!;
      var y2 = points[x2]!;
      var a = (y2 - y1) / (x2 - x1);
      var b = y1 - a * x1;
      for (var j = x1; j <= x2; j++) {
        _mapping[j] = (a * j + b).clamp(0, 255).round();
      }
    }
  }
}