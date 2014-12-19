#include "BitOutputStream.hpp"

// Writes a bit to the output stream.
void BitOutputStream::writeBit(int bit) { 
  // Set the next bit in buffer to the parameter bit
  buf = (bit << (7-bufi)) | buf;
  bufi++;

  // If the buffer is full
  if(bufi==8) {
    out.put(buf);

    // Reset the buffer
    buf = 0;
    bufi = 0;
  }
}

// Writes a character (byte) to the output stream.
void BitOutputStream::writeByte(int b) {
  out.put((char) b);
}

// Writes an integer (4 bytes) to the output stream.
void BitOutputStream::writeInt(int i) { 
  out.write((char*)(&i), sizeof(i));
}

// Writes any remaining bits in the buffer to the output stream,
// and clears the buffer.
void BitOutputStream::flush() { 
  out.put(buf);
  out.flush();
  buf = 0;
  bufi = 0;
}
