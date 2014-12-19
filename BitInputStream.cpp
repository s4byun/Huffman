#include "BitInputStream.hpp"

// Reads the next bit from the file and returns it as an int.
int BitInputStream::readBit() {
  // If all 8 bits of a char is read
  if(bufi == 8) {
    buf = in.get();   // Read the next 8 bits from input stream
    bufi = 0;
  }
  
  int bit = buf & 0x80; // Extract the MSB
  bufi++;           // Increment the index (# of bits read so far)
  buf = buf << 1;   // Move the next bit to be read to the MSB
  
  return bit >> 7;  // Move the extracted bit to the LSB 
}

// Not used in this program.
int BitInputStream::readByte() { 
  return 0;
}

// Reads a whole integer from the input stream.
int BitInputStream::readInt() { 
  int num = 0;
  in.read((char *)&num, sizeof(num));

  return (int) num;
}
