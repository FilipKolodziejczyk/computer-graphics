import 'package:flutter/material.dart';

import '../filters/linearFilter.dart';
import 'filterManager.dart';
import 'graph.dart';
import 'filterManagerActions.dart';
import 'filterList.dart';

class LinearFilterManagerState extends State<LinearFilterManager> {
  int? _selectedIndex;

  void add() => setState(() {
        widget.linearFilters.add(LinearFilter('New Filter', {0: 0, 255: 255}));
        _selectedIndex = widget.linearFilters.length - 1;
      });

  void remove(int index) => setState(() {
        widget.linearFilters.removeAt(index);
        if (_selectedIndex == index) _selectedIndex = null;
      });

  void select(int index) => setState(() => _selectedIndex = index);

  void addPoint(int x, int y) => setState(() {
        if (x < 0 || x > 255 || y < 0 || y > 255) return;
        if (_selectedIndex == null) return;

        widget.linearFilters[_selectedIndex!].points[x] = y;
        widget.linearFilters[_selectedIndex!].regenerate();
      });

  void removePoint(int x) => setState(() {
        if (_selectedIndex == null) return;
        if (!widget.linearFilters[_selectedIndex!].points.containsKey(x)) {
          return;
        }

        if (x == 0) {
          widget.linearFilters[_selectedIndex!].points[0] = 0;
        } else if (x == 255) {
          widget.linearFilters[_selectedIndex!].points[255] = 255;
        } else {
          widget.linearFilters[_selectedIndex!].points.remove(x);
          widget.linearFilters[_selectedIndex!].regenerate();
        }
      });

  void rename(String name) => setState(() {
        if (_selectedIndex != null) {
          widget.linearFilters[_selectedIndex!].name = name;
        }
      });

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Image Viewer'),
        backgroundColor: Theme.of(context).focusColor,
        automaticallyImplyLeading: false,
        actions: [
          AddPointButton(
            addPoint: addPoint,
            isSelected: _selectedIndex != null,
          ),
          AddButton(add: add),
          RenameButton(rename: rename, isSelected: _selectedIndex != null),
          ReturnButton(returnValue: widget.linearFilters),
        ],
      ),
      body: Row(
        children: [
          FilterList(
            linearFilters: widget.linearFilters,
            removeFilter: remove,
            selectFilter: select,
            selectedIndex: _selectedIndex,
          ),
          Graph(
            currentFilter: _selectedIndex != null
                ? widget.linearFilters[_selectedIndex!]
                : null,
            removePoint: removePoint,
          ),
        ],
      ),
    );
  }
}