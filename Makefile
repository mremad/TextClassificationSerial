##############################################################
# Makefile                                                   #
#                                                            #
# Author      : Chong Ding <cding@mymail.mines.edu>          #
# Version     : 0.1                                          #
# Date        : 20100915                                     #
# Discription : generic Makefile for making cuda programs    #
##############################################################

BIN               := TextClassification 
CXXFLAGS          := 


CUDA_INSTALL_PATH ?= /opt/cuda/6.0
CUDA_SDK_PATH ?= /opt/cuda/6.0


NVCC ?= $(CUDA_INSTALL_PATH)/bin/nvcc
INCD = -I"$(CUDA_SDK_PATH)/C/common/inc" -I"$(CUDA_INSTALL_PATH)/include" -I"./"
LIBS = -L"/opt/development/gpu/3.2/libcuda" -lcuda -L"$(CUDA_INSTALL_PATH)/lib64" -lcudart -L"$(CUDA_SDK_PATH)/C/common/lib"
CUDA_SDK?=3
COMMONFLAGS = -DCUDA_SDK=$(CUDA_SDK)
NVCCFLAGS := -arch sm_13 --ptxas-options=-v -O3 -G -g 


# files
CPP_SOURCES       := ConsolePrint.cpp CudaStd.cpp EM.cpp FileReader.cpp LabelFactory.cpp LinkedList.cpp SuperHash.cpp TestResults.cpp TextClassificationSerial.cpp 
CU_SOURCES        := FeatureConstructor.cu NaiveBayesClassifier.cu
HEADERS           := $(wildcard *.h)
CPP_OBJS          := $(patsubst %.cpp, %.o, $(CPP_SOURCES))
CU_OBJS           := $(patsubst %.cu, %.cu_o, $(CU_SOURCES))

%.cu_o : %.cu
	$(NVCC) $(NVCCFLAGS) -c $(INCD) -o $@ $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCD) -o $@ $<

$(BIN): $(CPP_OBJS) $(CU_OBJS)
	$(CXX) -o $(BIN) $(CU_OBJS) $(CPP_OBJS) $(LDFLAGS) $(INCD) $(LIBS)

clean:
	rm -f $(BIN) *.o *.cu_o


