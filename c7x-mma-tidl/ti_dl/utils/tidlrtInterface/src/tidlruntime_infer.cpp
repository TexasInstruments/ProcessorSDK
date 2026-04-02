/*
* Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "tidlruntime_infer.h"
#include <dirent.h>

/*
 * TIDLRT_InferenceSession CLASS BEGIN
 */
TIDLRT_InferenceSession::TIDLRT_InferenceSession(const std::map<std::string, std::string> &sessOptions)
{
    int32_t status = 0;
    m_session_created = 0;

    // Trim option key and value string
    std::map<std::string, std::string> options = trimStringMap(sessOptions);

    // Load libraries
    std::string lib_path;
    std::string tidl_tools_path = "";

    if(options.find("tidlToolsPath") != options.end())
    {
        tidl_tools_path = options.at("tidlToolsPath");
    }
    else if(options.find("tidl_tools_path") != options.end())
    {
        tidl_tools_path = options.at("tidl_tools_path");
    }

    if (tidl_tools_path == "")
    {
        lib_path = "libtidlrt_EP.so";
        m_infer_ops.lib = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    }
    else
    {
        lib_path = tidl_tools_path + "/libtidlrt_EP.so";
        m_infer_ops.lib = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if(!m_infer_ops.lib)
        {
            lib_path = "libtidlrt_EP.so";
            m_infer_ops.lib = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        }
    }

    if(!m_infer_ops.lib)
    {
        printf("[TIDLRT_InferenceSession] ERROR - %s\n", dlerror());
        return;
    }

    // Load function from library
	m_infer_ops.TIDL_setDefaultOptions = reinterpret_cast<decltype(m_infer_ops.TIDL_setDefaultOptions)>(dlsym(m_infer_ops.lib, "TIDL_setDefaultOptions"));
	m_infer_ops.TIDL_createInferSession = reinterpret_cast<decltype(m_infer_ops.TIDL_createInferSession)>(dlsym(m_infer_ops.lib, "TIDL_createInferSession"));
    m_infer_ops.TIDL_runInferSession = reinterpret_cast<decltype(m_infer_ops.TIDL_runInferSession)>(dlsym(m_infer_ops.lib, "TIDL_runInferSession"));
	m_infer_ops.TIDL_deleteInferSession = reinterpret_cast<decltype(m_infer_ops.TIDL_deleteInferSession)>(dlsym(m_infer_ops.lib, "TIDL_deleteInferSession"));

	if(m_infer_ops.TIDL_setDefaultOptions == nullptr ||
	   m_infer_ops.TIDL_createInferSession == nullptr ||
	   m_infer_ops.TIDL_runInferSession == nullptr ||
       m_infer_ops.TIDL_deleteInferSession == nullptr)
	{
		printf("[TIDLRT_InferenceSession] ERROR - %s\n", dlerror());
        return;
	}

    // Parse the options
    m_infer_ops.TIDL_setDefaultOptions(&m_infer_option.m_options);
    status = m_infer_option.TIDLRT_InferenceSession_Options_Parse(options);
    if (status == -1)
    {
        printf("[TIDLRT_InferenceSession] ERROR - Invalid options provided\n");
        return;
    }
    m_infer_state.m_options = m_infer_option.m_options;

    //Create the session
    status = m_infer_ops.TIDL_createInferSession(&m_infer_state);

    if (status == 0)
    {
        m_session_created = 1;
    }
    else
    {
        printf("[TIDLRT_InferenceSession] ERROR - Failed to create subgraph runtime, status=%d\n", status);
    }
}

int32_t TIDLRT_InferenceSession::TIDLRT_InferenceSession_Run(sTIDLRT_Tensor_t *in[], sTIDLRT_Tensor_t *out[])
{
    int32_t status = 0;
    if(m_session_created)
    {
        status = m_infer_ops.TIDL_runInferSession(&m_infer_state, in, out);
    }
    else
    {
        status = -1;
    }
    return status;
}

int32_t TIDLRT_InferenceSession::TIDLRT_InferenceSession_Delete()
{
    int32_t status = 0;
    if(m_session_created)
    {
        status = m_infer_ops.TIDL_deleteInferSession(&m_infer_state);
    }
    else
    {
        status = -1;
    }
    return status;
}

void TIDLRT_InferenceSession::TIDLRT_InferenceSession_DumpOptions()
{
    m_infer_option.TIDLRT_InferenceSession_Options_Dump();
}

TIDLRT_InferenceSession::~TIDLRT_InferenceSession()
{
    TIDLRT_InferenceSession_Delete();
}

/*
 * TIDLRT_InferenceSession CLASS END
 */