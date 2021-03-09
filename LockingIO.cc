/********************************************************************************
 *    Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH *
 *                                                                              *
 *              This software is distributed under the terms of the * GNU Lesser
 *General Public Licence version 3 (LGPL) version 3,        * copied verbatim in
 *the file "LICENSE"                       *
 ********************************************************************************/
#include "LockingIO.hh"

LockedFile::LockedFile(const std::string filename)
    : error(false), f(fopen(filename.c_str(), "a+")), fd(fileno(f)) {
  if (f == 0)
    std::cerr << "LockFile()::Cannot open file" << std::endl;
  // set EX Lock
  if (0 != flock(fd, LOCK_EX)) // build lock
    std::cerr << "Lockfile():: Failed to acquire lock" << std::endl;
}
LockedFile::~LockedFile() {
  if (flock(fd, LOCK_UN) != 0)
    std::cerr << "~LockedFile()::Failed to release lock" << std::endl;
  int err = fclose(f);
}
int LockedFile::WriteString(std::string content) {
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
