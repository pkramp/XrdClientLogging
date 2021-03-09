# XrdClientLogging 
An XRootD client plug-in to log bytes read and written by XRootD calls. Additionally, the protocol used is logged as well as timestamps.

# Usage
When using this plug-in, all high level XRootD calls (xrdcp, from TNetXNGFile in ROOT, etc.) to targets configured in the config file, should log the amount of bytes read or written together with timestamps to the config file as configured in the plug-in config.

# License
The XrdClientLogging plug-in is distributed under the terms of the GNU Lesser Public Licence version 3 (LGPLv3)

# Install:
To compile the plug-in, you need to set the XRD_PATH environmental variable to the toplevel of your XRootD installation.
```shell
export XRD_PATH=/path/to/xrootd
```
You can compile the plug-in library with :
```shell
make clean && make
```
If necessary, copy the .so to match the path in the "lib" option in the config:
```shell
cp XrdClientLogging.so /destination/path
```

# Plug-in configuration
This plug-in is configured via a plug-in configuration file in /etc/xrootd/client.plugins.d/.conf, or in the users ~/.xrootd.client.plugins.d/.conf
Additionally, XRootD allows to specify the config directory via the XRD_PLUGINCONFDIR environmental variable:
```shell
export XRD_PLUGINCONFDIR="/path/to/configDir"
```
Example configuration to write to logfile:
```shell
url = /*;root://*;http://*
loggingPath = /path/to/logfile
lib = /path/to/XrdClientLogging.so
enable = true
```
