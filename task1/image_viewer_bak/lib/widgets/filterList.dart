import 'package:flutter/material.dart';

import '../filters/linearFilter.dart';

class FilterList extends StatelessWidget {
  const FilterList({
    super.key,
    required this.linearFilters,
    required this.removeFilter,
    required this.selectFilter,
    required this.selectedIndex,
  });

  final List<LinearFilter> linearFilters;
  final Function(int) removeFilter;
  final Function(int) selectFilter;
  final int? selectedIndex;

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 250,
      color: Theme.of(context).hoverColor,
      child: Column(
        children: [
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Text("List of Filters",
                style: Theme.of(context).textTheme.titleMedium),
          ),
          Expanded(
            child: ListView.builder(
              itemCount: linearFilters.length,
              itemBuilder: (context, index) {
                return LinearFilterTile(
                  linearFilter: linearFilters[index],
                  removeFilter: removeFilter,
                  selectFilter: selectFilter,
                  index: index,
                  isSelected: selectedIndex == index,
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}

class LinearFilterTile extends StatelessWidget {
  const LinearFilterTile({
    super.key,
    required this.linearFilter,
    required this.removeFilter,
    required this.selectFilter,
    required this.index,
    required this.isSelected,
  });

  final LinearFilter linearFilter;
  final Function removeFilter;
  final Function selectFilter;
  final int index;
  final bool isSelected;

  @override
  Widget build(BuildContext context) {
    return ListTile(
      title: Text(
        linearFilter.name,
        style: Theme.of(context).textTheme.bodyMedium,
      ),
      selectedTileColor: Theme.of(context).hoverColor,
      trailing: IconButton(
        icon: const Icon(Icons.delete),
        tooltip: "Remove filter",
        onPressed: () => removeFilter(index),
      ),
      selected: isSelected,
      onTap: () => selectFilter(index),
    );
  }
}