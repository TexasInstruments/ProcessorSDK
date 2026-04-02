/*
*
* Copyright (c) 2025 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef TIDLRUNTIME_INFER_H
#define TIDLRUNTIME_INFER_H

#include "itidl_ti.h"
#include "tidlruntime_infer_options.h"
#include "tidlrt_ep.h"

#include <string>
#include <map>
#include <dlfcn.h>

typedef struct
{
    void *lib;
    decltype(&::TIDL_setDefaultOptions) TIDL_setDefaultOptions;
    decltype(&::TIDL_createInferSession) TIDL_createInferSession;
    decltype(&::TIDL_runInferSession) TIDL_runInferSession;
    decltype(&::TIDL_deleteInferSession) TIDL_deleteInferSession;
} inferOps;

class TIDLRT_InferenceSession
{
public:
    TIDLRT_InferenceSession(const std::map<std::string, std::string> &sessOptions);
    ~TIDLRT_InferenceSession();
    int32_t TIDLRT_InferenceSession_Run(sTIDLRT_Tensor_t *in[], sTIDLRT_Tensor_t *out[]);
    int32_t TIDLRT_InferenceSession_Delete();
    void TIDLRT_InferenceSession_DumpOptions();

public:
    uint8_t                         m_session_created;
    TIDL_TidlRtEPInferState         m_infer_state;
private:
    inferOps                        m_infer_ops;
    TIDLRT_InferenceSession_Options m_infer_option;
};


#endif  /* TIDLRUNTIME_INFER_H */