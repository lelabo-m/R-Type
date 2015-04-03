#include "MapDownloader.h"


MapDownloader::MapDownloader(const std::string &mapName) : _mapName(mapName), _downloadState(WAITING)
{
}

MapDownloader::~MapDownloader()
{
}

const std::string &MapDownloader::getMapName() const
{
	return _mapName;
}

MapDownloader::DownloadState MapDownloader::getDownloadState() const
{
	return _downloadState;
}

void MapDownloader::setDownloadState(DownloadState state)
{
	_downloadState = state;
}