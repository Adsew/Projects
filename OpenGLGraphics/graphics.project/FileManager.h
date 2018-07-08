#pragma once

#include "IManager.h"

#include <fstream>

// FileManager that provides I/O functionality
class FileManager final : protected IManager
{
private:
	FileManager();
	~FileManager();

public:
	FileManager(const FileManager&) = delete;
	FileManager& operator=(const FileManager&) = delete;

	static FileManager& instance()
	{
		static FileManager _instance;
		return _instance;
	}

	void update(float _deltaTime) override;
	void initialize() override;
	static bool loadFile(std::string* data, const char* path)
	{
		std::ifstream ifs(path, std::ifstream::in);

		if (!ifs.is_open())
		{
			ifs.close();
			return false;
		}

		// create a file buffer and empty contents into string
		data->assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		return true;
	}
};

