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

#ifdef BUILD_COMPILE
#include "tidlruntime_compile_options.h"
#include "itidl_ti.h"
#include <algorithm>
#include <dirent.h>

int32_t parseCompileSingleOption(sOptionMapping &entry, const std::map<std::string, std::string> &options, std::string option)
{
	int32_t status = 0;

	// Skip if already parsed
	if (entry.parseCount > 0)
	{
		return status;
	}

	for(const auto &i : options)
	{
		if (i.first == "" && i.second == "")
		{
			continue;
		}

		if(i.first == option)
		{
			int32_t parseCount = parseEntry(entry, (void *)entry.data, i.second);
			entry.parseCount = parseCount;
			status = 1;
			if (parseCount == 0)
			{
				printf("[TIDLRT_CompileSession_Options] ERROR - Could not parse %s to valid datatype\n", i.first.c_str());
				status = -1;
			}
			break;
		}
	}
	return status;
}

TIDLRT_CompileSession_Options::TIDLRT_CompileSession_Options()
{
    // Set defaults
}

int32_t TIDLRT_CompileSession_Options::TIDLRT_CompileSession_Options_Parse(const std::map<std::string, std::string> &options)
{
    int32_t status = 0;

	for(auto &entry : m_optionMap)
	{
		// Parse TIDLRT Option first
		status = parseCompileSingleOption(entry, options, entry.key.first);
		if (status == -1)
		{
			break;
		}
		else if (status == 1)
		{
			entry.parseType = 0;
		}
		else
		{
			// Parse OSRT Option if not parsed above
			status = parseCompileSingleOption(entry, options, entry.key.second);
			if (status == -1)
			{
				break;
			}
			if (status == 1)
			{
				entry.parseType = 1;
			}
		}
	}

	if (status == -1)
    {
        return status;
    }

	// Translate OSRT option if needed
	status = TIDLRT_CompileSession_Options_Translate(options);

    return status;
}

int32_t TIDLRT_CompileSession_Options::TIDLRT_CompileSession_Options_Translate(const std::map<std::string, std::string> &options)
{
	int32_t status = 0;

	for(auto &entry : m_optionMap)
	{
		std::pair<std::string,std::string> keys = entry.key;
		std::string tidl_key = keys.first;
		std::string osrt_key = keys.second;
		void *data = (void *)entry.data;

		if (entry.translate == true && entry.parseCount != 0 && entry.parseType == 1)
		{
			if (osrt_key == "advanced_options:quantization_scale_type")
			{
				int32_t *value = (int32_t *)data;
				std::map<int32_t, int32_t> mapping = {{0, TIDL_QuantStyleNP2Fixed},
													  {1, TIDL_QuantStyleP2Dynamic},
													  {3, TIDL_QuantStyleAsymNP2_TFL},
													  {4, TIDL_QuantStyleAsymNP2}};
				
				if(mapping.find(*value) == mapping.end())
				{
					printf("[TIDLRT_CompileSession_Options] ERROR - Unsupported quantization_scale_type. Supported values are (0,1,3,4)");
					return -1;
				}
				*value = mapping[*value];
			}

			else if(osrt_key == "accuracy_level")
			{
				int32_t *value = (int32_t *)data;
				std::map<int32_t, int32_t> valid_calibs = {{0, 64},
														   {1, 7},
														   {2, 132},
														   {9, 9}};
				if(valid_calibs.find(*value) == valid_calibs.end()) 
				{
					printf("[TIDLRT_CompileSession_Options] ERROR - Unsupported accuracy_level. Supported values are (0,1,2,9)");
					return -1;
				}
				*value = valid_calibs[*value];

				int bias_calibration = 1;
				int weight_clipping = 1;
				int activation_clipping = 1;
				int channel_wise_quantization = 0;
				int bias_clipping = 0;

				auto it_bias_cal = options.find("advanced_options:bias_calibration");
				if (it_bias_cal != options.end())
				{
					try
					{
						bias_calibration = std::stoi(it_bias_cal->second);
					}
					catch (const std::exception& e)
					{
						printf("[TIDLRT_CompileSession_Options] WARNING - Invalid bias_calibration value: %s. Using bias_calibration : 1\n", it_bias_cal->second.c_str());
					}
				}

				auto it_weight = options.find("advanced_options:weight_clipping");
				if (it_weight != options.end())
				{
					try
					{
						weight_clipping = std::stoi(it_weight->second);
					}
					catch (const std::exception& e)
					{
						printf("[TIDLRT_CompileSession_Options] WARNING - Invalid weight_clipping value: %s. Using weight_clipping : 1\n", it_weight->second.c_str());
					}
				}

				auto it_activation = options.find("advanced_options:activation_clipping");
				if (it_activation != options.end())
				{
					try
					{
						activation_clipping = std::stoi(it_activation->second);
					}
					catch (const std::exception& e)
					{
						printf("[TIDLRT_CompileSession_Options] WARNING - Invalid activation_clipping value: %s. Using activation_clipping : 1\n", it_activation->second.c_str());
					}
				}

				auto it_channel = options.find("advanced_options:channel_wise_quantization");
				if (it_channel != options.end())
				{
					try
					{
						channel_wise_quantization = std::stoi(it_channel->second);
					}
					catch (const std::exception& e)
					{
						printf("[TIDLRT_CompileSession_Options] WARNING - Invalid channel_wise_quantization value: %s. Using channel_wise_quantization : 0\n", it_channel->second.c_str());
					}
				}

				auto it_bias_clip = options.find("advanced_options:bias_clipping");
				if (it_bias_clip != options.end())
				{
					try
					{
						bias_clipping = std::stoi(it_bias_clip->second);
					}
					catch (const std::exception& e)
					{
						printf("[TIDLRT_CompileSession_Options] WARNING - Invalid bias_clipping value: %s. Using channel_wise_quantization : 0\n", it_bias_clip->second.c_str());
					}
				}

				if (m_options.quantizationStyle == TIDL_QuantStyleAsymNP2)
				{
					if((*value & TIDL_CalibNoOutlier) == 0)
					{
						*value = bias_calibration * TIDL_CalibOptionBiasCalibration; 
					}
				}

				if (*value == 9)
				{
					*value = activation_clipping * TIDL_CalibOptionActivationRange +
							 weight_clipping * TIDL_CalibOptionWeightRange +
							 bias_calibration * TIDL_CalibOptionBiasCalibration +
							 channel_wise_quantization * TIDL_CalibOptionPerChannelWeightQuantization +
							 bias_clipping * TIDL_CalibOptionBiasRange;
				}
			}

			else if(osrt_key == "tensor_bits")
			{
				int32_t *value = (int32_t *)data;
				std::vector<int32_t> valid_num_params{8, 16, 32};
				if(std::find(valid_num_params.begin(), valid_num_params.end(), *value) == valid_num_params.end())
				{
					printf("[TIDLRT_CompileSession_Options] ERROR - Unsupported tensor_bits. Supported values are (8,12,32)");
					return -1;
				}
				if(*value == 32)
				{
					printf("[TIDLRT_CompileSession_Options] WARNING - Compiling for floating point. Inference on target will not be supported for compiled model");
				}
			}

			else if(osrt_key == "ti_internal_reserved_1")
			{
				int32_t *value = (int32_t *)data;
				if (entry.parseCount >= TIDL_MAX_ALG_IN_BUFS)
				{
					entry.parseCount = TIDL_MAX_ALG_IN_BUFS - 1;
				}
				if (*(value + entry.parseCount - 1) != -1)
				{
					*(value + entry.parseCount) = -1;
					entry.parseCount += 1;

				}
			}

			else if(osrt_key == "artifacts_folder")
			{
				std::string artifacts_folder((char *)data);
				DIR* dir = opendir(artifacts_folder.c_str());
				if (dir == nullptr)
				{
					printf("[TIDLRT_CompileSession_Options] ERROR - Could not open artifacts_folder(%s)\n", artifacts_folder.c_str());
					status = -1;
					return status;
				}

				if(tidl_key == "outputNetFile")
				{
					std::string outputNetFile = artifacts_folder + "/subgraph_0_tidl_net.bin";
					strcpy((char *)data, outputNetFile.c_str());
				}
				else if(tidl_key == "outputParamsFile")
				{
					std::string outputParamsFile = artifacts_folder + "/subgraph_0_tidl_io_";
					strcpy((char *)data, outputParamsFile.c_str());
				}
			}

			else if(osrt_key == "tidl_tools_path")
			{

				std::string value((char *)data);

				if(tidl_key == "perfSimTool")
				{
					std::string perfSimTool = value + "/ti_cnnperfsim.out";
					strcpy((char *)data, perfSimTool.c_str());
				}
				else if(tidl_key == "graphVizTool")
				{
					std::string graphVizTool = value + "/tidl_graphVisualiser.out";
					strcpy((char *)data, graphVizTool.c_str());
				}
				else if(tidl_key == "tidlStatsTool")
				{
					std::string tidlStatsTool = value + "/PC_dsp_test_dl_algo.out";
					strcpy((char *)data, tidlStatsTool.c_str());
				}
				else if(tidl_key == "perfSimConfig")
				{
					std::string perfSimConfig = value + "/device_config.cfg";
					strcpy((char *)data, perfSimConfig.c_str());
				}
				else if(tidl_key == "modelDumpTool")
				{
					std::string modelDumpTool = value + "/tidl_dump.out";
					strcpy((char *)data, modelDumpTool.c_str());
				}
			}
		}
	}

	for(auto &entry : m_optionMap)
	{
		// Automatically set model type based on model extension if not provided
		if (entry.key.first == "modelType" && entry.parseCount == 0)
		{
			char *last_dot_occurance = strrchr((char *)m_options.inputNetFile, '.');
			if (last_dot_occurance != NULL)
			{
				char *extension = last_dot_occurance + 1;
				if (strcmp(extension, "prototxt") == 0)
				{
					m_options.modelType = 0;
				}
				else if (strcmp(extension, "pb") == 0)
				{
					m_options.modelType = 1;
				}
				else if (strcmp(extension, "onnx") == 0)
				{
					m_options.modelType = 2;
				}
				else if (strcmp(extension, "tflite") == 0)
				{
					m_options.modelType = 3;
				}
			}
		}
	}

	return status;
}

void TIDLRT_CompileSession_Options::TIDLRT_CompileSession_Options_Dump()
{
    printf("************************ Compile Options ************************\n");
    for(const auto &entry : m_optionMap)
    {
        void *data = entry.data;
        std::string line = getEntryString(entry, data);
        if (line != "")
        {
            printf("%s\n",line.c_str());
        }
    }
    printf("*******************************************************************\n");
}
#endif /* BUILD_COMPILE */