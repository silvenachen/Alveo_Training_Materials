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
BOARD_VERSION := 5_202210_1
PLATFORM := xilinx_u50_gen3x16_xdma_$(BOARD_VERSION)
LAB_NAME := accel_optimization

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
PROJECT_NAME := opt_4_kernel_parallel
ROOT_REPO := /home/xilinx/training
SRC_REPO := $(ROOT_REPO)/$(LAB_NAME)/lab/src/$(PROJECT_NAME)
PROJECT_DIR := $(ROOT_REPO)/$(LAB_NAME)/lab
PREBUILT_DIR := $(ROOT_REPO)/$(LAB_NAME)/support/prebuilt_opt_4

# re-using the xclbin from the baseline project
XCLBIN_DIR := $(PROJECT_DIR)/build/opt_1_baselining/$(TARGET)

# =========================================================
# Host Application and Kernel Source files repository
# =========================================================
HOST_SRC_CPP := $(SRC_REPO)/host_4.cpp 
HOST_SRC_CPP += $(SRC_REPO)/help_functions.cpp
HOST_SRC_H   += $(SRC_REPO)/help_functions.h 
HOST_SRC_H   += $(SRC_REPO)/kernel.h

KERNEL_KA_SRC_CPP = $(SRC_REPO)/K_KA.cpp
KERNEL_KB_SRC_CPP = $(SRC_REPO)/K_KB.cpp
KERNEL_KCalc_SRC_CPP = $(SRC_REPO)/K_KCalc.cpp
KERNEL_KpB_SRC_CPP = $(SRC_REPO)/K_KpB_1.cpp
KERNEL_SRC_H   = $(SRC_REPO)/kernel.h
KERNEL_SRC_H_DIR := $(SRC_REPO)

BUILD_DIR := $(PROJECT_DIR)/build/$(PROJECT_NAME)/$(TARGET)

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
build: $(BUILD_DIR)/$(HOST_EXE) $(XCLBIN_DIR)/$(XCLBIN) $(BUILD_DIR)/$(EMCONFIG_FILE)

# Build the design and then run host application
# Only run host application
run: #build
	pwd	
	cp xrt.ini $(BUILD_DIR);
ifeq ($(TARGET), hw)
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE;    ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(XCLBIN_DIR)/$(XCLBIN) ;
else
ifeq ($(PREBUILT), NO)	
	cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(XCLBIN_DIR)/$(XCLBIN) 
else ifeq ($(PREBUILT), YES)
	cp xrt.ini $(PREBUILT_DIR);
	cp $(BUILD_DIR)/$(HOST_EXE) $(PREBUILT_DIR)
	#cp $(BUILD_DIR)/emconfig.json $(PREBUILT_DIR)
	cd $(PREBUILT_DIR) && XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(PLATFORM_VENDOR) $(PLATFORM) $(PREBUILT_DIR)/$(XCLBIN) ;
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
	rm -rf $(BUILD_DIR)/$(XCLBIN_DIR)/$(XCLBIN) $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(EMCONFIG_FILE) $(BUILD_DIR)/$(XO_NAME).xo $(BUILD_DIR)/*.ltx

#<copyright-disclaimer-start>
#  ************************************************************************************************************
#  * ?? Copyright 2022 Xilinx, Inc. All rights reserved.                                                       *
#  * This file contains confidential and proprietary information of Xilinx, Inc. and                          *
#  * is protected under U.S. and international copyright and other intellectual property laws.                *
#  * DISCLAIMER                                                                                               *
#  * This disclaimer is not a license and does not grant any rights to the materials distributed              *
#  * herewith. Except as otherwise provided in a valid license issued to you by Xilinx, and to the            *
#  * maximum extent permitted by applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS"               *
#  * AND WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,                 *
#  * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,     *
#  * OR FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether in contract or tort,  *
#  * including negligence, or under any other theory of liability) for any loss or damage of any kind or      *
#  * nature related to, arising under or in connection with these materials, including for any direct, or     *
#  * any indirect, special, incidental, or consequential loss or damage (including loss of data, profits,     *
#  * goodwill, or any type of loss or damage suffered as a result of any action brought by a third party)     *
#  * even if such damage or loss was reasonably foreseeable or Xilinx had been advised of the possibility     *
#  * of the same.                                                                                             *
#  * CRITICAL APPLICATIONS                                                                                    *
#  * Xilinx products are not designed or intended to be fail-safe, or for use in any application requiring    *
#  * fail-safe performance, such as life-support or safety devices or systems, Class III medical devices,     *
#  * nuclear facilities, applications related to the deployment of airbags, or any other applications that    *
#  * could lead to death, personal injury, or severe property or environmental damage (individually and       *
#  * collectively, "Critical Applications"). Customer assumes the sole risk and liability of any use of       *
#  * Xilinx products in Critical Applications, subject only to applicable laws and regulations governing      *
#  * limitations on product liability.                                                                        *
#  * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT ALL TIMES.                 *
#  *                                                                                                          *
#  ************************************************************************************************************
#<copyright-disclaimer-end>
