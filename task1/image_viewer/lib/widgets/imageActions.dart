import 'package:file_picker/file_picker.dart';
import 'package:flutter/material.dart';

class SaveButton extends StatelessWidget {
  const SaveButton({
    super.key,
    required this.imageExists,
    required this.saveImage,
  });

  final bool imageExists;
  final Function(String path) saveImage;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.save_as),
      tooltip: 'Save image',
      hoverColor: Colors.transparent,
      onPressed: () async {
        if (!imageExists) return;

        String? path = await FilePicker.platform.saveFile(
          dialogTitle: 'Save image',
          fileName: 'image.png',
          type: FileType.image,
          lockParentWindow: true,
          allowedExtensions: ['png', 'jpg', 'jpeg'],
        );

        if (path != null) saveImage(path);
      },
    );
  }
}

class RestoreButton extends StatelessWidget {
  const RestoreButton({super.key, required this.restoreOriginalImage});
  final Function() restoreOriginalImage;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.refresh),
      tooltip: 'Restore original image',
      hoverColor: Colors.transparent,
      onPressed: () => restoreOriginalImage(),
    );
  }
}

class OpenButton extends StatelessWidget {
  const OpenButton({super.key, required this.loadImage});
  final Function(String path) loadImage;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.folder),
      tooltip: 'Open image',
      hoverColor: Colors.transparent,
      onPressed: () async {
        FilePickerResult? result = await FilePicker.platform.pickFiles(
          type: FileType.image,
          dialogTitle: 'Open image',
          lockParentWindow: true,
        );
        var path = result?.files.single.path;

        if (path != null) loadImage(path);
      },
    );
  }
}