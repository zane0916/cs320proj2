#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void directmapcache(unsigned long long address, int* cache, int &tothits, int &totmiss, int cachesize) {
  int i = address % (*cache).size();
  address = address >> 5;
  if (cachesize == 1) {
    address = address >> 5;
  }
  else if (cachesize == 4) {
    address = address >> 7;
  }
  else if (cachesize == 16) {
    address = address >> 9;
  }
  else if (cachesize == 32) {
    address = address >> 10;
  }
  if (cache[i] == address) {
    tothits += 1;
  }
  else {
    totmiss += 1;
    cache[i] = address;
  }
}

int main(int argc, char *argv[]) {
  unsigned long long addr;
  string behavior;
  string line;

  //all cacheline sizes are 32 bits, need 5 offset bits
  int numblocks = 32;
  int* direcmap1 = new int[numblocks];
  int* direcmap4 = new int[numblocks * 4];
  int* direcmap16 = new int[numblocks * 16];
  int* direcmap32 = new int[numblocks * 32];
  int direcmaphits1 = 0;
  int direcmapmiss1 = 0;
  int direcmaphits4 = 0;
  int direcmapmiss4 = 0;
  int direcmaphits16 = 0;
  int direcmapmiss16 = 0;
  int direcmaphits32 = 0;
  int direcmapmiss32 = 0;
  
  ifstream infile(argv[1]);
  ofstream outfile(argv[2]);

  while (getline(infile, line)) {
    stringstream s(line);
    s >> behavior >> hex >> addr;

    directmapcache(addr, direcmap1, direcmaphits1, direcmapmiss1, 1);
    directmapcache(addr, direcmap4, direcmaphits4, direcmapmiss4, 4);
    directmapcache(addr, direcmap16, direcmaphits16, direcmapmiss16, 16);
    directmapcache(addr, direcmap32, direcmaphits32, direcmapmiss32, 32);
  }

  outfile << direcmaphits1 << ", " << direcmapmiss1 << "; ";
  outfile << direcmaphits4 << ", " << direcmapmiss4 << "; ";
  outfile << direcmaphits16 << ", " << direcmapmiss16 << "; ";
  outfile << direcmaphits32 << ", " << direcmapmiss32 << "; " << endl;

  delete[] direcmap1;
  delete[] direcmap4;
  delete[] direcmap16;
  delete[] direcmap32;
  infile.close();
  outfile.close();
  return 0;
}
