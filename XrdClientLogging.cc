/********************************************************************************
 *    Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH *
 *                                                                              *
 *              This software is distributed under the terms of the * GNU Lesser
 *General Public Licence version 3 (LGPL) version 3,        * copied verbatim in
 *the file "LICENSE"                       *
 ********************************************************************************/

#include "XrdClientLogging.hh"
using namespace XrdCl;
XrdVERSIONINFO(XrdClGetPlugIn, ClientLogging);

namespace ClientLogging {
class ClientLoggingFile : public XrdCl::FilePlugIn, public XrdCl::File {
  private:
  ///@file file for local access
  std::string path;
  uint32_t writtenSize;
  uint32_t readSize;
  unsigned long long startTime;
  unsigned long long stopTime;
  std::string loggingPath;

  public:
  // Constructor
  ClientLoggingFile(std::string x, std::string loggingPath)
      : XrdCl::File(false),
	loggingPath(loggingPath),
	writtenSize(0),
	readSize(0)
  {
  }

  // Destructor
  ~ClientLoggingFile()
  {
    // Create the file to log in
    LockedFile lFile(loggingPath + "/XrdClientLogging.log");
    std::stringstream ss;
    std::string url1;
    XrdCl::File::GetProperty( "LastURL", url1 );
    ss << (readSize ? "r" : "w") << "," << url1.substr(0, 7) << "," << path
       << "," << (readSize ? readSize : writtenSize) << "," << startTime << ","
       << stopTime << "," << stopTime - startTime << "\n";
    lFile.WriteString(ss.str());
  }

  virtual XRootDStatus Open(const std::string &url, OpenFlags::Flags flags,
			    Access::Mode mode, ResponseHandler *handler,
			    uint16_t timeout)
  {
    using namespace std::chrono;
    path = url;
    startTime =
	duration_cast<milliseconds>(system_clock::now().time_since_epoch())
	    .count();
    return XrdCl::File::Open(url, flags, mode, handler,
						  timeout);
  }

  virtual XRootDStatus Close(ResponseHandler *handler, uint16_t timeout)
  {
    using namespace std::chrono;
    stopTime =
	duration_cast<milliseconds>(system_clock::now().time_since_epoch())
	    .count();
    return XrdCl::File::Close(handler,
						   timeout);  // call base
  }

  virtual XRootDStatus Read(uint64_t offset, uint32_t length, void *buffer,
			    XrdCl::ResponseHandler *handler, uint16_t timeout)
  {
    readSize += length;
    return XrdCl::File::Read(offset, length, buffer,
						  handler,
						  timeout);  // call base
  }

  virtual XRootDStatus Write(uint64_t offset, uint32_t size, const void *buffer,
			     ResponseHandler *handler, uint16_t timeout = 0)
  {
    writtenSize += size;
    return XrdCl::File::Write(
	offset, size, buffer, handler, timeout);  // call base
  }
  
  
  virtual bool IsOpen()  const    {
  return XrdCl::File::IsOpen();
}
virtual XRootDStatus Stat(bool force,ResponseHandler *handler,uint16_t timeout) {	
  return XrdCl::File::Stat(force,handler,timeout);
}

};
class ClientLoggingFs : public XrdCl::FileSystemPlugIn {
  private:
  public:
  XrdCl::FileSystem fs;
  // Constructor
  ClientLoggingFs(std::string url) : fs(url, false) {}
  // Destructor
  ~ClientLoggingFs() {}
};
};  // namespace ClientLogging
namespace XrdClientLoggingFactory {
XCLFactory::XCLFactory(const std::map<std::string, std::string> &config)
    : XrdCl::PlugInFactory(), loggingPath(config.at("loggingPath"))
{
}

XCLFactory::~XCLFactory() = default;

XrdCl::FilePlugIn *XCLFactory::CreateFile(const std::string &url)
{
  return static_cast<XrdCl::FilePlugIn *>(
      new ClientLogging::ClientLoggingFile(url, loggingPath));
}

XrdCl::FileSystemPlugIn *XCLFactory::CreateFileSystem(const std::string &url)
{
  return static_cast<XrdCl::FileSystemPlugIn *>(
      new ClientLogging::ClientLoggingFs(url));
}
}  // namespace XrdClientLoggingFactory
extern "C" {
void *XrdClGetPlugIn(const void *arg)
{
  const std::map<std::string, std::string> &pconfig =
      *static_cast<const std::map<std::string, std::string> *>(arg);
  void *plugIn = new XrdClientLoggingFactory::XCLFactory(pconfig);
  return plugIn;
}
}
