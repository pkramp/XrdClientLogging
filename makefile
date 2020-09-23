
ifdef DEBUG
DBG="debug"
endif

ifndef XRD_PATH
$(error XRD_PATH is not set, set XRD_PATH to your XRootD installation)
endif
all:XrdClientLogging.so
 
XrdClientLogging.so: 
	g++ -g3 -fPIC  -I$(XRD_PATH)/include/xrootd -I $(CPPUNIT_PATH)/include/cppunit -I./src/ -c *.cc -std=c++14
	g++ -shared  -L$(XRD_PATH)/lib -Wl,-soname,XrdClientLogging.so.1,--export-dynamic -o XrdClientLogging.so *.o -lXrdUtils -lXrdCl -lXrdPosix 
	
#test: XrdClientLogging.so
#	@./test/xrdcp_Test.sh $(DBG)

clean:clean_o clean_lib clean_exe 

clean_o:
	@-rm  *.o 
clean_lib:
	@-rm *.so
clean_exe:
	@-rm -rf *.exe

.PHONY: test
