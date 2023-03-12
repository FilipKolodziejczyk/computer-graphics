import 'dart:io';

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:window_size/window_size.dart';

import 'widgets/imageActions.dart';
import 'state.dart';
import 'widgets/filterActions.dart';
import 'widgets/filterManager.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    setWindowTitle('Image Viewer');
    setWindowMinSize(const Size(800, 600));
    setWindowMaxSize(Size.infinite);
  }

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'Image Viewer',
        debugShowCheckedModeBanner: false,
        theme: ThemeData(useMaterial3: true),
        home: ChangeNotifierProvider(
          create: (context) => MyAppState(),
          child: const MyHomePage(),
        ));
  }
}

class MyHomePage extends StatelessWidget {
  const MyHomePage({super.key});

  @override
  Widget build(BuildContext context) {
    var appState = context.watch<MyAppState>();
    var currentImage = appState.currentImage;
    var loadImage = appState.loadImage;
    var saveImage = appState.saveImage;
    var restoreOriginalImage = appState.restoreOriginalImage;
    var applyFilter = appState.applyFilter;
    var linearFilters = appState.linearFilters;
    var functionalFilters = appState.functionalFilters;
    var convolutionFilters = appState.convolutionFilters;
    var updateLinearFilter = appState.updateLinearFilter;

    return Scaffold(
        appBar: AppBar(
          title: const Text('Image Viewer'),
          backgroundColor: Theme.of(context).focusColor,
          actions: [
            FunctionalFiltersButton(
              applyFilter: applyFilter,
              functionalFilters: functionalFilters + linearFilters,
            ),
            const SizedBox(width: 20),
            ConvolutionFiltersButton(
              applyFilter: applyFilter,
              convolutionFilters: convolutionFilters,
            ),
            const SizedBox(width: 10),
            OpenButton(loadImage: loadImage),
            RestoreButton(restoreOriginalImage: restoreOriginalImage),
            SaveButton(imageExists: currentImage != null, saveImage: saveImage),
            ManageLinearFiltersButton(
              linearFilters: linearFilters,
              updateLinearFilters: updateLinearFilter,
            ),
          ],
        ),
        body: ChangeNotifierProvider(
          create: (context) => appState,
          child: const ImageWrapper(),
        ));
  }
}

class ImageWrapper extends StatelessWidget {
  const ImageWrapper({super.key});

  @override
  Widget build(BuildContext context) {
    var appState = context.watch<MyAppState>();
    var currentImage = appState.currentImage;

    return Center(
      child: currentImage != null
          ? RawImage(image: currentImage)
          : const Text('No image selected'),
    );
  }
}