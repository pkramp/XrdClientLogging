/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "XrdClientLogging.hh"
#include <exception>
#include <cstdlib>
#include <string>
#include <utility>
#include <assert.h>
#include <exception>
#include <string>
using namespace XrdCl;
XrdVERSIONINFO(XrdClGetPlugIn, ClientLogging);

namespace ClientLogging {
class ClientLoggingFile : public XrdCl::FilePlugIn {
private:
	///@file file for local access
	XrdCl::File xfile{false};
	std::string path;
	uint32_t writtenSize;
	uint32_t readSize;
	unsigned long long startTime;
	unsigned long long stopTime;
	std::string loggingPath;
	
public:

	//Constructor
	ClientLoggingFile(std::string x, std::string loggingPath):xfile(false), loggingPath(loggingPath), writtenSize(0), readSize(0){
	}

	//Destructor
	~ClientLoggingFile() {
		//Create the file to log in
		LockedFile lFile(loggingPath+"/XrdClientLogging.log");
		std::stringstream ss;
		ss <<  (readSize? "r" : "w") << "," << path.substr(0,7) << "," << path << "," <<  
		(readSize? readSize : writtenSize) << "," << startTime << "," << stopTime << "," 
			<< stopTime - startTime << "\n" ;
		lFile.WriteString(ss.str());
	}

	//Open()
	virtual XRootDStatus Open( const std::string &url,
	                           OpenFlags::Flags   flags,
	                           Access::Mode       mode,
	                           ResponseHandler   *handler,
	                           uint16_t           timeout ) {
		path = url;
		startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return xfile.Open(url,flags,mode,handler,timeout);
	}

	virtual XRootDStatus Close(ResponseHandler *handler,uint16_t timeout) {
		stopTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return xfile.Close(handler,timeout);
	}

	virtual bool IsOpen()  const    {
		return xfile.IsOpen();
	}
	virtual XRootDStatus Stat(bool force,ResponseHandler *handler,uint16_t timeout) {	
		return xfile.Stat(force,handler,timeout);
	}

	virtual XRootDStatus Read(uint64_t offset,uint32_t length,
	                          void  *buffer,XrdCl::ResponseHandler *handler,
	                          uint16_t timeout ) {
		readSize += length;
		return xfile.Read(offset,length,buffer,handler,timeout);
	}

	virtual XRootDStatus Write( uint64_t         offset,
	                    uint32_t         size,
	                    const void      *buffer,
	                    ResponseHandler *handler,
	                    uint16_t         timeout = 0 ) {
		writtenSize += size;
		return xfile.Write(offset,size,buffer,handler,timeout);
	}
	virtual XRootDStatus Truncate( uint64_t size, ResponseHandler *handler, uint16_t timeout ) {	
		return xfile.Truncate(size, handler, timeout);
	}

	virtual XRootDStatus VectorRead( const ChunkList &chunks,
                                 void            *buffer,
                                 ResponseHandler *handler,
                                 uint16_t         timeout ) {
		return xfile.VectorRead( chunks, buffer, handler, timeout );
	}
};
class ClientLoggingFs : public XrdCl::FileSystemPlugIn {
private:
public:
	XrdCl::FileSystem fs;
	//Constructor
	ClientLoggingFs(std::string url):fs(url,false) {
	}
	//Destructor
	~ClientLoggingFs() {
	}
};
};
namespace XrdClientLoggingFactory {
XOLFactory::XOLFactory( const std::map<std::string, std::string> &config ) :
	XrdCl::PlugInFactory() {
	XrdCl::Log *log = DefaultEnv::GetLog();
	log->Debug( 1, "XrdClientLoggingFactory::Constructor" );
	if (config.find("loggingPath") != config.end()) {
		loggingPath = config.at("loggingPath");
	}
}

XOLFactory::~XOLFactory() {
}

XrdCl::FilePlugIn * XOLFactory::CreateFile( const std::string &url ) {
	return static_cast<XrdCl::FilePlugIn *> (new ClientLogging::ClientLoggingFile(url, loggingPath)) ;
}

XrdCl::FileSystemPlugIn * XOLFactory::CreateFileSystem(const std::string &url) {
	return static_cast<XrdCl::FileSystemPlugIn *> (new ClientLogging::ClientLoggingFs(url)) ;

}
}
extern "C" {
	void *XrdClGetPlugIn(const void *arg) {
		const std::map<std::string, std::string> &pconfig = *static_cast <const std::map<std::string, std::string> *>(arg);
		void * plugIn = new XrdClientLoggingFactory::XOLFactory(pconfig);
		return plugIn;
	}
}
