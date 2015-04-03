#include "MapSelected.h"

MapSelected *MapSelected::inst = NULL;

MapSelected::MapSelected()
{
}

MapSelected::~MapSelected()
{
}

MapSelected *MapSelected::getInst()
{
	if (inst == NULL)
		inst = new MapSelected();
	return inst;
}

void MapSelected::deleteInst()
{
	if (inst)
	{
		delete inst;
	}
	inst = NULL;
}

const std::string 		&MapSelected::getMap() const
{
	return this->_mapName;
}

void					MapSelected::setMap(const std::string &name)
{
	this->_mapName = name;
}