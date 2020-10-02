#include <sys/file.h>
#include <iostream>
#include <string>
#define LM_ALL

class LockedFile {
  FILE* f;
  int fd;
  bool error;
  public:
  bool haserror() { return error; }
  LockedFile(const std::string filename);
  ~LockedFile();
  int WriteString(std::string content);
};
