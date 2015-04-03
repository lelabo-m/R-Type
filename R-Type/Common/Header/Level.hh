//
// Level.hh for Header in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Common/Header
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Mon Dec 15 20:23:43 2014 Alexandre Paillier
// Last update Mon Jan 05 19:20:45 2015 Alexandre Paillier
//

#ifndef LEVEL_HH_
# define LEVEL_HH_

# include <string>
# include <list>
# include "XML_Level.hh"

struct LevelRef
{
public:
    std::string	name;
    float	value;
    explicit LevelRef(const std::string &name, float value);
};

struct LevelElem
{
public:
    std::string		name;
    unsigned int	posX;
    unsigned int	posY;
    unsigned int	time;
    LevelElem		&operator=(const LevelElem &);
    explicit LevelElem(const std::string &name, unsigned int posX, unsigned int posY, unsigned int time);
    ~LevelElem(void);
};

typedef std::list<LevelElem>	LevelContent;

class Level
{
private:
    std::list<LevelRef>		_levelRefs;
    std::list<LevelElem>	_levelElems;
    void 			read(const XML_Level &xml);
    void 			write(XML_Level &xml);
public:
    Level(void);
    Level(const std::list<LevelRef> &levelRefs, const std::list<LevelElem> &levelElems);
    const std::list<LevelRef>	getLevelRefs(void) const;
    const std::list<LevelElem>	getLevelElems(void) const;
    bool 			readFromFile(const std::string &filename);
    bool 			readFromString(const std::string &string);
    bool 			writeToFile(const std::string &filename);
    bool 			writeToString(std::string &string);
    ~Level(void);
};

#endif // !LEVEL_HH_
