#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "TCPFrame.h"
#include "FileSystem.hpp"
#include "FileManager.h"

namespace FileManager
{
	std::string *readFile(const std::string &file_path)
	{
		try
		{
			std::ifstream fin(file_path, std::ios::binary);
			if (!fin)
				throw std::runtime_error("File not found");
			std::stringstream str_stream;
			str_stream << fin.rdbuf();

			std::string *msg = new std::string(str_stream.str());
			fin.close();
			return msg;
		}
		catch (std::exception &e)
		{
			throw e;
		}
	}

	void writeFile(const std::string &path, const std::string &file)
	{
		try
		{
			std::string new_path = FileSystem::BuildPath(FileSystem::DirName(path));
			new_path = new_path + FileSystem::FileName(path);
			std::fstream fs(new_path, std::fstream::out | std::fstream::binary);

			if (!fs)
				throw std::runtime_error("Failed to create/open file");
			fs << file;
			fs.close();
		}
		catch (std::exception &e)
		{
			throw e;
		}
	}

	void writeFileFromFrame(TCPFrame *frame)
	{
		try
		{
			std::string file_path = &frame->getData()[TCP_HEADER_LENGTH];
			std::string data;

			data.insert(0, &frame->getData()[TCP_HEADER_LENGTH + file_path.size() + 1],
				frame->getHeader().getDataLength() - file_path.size() - 1);
			writeFile(file_path, data);
		}
		catch (std::exception &e)
		{
			throw e;
		}
	}
}