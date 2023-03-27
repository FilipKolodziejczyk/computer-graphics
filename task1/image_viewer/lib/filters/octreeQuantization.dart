import 'dart:typed_data';
import 'dart:math';
import 'package:tuple/tuple.dart';

class OctreeNode {
  bool isLeaf;
  int sumR = 0;
  int sumG = 0;
  int sumB = 0;
  int count = 0;
  int depth;
  List<OctreeNode?> children = List.filled(8, null);

  OctreeNode(this.isLeaf, this.depth);
}

var rng = Random();

class Octree {
  OctreeNode? root;
  int leafCount = 0;
  int maxLeafCount;
  List<List<OctreeNode>> innerNodes = List.generate(8, (index) => []);

  Octree(this.maxLeafCount);

  int childIndex(int r, int g, int b, int depth) {
    var bitR = (r >> (7 - depth)) & 1;
    var bitG = (g >> (7 - depth)) & 1;
    var bitB = (b >> (7 - depth)) & 1;
    return (bitR << 2) | (bitG << 1) | bitB;
  }

  OctreeNode createNode(int depth) {
    var newNode = OctreeNode(depth == 8, depth);
    if (newNode.isLeaf) {
      leafCount++;
    } else {
      innerNodes[depth].add(newNode);
    }

    return newNode;
  }

  void addRecursive(int r, int g, int b, OctreeNode parent, int depth) {
    if (parent.isLeaf) {
      parent.sumR += r;
      parent.sumG += g;
      parent.sumB += b;
      parent.count++;
    } else {
      var index = childIndex(r, g, b, depth);
      if (parent.children[index] == null) {
        parent.children[index] = createNode(depth + 1);
      }
      addRecursive(r, g, b, parent.children[index]!, depth + 1);
    }
  }

  void add(int r, int g, int b) {
    root ??= createNode(0);
    addRecursive(r, g, b, root!, 0);
  }

  void reduce() {
    var i = 7;
    for (; i >= 0; i--) {
      if (innerNodes[i].isNotEmpty) {
        break;
      }
    }

    var node = innerNodes[i].removeLast();
    var removedCount = 0;

    for (var j = 0; j < 8; j++) {
      if (node.children[j] != null) {
        node.sumR += node.children[j]!.sumR;
        node.sumG += node.children[j]!.sumG;
        node.sumB += node.children[j]!.sumB;
        node.count += node.children[j]!.count;
        node.children[j] = null;
        removedCount++;
      }
    }

    node.isLeaf = true;
    leafCount += 1 - removedCount;
  }

  Tuple3<int, int, int> find(int r, int g, int b) {
    OctreeNode node = root!;
    var depth = 0;
    while (!node.isLeaf) {
      var index = childIndex(r, g, b, depth++);
      node = node.children[index]!;
    }

    return Tuple3<int, int, int>((node.sumR / node.count).round(),
        (node.sumG / node.count).round(), (node.sumB / node.count).round());
  }
}

Uint8List applyOctreeQuantization(
    Uint8List subpixels, int width, int height, int maxLeafCount) {
  var octree = Octree(maxLeafCount);

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    octree.add(subpixels[i], subpixels[i + 1], subpixels[i + 2]);
    while (octree.leafCount > octree.maxLeafCount) {
      octree.reduce();
    }
  }

  for (var i = 0; i < subpixels.lengthInBytes; i += 4) {
    var color = octree.find(subpixels[i], subpixels[i + 1], subpixels[i + 2]);
    subpixels[i] = color.item1;
    subpixels[i + 1] = color.item2;
    subpixels[i + 2] = color.item3;
  }

  return subpixels;
}