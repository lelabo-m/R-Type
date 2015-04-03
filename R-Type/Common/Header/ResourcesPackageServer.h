#ifndef _RESOURCESPACKAGESERVER_H_
# define _RESOURCESPACKAGESERVER_H_

#include "ResourcesPackage.h"

class ResourcesPackageServer : public ResourcesPackage
{
public:
	ResourcesPackageServer(FileToSend, std::list<FileToSend> &, std::list<Animation> &);
	~ResourcesPackageServer();
};

#endif