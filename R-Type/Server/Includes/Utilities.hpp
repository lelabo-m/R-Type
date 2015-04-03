#ifndef _UTILITIES_HPP_
# define _UTILITIES_HPP_

#include <string>
#include <vector>
#include "FileSystem.hpp"

namespace Utilities
{
	static std::string getFilesNames_directory(const std::string &directory, const std::string mask)
	{
		std::vector<std::string> vec;

		FileSystem::ListDirectory(directory, mask, vec);
		for (auto it = vec.begin(); it != vec.end();)
		{
			if (FileSystem::FileExt(*it) != std::string(".map"))
			{
				it = vec.erase(it);
			}
			else
			{
				*it = FileSystem::FileName(*it);
				it++;
			}
		}

		if (vec.size() == 0)
			return "";

		std::string files;
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			files = files + *it + ",";
		}
		return files;
	}
}

#endif
