#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <list>
#include <string>
#include "GameObject.h"

class IPlugin
{
public:
	virtual ~IPlugin() {};
	virtual std::list<SceneObject*>	Package() const = 0;
	virtual bool					CheckDependencies(const std::string&) const = 0;
};

#endif /* !PLUGIN_H_ */