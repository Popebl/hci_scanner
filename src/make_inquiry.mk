cc = g++
deps = $(shell ls | grep .h)
src = $(shell ls | grep .cpp)
obj = $(src:%.cpp=%.o)

CXXFLAGS += -DBUILD_TYPE=0

objs: $(obj)
	echo "Pagetimeout build complete"