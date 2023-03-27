import 'package:flutter/material.dart';
import '../filters/averageDithering.dart';
import '../filters/grayscaleConversion.dart';
import '../filters/octreeQuantization.dart';

class ConvertToGrayscaleButton extends StatelessWidget {
  const ConvertToGrayscaleButton({super.key, required this.applyFilter});
  final Function applyFilter;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.format_color_reset_outlined),
      tooltip: 'Convert to grayscale',
      hoverColor: Colors.transparent,
      padding: const EdgeInsets.only(right: 10),
      onPressed: () => applyFilter(convertToGrayscale),
    );
  }
}

class AverageDitheringButton extends StatelessWidget {
  AverageDitheringButton({super.key, required this.applyFilter});
  final Function applyFilter;
  final TextEditingController _controller = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.vrpano),
      tooltip: 'Apply average dithering',
      hoverColor: Colors.transparent,
      onPressed: () {
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            title: const Text('Setup average dithering'),
            content: TextField(
              decoration: const InputDecoration(
                  hintText: 'Enter number of grayscale levels'),
              controller: _controller,
            ),
            actions: <Widget>[
              TextButton(
                child: const Text('Cancel'),
                onPressed: () => Navigator.of(context).pop(),
              ),
              TextButton(
                child: const Text('Apply'),
                onPressed: () {
                  if (_controller.text.isNotEmpty &&
                      int.parse(_controller.text) >= 2) {
                    applyFilter(
                        applyAverageDithering, int.parse(_controller.text));
                    Navigator.of(context).pop();
                  }
                },
              ),
            ],
          ),
        );
      },
    );
  }
}

class AverageDitheringYCbCrButton extends StatelessWidget {
  AverageDitheringYCbCrButton({super.key, required this.applyFilter});
  final Function applyFilter;
  final TextEditingController _controller = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.vrpano_outlined),
      tooltip: 'Apply average dithering with YCbCr conversion',
      hoverColor: Colors.transparent,
      onPressed: () {
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            title: const Text('Setup average dithering with YCbCr conversion'),
            content: TextField(
              decoration: const InputDecoration(
                  hintText: 'Enter number of color channels'),
              controller: _controller,
            ),
            actions: <Widget>[
              TextButton(
                child: const Text('Cancel'),
                onPressed: () => Navigator.of(context).pop(),
              ),
              TextButton(
                child: const Text('Apply'),
                onPressed: () {
                  if (_controller.text.isNotEmpty &&
                      int.parse(_controller.text) >= 2) {
                    applyFilter(applyAverageDitheringYCbCr,
                        int.parse(_controller.text));
                    Navigator.of(context).pop();
                  }
                },
              ),
            ],
          ),
        );
      },
    );
  }
}

class OctreeQuantizationButton extends StatelessWidget {
  OctreeQuantizationButton({super.key, required this.applyFilter});
  final Function applyFilter;
  final TextEditingController _controller = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.account_tree),
      tooltip: 'Apply octree quantization',
      hoverColor: Colors.transparent,
      onPressed: () {
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            title: const Text('Setup octree quantization'),
            content: TextField(
              decoration: const InputDecoration(
                  hintText: 'Enter max size of color palette'),
              controller: _controller,
            ),
            actions: <Widget>[
              TextButton(
                child: const Text('Cancel'),
                onPressed: () => Navigator.of(context).pop(),
              ),
              TextButton(
                child: const Text('Apply'),
                onPressed: () {
                  if (_controller.text.isNotEmpty &&
                      int.parse(_controller.text) > 0) {
                    applyFilter(
                        applyOctreeQuantization, int.parse(_controller.text));
                    Navigator.of(context).pop();
                  }
                },
              ),
            ],
          ),
        );
      },
    );
  }
}