/********************************************************************************
 *    Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH *
 *                                                                              *
 *              This software is distributed under the terms of the * GNU Lesser
 *General Public Licence version 3 (LGPL) version 3,        * copied verbatim in
 *the file "LICENSE"                       *
 ********************************************************************************/
#ifndef __XRDCLIENT_LOGGING_PLUGIN_HH___
#define __XRDCLIENT_LOGGING_PLUGIN_HH___
#include <chrono>
#include <string>
#include "LockingIO.hh"
#include "XrdCl/XrdClFile.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClPlugInInterface.hh"
#include "XrdCl/XrdClDefaultEnv.hh"
#include "XrdVersion.hh"

namespace XrdClientLoggingFactory {
//----------------------------------------------------------------------------
// Plugin factory
//----------------------------------------------------------------------------
class XCLFactory : public XrdCl::PlugInFactory {
  public:
  //------------------------------------------------------------------------
  // Constructore
  //------------------------------------------------------------------------
  XCLFactory(const std::map<std::string, std::string> &config);
  //------------------------------------------------------------------------
  // Destructor
  virtual ~XCLFactory();

  //------------------------------------------------------------------------
  // Create a file plug-in for the given URL
  //------------------------------------------------------------------------
  virtual XrdCl::FilePlugIn *CreateFile(const std::string &url);

  //------------------------------------------------------------------------
  // Create a file system plug-in for the give/Lon URL
  virtual XrdCl::FileSystemPlugIn *CreateFileSystem(const std::string &url);
  //------------------------------------------------------------------------
  // load default config:
  void loadDefaultConf(std::map<std::string, std::string> &);

  private:
  std::string loggingPath;
};
};

#endif  // __XRDCLIENT_LOGGING_PLUGIN_HH___
