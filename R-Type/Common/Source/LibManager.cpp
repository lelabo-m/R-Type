#include <iostream>
#include <vector>
#include <algorithm>
#include "LibManager.h"

LibManager::LibManager()
{
#ifdef _WIN32 // WINDOWS
	this->_loader = new WinLibLoader();
#else // LINUX
	this->_loader = new LinuxLibLoader();
#endif
	this->_libs.clear();
	this->_funcs.clear();
}

LibManager::~LibManager()
{
	for (auto node : this->_libs)
	{
		this->_loader->CloseLib(node.second);
		node.second = nullptr;
	}
	this->_libs.clear();
	this->_funcs.clear();
	delete this->_loader;
}

bool	LibManager::LoadLibrary(const std::string& path)
{
	if (FileSystem::TypeOfFile(path) != FileSystem::FileType::INVALID)
	{
		if (FileSystem::TypeOfFile(path) == FileSystem::FileType::DIRECTORY)
		{
			std::vector<std::string>	files;
			FileSystem::ListDirectory(path, "*", files);
			for (auto subpath : files)
				LoadLibrary(subpath);
		}
		else if (FileSystem::TypeOfFile(path) == FileSystem::FileType::REGULAR
			&& FileSystem::FileExt(path) == LIBEXT)
		{
			LIB l = _loader->OpenLib(path);
			_libs[FileSystem::FileShortName(path)] = l;
		}
		else
			std::cerr << "Warning : Is not a valid lib : " << path << std::endl;
	}
	else
		throw new LoadingException(std::string("Path does not exist : " + path).c_str());
	return true;
}

bool	LibManager::LoadLibraryAs(const std::string& name, const std::string& path)
{
	if (FileSystem::TypeOfFile(path) != FileSystem::FileType::INVALID)
	{
		if (FileSystem::TypeOfFile(path) == FileSystem::FileType::REGULAR && FileSystem::FileExt(path) == LIBEXT)
		{
			LIB l = _loader->OpenLib(path);
			_libs[name] = l;
		}
		else
			std::cerr << "Warning : Is not a valid lib : " << path << std::endl;
	}
	else
		throw new LoadingException(std::string("Path does not exist : " + path).c_str());
	return true;
}

bool	LibManager::LoadFunction(const std::string& lib, const std::string& func)
{
	LIB	l = _libs[lib];
	if (!l)
		return false;
	FUNC f = _loader->OpenFunc(l, func);
	_funcs[lib][func] = f;
	return true;
}