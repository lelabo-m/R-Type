#ifndef	_MAPSELECTED_H
#define _MAPSELECTED_H

#include <string>

class MapSelected
{
private:
	std::string			_mapName;
	static MapSelected	*inst;

private:
	const MapSelected &operator=(const MapSelected &) { return *this; };
	const MapSelected &operator=(const MapSelected &&) { return *this; };
	MapSelected();
	~MapSelected();

public:
	static MapSelected		*getInst();
	static void				deleteInst();
	const std::string 		&getMap() const;
	void					setMap(const std::string &);
};

#endif
