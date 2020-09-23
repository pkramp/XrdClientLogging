#include "LockingIO.hh"
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

LockedFile::LockedFile(const std::string filename)
    : error(false), f(fopen(filename.c_str(), "a+")), fd(fileno(f))
{
  if (f == 0) std::cerr << "LockFile()::Cannot open file" << std::endl;
  // set EX Lock
  if (0 != flock(fd, LOCK_EX)) {  // build lock
    cerr << "Lockfile():: Failed to acquire lock" << endl;
  }
}
LockedFile::~LockedFile()
{
  if (flock(fd, LOCK_UN) != 0)
    std::cerr << "~LockedFile()::Failed to release lock" << endl;
  int err = fclose(f);
}
int LockedFile::WriteString(std::string content)
{
  fflush(f);
  unsigned long pos = ftell(f);
  while (true) {
    fseek(f, pos, SEEK_SET);
    if (fprintf(f, "%s", content.c_str()) != content.size())
      std::cerr << "not enough bytes written" << std::endl;
    else
      break;
  }
  return 0;
}
