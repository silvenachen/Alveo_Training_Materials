.PHONY: help

help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make build TARGET=<sw_emu/hw_emu/hw>"
	@echo  "  Command to generate the design for specified target"
	@echo  "  Default TARGET is sw_emu"
	@echo  ""
	@echo  "  make run TARGET=<sw_emu/hw_emu/hw>"
	@echo  "  Command to generate,run and verifiy the design for specified target"
	@echo  ""
	@echo  "  make clean TARGET=<sw_emu/hw_emu/hw>"
	@echo  "  Command to remove the generated files for specified target"
	@echo  ""
	@echo  "  make view_run_summary TARGET=<sw_emu/hw_emu/hw>"
	@echo  "  Command to load run summary report in vitis_analyzer utility"
	@echo  ""
	@echo  "  make view_timeline_trace TARGET=<sw_emu/hw_emu/hw>"
	@echo  "  Command to view application timeline report in vitis_analyzer utility"
	@echo  ""

# =========================================================
# platform selection
# =========================================================
PLATFORM_VENDOR := Xilinx
PLATFORM := xilinx_u50_gen3x16_xdma_201920_3

# =========================================================
# TARGET can be set as:
#   sw_emu: software emulation
#   hw_emu: hardware Emulation
#   hw    : hardware run
# =========================================================
TARGET := sw_emu
PREBUILT := NO

# =========================================================
# Below listed the different steps of the design.
# =========================================================
#OPT_1_BASE 
#OPT_2_CLENQUEUEMEM_API 
#OPT_3_SINGLE_API 
#OPT_4_KERNEL_PARALLEL 
#OPT_5_BURST_TRANSFER 
#OPT_6_ARRAY 

# =========================================================
# Below are the names for host executable and xclbin.
# Please keep it unchanged.
# =========================================================
HOST_EXE := host.exe
XO_KA := KA.$(TARGET)
XO_KB := KB.$(TARGET)
XO_KCalc := KCalc.$(TARGET)
XO_KpB := KpB.$(TARGET)
XCLBIN := kernels.$(TARGET).xclbin

# =========================================================
# Source directory
# =========================================================
ROOT_REPO := /home/xilinx/training

SRC_REPO := $(ROOT_REPO)/optimization/lab/src/opt_1_baselining

PROJECT_DIR := $(ROOT_REPO)/optimization/lab

PREBUILT_DIR := $(ROOT_REPO)/optimization/support/prebuilt_opt_1

# =========================================================
# Host Application and Kernel Source files repository
# =========================================================
HOST_SRC_CPP := $(SRC_REPO)/host_1.cpp 
HOST_SRC_CPP += $(SRC_REPO)/help_functions.cpp
HOST_SRC_H   += $(SRC_REPO)/help_functions.h 
HOST_SRC_H   += $(SRC_REPO)/kernel.h

KERNEL_KA_SRC_CPP = $(SRC_REPO)/K_KA.cpp
KERNEL_KB_SRC_CPP = $(SRC_REPO)/K_KB.cpp
KERNEL_KCalc_SRC_CPP = $(SRC_REPO)/K_KCalc.cpp
KERNEL_KpB_SRC_CPP = $(SRC_REPO)/K_KpB_1.cpp
KERNEL_SRC_H   = $(SRC_REPO)/kernel.h
KERNEL_SRC_H_DIR := $(SRC_REPO)

BUILD_DIR := $(PROJECT_DIR)/build/opt_1_baselining/$(TARGET)

# =========================================================
# Host Compiler Global Settings and Include Libraries
# =========================================================
#CXXFLAGS :=
#CXXFLAGS += -D__USE_XOPEN2K8
#CXXFLAGS += -D__USE_XOPEN2K8
CXXFLAGS += -I$(XILINX_XRT)/include/
CXXFLAGS += -I$(XILINX_VIVADO)/include/
CXXFLAGS += -I$(SRC_REPO)
CXXFLAGS += -O0 -g -Wall -fmessage-length=0 -std=c++1y

CXXLDFLAGS := -L$(XILINX_XRT)/lib/
CXXLDFLAGS += -lxilinxopencl -lpthread -lrt

# =========================================================
# Kernel Compiler and Linker Flags
# =========================================================
VPPFLAGS := -t $(TARGET)
VPPFLAGS += --config design.cfg
VPPFLAGS += -I$(KERNEL_SRC_H_DIR)
VPPFLAGS += --temp_dir $(BUILD_DIR)
VPPFLAGS += --log_dir $(BUILD_DIR)
LDCLFLAGS = --profile_kernel data:all:all:all

# =========================================================
# Host Executable File Generation
# =========================================================
$(BUILD_DIR)/$(HOST_EXE): $(HOST_SRC_CPP) $(HOST_SRC_H)
	mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(HOST_SRC_CPP) $(CXXLDFLAGS) -o $@

# =========================================================
# Kernel XO and Xclbin File Generation
# =========================================================
# Compiling KA
$(BUILD_DIR)/$(XO_KA).xo: $(KERNEL_KA_SRC_CPP) $(KERNEL_SRC_H)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c -k KA $(KERNEL_KA_SRC_CPP) -o $@

# Compiling KB
$(BUILD_DIR)/$(XO_KB).xo: $(KERNEL_KB_SRC_CPP) $(KERNEL_SRC_H)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c -k KB $(KERNEL_KB_SRC_CPP) -o $@

# Compiling KCalc
$(BUILD_DIR)/$(XO_KCalc).xo: $(KERNEL_KCalc_SRC_CPP) $(KERNEL_SRC_H)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c -k KCalc $(KERNEL_KCalc_SRC_CPP) -o $@

# Compiling KpB
$(BUILD_DIR)/$(XO_KpB).xo: $(KERNEL_KpB_SRC_CPP) $(KERNEL_SRC_H)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c -k KpB $(KERNEL_KpB_SRC_CPP) -o $@

$(BUILD_DIR)/$(XCLBIN): $(BUILD_DIR)/$(XO_KA).xo $(BUILD_DIR)/$(XO_KB).xo $(BUILD_DIR)/$(XO_KCalc).xo $(BUILD_DIR)/$(XO_KpB).xo $(PROJECT_DIR)/src/KVConstAdd.xo
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -l $(LDCLFLAGS) -o $@ $(BUILD_DIR)/$(XO_KA).xo $(BUILD_DIR)/$(XO_KB).xo $(BUILD_DIR)/$(XO_KCalc).xo $(BUILD_DIR)/$(XO_KpB).xo $(PROJECT_DIR)/src/KVConstAdd.xo

# =========================================================
# Emulation Files Generation
# =========================================================
EMCONFIG_FILE = emconfig.json

$(BUILD_DIR)/$(EMCONFIG_FILE):
	 emconfigutil --nd 1  --platform $(PLATFORM) --od $(BUILD_DIR)

# =========================================================
# Primary build targets 
#  ==> build
#  ==> run
#  ==> view_run_summary
#  ==> clean
# =========================================================
.PHONY: all clean

# Build the design without running host application
build: $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(EMCONFIG_FILE)

# Build the design and then run host application
# Only run host application
run: #build
	pwd	
	cp xrt.ini $(BUILD_DIR);

ifeq ($(TARGET), hw)
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE;    ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(XCLBIN) ;
else
ifeq ($(PREBUILT), NO)	
	cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(XCLBIN) ;
else ifeq ($(PREBUILT), YES)
	cd $(PREBUILT_DIR) && XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(XCLBIN) ;
else
endif
endif

# View profile summary report in Vitis Analyzer GUI
view_run_summary:
ifeq ($(TARGET), sw_emu)
	cd $(BUILD_DIR) && vitis_analyzer xrt.run_summary
else 
ifeq ($(PREBUILT), NO)
	cd $(BUILD_DIR) && vitis_analyzer xrt.run_summary
else ifeq ($(PREBUILT), YES)
	cd $(PREBUILT_DIR) && vitis_analyzer xrt.run_summary
endif	
endif

# Clean generated files
clean:
	rm -rf $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(EMCONFIG_FILE) $(BUILD_DIR)/$(XO_NAME).xo $(BUILD_DIR)/*.ltx
