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

#include "tidlruntime_infer_options.h"
#include <dirent.h>

int32_t parseSingleOption(sOptionMapping &entry, const std::map<std::string, std::string> &options, std::string option)
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
				printf("[TIDLRT_InferenceSession_Options] ERROR - Could not parse %s to valid datatype\n", i.first.c_str());
				status = -1;
			}
			break;
		}
	}
	return status;
}


TIDLRT_InferenceSession_Options::TIDLRT_InferenceSession_Options()
{
    // pass
}

int32_t TIDLRT_InferenceSession_Options::TIDLRT_InferenceSession_Options_Parse(const std::map<std::string, std::string> &options)
{
    int32_t status = 0;

	for(auto &entry : m_optionMap)
	{
		// Parse TIDLRT Option first
		status = parseSingleOption(entry, options, entry.key.first);
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
			status = parseSingleOption(entry, options, entry.key.second);
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

    status = TIDLRT_InferenceSession_Options_Translate(options);

    return status;
}

int32_t TIDLRT_InferenceSession_Options::TIDLRT_InferenceSession_Options_Translate(const std::map<std::string, std::string> &options)
{
	int32_t status = 0;

	for(auto &entry : m_optionMap)
	{
		if (entry.translate == true && entry.parseCount != 0 && entry.parseType == 1)
		{
			std::pair<std::string,std::string> keys = entry.key;
			std::string tidl_key = keys.first;
			std::string osrt_key = keys.second;
			void *data = (void *)entry.data;

			if(osrt_key == "artifacts_folder")
			{
				std::string artifacts_folder((char *)data);
				DIR* dir = opendir(artifacts_folder.c_str());
                if (dir == nullptr)
                {
                    printf("[TIDLRT_InferenceSession_Options] ERROR - Could not open artifacts_folder(%s)\n", artifacts_folder.c_str());
                    status = -1;
                    return status;
                }

                dirent* entry;
                std::vector<std::string> files = {};
                std::vector<std::string> file_identifiers = {};
                std::string file_identifiers_string = "";
                if(tidl_key == "netBinFile")
				{
					file_identifiers.push_back("_net_");
                    file_identifiers.push_back("_net.");
				}
				else if(tidl_key == "paramsBinFile")
				{
                    file_identifiers.push_back("_io_");
				}

                for (size_t i = 0; i < file_identifiers.size(); i++)
                {
                    file_identifiers_string += file_identifiers[i];
                    if (i < file_identifiers.size() - 1)
                    {
                        file_identifiers_string += " or ";
                    }
                }

                while ((entry = readdir(dir)) != nullptr)
                {
                    std::string filename = std::string(entry->d_name);

                    if(filename == "." || filename == "..")
                    {
                        continue;
                    }

                    if (std::count(filename.begin(), filename.end(), '.') > 1)
                    {
                        continue;
                    }

                    std::string suffix = ".bin";
                    if (filename.length() >= suffix.length() &&
                        filename.compare(filename.length() - suffix.length(), suffix.length(), suffix) == 0)
                    {
                        for (size_t i = 0; i < file_identifiers.size(); i++)
                        {
                            if (filename.find(file_identifiers[i]) != std::string::npos)
                            {
                                files.push_back(filename);
                                break;
                            }
                        }
                    }
                }

                if (files.size() == 0)
                {
                    printf("[TIDLRT_InferenceSession_Options] ERROR - artifacts_folder(%s) does not have any binary file with identifier %s\n", artifacts_folder.c_str(), file_identifiers_string.c_str());
                    status = -1;
                    return status;
                }
                else if (files.size() > 1)
                {
                    printf("[TIDLRT_InferenceSession_Options] ERROR - artifacts_folder(%s) has multiple binary files with identifier %s\n", artifacts_folder.c_str(), file_identifiers_string.c_str());
                    status = -1;
                    return status;
                }

                if(tidl_key == "netBinFile")
				{
					std::string outputNetFile = artifacts_folder + "/" + files[0];
					strcpy((char *)data, outputNetFile.c_str());
				}
				else if(tidl_key == "paramsBinFile")
				{
					std::string outputParamsFile = artifacts_folder + "/" + files[0];
					strcpy((char *)data, outputParamsFile.c_str());
                }
			}
		}
	}

	return status;
}

void TIDLRT_InferenceSession_Options::TIDLRT_InferenceSession_Options_Dump()
{
    printf("\n************************ Inference Options ************************\n");
    for(const auto &entry : m_optionMap)
    {
        void *data = entry.data;
        std::string line = getEntryString(entry, data);
        if (line != "")
        {
            printf("%s\n",line.c_str());
        }
    }
    printf("*******************************************************************\n\n");
}
