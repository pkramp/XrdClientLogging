/********************************************************************************
 *    Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef __XRDCLIENT_LOGGING_PLUGIN_HH___
#define __XRDCLIENT_LOGGING_PLUGIN_HH___
#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include "XrdVersion.hh"
#include "XrdCl/XrdClPlugInInterface.hh"
#include "XrdCl/XrdClFile.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClUtils.hh"
#include "XrdCl/XrdClLog.hh"
#include "XrdCl/XrdClDefaultEnv.hh"
#include "LockingIO.hh"
#include <fstream>
#include <ctime>
#include <chrono>


namespace XrdClientLoggingFactory{
//----------------------------------------------------------------------------
// Plugin factory
//----------------------------------------------------------------------------
class XOLFactory : public XrdCl::PlugInFactory {
	public:
		//------------------------------------------------------------------------
		// Constructore
		//------------------------------------------------------------------------
		XOLFactory(const std::map<std::string, std::string> &config);
		//------------------------------------------------------------------------
		// Destructor
		virtual ~XOLFactory();

		//------------------------------------------------------------------------
		// Create a file plug-in for the given URL
		//------------------------------------------------------------------------
		virtual XrdCl::FilePlugIn *CreateFile( const std::string &url );

		//------------------------------------------------------------------------
		// Create a file system plug-in for the give/Lon URL
		virtual XrdCl::FileSystemPlugIn *CreateFileSystem(const std::string &url);
		//------------------------------------------------------------------------
		// load default config:
		void	loadDefaultConf( std::map<std::string,std::string>&);
		

	private:
		std::string loggingPath;
};
};

#endif // __XRDCLIENT_LOGGING_PLUGIN_HH___
