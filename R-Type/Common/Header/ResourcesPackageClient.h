#ifndef _RESOURCESPACKAGECLIENT_H_
# define _RESOURCESPACKAGECLIENT_H_

#include "ResourcesPackage.h"

class ResourcesPackageClient : public ResourcesPackage
{
public:
	ResourcesPackageClient(FileToSend, std::list<FileToSend> &, std::list<Animation> &);
	~ResourcesPackageClient();
};

#endif