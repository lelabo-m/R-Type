//
// PluginStorage.cpp for Sources in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Server/Sources
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Sun Dec 14 18:58:39 2014 Alexandre Paillier
// Last update Mon Jan  5 16:31:42 2015 Clément Roche
//

#include <iostream>
#include "PluginStorage.hh"
#include "Plugin.h"

PluginStorage::PluginStorage(void) {}

PluginStorage::~PluginStorage(void)
{
   for (auto it : _objects)
   {
      if (it.second.sObject)
      {
	 delete it.second.sObject;
      }
   }
}

void PluginStorage::initialize(const std::string &path)
{
	std::lock_guard<std::mutex> lock(_mutex);

	std::vector<std::string> dirs;

	FileSystem::ListDirectory(path, dirs);
	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
	   std::cout << "Loading elem:" << FileSystem::FileName(*it) << std::endl;
		std::vector<std::string> files;
		std::string dirName = FileSystem::FileName(*it);
		FileSystem::ListDirectory(*it, "*", files);
		for (auto it_ = files.begin(); it_ != files.end(); ++it_)
		{
		   if (FileSystem::FileName(*it_) == (dirName + LIBEXT))
			{
				IPlugin *(*loadPlugin)(void);
				IPlugin *p = nullptr;
				try
				{
					this->_libM.LoadLibrary(*it_);
					this->_libM.LoadFunction(FileSystem::FileShortName(*it_), "loadPlugin");
					loadPlugin = this->_libM.GetFunction<IPlugin*(*)(void)>(FileSystem::FileShortName(*it_), "loadPlugin");
					p = loadPlugin();
				}
				catch (LoadingException &e)
				{
				   std::cerr << "Couldn't load " << FileSystem::FileName(*it_) << " (" << e.what() << ")" << std::endl;
				}
				if (p)
				{
					if (p->CheckDependencies(*it))
					{
						std::list<SceneObject *> list = p->Package();
						for (auto _it = list.begin(); _it != list.end(); ++_it)
						{
							std::cout << "Load plugin : " << (*_it)->Name() << std::endl;
							ObjectData o;
							o.id = this->_objects.size();
							o.folder = *it;
							o.sObject = *_it;
							this->_objects.insert(std::pair<std::string, ObjectData>(o.sObject->Name(), o));
						}
					}
					else
						std::cerr << "Warning : failed to load plugin!" << std::endl;
				}
			}
		}
	}
}

ObjectFactory*		PluginStorage::buildFactory(void)
{
	std::lock_guard<std::mutex> lock(_mutex);

	ObjectFactory*		factory = new ObjectFactory();
	for (auto obj : _objects)
	{
		factory->Learn(obj.first, obj.second.sObject);
		factory->Register(obj.first, obj.second.id);
	}
    return factory;
}

ResourcesPackageServer*		PluginStorage::buildPackage(const FileToSend& map, PackageType type)
{
	std::lock_guard<std::mutex> lock(_mutex);

	std::list<FileToSend>	_files;
	std::list<Animation>	_anims;
	for (auto obj : _objects)
	{
		if ((type == PackageType::EDITOR && obj.second.sObject->Type() == ObjectType::MONSTER)
			|| type == PackageType::GAME)
		{
			try
			{
				std::vector<FileToSend>	ofiles = obj.second.sObject->Files(obj.second.folder, obj.second.id);
				std::vector<Animation>	oanims = obj.second.sObject->Animations(obj.second.id);
				_files.insert(_files.end(), ofiles.begin(), ofiles.end());
				_anims.insert(_anims.end(), oanims.begin(), oanims.end());
			}
			catch (std::exception& e)
			{
				std::cout << "Exception = " << e.what() << std::endl;
			}
		}
	}
	return new ResourcesPackageServer(map, _files, _anims);
}
