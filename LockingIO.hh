//#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

//#define LM_NNNN
#define LM_ALL

class LockedFile {
  FILE* f;
  int fd;
  bool error;  // rwmode true=>write, false=>read
  public:
  bool haserror() { return error; }
  LockedFile(const std::string filename);
  ~LockedFile();
  int WriteString(std::string content);
};
