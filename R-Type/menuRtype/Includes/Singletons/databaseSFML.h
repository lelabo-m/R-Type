#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "FileSystem.hpp"

#define SEPARATE		std::string(SLASHSEP)

#ifdef _WIN32
	#define	DATABASE_PATH	std::string(".." + SEPARATE + ".." + SEPARATE + "menuRtype" + SEPARATE + "Others")
#else
	#define	DATABASE_PATH	std::string(".." + SEPARATE + "menuRtype" + SEPARATE + "Others")
#endif

class DatabaseException : public std::runtime_error
{
public:
	DatabaseException(const char *msg = "") : std::runtime_error(msg) {};
	DatabaseException(const DatabaseException& other) : std::runtime_error(other.what()) {}
	DatabaseException(const DatabaseException&& other) : std::runtime_error(other.what()) {}
	DatabaseException&	operator=(const DatabaseException&) { return *this; }
	DatabaseException&	operator=(DatabaseException&&) { return *this; }
};

class databaseSFML
{
private:
	static databaseSFML *inst;

private:
	std::map<std::string, sf::Texture>		_imgDB;
	std::map<std::string, sf::Font>			_fontDB;

private:
	const databaseSFML &operator=(const databaseSFML &) { return *this; };
	const databaseSFML &operator=(const databaseSFML &&) { return *this; };
	databaseSFML(const databaseSFML &);
	databaseSFML();
	~databaseSFML();

public:
	static databaseSFML *getDatabase();
	static void			deleteDatabase();

public:
	void	addTexture(const std::string &, const sf::Texture &);
	void	addFont(const std::string &, const sf::Font &);
	const	sf::Texture			&getTexture(const std::string &) const;
	const	sf::Font			&getFont(const std::string &) const;
};