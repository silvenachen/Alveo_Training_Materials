/******************************************************************************
 *
 * history:
 *    2022.1 - wk - whitespace cleanup
 *    2021.1 - sb - initial
 *
 *
 * <copyright-disclaimer-start>>
 *   ************************************************************************************************************
 *   * © Copyright 2021-2022 Xilinx, Inc. All rights reserved.                                                  *
 *   * This file contains confidential and proprietary information of Xilinx, Inc. and                          *
 *   * is protected under U.S. and international copyright and other intellectual property laws.                *
 *   * DISCLAIMER                                                                                               *
 *   * This disclaimer is not a license and does not grant any rights to the materials distributed              *
 *   * herewith. Except as otherwise provided in a valid license issued to you by Xilinx, and to the            *
 *   * maximum extent permitted by applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS"               *
 *   * AND WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,                 *
 *   * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,     *
 *   * OR FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether in contract or tort,  *
 *   * including negligence, or under any other theory of liability) for any loss or damage of any kind or      *
 *   * nature related to, arising under or in connection with these materials, including for any direct, or     *
 *   * any indirect, special, incidental, or consequential loss or damage (including loss of data, profits,     *
 *   * goodwill, or any type of loss or damage suffered as a result of any action brought by a third party)     *
 *   * even if such damage or loss was reasonably foreseeable or Xilinx had been advised of the possibility     *
 *   * of the same.                                                                                             *
 *   * CRITICAL APPLICATIONS                                                                                    *
 *   * Xilinx products are not designed or intended to be fail-safe, or for use in any application requiring    *
 *   * fail-safe performance, such as life-support or safety devices or systems, Class III medical devices,     *
 *   * nuclear facilities, applications related to the deployment of airbags, or any other applications that    *
 *   * could lead to death, personal injury, or severe property or environmental damage (individually and       *
 *   * collectively, "Critical Applications"). Customer assumes the sole risk and liability of any use of       *
 *   * Xilinx products in Critical Applications, subject only to applicable laws and regulations governing      *
 *   * limitations on product liability.                                                                        *
 *   * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT ALL TIMES.                 *
 *   *                                                                                                          *
 *   ************************************************************************************************************
 *
 * <copyright-disclaimer-end>>
 *
 ******************************************************************************/


/*******************************************************************************
** HOST Code
*******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include <CL/cl.h>

#include "help_functions.h"
#include "kernel.h"

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
//#include <CL/cl.hpp>

//#define ALL_MESSAGES

// ********************************************************************************** //
// ---------------------------------------------------------------------------------- //
//                          M A I N    F U N C T I O N                                //
// ---------------------------------------------------------------------------------- //
// ********************************************************************************** //

int main(int argc, char* argv[])
{
   cout << endl;


   // ============================================================================
   // Step 1: Check Command Line Arguments
   // ============================================================================
   //    o) argv[1] Platfrom Vendor
   //    o) argv[2] Device Name
   //    o) argv[3] XCLBIN file
   //    o) argv[4] DataIn_1 file
   //    o) argv[5] DataIN_2 file
   // ============================================================================
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 1) Check Command Line Arguments                      " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   if (argc != 6)
   {
      cout << "HOST-Error: Incorrect command line syntax " << endl;
      cout << "HOST-Info:  Usage: " << argv[0] << " <Platform_Vendor> <Device_Name> <XCLBIN_File> <DataIn_1_File> <DataIn_2_File>" << endl << endl;
      return EXIT_FAILURE;
   }

   const char* Target_Platform_Vendor   = argv[1];
   const char* Target_Device_Name       = argv[2];
   const char* xclbinFilename           = argv[3];
   const char* DataIn_1_FileName        = argv[4];
   const char* DataIn_2_FileName        = argv[5];
   cout << "HOST-Info: Platform_Vendor : " << Target_Platform_Vendor << endl;
   cout << "HOST-Info: Device_Name     : " << Target_Device_Name << endl;
   cout << "HOST-Info: XCLBIN_file     : " << xclbinFilename << endl;
   cout << "HOST-Info: DataIn_1_File   : " << DataIn_1_FileName << endl;
   cout << "HOST-Info: DataIn_2_File   : " << DataIn_2_FileName << endl;

   // ============================================================================
   // Step 2: Detect Target Platform and Target Device in a system.
   //         Create Context and Command Queue.
   // ============================================================================
   // Variables:
   //   o) Target_Platform_Vendor[] - defined as main() input argument
   //   o) Target_Device_Name[]     - defined as main() input argument
   //
   // After that
   //   o) Create a Context
   //   o) Create a Command Queue
   // ============================================================================
   cout << endl;
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 2) Detect Target Platform and Target Device in a system " << endl;
   cout << "HOST-Info:          Create Context and Command Queue                     " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   cl_uint         ui;

   cl_platform_id      *Platform_IDs;
   cl_uint             Nb_Of_Platforms;
   cl_platform_id      Target_Platform_ID;
   bool                Platform_Detected;
   char                *platform_info;

   cl_device_id        *Device_IDs;
   cl_uint             Nb_Of_Devices;
   cl_device_id        Target_Device_ID;
   bool                Device_Detected;
   char                *device_info;

   cl_context          Context;
   cl_command_queue    Command_Queue;

   cl_int              errCode;
   size_t              size;

   // ------------------------------------------------------------------------------------
   // Step 2.1: Get All PLATFORMS, then search for Target_Platform_Vendor (CL_PLATFORM_VENDOR)
   // ------------------------------------------------------------------------------------

   // Get the number of platforms
   // ..................................................
   // errCode = clGetPlatformIDs(0, NULL, &Nb_Of_Platforms);
   errCode = clGetPlatformIDs(0, NULL, &Nb_Of_Platforms);
   if (errCode != CL_SUCCESS || Nb_Of_Platforms <= 0) {
      cout << endl << "HOST-Error: Failed to get the number of available platforms" << endl << endl;
      return EXIT_FAILURE;
   }

   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Number of detected platforms : " << Nb_Of_Platforms << endl;
   #endif

   // Allocate memory to store platforms
   // ..................................................
   Platform_IDs = new cl_platform_id[Nb_Of_Platforms];
   if (!Platform_IDs) {
      cout << endl << "HOST-Error: Out of Memory during memory allocation for Platform_IDs" << endl << endl;
      return EXIT_FAILURE;
   }

   // Get and store all PLATFORMS
   // ..................................................
   errCode = clGetPlatformIDs(Nb_Of_Platforms, Platform_IDs, NULL);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to get the available platforms" << endl << endl;
      return EXIT_FAILURE;
   }

   // Search for Platform (ex: Xilinx) using: CL_PLATFORM_VENDOR = Target_Platform_Vendor
   // ....................................................................................
   Platform_Detected = false;
   for (ui = 0; ui < Nb_Of_Platforms; ui++) {

      errCode = clGetPlatformInfo(Platform_IDs[ui], CL_PLATFORM_VENDOR, 0, NULL, &size);
      if (errCode != CL_SUCCESS) {
         cout << endl << "HOST-Error: Failed to get the size of the Platofrm parameter " << "CL_PLATFORM_VENDOR" << " value " << endl << endl;
         return EXIT_FAILURE;
      }

      platform_info = new char[size];
      if (!platform_info) {
         cout << endl << "HOST-Error: Out of Memory during memory allocation for Platform Parameter " << "CL_PLATFORM_VENDOR" << endl << endl;
         return EXIT_FAILURE;
      }

      errCode = clGetPlatformInfo(Platform_IDs[ui], CL_PLATFORM_VENDOR, size, platform_info , NULL);
      if (errCode != CL_SUCCESS) {
         cout << endl << "HOST-Error: Failed to get the " << "CL_PLATFORM_VENDOR" << " platform info" << endl << endl;
         return EXIT_FAILURE;
      }

      // Check if the current platform matches Target_Platform_Vendor
      // .............................................................
      if (strcmp(platform_info, Target_Platform_Vendor) == 0) {
         Platform_Detected        = true;
         Target_Platform_ID       = Platform_IDs[ui];
         #ifdef ALL_MESSAGES
         cout << "HOST-Info: Selected platform            : " << Target_Platform_Vendor << endl << endl;
         #endif
      }
   }

   if (Platform_Detected == false) {
      cout << endl << "HOST-Error: Failed to get detect " << Target_Platform_Vendor << " platform" << endl << endl;
      return EXIT_FAILURE;
   }

   // ------------------------------------------------------------------------------------
   // Step 2.2:  Get All Devices for selected platform Target_Platform_ID
   //            then search for Xilinx platform (CL_DEVICE_NAME = Target_Device_Name)
   // ------------------------------------------------------------------------------------

   // Get the Number of Devices
   // ............................................................................
   errCode = clGetDeviceIDs(Target_Platform_ID, CL_DEVICE_TYPE_ALL, 0, NULL, &Nb_Of_Devices);

   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to get the number of available Devices" << endl << endl;
      return EXIT_FAILURE;
   }
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Number of available devices  : " << Nb_Of_Devices << endl;
   #endif

   Device_IDs = new cl_device_id[Nb_Of_Devices];
   if (!Device_IDs) {
      cout << endl << "HOST-Error: Out of Memory during memory allocation for Device_IDs" << endl << endl;
      return EXIT_FAILURE;
   }

   errCode = clGetDeviceIDs(Target_Platform_ID, CL_DEVICE_TYPE_ALL, Nb_Of_Devices, Device_IDs, NULL);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to get available Devices" << endl << endl;
      return EXIT_FAILURE;
   }

   // Search for CL_DEVICE_NAME = Target_Device_Name
   // ............................................................................
   Device_Detected = false;
   for (ui = 0; ui < Nb_Of_Devices; ui++) {
      errCode = clGetDeviceInfo(Device_IDs[ui], CL_DEVICE_NAME, 0, NULL, &size);
      if (errCode != CL_SUCCESS) {
         cout << endl << "HOST-Error: Failed to get the size of the Device parameter value " << "CL_DEVICE_NAME" << endl << endl;
         return EXIT_FAILURE;
      }

      device_info = new char[size];
      if (!device_info) {
         cout << endl << "HOST-Error: Out of Memory during memory allocation for Device parameter " << "CL_DEVICE_NAME" << " value " << endl << endl;
         return EXIT_FAILURE;
      }

      errCode = clGetDeviceInfo(Device_IDs[ui], CL_DEVICE_NAME, size, device_info, NULL);
      if (errCode != CL_SUCCESS) {
         cout << endl << "HOST-Error: Failed to get the " << "CL_DEVICE_NAME" << " device info" << endl << endl;
         return EXIT_FAILURE;
      }

      // Check if the current device matches Target_Device_Name
      // ............................................................................
      if (strcmp(device_info, Target_Device_Name) == 0) {
         Device_Detected        = true;
         Target_Device_ID       = Device_IDs[ui];
      }
   }

   if (Device_Detected == false) {
      cout << endl << "HOST-Error: Failed to get detect " << Target_Device_Name << " device" << endl << endl;
      return EXIT_FAILURE;
   } else {
      #ifdef ALL_MESSAGES
      cout << "HOST-Info: Selected device              : " << Target_Device_Name << endl << endl;
      #endif
   }

   // ------------------------------------------------------------------------------------
   // Step 2.3: Create Context
   // ------------------------------------------------------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Creating Context ... " << endl;
   #endif
   Context = clCreateContext(0, 1, &Target_Device_ID, NULL, NULL, &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to create a Context" << endl << endl;
      return EXIT_FAILURE;
   }

   // ------------------------------------------------------------------------------------
   // Step 2.4: Create Command Queue (commands are executed in-order)
   // ------------------------------------------------------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Creating Command Queue ... " << endl;
   #endif
   Command_Queue = clCreateCommandQueue(Context, Target_Device_ID, 0, &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to create a Command Queue" << endl << endl;
      return EXIT_FAILURE;
   }

   // ============================================================================
   // Step 3: Create Program and Kernel
   // ============================================================================
   //   o) Create a Program from a Binary File and Build it
   //   o) Create a Kernel
   // ============================================================================
   #ifdef ALL_MESSAGES
   cout << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 3) Create Program and Kernel                            " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   // ------------------------------------------------------------------
   // Step 3.1: Load Binary File from a disk to Memory
   // ------------------------------------------------------------------
   unsigned char *xclbin_Memory;
   int program_length;

   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Loading " << xclbinFilename << " binary file to memory ..." << endl;
   #endif

   program_length = loadFile2Memory(xclbinFilename, (char **) &xclbin_Memory);

   if (program_length < 0) {
      cout << endl << "HOST-Error: Failed to load " << xclbinFilename << " binary file to memory" << endl << endl;
      return EXIT_FAILURE;
   }

   // ------------------------------------------------------------
   // Step 3.2: Create a program using a Binary File
   // ------------------------------------------------------------
   size_t     Program_Length_in_Bytes;
   cl_program Program;
   cl_int     Binary_Status;

   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Creating Program with Binary ..." << endl;
   #endif
   Program_Length_in_Bytes = program_length;
   Program = clCreateProgramWithBinary(Context, 1, &Target_Device_ID, &Program_Length_in_Bytes,
                                        (const unsigned char **) &xclbin_Memory, &Binary_Status, &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to create a Program from a Binary" << endl << endl;
      return EXIT_FAILURE;
   }

   // -------------------------------------------------------------
   // Step 3.3: Build (compiles and links) a program executable from binary
   // -------------------------------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Building the Program ..." << endl;
   #endif

   errCode = clBuildProgram(Program, 1, &Target_Device_ID, NULL, NULL, NULL);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to build a Program Executable" << endl << endl;
      return EXIT_FAILURE;
   }

   // -------------------------------------------------------------
   // Step 3.4: Create a Kernel we wish to run
   // -------------------------------------------------------------
   cl_kernel Kernel;
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Creating a Kernel: K_VADD ..." << endl;
   #endif

   Kernel = clCreateKernel(Program, "K_VADD", &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to create a Kernel" << endl << endl;
      return EXIT_FAILURE;
   }


   // ================================================================
   // Step 4: Prepare Data to Run Kernel
   // ================================================================
   //   o) Read data from DataIn_1_FileName -> DataIn_1 array
   //   o) Read data from DataIn_2_FileName -> DataIn_2 array
   //   o) Allocate Memory to store the results: RES array
   //   o) Create Buffers in Global Memory to store data
   //   o) Copy Input Data from Host to Global Memory
   // ================================================================

   int *DataIn_1, *DataIn_2, *RES;
   int Nb_Of_Elements;
   int tmp_nb;

   #ifdef ALL_MESSAGES
   cout << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 4) Prepare Data to Run Kernel                           " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   // ------------------------------------------------------------------
   // Step 4.1: Read data from DataIn_1_FileName -> DataIn_1 array
   //           Read data from DataIn_2_FileName -> DataIn_2 array
   //           Allocate Memory to store the results: RES array
   // ------------------------------------------------------------------

   cout << "HOST-Info: Reading Input data from the " << DataIn_1_FileName << " file ... ";
   DataIn_1 = read_vector_file(DataIn_1_FileName, &tmp_nb);
   if (tmp_nb != MAX_Nb_Of_Elements) {
      cout << endl << "HOST-Error: Number of elements in the " << DataIn_1_FileName << " should be " << MAX_Nb_Of_Elements << " instead of " << tmp_nb << endl << endl;
      return EXIT_FAILURE;
   }
   cout << "Read " << tmp_nb << " values" << endl;

   cout << "HOST-Info: Reading Input data from the " << DataIn_2_FileName << " file ... " ;
   DataIn_2 = read_vector_file(DataIn_2_FileName, &tmp_nb);
   if (tmp_nb != MAX_Nb_Of_Elements) {
      cout << endl << "HOST-Error: Number of elements in the " << DataIn_2_FileName << " should be " << MAX_Nb_Of_Elements << " instead of " << tmp_nb << endl << endl;
      return EXIT_FAILURE;
   }
   cout << "Read " << tmp_nb << " values" << endl;

   void *ptr=nullptr;
   if (posix_memalign(&ptr,4096,MAX_Nb_Of_Elements*sizeof(int))) {
      cout << endl << "HOST-Error: Out of Memory during memory allocation for RES array" << endl << endl;
      return EXIT_FAILURE;
   }
   RES = reinterpret_cast<int*>(ptr);


   Nb_Of_Elements = MAX_Nb_Of_Elements;

   // ------------------------------------------------------------------
   // Step 4.2: Create Buffers in Global Memory to store data
   //             o) GlobMem_BUF_DataIn_1 - stores DataIn_1
   //             o) GlobMem_BUF_DataIn_2 - stores DataIn_2
   //             o) GlobMem_BUF_RES      - stores RES
   // ------------------------------------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Allocating buffers in Global Memory to store Input and Output Data ..." << endl;
   #endif
   cl_mem   GlobMem_BUF_DataIn_1,
         GlobMem_BUF_DataIn_2,
         GlobMem_BUF_RES;

   // Allocate Global Memory for GlobMem_BUF_DataIn_1
   // .......................................................
   GlobMem_BUF_DataIn_1 = clCreateBuffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, Nb_Of_Elements * sizeof(int), DataIn_1, &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_DataIn_1" << endl << endl;
      return EXIT_FAILURE;
   }

   // Allocate Global Memory for GlobMem_BUF_DataIn_2
   // .......................................................
   GlobMem_BUF_DataIn_2 = clCreateBuffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, Nb_Of_Elements * sizeof(int), DataIn_2, &errCode);
   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_DataIn_2" << endl << endl;
      return EXIT_FAILURE;
   }

   // Allocate Global Memory for GlobMem_BUF_RES
   // .......................................................
   GlobMem_BUF_RES = clCreateBuffer(Context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, Nb_Of_Elements * sizeof(int), RES, &errCode);


   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_RES" << endl << endl;
      return EXIT_FAILURE;
   }

   // ------------------------------------------------------
   // Step 4.3: Copy Input data from Host to Global Memory
   // ------------------------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST_Info: Copy Input data to Global Memory ..." << endl;
   #endif
   errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_DataIn_1, 0, 0, NULL, NULL);

   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to write DataIn_1 to GlobMem_BUF_DataIn_1" << endl << endl;
      return EXIT_FAILURE;
   }

   errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_DataIn_2, 0, 0, NULL, NULL);
   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to write DataIn_2 to GlobMem_BUF_DataIn_2" << endl << endl;
      return EXIT_FAILURE;
   }

   // ============================================================================
   // Step 5: Set Kernel Arguments and Execute Kernel
   // ============================================================================
   // ----------------------------------------------------
   //  Argument Nb    Description
   // ----------------------------------------------------
   //     0           GlobMem_BUF_DataIn_1
   //     1           GlobMem_BUF_DataIn_2
   //     2           GlobMem_BUF_RES
   // ============================================================================
   #ifdef ALL_MESSAGES
   cout << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 5) Set Kernel Arguments and Execute Kernel              " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   // ----------------------------------------
   // Step 5.1: Set Kernel Arguments
   // ----------------------------------------
   #ifdef ALL_MESSAGES
   cout << "HOST-Info: Setting Kernel arguments ..." << endl;
   #endif
   errCode  = clSetKernelArg(Kernel, 0, sizeof(cl_mem), &GlobMem_BUF_DataIn_1);
   errCode |= clSetKernelArg(Kernel, 1, sizeof(cl_mem), &GlobMem_BUF_DataIn_2);
   errCode |= clSetKernelArg(Kernel, 2, sizeof(cl_mem), &GlobMem_BUF_RES);

   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-ERROR: Failed to set Kernel arguments" << endl << endl;
      return EXIT_FAILURE;
   }

   // ----------------------------------------
   // Step 5.2: Execute Kernel
   // ----------------------------------------
   size_t globalSize[1];
   size_t localSize[1];
   globalSize[0] = 1;
   localSize[0]  = 1;

   cout << "HOST-Info: Executing Kernel ..." << endl;

   errCode = clEnqueueTask(Command_Queue, Kernel, 0, NULL, NULL);

   if (errCode != CL_SUCCESS) {
      cout << endl << "HOST-Error: Failed to execute Kernel" << endl << endl;
      return EXIT_FAILURE;
   }

   // ============================================================================
   // Step 6: Read and Store the Output Results
   // ============================================================================
   // The Output Results are stored in the RES.txt file
   //
   // ============================================================================
   #ifdef ALL_MESSAGES
   cout << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   cout << "HOST-Info: (Step 6) Read, Store and Check the Output Results             " << endl;
   cout << "HOST-Info: ============================================================= " << endl;
   #endif

   // ------------------------------------------------------
   // Step 6.1: Read output Result to Host memory (RES)
   // ------------------------------------------------------
   cout << "HOST-Info: The Output Result file: RES.txt" << endl;
   errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_RES, CL_MIGRATE_MEM_OBJECT_HOST, 0, NULL, NULL);

   if (errCode != CL_SUCCESS) {
      cout << endl << "Host-Error: Failed to write RES from GlobMem_BUF_RES" << endl << endl;
      return EXIT_FAILURE;
   }
   ::clFinish(Command_Queue);

   // ------------------------------------------------------
   // Step 6.2: Write RES to the RES.txt file
   // ------------------------------------------------------
   fstream RES_File;


   RES_File.open("RES.txt",ios::out);
   if (! RES_File.is_open()) {
      cout << endl << "HOST-Error: Failed to open the RES.txt file for write" << endl << endl;
      return EXIT_FAILURE;
   }

   for (int i=0; i < Nb_Of_Elements; i++) {
      RES_File << RES[i] << endl;
   }
   RES_File.close();

   // ------------------------------------------------------
   // Step 6.3: Check correctness of the output results
   // ------------------------------------------------------
   int Gold_Res;
   bool error_detected = false;
   cout << endl << "Host-Info: ========================================================" << endl;
   cout <<         "Host-Info: Verifying final results (only failed tests are printed)" << endl;
   for (int i=0; i< Nb_Of_Elements; i++) {
      Gold_Res = DataIn_1[i] + DataIn_2[i];

      if (RES[i] != Gold_Res) {
         if (error_detected == false) {
            cout <<         "Host-Info: --------------------------------------------------------" << endl;
            cout <<         "Host-Info: " << setw(10) << "DataIn_1" << setw(10) << "DataIn_2" << setw(10) << "Expected" << setw(10) << "Actual" << setw(10) << "Status" << endl;
            cout <<         "Host-Info: --------------------------------------------------------" << endl;
         }
         cout << "Host-Info: " << setw(10) << DataIn_1[i] << setw(10) << DataIn_2[i] << setw(10) << Gold_Res << setw(10) << RES[i];
         cout << setw(10) << "Error" << endl;
         error_detected = true;
      }
   }
   cout << "Host-Info: ========================================================" << endl;


   if (error_detected == false) {
      cout << "Host-Info: Test Successful" << endl;
   } else {
      cout << "Host-Error: Test Failed" << endl;
   }


   // ============================================================================
   // Step 7: Release Allocated Resources
   // ============================================================================
   clReleaseDevice(Target_Device_ID);

   clReleaseMemObject(GlobMem_BUF_DataIn_1);
   clReleaseMemObject(GlobMem_BUF_DataIn_2);
   clReleaseMemObject(GlobMem_BUF_RES);

   clReleaseKernel(Kernel);
   clReleaseProgram(Program);
   clReleaseCommandQueue(Command_Queue);
   clReleaseContext(Context);
   free(Platform_IDs);
   free(Device_IDs);
   free(DataIn_1);
   free(DataIn_2);
   free(RES);


   cout << endl << "HOST-Info: DONE" << endl << endl;

   return EXIT_SUCCESS;
}
