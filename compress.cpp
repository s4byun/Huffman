// compress.cpp

#include "HCTree.hpp"

#include <vector>
#include <iostream>
#include <fstream>

#define ARG_COUNT 3

using namespace std;

int main(int argc, char** argv) {
  HCTree tree;      // Huffman tree
  ifstream infile;  // Input file
  ofstream outfile; // Output file
  vector<int> counts(256, 0); // Keeps track of each character's frequency
  int nextChar;   // Used to store the character read from the input file    
  int total = 0;

  // If the number of command-line arguments is not 3
  if(argc != ARG_COUNT) {
    cerr << "ERROR: Incorrect number of arguments" << endl;
    exit(0);
  }
  
  infile.open(argv[1], ios::binary); 
  outfile.open(argv[2], ios::binary);  

  // If the file is good and has something
  if(!infile.good() || infile.peek() == EOF) {
    cerr << "ERROR: Check input file" << endl;
    exit(0);
  }

  // Loop until there is no more to be read
  while((nextChar = infile.get()) != EOF) {
      counts[nextChar]++;
      total++;
  }
 
  tree.build(counts);
  
  infile.clear(); // Required to rewind the file, since it already reached EOF
  infile.seekg(0, ios::beg); // Rewind the file to beginning
  
  BitInputStream in(infile);
  BitOutputStream out(outfile);

  out.writeInt(total); // Write the total number of characters in input file.

  // Write the entire counts vector at the header of the output file 
  for(unsigned long i=0; i<counts.size(); i++) {
    out.writeInt(counts[i]);
  }

  // Encode every character in the input file and write it to the output file.
  while((nextChar = infile.get()) != EOF) {
    tree.encode(nextChar, out);
  }
  
  // Write any remaining bits in the BitOutputStream buffer to the output file.
  out.flush();
}
