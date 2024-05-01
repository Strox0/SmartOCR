#include "OCR.h"
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>

#include "ErrorCodes.h"
#include "ResourceManager.h"
#include "resource.h"

#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"

bool CheckExtensions(const std::string& file_ext, const std::string& extensions)
{
	if (extensions.find(file_ext) != std::string::npos)
		return true;

	return false;
}


OCR::OCR(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		m_error_code = ERROR_FILE_DOES_NOT_EXIST;
		return;
	}

	if (std::filesystem::is_directory(path))
		m_directory = true;
	else if (std::filesystem::is_regular_file(path))
	{
		m_directory = false;

		if (!CheckExtensions(std::filesystem::path(path).extension().string(), ".jpg,.tiff,.png,.bmp,.webp,.raw,.heif,.jpeg,.tif,.heic"))
		{
			m_error_code = ERROR_INVALID_FILE_FORMAT;
			return;
		}
	}
	else
	{
		m_error_code = ERROR_INVALID_FILE_FORMAT;
		return;
	}

	m_error_code = ERROR_SUCCESS;
	m_path = path;
}

void OCR::SetConfig(const ConfigRes& config)
{
	m_config = config;
	m_config_set = true;
}

void OCR::Process()
{
	if (m_error_code != ERROR_SUCCESS)
		return;

	std::filesystem::path tessdata_folder = std::filesystem::temp_directory_path() += std::filesystem::path("\\tessdata");

	if (!std::filesystem::exists(tessdata_folder))
		std::filesystem::create_directory(tessdata_folder);

	if (!std::filesystem::exists(tessdata_folder / std::filesystem::path("eng.traineddata")))
	{
		ResourceManager resc(IDR_BINARY1, L"BINARY");

		if (resc.Error())
		{
			m_error_code = ERROR_COULD_NOT_LOAD_RESOURCE;
			return;
		}

		resc.Owned(true);

		std::ofstream file(tessdata_folder.string() + "\\eng.traineddata", std::ios::binary);
		file.write((char*)resc.Get(), resc.Size());
		file.close();
	}


	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();

	if (api->Init(tessdata_folder.string().c_str(), "eng"))
	{
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	std::vector<std::string> results;

	if (m_directory)
	{
		for (const auto& i : std::filesystem::directory_iterator(m_path))
		{
			if (!i.is_regular_file())
				continue;

			if (!CheckExtensions(i.path().extension().string(), ".jpg,.tiff,.png,.bmp,.webp,.raw,.heif,.jpeg,.tif,.heic"))
				continue;

			std::cout << "Processing file: " << i.path() << "\n";

			Pix* image = pixRead(i.path().string().c_str());
			api->SetImage(image);

			results.emplace_back(api->GetUTF8Text());

			pixDestroy(&image);
		}
	}
	else
	{
		std::cout << "Processing file: " << m_path << "\n";

		Pix* image = pixRead(m_path.c_str());
		api->SetImage(image);

		results.emplace_back(api->GetUTF8Text());

		pixDestroy(&image);
	}

	api->End();

	CreateOutput(results);
}

void OCR::CreateOutput(std::vector<std::string>& results)
{
	std::cout << "Generating output...\n";

	if (m_config_set)
	{
		std::ofstream output("output.csv");

		for (const auto& [names,ng,ns] : m_config)
		{
			output << names << ",";
		}

		output << "\n";

		for (const auto& result : results)
		{
			bool did_match = false;
			for (const auto& [name,reg_group,reg_stm] : m_config)
			{
				std::regex reg(reg_stm,std::regex_constants::ECMAScript | std::regex_constants::icase);
				std::smatch match;

				if (std::regex_search(result, match, reg))
				{
					if (match.size() > reg_group)
					{
						output << match[reg_group] << ",";
						did_match = true;
					}
				}
			}

			if (did_match)
				output << "\n";
		}

		output.close();
	}
	else
	{
		std::ofstream output("output.txt");

		for (const auto& result : results)
		{
			output << result << "\n";
		}

		output.close();
	}

	std::cout << "Output generated.\n";
}
