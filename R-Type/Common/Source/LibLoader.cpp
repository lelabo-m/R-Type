#include <iostream>
#include "LibLoader.h"
// ########### Loader #########

#ifdef _WIN32 // WINDOWS

WinLibLoader::WinLibLoader() {}
WinLibLoader::~WinLibLoader() {}

LIB		WinLibLoader::OpenLib(const std::string& path)
{
	std::wstring widestr;
	widestr.assign(path.begin(), path.end());
	LIB l = LoadLibraryW(widestr.c_str());
	if (!l)
		throw new LoadingException(std::string("Cannot open library : " + path).c_str());
	return l;
}

FUNC	WinLibLoader::OpenFunc(LIB l, const std::string& fname)
{
	if (!l)
		return nullptr;
	FUNC	f = GetProcAddress(l, fname.c_str());
	if (!f)
		throw new LoadingException(std::string("Cannot open function : " + fname).c_str());
	return f;
}

bool	WinLibLoader::CloseLib(LIB l)
{
	if (!FreeLibrary(l))
		throw new LoadingException(std::string("Cannot close lib").c_str());
	return true;
}

#else // LINUX

LinuxLibLoader::LinuxLibLoader() {}
LinuxLibLoader::~LinuxLibLoader() {}

#include <iostream>

LIB		LinuxLibLoader::OpenLib(const std::string& p)
{
   std::string path = p;
	LIB l = dlopen(path.c_str(), RTLD_NOW);
	if (!l)
	   throw LoadingException(std::string("Cannot open library : " + path + " " + dlerror()).c_str());
	return l;
}

FUNC	LinuxLibLoader::OpenFunc(LIB l, const std::string& fname)
{
	if (!l)
		return nullptr;
	FUNC	f = dlsym(l, fname.c_str());
	if (!f)
		throw LoadingException(std::string("Cannot open function : " + fname).c_str());
	return f;
}

bool	LinuxLibLoader::CloseLib(LIB l)
{
	if (dlclose(l))
		throw LoadingException(std::string("Cannot close lib").c_str());
	return true;
}

#endif
