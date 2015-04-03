#ifndef LIBLOADER_HH_
#define LIBLOADER_HH_

#include <string>
#include <stdexcept>
#include "FileSystem.hpp"

#ifdef _WIN32 // WINDOWS

#include <windows.h>
#define LIBEXT ".dll"
typedef HMODULE LIB;
typedef void* FUNC;

#else // LINUX

#include <dlfcn.h>
#define LIBEXT ".so"
typedef void* LIB;
typedef void* FUNC;

#endif

// ########### Exception #########

class LoadingException : public std::runtime_error
{
public:
	LoadingException(const char *msg = "") : std::runtime_error(msg) {};
	LoadingException(const LoadingException& other) : std::runtime_error(other.what()) {}
	LoadingException(const LoadingException&& other) : std::runtime_error(other.what()) {}
	LoadingException&	operator=(const LoadingException&) { return *this; }
	LoadingException&	operator=(const LoadingException&&) { return *this; }
};

class LibLoader
{
public:
	LibLoader() {};
	virtual ~LibLoader() {};
	virtual LIB			OpenLib(const std::string& path) = 0;
	virtual FUNC		OpenFunc(LIB l, const std::string& fname) = 0;
	virtual bool		CloseLib(LIB l) = 0;
};

#ifdef _WIN32 // WINDOWS

class WinLibLoader : public LibLoader
{
public:
	WinLibLoader();
	~WinLibLoader();
	LIB		OpenLib(const std::string& path);
	FUNC	OpenFunc(LIB l, const std::string& fname);
	bool	CloseLib(LIB l);
}; 

#else // LINUX

class LinuxLibLoader : public LibLoader
{
public:
	LinuxLibLoader();
	~LinuxLibLoader();
	LIB		OpenLib(const std::string& path);
	FUNC	OpenFunc(LIB l, const std::string& fname);
	bool	CloseLib(LIB l);
};

#endif

#endif /* !LIBLOADER_HH_ */