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
/*
void setcache(unsigned long long addr, int** cache, int &tothits, vector<int> ldr, int numways) {
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
  for (int f = 0; f < numways; f++) {
    if (cache[f][i] == addr) {
      tothits += 1;
      for (int temp = 0; temp < ldr.size(); temp++) {
	if (ldr[temp] == addr) {
	  found = temp;
	  break;
	}
      }
      ldr.erase(ldr.begin() + found);
      ldr.insert(ldr.begin(), addr);
    }
  }
  int oldest = -1;
  for (int f = 0; f < numways; f++) {
    for (int temp = 0; temp < 512; temp++) {
      if (ldr[temp] == cache[f][i] && oldest < temp) {
	oldest = temp;
      }
    }
  }
  ldr.erase(ldr.begin() + oldest);
  ldr.insert(ldr.begin(), addr);
}
*/
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
  int direcmapmiss1 = 0;
  int direcmapmiss4 = 0;
  int direcmapmiss16 = 0;
  int direcmapmiss32 = 0;
  /*
  //set associative is 16KB, so for 2, 4, 8, 16 ways
  int** set2 = new int*[2];
  for (int temp = 0; temp < 2; temp++) {
    set2[temp] = new int[256];
  }
  int** set4 = new int*[4];
  for (int temp = 0; temp < 4; temp++) {
    set4[temp] = new int[128];
  }
  int** set8 = new int*[8];
  for (int temp = 0; temp < 8; temp++) {
    set8[temp] = new int[64];
  }
  int** set16 = new int*[16];
  for (int temp = 0; temp < 16; temp++) {
    set16[temp] = new int[32];
  }
  int sethit2 = 0;
  int sethit4 = 0;
  int sethit8 = 0;
  int sethit16 = 0;
  vector<int> ldr2 (512, 0);
  vector<int> ldr4 (512, 0);
  vector<int> ldr8 (512, 0);
  vector<int> ldr16 (512, 0);
  */
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
    /*
    setcache(addr, set2, sethit2, ldr2, 2);
    setcache(addr, set4, sethit4, ldr4, 4);
    setcache(addr, set8, sethit8, ldr8, 8);
    setcache(addr, set16, sethit16, ldr16, 16);   
    */
  }

  outfile << direcmaphits1 << "," << numlines << "; ";
  outfile << direcmaphits4 << "," << numlines << "; ";
  outfile << direcmaphits16 << "," << numlines << "; ";
  outfile << direcmaphits32 << "," << numlines << "; " << endl;

  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; " << endl;

  outfile << "0," << numlines << "; " << endl;
  outfile << "0," << numlines << "; " << endl;
  
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; " << endl;

  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; " << endl;
  
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; ";
  outfile << "0," << numlines << "; " << endl;
  /*
  outfile << sethit2 << "," << numlines << "; ";
  outfile << sethit4 << "," << numlines << "; ";
  outfile << sethit8 << "," << numlines << "; ";
  outfile << sethit16 << "," << numlines << "; " << endl;
  */
  delete[] direcmap1;
  delete[] direcmap4;
  delete[] direcmap16;
  delete[] direcmap32;
  /*
  for (int temp = 0; temp < 2; temp ++) {
    delete[] set2[temp];
  }
  delete[] set2;
  for (int temp = 0; temp < 4; temp ++) {
    delete[] set4[temp];
  }
  delete[] set4;
  for (int temp = 0; temp < 8; temp ++) {
    delete[] set8[temp];
  }
  delete[] set8;
  for (int temp = 0; temp < 16; temp ++) {
    delete[] set16[temp];
  }
  delete[] set16;
  */
  infile.close();
  outfile.close();
  return 0;
}
