//
// PluginStorage.hh for Includes in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Server/Includes
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Sun Dec 14 19:03:30 2014 Alexandre Paillier
// Last update Sat Dec 20 16:21:30 2014 Clément Roche
//

#ifndef PLUGINSTORAGE_HH_
# define PLUGINSTORAGE_HH_

# include "ResourcesPackageServer.h"

# include <mutex>
# include <map>
# include <string>
# include "LibManager.h"
# include "GameObject.h"
# include "Factory.hpp"


class ObjectFactory : public Factory<std::string, SceneObject>
{
public:
	size_t	ClassId(const std::string& name)
	{
		if (_table.find(name) == _table.end())
			return 0;
		return _table[name];
	}
	void	Register(const std::string& name, size_t id)
	{
		_table[name] = id;
	}
protected:
	std::map<std::string, size_t>	_table;
};

struct ObjectData
{
    std::string		folder;
    size_t			id;
    SceneObject		*sObject;
};

enum PackageType { EDITOR, GAME };
class PluginStorage
{
private:
    LibManager								_libM;
	std::mutex								_mutex;
    std::map<std::string, ObjectData>		_objects;
public:
    PluginStorage(void);
    ~PluginStorage(void);
    void						initialize(const std::string &path);
   ObjectFactory*		buildFactory();
	ResourcesPackageServer*		buildPackage(const FileToSend&, PackageType = GAME);
};

#endif // !PLUGINSTORAGE_HH_
