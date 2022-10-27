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

// -------------------------------------------------
// Kernel: KCalc
// -------------------------------------------------
extern "C"
{
   void KCalc(int *A, int *B, int *R)
   {
      #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
      #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
      #pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
      #pragma HLS INTERFACE s_axilite port=A bundle=control
      #pragma HLS INTERFACE s_axilite port=B bundle=control
      #pragma HLS INTERFACE s_axilite port=R bundle=control
      #pragma HLS INTERFACE s_axilite port=return bundle=control

      int val1, val2;
      int TMP_R[SIZE_RES];

      int TMP_A[SIZE_RES], TMP_B[SIZE_RES];
      for (int i=0; i<SIZE_RES; i++) {
         #pragma HLS PIPELINE
         TMP_A[i] = A[i]; TMP_B[i] = B[i];
      }


      for (int i=0; i<SIZE_RES; i++) {
         val1 = (TMP_A[i] - TMP_B[i]) * (TMP_A[i] + TMP_B[i]);

         if (val1 >= 0)
            val2 = val1 % 3;
         else
            val2 = (val1 % 6) * val1;

         TMP_R[i] = val2;
      }

      for (int i=0; i<SIZE_RES; i++) {
         #pragma HLS PIPELINE
         R[i] = TMP_R[i];
      }
   }
}
