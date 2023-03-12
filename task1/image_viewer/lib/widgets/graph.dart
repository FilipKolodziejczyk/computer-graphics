import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';

import '../filters/linearFilter.dart';

class Graph extends StatelessWidget {
  const Graph({
    super.key,
    required this.currentFilter,
    required this.removePoint,
  });

  final LinearFilter? currentFilter;
  final Function removePoint;

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: Center(
        child: Container(
          padding: const EdgeInsets.all(20.0),
          child: LineChart(
            swapAnimationDuration: const Duration(milliseconds: 400),
            LineChartData(
              minX: 0,
              maxX: 255,
              minY: 0,
              maxY: 255,
              titlesData: FlTitlesData(show: false),
              lineBarsData: [
                LineChartBarData(
                  spots: currentFilter?.points.entries
                          .map((e) =>
                              FlSpot(e.key.toDouble(), e.value.toDouble()))
                          .toList() ??
                      [],
                ),
              ],
              lineTouchData: LineTouchData(
                enabled: true,
                touchCallback:
                    (FlTouchEvent event, LineTouchResponse? touchResponse) {
                  if (event is FlTapUpEvent) {
                    if (touchResponse?.lineBarSpots?.isNotEmpty ?? false) {
                      removePoint(touchResponse!.lineBarSpots![0].x.toInt());
                    }
                  }
                },
                touchTooltipData: LineTouchTooltipData(
                  tooltipBgColor: Colors.grey,
                  getTooltipItems: (List<LineBarSpot> touchedBarSpots) {
                    return touchedBarSpots.map((barSpot) {
                      final flSpot = barSpot;
                      return LineTooltipItem(
                        "(${flSpot.x.toInt()}, ${flSpot.y.toInt()})",
                        const TextStyle(color: Colors.white),
                      );
                    }).toList();
                  },
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}