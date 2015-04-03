//
// Scores.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Fri Jan  2 15:09:10 2015 Clément Roche
// Last update Mon Jan  5 16:13:19 2015 Clément Roche
//

#include <iostream>
#include <exception>
#include "Scores.hh"

Scores::Scores(void) : _doc(0)
{
}

Scores::~Scores(void)
{
   if (_doc)
   {
      _doc->Clear();
      delete _doc;
   }
}

void	Scores::parse(void)
{
   if (!_doc)
      return ;
   tinyxml2::XMLHandle h(_doc);
   tinyxml2::XMLElement *m = 0;

   m = h.FirstChildElement("Map").ToElement();
   if (m)
   {
      tinyxml2::XMLElement *e = 0;
      while (m)
      {
	 e = m->FirstChildElement("Score");
	 if (e && m->Attribute("Name"))
	 {
	    Scores::scoremap scores;
	    while (e)
	    {
	       if (e->Attribute("Name"))
	       {
		  scores[e->Attribute("Name")] = e->IntAttribute("Value");
	       }
	       if (e->NextSibling())
		  e = e->NextSibling()->ToElement();
	       else
		  e = 0;
	    }
	    _scores[m->Attribute("Name")] = scores;
	 }
	 (m->NextSibling()) ? (m = m->NextSibling()->ToElement()) : (m = 0);
      }
   }
   else
      std::cerr << "[SCORES] Invalid score file !" << std::endl;
}

bool	Scores::load(std::string &filename)
{
   if (_doc)
      delete _doc;
   _doc = new tinyxml2::XMLDocument;
   if (!_doc || _doc->LoadFile(filename.c_str()))
      return false;
   parse();
   return true;
}

void	Scores::save(std::string &filename)
{
   if (!_doc)
      _doc = new tinyxml2::XMLDocument;
   else
      _doc->Clear();

   tinyxml2::XMLDeclaration *decl = _doc->NewDeclaration(0);
   _doc->LinkEndChild(decl);
   for (auto mit = _scores.begin(); mit != _scores.end(); ++mit)
   {
      tinyxml2::XMLElement *em = _doc->NewElement("Map");
      em->SetAttribute("Name", mit->first.c_str());
      for (auto it = mit->second.begin(); it != mit->second.end(); ++it)
      {
	 tinyxml2::XMLElement *e = _doc->NewElement("Score");
	 e->SetAttribute("Name", it->first.c_str());
	 e->SetAttribute("Value", std::to_string(it->second).c_str());
	 em->LinkEndChild(e);
      }
      _doc->LinkEndChild(em);
   }

   _doc->SaveFile(filename.c_str());
   std::cout << "[SCORES] saved " << filename << std::endl;
}

const std::map<std::string, int>	&Scores::getScore(const std::string mapName)
{
   try
   {
      _scores.at(mapName);
   }
   catch (std::exception &)
   {
      throw std::runtime_error("No score for this map !");
   }
   return (_scores.at(mapName));
}

void	Scores::setScore(const std::string mapName, const std::string player, int score)
{
   _scores[mapName][player] = score;
}
