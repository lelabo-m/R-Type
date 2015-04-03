#include "ResourcesPackageServer.h"

ResourcesPackageServer::ResourcesPackageServer(FileToSend f,
												std::list<FileToSend> & ft,
												std::list<Animation> &a) :
												ResourcesPackage(f, ft, a, TCPMessageFactoryServer::getInstance(), Protocole::Server::SEND_ANIMATION)
{
}

ResourcesPackageServer::~ResourcesPackageServer()
{
}