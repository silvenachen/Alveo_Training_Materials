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

#include "kernel.h"

// #define USE_BURST_TRANSFER_ARRAY_PARTITION

// -------------------------------------------------
// Kernel: KB
// -------------------------------------------------
extern "C"
{
   void KB(int *A, int *R)
   {
      #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
      #pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
      #pragma HLS INTERFACE s_axilite port=A bundle=control
      #pragma HLS INTERFACE s_axilite port=R bundle=control
      #pragma HLS INTERFACE s_axilite port=return bundle=control

      int val;
      int TMP_BUF[SIZE_BUF_KpB];

      #ifdef USE_BURST_TRANSFER_ARRAY_PARTITION
         #pragma HLS array_partition variable=TMP_BUF block factor=3 dim=1
      #endif

      loop_burst_data:
      for (int i=0; i<SIZE_BUF_KpB; i++) {
         TMP_BUF[i] = A[i]+10;
      }

      loop_st_1:
      for (int i=0; i<SIZE_BUF_KB; i++) {
         val  =  TMP_BUF[i]*3 + TMP_BUF[i+SIZE_RES]*5 + TMP_BUF[i+2*SIZE_RES]*7;
         R[i] = val;
      }
   }
}
