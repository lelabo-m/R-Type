#include "ResourcesPackageClient.h"

ResourcesPackageClient::ResourcesPackageClient(FileToSend f,
	std::list<FileToSend> & ft,
	std::list<Animation> &a) :
	ResourcesPackage(f, ft, a, TCPMessageFactoryClient::getInstance(), Protocole::Client::SEND_ANIMATION)
{
}

ResourcesPackageClient::~ResourcesPackageClient()
{
}