# XrdOpenLocal 
An XRootD client plug-in to log bytes read and written by XRootD calls


# Warning
The source tree includes private header files of the XrdCl source tree and needs to be kept in sync with the XRootD version source used!

# Plug-in configuration

This plug-in is configured via a plug-in configuration file in /etc/xrootd/client.plugins.d/.conf, or in the users ~/.xrootd.client.plugins.d/.conf

Example configuration to write to logfile:
```shell
url = file://localhost/* 
loggingPath = /path/to/logfile
lib = /path/to/XrdClientLogging.so
enable = true
```
## As default plug-in

Additionally, XRootD allows to set a client plug-in as default to use via the XRD_PLUGIN environmental variable.
To use the plug-in, you need to set:

```shell
export XRD_DEFAULT_PLUGIN_CONF="/path/to/configDir/XrdClientLogging.conf"
export XRD_PLUGINCONFDIR="/path/to/configDir"
```

# Install:
To compile the plug-in, you need to set the XRD_PATH environmental variable to the toplevel of your XRootD installation.

You can compile the plug-in library with :
```shell
make
```

# Usage
When using this plug-in, all high level XRootD calls (xrdcp, from TNetXNGFile in ROOT, etc.) to targets configured in the config file, should log the amount of bytes read or written together with timestamps to the config file at 

# License
The XrdClientLogging plug-in is distributed under the terms of the GNU Lesser Public Licence version 3 (LGPLv3)
