#ifndef OBJECTREPRESENTATION_HPP_
#define OBJECTREPRESENTATION_HPP_

#include <string>
#include <sstream>
#include <list>
#include "FileSystem.hpp"

struct FileToSend
{
	FileToSend(const std::string& l, const std::string& r) : local(l), remote(r) {}
	FileToSend(const FileToSend& other) : local(other.local), remote(other.remote) {}
	FileToSend(const FileToSend&& other) : local(std::move(other.local)), remote(std::move(other.remote)) {}
	FileToSend&	operator=(const FileToSend& other)
	{
		local = other.local;
		remote = other.remote;
		return *this;
	}
	FileToSend&	operator=(const FileToSend&& other)
	{
		local = std::move(other.local);
		remote = std::move(other.remote);
		return *this;
	}
	static std::string	Name(const std::string& path) { return FileSystem::FileShortName(path); };
	static size_t		Id(const std::string& name)
	{
		std::string tmp = Name(name);
		size_t pos = tmp.find("_");
		if (pos != std::string::npos)
			return std::stoul(tmp.substr(0, pos));
		return 0;
	};
public:
	std::string	local;
	std::string	remote;
};

struct Animation
{
	Animation()
		: objname(""), classid(0), sizex(0), sizey(0), index(0), number(0), fileid(0), animid(0), duration(0) {}
	Animation(const std::string o, size_t c, size_t x, size_t y, size_t i, size_t n, size_t f, size_t a, size_t d)
		: objname(o), classid(c), sizex(x), sizey(y), index(i), number(n), fileid(f), animid(a), duration(d) {}
	Animation(const Animation& other)
		: objname(other.objname), classid(other.classid), sizex(other.sizex), sizey(other.sizey), index(other.index), number(other.number),
		fileid(other.fileid), animid(other.animid), duration(other.duration) {}
	Animation(const Animation&& other)
		: objname(std::move(other.objname)), classid(std::move(other.classid)), sizex(std::move(other.sizex)), sizey(std::move(other.sizey)),
		index(std::move(other.index)), number(std::move(other.number)), fileid(std::move(other.fileid)),
		animid(std::move(other.animid)), duration(std::move(other.duration)) {}

	Animation&	operator=(const Animation& other)
	{
		objname = other.objname;
		classid = other.classid;
		sizex = other.sizex;
		sizey = other.sizey;
		index = other.index;
		number = other.number;
		fileid = other.fileid;
		animid = other.animid;
		duration = other.duration;
		return *this;
	}

	Animation&	operator=(const Animation&& other)
	{
		objname = std::move(other.objname);
		classid = std::move(other.classid);
		sizex = std::move(other.sizex);
		sizey = std::move(other.sizey);
		index = std::move(other.index);
		number = std::move(other.number);
		fileid = std::move(other.fileid);
		animid = std::move(other.animid);
		duration = std::move(other.duration);
		return *this;
	}

	static std::string	FileName(const Animation& a)
	{
		return std::to_string(a.classid) + "_" + std::to_string(a.fileid);
	}
	static std::string	Serialize(const Animation& a)
	{
		std::stringstream	ss;
		ss << a.objname << " " << a.classid << " " << a.sizex << " " << a.sizey << " "
			<< a.index << " " << a.number << " " << a.fileid << " " << a.animid << " " << a.duration;
		return ss.str();
	}
	static Animation	UnSerialize(const std::string& str)
	{
		std::stringstream	ss;
		ss << str;
		Animation a;
		ss >> a.objname >> a.classid >> a.sizex >> a.sizey >> a.index >> a.number
			>> a.fileid >> a.animid >> a.duration;
		return a;
	}
public:
	std::string	objname;
	size_t	classid;
	size_t	sizex;
	size_t	sizey;
	size_t	index;
	size_t	number;
	size_t	fileid;
	size_t	animid;
	size_t	duration;
};

#endif /* !OBJECTREPRESENTATION_HPP_ */