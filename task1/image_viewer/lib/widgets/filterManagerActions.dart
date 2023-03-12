import 'package:flutter/material.dart';
import 'package:image_viewer/filters/linearFilter.dart';

class AddButton extends StatelessWidget {
  const AddButton({
    super.key,
    required this.add,
  });

  final Function() add;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.add),
      tooltip: 'New filter',
      hoverColor: Colors.transparent,
      onPressed: add,
    );
  }
}

class ReturnButton extends StatelessWidget {
  const ReturnButton({
    super.key,
    required this.returnValue,
  });

  final List<LinearFilter> returnValue;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.arrow_back),
      tooltip: 'Return to Image Viewer',
      hoverColor: Colors.transparent,
      onPressed: () => Navigator.pop(context, returnValue),
    );
  }
}

class RenameButton extends StatelessWidget {
  RenameButton({
    super.key,
    required this.rename,
    required this.isSelected,
  });

  final Function(String name) rename;
  final TextEditingController _controller = TextEditingController();
  final bool isSelected;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.drive_file_rename_outline),
      tooltip: 'Rename filter',
      hoverColor: Colors.transparent,
      onPressed: () => isSelected
          ? showDialog(
              context: context,
              builder: (context) => AlertDialog(
                title: const Text('Rename Filter'),
                content: TextField(
                  decoration: const InputDecoration(hintText: 'Enter new name'),
                  controller: _controller,
                ),
                actions: <Widget>[
                  TextButton(
                    child: const Text('Cancel'),
                    onPressed: () => Navigator.of(context).pop(),
                  ),
                  TextButton(
                    child: const Text('Rename'),
                    onPressed: () {
                      if (_controller.text.isNotEmpty) {
                        rename(_controller.text);
                        Navigator.of(context).pop();
                      }
                    },
                  ),
                ],
              ),
            )
          : null,
    );
  }
}

class AddPointButton extends StatelessWidget {
  AddPointButton({
    super.key,
    required this.addPoint,
    required this.isSelected,
  });

  final Function(int x, int y) addPoint;
  final bool isSelected;
  final TextEditingController _xController = TextEditingController();
  final TextEditingController _yController = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.add_chart),
      tooltip: 'Add or modify point',
      hoverColor: Colors.transparent,
      onPressed: () => isSelected
          ? showDialog(
              context: context,
              builder: (context) => AlertDialog(
                title: const Text('Add or modify point'),
                content: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: <Widget>[
                    TextField(
                      decoration:
                          const InputDecoration(hintText: 'Enter x value'),
                      controller: _xController,
                    ),
                    TextField(
                      decoration:
                          const InputDecoration(hintText: 'Enter y value'),
                      controller: _yController,
                    ),
                  ],
                ),
                actions: <Widget>[
                  TextButton(
                    child: const Text('Cancel'),
                    onPressed: () => Navigator.of(context).pop(),
                  ),
                  TextButton(
                    child: const Text('Add/Modify'),
                    onPressed: () {
                      var x = int.tryParse(_xController.text);
                      var y = int.tryParse(_yController.text);

                      if (x != null &&
                          y != null &&
                          x >= 0 &&
                          x <= 255 &&
                          y >= 0 &&
                          y <= 255) {
                        addPoint(x, y);
                        Navigator.of(context).pop();
                      }
                    },
                  ),
                ],
              ),
            )
          : null,
    );
  }
}