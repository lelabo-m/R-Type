#ifndef _FILEMANAGER_H_
# define _FILEMANAGER_H_

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include "TCPFrame.h"
#include "FileSystem.hpp"

namespace FileManager
{
	std::string *readFile(const std::string &file_path);
	void writeFile(const std::string &path, const std::string &file);
	void writeFileFromFrame(TCPFrame *frame);
};

#endif
