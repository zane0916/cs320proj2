#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void directmapcache(unsigned long long address, int* cache, int &tothits, int cachesize) {
  address = address >> 5;
  int i;
  if (cachesize == 1) {
    i = address % 32;
    address = address >> 5;
  }
  else if (cachesize == 4) {
    i = address % 128;
    address = address >> 7;
  }
  else if (cachesize == 16) {
    i = address % 512;
    address = address >> 9;
  }
  else if (cachesize == 32) {
    i = address % 1024;
    address = address >> 10;
  }
  if (cache[i] == address) {
    tothits += 1;
  }
  else {
    cache[i] = address;
  }
}

void setcache(unsigned long long addr, int** cache, int &tothits, int numways) {
  addr >> 5;
  int i;
  int found = -1;
  if (numways == 2) {
    i = addr % 256;
    addr = addr >> 8;
  }
  else if (numways == 4) {
    i = addr % 128;
    addr = addr >> 7;
  }
  else if (numways == 8) {
    i = addr % 64;
    addr = addr >> 6;
  }
  else if (numways == 16) {
    i = addr % 32;
    addr = addr >> 5;
  }
  
  for (int way = 1; way < numways + 1; way++) {
    //if found
    if (cache[way][i] == addr) {
      tothits += 1;
      //find location in lru
      for (int j = 0; j < numways; j++) {
	if (cache[0][j] == way) {
	  found = j;
	  break;
	}
      }
      //remove and insert in front of lru
      if (found > 0) {
	for (int temp = found - 1; temp >= 0; temp--) {
	  cache[0][temp + 1] = cache[0][temp];
	}
	cache[0][0] = way;
	break;
      }
    }
  }
  //otherwise, not found
  if (found == -1) {
    //replace lru at end of lru queue
    int replace = cache[0][numways - 1];
    for (int temp = numways - 2; temp >= 0; temp--) {
      cache[0][temp + 1] = cache[0][temp];
    }
    cache[0][0] = replace;
    //replace the replace value in cache with addr
    cache[replace][i] = addr;
  }
}

int main(int argc, char *argv[]) {
  unsigned long long addr;
  string behavior;
  string line;
  int numlines = 0;

  //all cacheline sizes are 32 bits, need 5 offset bits
  int* direcmap1 = new int[32];
  int* direcmap4 = new int[128];
  int* direcmap16 = new int[512];
  int* direcmap32 = new int[1024];
  int direcmaphits1 = 0;
  int direcmaphits4 = 0;
  int direcmaphits16 = 0;
  int direcmaphits32 = 0;
  
  //set associative is 16KB, so for 2, 4, 8, 16 ways
  int** set2 = new int*[3];
  set2[0] = new int[2];
  for (int i = 0; i < 2; i++) {
    set2[0][i] = 0;
  }
  for (int temp = 1; temp < 3; temp++) {
    set2[temp] = new int[256];
    for (int j = 0; j < 256; j++) {
      set2[temp][j] = 0;
    }
  }
  
  int** set4 = new int*[5];
  set4[0] = new int[4];
  for (int i = 0; i < 4; i++) {
    set4[0][i] = 0;
  }
  for (int temp = 1; temp < 5; temp++) {
    set4[temp] = new int[128];
    for (int j = 0; j < 128; j++) {
      set4[temp][j] = 0;
    }
  }

  int** set8 = new int*[9];
  set8[0] = new int[8];
  for (int i = 0; i < 8; i++) {
    set8[0][i] = 0;
  }
  for (int temp = 1; temp < 9; temp++) {
    set8[temp] = new int[64];
    for (int j = 0; j < 64; j++) {
      set8[temp][j] = 0;
    }
  }

  int** set16 = new int*[17];
  set16[0] = new int[16];
  for (int i = 0; i < 16; i++) {
    set16[0][i] = 0;
  }
  for (int temp = 1; temp < 17; temp++) {
    set16[temp] = new int[32];
    for (int j = 0; j < 32; j++) {
      set16[temp][j] = 0;
    }
  }
  int sethit2 = 0;
  int sethit4 = 0;
  int sethit8 = 0;
  int sethit16 = 0;
  
  ifstream infile(argv[1]);
  ofstream outfile(argv[2]);

  while (getline(infile, line)) {
    stringstream s(line);
    s >> behavior >> hex >> addr;
    numlines += 1;
    
    directmapcache(addr, direcmap1, direcmaphits1, 1);
    directmapcache(addr, direcmap4, direcmaphits4, 4);
    directmapcache(addr, direcmap16, direcmaphits16, 16);
    directmapcache(addr, direcmap32, direcmaphits32, 32);
    
    setcache(addr, set2, sethit2, 2);
    setcache(addr, set4, sethit4, 4);
    setcache(addr, set8, sethit8, 8);
    setcache(addr, set16, sethit16, 16);   
    
  }

  outfile << direcmaphits1 << "," << numlines << "; ";
  outfile << direcmaphits4 << "," << numlines << "; ";
  outfile << direcmaphits16 << "," << numlines << "; ";
  outfile << direcmaphits32 << "," << numlines << ";" << endl;

  outfile << sethit2 << "," << numlines << "; ";
  outfile << sethit4 << "," << numlines << "; ";
  outfile << sethit8 << "," << numlines << "; ";
  outfile << sethit16 << "," << numlines << ";" << endl;

  outfile << "0," << numlines << ";" << endl;
  outfile << "0," << numlines << ";" << endl;
  
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << ";" << endl;

  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << ";" << endl;
  
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << ";" << endl;

  delete[] direcmap1;
  delete[] direcmap4;
  delete[] direcmap16;
  delete[] direcmap32;

  for (int temp = 0; temp < 3; temp ++) {
    delete[] set2[temp];
  }
  delete[] set2;
  for (int temp = 0; temp < 5; temp ++) {
    delete[] set4[temp];
  }
  delete[] set4;
  for (int temp = 0; temp < 9; temp ++) {
    delete[] set8[temp];
  }
  delete[] set8;
  for (int temp = 0; temp < 17; temp ++) {
    delete[] set16[temp];
  }
  delete[] set16;

  infile.close();
  outfile.close();
  return 0;
}
