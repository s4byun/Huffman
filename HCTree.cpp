// HCTree.cpp

#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCTree.hpp"
#include <stack>

/*
 * Destructor
 * Deletes all the nodes from heap, so there is no memory leak.
 */
HCTree::~HCTree() {
  deleteAllNodes(root);
}

/*
 * Build the Huffman tree
 * Parameter: a vector that contains each character's frequency.
 * Result: characters with frequency 0 are discarded, and the rest are
 *         sorted in increasing order of frequency. The sorted characters
 *         are used to build the Huffman tree.
 *         Root pointer is at the node with the most frequent character.
 */
void HCTree::build(const vector<int>& freqs) {
  // Priority queue is used to sort the nodes in increasing order of frequency.
  priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq; 

  int index = 0;  // Used to store new nodes in leaves array only when the
                  // corresponding frequency is not 0.
  
  // Loop to make new nodes and push them into pq.
  for(unsigned long i=0; i<freqs.size(); i++) {
    // If the frequency of character i is not 0
    if(freqs[i] != 0) {
      // Allocate memory for a new node
      leaves[index] = new HCNode(freqs[i], (int) i);
      pq.push(leaves[index]);
      index++;
    }
  }

  // Build the Huffman tree bottom-up until there is only the root node left.
  while(pq.size() > 1) {

    // nodes 1 and 2 are the two nodes with the least counts in pq.
    HCNode* node1 = pq.top();
    pq.pop();
    HCNode* node2 = pq.top();
    pq.pop();

    // Make a new node whose count is the sum of the counts of nodes 1 and 2.
    HCNode* newNode = new HCNode(node1->count + node2->count, '\0');

    // Assign the new node's children to be nodes 1 and 2.
    newNode->c0 = node1;  
    newNode->c1 = node2;  

    // Assign the parent of node1 and node2 to be the new node.
    node1->p = node2->p = newNode;  

    pq.push(newNode); // push the new node into the priority queue.
  }

  // Last node in the pq is the root node.
  root = pq.top();
}

/*
 * Encodes a symbol using the Huffman tree.
 * Parameters: symbol - the symbol we are encoding
 *             out - BitOutputStream (our own implementation of a buffered
 *                   output stream)
 *
 * Result: The symbol gets encoded following the path in the Huffman tree
 *         to find that symbol, and that path, in 0s and 1s, gets written
 *         on the output file.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  HCNode* node(0);    // Placeholder used to traverse the tree
  stack<int> code;    // To implement the LIFO order of the traversal path
  
  // Finds the leaf node whose key matches the symbol
  for(unsigned long i=0; i<leaves.size(); i++) {
    if(leaves[i]->symbol == symbol) {
      node = leaves[i];
      break;
    }
  }
  
  // Loop until the root node is reached
  while(node->p != 0) {
    
    // If the node is the left child of its parent, push 0 to stack.
    // Otherwise, push 1.
    (node == node->p->c0) ? code.push(0) : code.push(1);

    // Traverse up the tree.
    node = node->p;   
  }

  // Write all the bits in the stack in FILO order.
  while(!code.empty()) {
    out.writeBit(code.top());
    code.pop();
  }
}

/*
 * Decodes a symbol using the Huffman tree.
 * Parameter: BitInputStream (our own implementation of a buffered input stream)
 *
 * Result: This function reads from a file bit by bit, traversing our Huffman
 *         tree down to the left when the bit is 0, and to the right otherwise.
 *        
 *         Returns the symbol of the leaf node it ends up at.
 */

int HCTree::decode(BitInputStream& in) const {
  
  HCNode* node = root;  // Traversal starts from the root.
  int direction;        // Stores the next bit read from the input file.
  
  // Loop until the leaf node is reached.
  while(node->c0 || node->c1) {
    direction = in.readBit();

    // If the next bit is 0, go down to the left child.
    // Otherwise, go right.
    node = (direction == 0) ? node->c0 : node->c1;
  }
  return (int) node->symbol;
}

// Final Submission ends here.





//~   *****************************************************************   ~//
//~~  *                                                               *  ~~//
//~~~ * CHECKPOINT FUNCTIONS THAT ARE NOT USED (Deprecated functions) * ~~~// 
//~~  *                                                               *  ~~//
//~   *****************************************************************   ~//


/*
 * Encodes a symbol using the Huffman tree.
 * Parameters: symbol - the symbol we are encoding
 *             out - the output stream (file in our case)
 * Result: The symbol gets encoded following the path in the Huffman tree
 *         to find that symbol, and that path (left=0, right=1) gets written
 *         on the output file.
 */
void HCTree::encode(byte symbol, ofstream& out) const {
  HCNode* node(0);    // Placeholder for the node with the symbol we're looking for.
  
  stack<char> code;   // Since we're traversing the tree from the leaf to root, the
                      // encoding path is reversed if it's written in the traversal
                      // order. To prevent this in a easy way, I used a stack.

  // Locate the node with the symbol
  for(unsigned long i=0; i<leaves.size(); i++) {
    if(leaves[i]->symbol == symbol) {
      node = leaves[i];
      break;
    }
  }

  // Go up the parent pointer all the way up to the root, saving the path in
  // the stack.
  while(node->p != 0) {
    // If the node is the left child of its parent, push '0' to the stack.
    // Otherwise, push '1' to the stack.
    if(node == node->p->c0) {
      code.push('0');
    } else {
      code.push('1');
    }

    // Traversing up the tree.
    node = node->p;
  }

  // Write the encoded symbol to the output file
  while(!code.empty()) {
    out.put(code.top());
    code.pop();
  }
}

/*
 * Decodes a symbol using the Huffman tree.
 * Parameter: The input stream (file in our case) 
 * Result: The input file contains a line of '1's and '0's.
 *         This function reads the file character by character, and traverses
 *         the Huffman tree to the left when it encounters '0' and to the right
 *         when it encounters '1'.
 *        
 *         Returns the symbol of the leaf node it ends up at.
 */
int HCTree::decode(ifstream& in) const {
  // If there is no more to be read, return EOF.
  if(in.peek() == EOF) {
    return EOF;
  }

  HCNode* node = root;  // Start the traversal at the root
  int direction;        // The character read from the input file.

  // Keep going down the tree until the leaf node is reached.
  while(node->c0 || node->c1) {
    direction = in.get();   // Read a character from the input file.
    // If the character is '0', go down to the left child.
    if(direction == '0') {
      node = node->c0;
    }

    // If the character is '1', go down to the right child.
    else {
      node = node->c1;
    }
  }

  // Return the leaf node's symbol.
  return (int) node->symbol;
}
