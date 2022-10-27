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

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "help_functions.h"

// =========================================
// Helper Function: Loads program to memory
// =========================================
int loadFile2Memory(const char *filename, char **result) {

    int size = 0;

    std::ifstream stream(filename, std::ifstream::binary);
    if (!stream) {
        return -1;
    }

    stream.seekg(0, stream.end);
    size = stream.tellg();
    stream.seekg(0, stream.beg);

    *result = new char[size + 1];
    stream.read(*result, size);
    if (!stream) {
        return -2;
    }
    stream.close();
    (*result)[size] = 0;
    return size;
}

// ==============================================
// Helper Function: Reads an array of int values
// ==============================================
int *read_vector_file(const char *File_Name, int *Nb_Of_Elements) {
  int val;
  int *Array;
  int temp_Nb_Of_Elements;
  fstream DataIn_File;
  void *ptr=nullptr;


  DataIn_File.open(File_Name,ios::in);
  if (! DataIn_File.is_open()) {
      cout << endl << "HOST-Error: Failed to open the " <<  File_Name << " for read" << endl << endl;
      exit(1);
  }

  temp_Nb_Of_Elements = 0;
  while (DataIn_File >> val) temp_Nb_Of_Elements ++;
  DataIn_File.close();

  if (posix_memalign(&ptr,4096,temp_Nb_Of_Elements*sizeof(int))) {
     cout << endl << "HOST-Error: Out of Memory during memory allocation for Array" << endl << endl;
      exit(1);
  }
  Array = reinterpret_cast<int*>(ptr);

  DataIn_File.open(File_Name,ios::in);
  if (! DataIn_File.is_open()) {
      cout << endl << "HOST-Error: Failed to open the " <<  File_Name << " fore read" << endl << endl;
      exit(1);
  }

  temp_Nb_Of_Elements = 0;
  while (DataIn_File >> Array[temp_Nb_Of_Elements])
      temp_Nb_Of_Elements ++;
  DataIn_File.close();

  *Nb_Of_Elements = temp_Nb_Of_Elements;

  return Array;
}

// ==================================================
// Helper Function: Generate an array of init values
//   Period - defines the number of unique values
//            for example, if Period=4 then we will
//            have the following values:
//            0 1 2 3 0 1 2 3
// ==================================================
void gen_int_values(int *Array, int Nb_Of_Elements, int Period) {
   int val;

   val = 0;
   for (int i=0; i<Nb_Of_Elements; i++) {
      Array[i] = val;
      val ++;
      if (val == Period) val = 0;
   }
}

// ==================================================
// Helper Function: Print table of integer values
// ==================================================
void print_int_table(int *Array, int Nb_Of_Elements, int Nb_Of_Columns, int Nb_Of_Char_Per_Column) {
   int col;

   col = 0;
   for (int i=0; i<Nb_Of_Elements; i++) {
      cout << setw(Nb_Of_Char_Per_Column) << Array[i];
      col ++;
      if (col == Nb_Of_Columns) {
         col = 0;
         cout << endl;
      }
   }

}

