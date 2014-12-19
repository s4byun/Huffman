// HCNode.cpp

#include <iostream>
#include "HCNode.hpp"

using namespace std;

/*
 * Compare function used to sort the nodes in priority queue
 */
bool HCNode::operator<(const HCNode& other) {
  // Lower count has higher priority
  if(count != other.count) return count > other.count;

  // If the counts are equal, lower symbol, according to ascii value,
  // has higher priority
  return symbol < other.symbol;
}

