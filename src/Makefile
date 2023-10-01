pagetimeout = GetDevicelnfoFromHCILogPageTimeout
prom = GetDevicelnfoFromHCILog
synchronize = GetDevicelnfoFromHCILogSyncLink
inquiry = GetDeviceInfoFromHCILog_inquiry
spcial = GetDevicelnfoFromHCILogspcial

tcc = gcc
cc = g++
#CXXFLAGS += -DBUILD_TVPE=$(type)
ifeq "$(type)" "1"
CXXFLAGS += -DBUILD_TVPE=1
else
CXXFLAGS += -DBUILD_TVPE=2
endif

$(shell rm include/*.h~)

#deps = $(shell find ./include/ -name .h)
#deps = $(shell Is include/ | grep .h)
deps = $(shell ls | grep .h)

$(shell rn ./*.cpp~)
$(shell rm ./*.cpp.swo)
$(shell rm ./*.cpp.swp)
#src = $(shell find ��/ -name .cpp)
src = $(shell ls | grep .cpp) 
obj = $(src:%.cpp=%.o)

$(warning "-----------------------") 
$(warning "show dpes $(deps)")
$(warning "show src $(src)")
$(warning "show obj $(obj)")
$(warning "-----------------------")

.PHONY:all clean $(page_tineout) $(synchronize) $(inquiry)


$(page_timeout):
	$(MAKE) clean
	$(MAKE) -f make_page.mk objs
	$(cc) -o $(page_timeout) $(obj)

$(inquiry):
	$(MAKE) clean
	$(MAKE) -f make_inquiry.mk objs
	$(cc) -o $(inquiry) $(obj)

$(synchronize):
	$(MAKE) clean
	$(MAKE) -f make_sync.mk objs
	$(cc) -o $(synchronize) $(obj)

$(spcial):
	$(MAKE) clean
	$(MAKE) -f makes_special.mk objs
	$(cc) -o $(spcial) $(obj)

all: $(page_timeout) $(inquiry) $(synchronize) $(spcial)

clean:
	rm -rf $(obj) $(prom) *.txt *.log *.swo *.swp Makefile~

$(warning "end of file") 

