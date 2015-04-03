//
// Level.cpp for Source in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Common/Source
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Mon Dec 15 20:23:02 2014 Alexandre Paillier
// Last update Mon Jan 05 19:23:32 2015 Alexandre Paillier
//

#include <stdexcept>
#include "Level.hh"

LevelRef::LevelRef(const std::string &name, float value) : name(name), value(value) {}

LevelElem::LevelElem(const std::string &name, unsigned int posX, unsigned int posY, unsigned int time) : name(name), posX(posX), posY(posY), time(time) {}

LevelElem &LevelElem::operator=(const LevelElem &le)
{
    this->name = le.name;
    this->posX = le.posX;
    this->posY = le.posY;
    this->time = le.time;
    return *this;
}

LevelElem::~LevelElem(void) {}

Level::Level(void) {}
Level::Level(const std::list<LevelRef> &levelRefs, const std::list<LevelElem> &levelElems) : _levelRefs(levelRefs), _levelElems(levelElems) {}

Level::~Level(void) {}

const std::list<LevelRef> Level::getLevelRefs(void) const
{
    return this->_levelRefs;
}

const std::list<LevelElem> Level::getLevelElems(void) const
{
    return this->_levelElems;
}

void Level::read(const XML_Level &xml)
{
    // LevelRefs
    for (auto it = xml.referenceIndex.c.cbegin(); it != xml.referenceIndex.c.cend(); ++it)
    {
	if (it->second.size())
	    this->_levelRefs.push_back(LevelRef(it->first, std::stof(it->second.find("value")->second)));
    }
    // LevelElems
    for (auto it = xml.content.c.cbegin(); it != xml.content.c.cend(); ++it)
    {
	XML_Level::Content::MapType::const_iterator it_[4];

	if ((it_[0] = it->find("name")) != it->end() && (it_[1] = it->find("posX")) != it->end()
		&& (it_[2] = it->find("posY")) != it->end() && (it_[3] = it->find("time")) != it->end())
	{
	    try
    	    {
		this->_levelElems.push_back(LevelElem(it_[0]->second, std::stoul(it_[1]->second), std::stoul(it_[2]->second), std::stoul(it_[3]->second)));
	    }
	    catch (std::logic_error &) {}
	}
    }
}

void Level::write(XML_Level &xml)
{
    // LevelRefs
    for (auto it = this->_levelRefs.cbegin(); it != this->_levelRefs.cend(); ++it)
    {
	XML_Level::ReferenceIndex::MapRType map;

	map.insert(XML_Level::ReferenceIndex::MapRPairType("value", std::to_string(it->value)));
	xml.referenceIndex.c.insert(XML_Level::ReferenceIndex::MapPairType(it->name, map));
    }
    // LevelElems
    for (auto it = this->_levelElems.cbegin(); it != this->_levelElems.cend(); ++it)
    {
	XML_Level::Content::MapType map;
	map.insert(XML_Level::Content::MapPairType("name", it->name));
	map.insert(XML_Level::Content::MapPairType("posX", std::to_string(it->posX)));
	map.insert(XML_Level::Content::MapPairType("posY", std::to_string(it->posY)));
	map.insert(XML_Level::Content::MapPairType("time", std::to_string(it->time)));
	xml.content.c.push_back(map);
    }
}

bool Level::readFromFile(const std::string &filename)
{
    XML_Level xml(filename.c_str(), XML_Level::READ);

    if (!xml.readFromFile())
	return false;
    this->read(xml);
    return true;
}

bool Level::readFromString(const std::string &string)
{
    XML_Level xml(NULL, XML_Level::NONE);

    if (!xml.readFromString(string))
	return false;
    this->read(xml);
    return true;
}

bool Level::writeToFile(const std::string &filename)
{
    XML_Level xml(filename.c_str(), XML_Level::READWRITE);

    this->write(xml);
    return xml.writeToFile();
}

bool Level::writeToString(std::string &string)
{
    XML_Level xml(NULL, XML_Level::NONE);

    this->write(xml);
    return xml.writeToString(string);
}
