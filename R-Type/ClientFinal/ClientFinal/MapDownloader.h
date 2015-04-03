#ifndef _MAPDOWNLOADER_H_
# define _MAPDOWNLOADER_H_

#include <string>

class MapDownloader
{
public:
	enum DownloadState
	{
		WAITING,
		DOWNLOADING,
		FAILED,
		FINISHED
	};

	MapDownloader(const std::string &);
	~MapDownloader();
	const std::string &getMapName() const;
	DownloadState getDownloadState() const;
	void setDownloadState(DownloadState);
protected:
	const std::string _mapName;
	DownloadState _downloadState;
};

#endif