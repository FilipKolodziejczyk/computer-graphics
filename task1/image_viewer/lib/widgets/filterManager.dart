import 'package:flutter/material.dart';

import '../filters/linearFilter.dart';
import 'filterManagerState.dart';

class ManageLinearFiltersButton extends StatelessWidget {
  const ManageLinearFiltersButton({
    super.key,
    required this.linearFilters,
    required this.updateLinearFilters,
  });

  final List<LinearFilter> linearFilters;
  final Function updateLinearFilters;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      icon: const Icon(Icons.settings),
      tooltip: "Manage linear filters",
      hoverColor: Colors.transparent,
      onPressed: () => modifyLinearFilters(context, linearFilters)
          .then((value) => updateLinearFilters(value)),
    );
  }
}

Future<void> modifyLinearFilters(
    BuildContext context, List<LinearFilter> linearFilters) async {
  final result = await Navigator.push(
    context,
    MaterialPageRoute(
      builder: (context) => LinearFilterManager(linearFilters: linearFilters),
    ),
  );
  return result;
}

class LinearFilterManager extends StatefulWidget {
  const LinearFilterManager({
    super.key,
    required this.linearFilters,
  });

  final List<LinearFilter> linearFilters;

  @override
  LinearFilterManagerState createState() => LinearFilterManagerState();
}