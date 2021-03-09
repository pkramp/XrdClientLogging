/********************************************************************************
 *    Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH *
 *                                                                              *
 *              This software is distributed under the terms of the * GNU Lesser
 *General Public Licence version 3 (LGPL) version 3,        * copied verbatim in
 *the file "LICENSE"                       *
 ********************************************************************************/
#include <iostream>
#include <string>
#include <sys/file.h>
#define LM_ALL

class LockedFile {
  FILE *f;
  int fd;
  bool error;

public:
  bool haserror() { return error; }
  LockedFile(const std::string filename);
  ~LockedFile();
  int WriteString(std::string content);
};
