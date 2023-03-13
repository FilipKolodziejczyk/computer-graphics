import 'package:flutter/material.dart';

import '../filters/filter.dart';

class FunctionalFiltersButton extends StatelessWidget {
  const FunctionalFiltersButton({
    super.key,
    required this.applyFilter,
    required this.functionalFilters,
  });

  final Function applyFilter;
  final List<Filter> functionalFilters;

  @override
  Widget build(BuildContext context) {
    return PopupMenuButton(
      tooltip: "Apply functional filter",
      child: const Icon(Icons.filter),
      itemBuilder: (context) => functionalFilters
          .map((filter) => PopupMenuItem(
                value: filter,
                child: Text(filter.name),
              ))
          .toList(),
      onSelected: (Filter filter) => applyFilter(filter.apply),
    );
  }
}

class ConvolutionFiltersButton extends StatelessWidget {
  const ConvolutionFiltersButton({
    super.key,
    required this.applyFilter,
    required this.convolutionFilters,
  });

  final Function applyFilter;
  final List<Filter> convolutionFilters;

  @override
  Widget build(BuildContext context) {
    return PopupMenuButton(
      tooltip: "Apply convolution filter",
      child: const Icon(Icons.filter_b_and_w),
      itemBuilder: (context) => convolutionFilters
          .map((filter) => PopupMenuItem(
                value: filter,
                child: Text(filter.name),
              ))
          .toList(),
      onSelected: (Filter filter) => applyFilter(filter.apply),
    );
  }
}