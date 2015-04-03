//
// XML_Level.hh for Header in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Common/Header
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Sun Dec 14 16:05:53 2014 Alexandre Paillier
// Last update Wed Dec 17 13:40:52 2014 Alexandre Paillier
//

#ifndef XML_LEVEL_HH_
# define XML_LEVEL_HH_

# include <string>
# include <ctime>
# include <map>
# include <list>
# include <cstdio>
# include "tinyxml2.h"

# ifndef _WIN32 // Thank you g++ ...
#  undef major
#  undef minor
# endif

// XML TAGS / ATTRIBUTES

# define TAG_HEADER "header"
# define TAG_BUILDER "builder"
# define TAG_BUILDTIME "buildTime"
# define TAG_VERSION "version"

# define TAG_REFINDEX "referenceIndex"
# define TAG_REFERENCE "reference"
# define ATTR_REFNAME "name"

# define TAG_CONTENT "content"
# define TAG_CONTENTOBJ "obj"

//

class XML_Level
{
public:
	struct Version; // forward declaration
private:
	// Inner types

	class IXmlNode
	{
	protected:
		virtual void read(tinyxml2::XMLDocument &doc) = 0;
		virtual void write(tinyxml2::XMLDocument &doc) = 0;
	public:
		virtual ~IXmlNode(void);
	};
	typedef char* Mode;
	// Properties

	FILE *_file;
	const Mode _mode;
	std::string _filename;
	static const Mode WRITE;
	void read(tinyxml2::XMLDocument &doc);
	void write(tinyxml2::XMLDocument &doc);
public:
	// Inner types

	class Header : IXmlNode
	{
	private:
		friend class XML_Level;
		void getBuildData(void);
		XML_Level::Version *parseVersion(const tinyxml2::XMLNode *node) const;
		virtual void read(tinyxml2::XMLDocument &doc);
		virtual void write(tinyxml2::XMLDocument &doc);
	public:
		XML_Level::Version *version;
		std::string name;
		std::string builder;
		std::time_t buildTime;
		Header(void);
		virtual ~Header(void);
	};
	class ReferenceIndex : IXmlNode
	{
	private:
		friend class XML_Level;
		virtual void read(tinyxml2::XMLDocument &doc);
		virtual void write(tinyxml2::XMLDocument &doc);
	public:
		typedef std::string MapLType;
		typedef std::string MapRLType;
		typedef std::string MapRRType;
		typedef std::map<MapRLType, MapRRType> MapRType;
		typedef std::pair<MapRLType, MapRRType> MapRPairType;
		typedef std::map<MapLType, MapRType> MapType;
		typedef std::pair<MapLType, MapRType> MapPairType;
		MapType c;
	};
	class Content : IXmlNode
	{
	private:
		friend class XML_Level;
		virtual void read(tinyxml2::XMLDocument &doc);
		virtual void write(tinyxml2::XMLDocument &doc);
	public:
		typedef std::string MapLType;
		typedef std::string MapRType;
		typedef std::map<MapLType, MapRType> MapType;
		typedef std::pair<MapLType, MapRType> MapPairType;
		typedef std::list<MapType> ListType;
		ListType c;
	};
	struct Version
	{
	public:
		const unsigned char major;
		const unsigned char minor;
		const unsigned char revision;
		Version(unsigned char major, unsigned char minor, unsigned char revision);
	};
	// Properties

	static const Mode READ;
	static const Mode READWRITE;
	static const Mode NONE;
	Header header;
	ReferenceIndex referenceIndex;
	Content content;
	// Methods

	explicit XML_Level(const char *filename, const Mode mode);
	bool readFromFile(void);
	bool readFromString(const std::string &string);
	bool writeToFile(void);
	bool writeToString(std::string &string);
	~XML_Level(void);
};

#endif // !XML_Level_HH_
