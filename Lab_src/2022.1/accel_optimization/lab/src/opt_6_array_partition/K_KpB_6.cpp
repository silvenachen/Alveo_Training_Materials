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


#include <string.h>

#include "kernel.h"

void data_read (int* A, int* B, int* A_tmp, int* B_tmp){
   memcpy(A_tmp,A,SIZE_BUF_KpB * sizeof (int));
   memcpy(B_tmp,B,SIZE_BUF_KpB * sizeof (int));
}


extern "C"
{
   void KpB(int *A, int *B, int *R)
   {
      int TMP_RES[SIZE_BUF_KpB];

      #pragma HLS INTERFACE s_axilite port=A bundle=control
      #pragma HLS INTERFACE s_axilite port=B bundle=control
      #pragma HLS INTERFACE s_axilite port=R bundle=control
      #pragma HLS INTERFACE s_axilite port=return bundle=control

//    #pragma HLS dataflow

//    #pragma HLS INTERFACE m_axi port=A  offset=slave bundle=gmem max_read_burst_length=256 max_write_burst_length=256
//    #pragma HLS INTERFACE m_axi port=B  offset=slave bundle=gmem max_read_burst_length=256 max_write_burst_length=256
//    #pragma HLS INTERFACE m_axi port=R  offset=slave bundle=gmem max_read_burst_length=256 max_write_burst_length=256

      int A_tmp[SIZE_BUF_KpB], B_tmp[SIZE_BUF_KpB], R_tmp[SIZE_BUF_KpB];

      data_read(A,B,A_tmp,B_tmp);

      for(int i=0; i < SIZE_BUF_KpB; i+=1) {
         TMP_RES[i] = A_tmp[i] + B_tmp[i];
      }

      for(int i=0; i < SIZE_BUF_KpB; i+=1) {
         R_tmp[i] = TMP_RES[i] % 3;
      }

      memcpy(R,R_tmp,SIZE_BUF_KpB * sizeof (int));

   }
}
