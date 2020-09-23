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

#include "LockingIO.hh"

LockedFile::LockedFile(const std::string filename, bool read, bool write)
    : error(false), existed(true), wmode(write)
{
  int pos = filename.find_last_of("/");

  // test if dir exists, generate otherwise
  if (pos != string::npos) {
    string path = filename.substr(0, pos);
    if (opendir(path.c_str()) == NULL)
      mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }

  // check if file already exists:

  struct stat buffer;
  if (stat(filename.c_str(), &buffer) == 0) {
    f = fopen(filename.c_str(), "a+");
  }
  else {
    if (write) {
      f = fopen(filename.c_str(), "a+");
    }
    else {
      existed = false;
    }
  }
  if (f == 0) {
    std::cerr << "LockFile()::Cannot open file" << std::endl;
  }

  // get file descriptor
  fd = fileno(f);
  // set EX Lock
  if (0 == flock(fd, write ? LOCK_EX : LOCK_SH)) {  // build lock
  }
  else {
    cerr << "Lockfile():: Failed to acquire lock" << endl;
    error = true;
  }
}
LockedFile::~LockedFile()
{
  if (flock(fd, LOCK_UN) != 0)
    cerr << "~LockedFile()::Failed to release lock" << endl;
  std::cerr << "~LockedFile" << std::endl;

  int err = fclose(f);
}
int LockedFile::WriteString(std::string content)
{
  if (wmode) {
    int bytes = 0;
    fflush(f);
    unsigned long pos = ftell(f);
    do {
      fseek(f, pos, SEEK_SET);
      bytes = fprintf(f, "%s", content.c_str());
      if (bytes != content.size()) {
	std::cerr << "not enough bytes written" << std::endl;
      }
    } while (bytes < content.size());
    //  ftruncate(fileno(f), content.size());
  }
  else {
    error = true;
    std::cerr << "WriteString::Cannot write to a file opened in readonly mode"
	      << std::endl;
    return -1;
  }
  return 0;
}
std::vector<std::string> LockedFile::ReadLines()
{
  error |= !existed;
  std::vector<std::string> lines;
  if (error) return lines;  // return if error or not existed
  fseek(f, 0, 0);

  ssize_t r;
  char* line = NULL;
  size_t line_size = 0;
  while ((r = getline(&line, &line_size, f)) != -1) {
    lines.emplace_back(line);
    // reset vars for getline
    free(line);
    line_size = 0;
    line = NULL;
  }
  if (lines.size() == 0) {
    std::cerr << "zero lines, is file empty?" << std::endl;
    error = true;
  }
  return lines;
};
