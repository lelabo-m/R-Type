//
// XML_Level.cpp for Source in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Common/Source
//
// Made by Alexandre Paillier
// Login   <pailli_a@epitech.net>
//
// Started on  Sun Dec 14 16:05:45 2014 Alexandre Paillier
// Last update Wed Dec 17 13:42:01 2014 Alexandre Paillier
//

#include <sstream>
#include <iomanip>
#include "XML_Level.hh"

// XML_Level

const XML_Level::Mode XML_Level::READ = const_cast<XML_Level::Mode>("rb");
const XML_Level::Mode XML_Level::READWRITE = const_cast<XML_Level::Mode>("ab+");
const XML_Level::Mode XML_Level::WRITE = const_cast<XML_Level::Mode>("w+");
const XML_Level::Mode XML_Level::NONE = const_cast<XML_Level::Mode>("");

void XML_Level::read(tinyxml2::XMLDocument &doc)
{
    this->header.read(doc);
    this->referenceIndex.read(doc);
    this->content.read(doc);
}

void XML_Level::write(tinyxml2::XMLDocument &doc)
{
    this->header.write(doc);
    this->referenceIndex.write(doc);
    this->content.write(doc);
}

bool XML_Level::readFromFile(void)
{
    tinyxml2::XMLDocument doc;

    if (this->_mode == XML_Level::NONE || !this->_file || doc.LoadFile(this->_file) != tinyxml2::XML_SUCCESS)
	return false;
    this->read(doc);
    return true;
}

bool XML_Level::readFromString(const std::string &string)
{
    tinyxml2::XMLDocument doc;

    if (doc.Parse(string.c_str()) != tinyxml2::XML_SUCCESS)
	return false;
    this->read(doc);
    return true;
}

bool XML_Level::writeToFile(void)
{
    tinyxml2::XMLDocument doc;
    bool retV;

    if (this->_mode != XML_Level::READWRITE || !this->_file)
	return false;
#ifdef _WIN32
    ::freopen_s(&this->_file, this->_filename.c_str(), XML_Level::WRITE, this->_file);
#else
    this->_file = std::freopen(this->_filename.c_str(), XML_Level::WRITE, this->_file);
#endif
    if (!this->_file)
	return false;
    this->write(doc);
    retV = (doc.SaveFile(this->_file) == tinyxml2::XML_SUCCESS) ? true : false;
#ifdef _WIN32
    ::freopen_s(&this->_file, this->_filename.c_str(), this->_mode, this->_file);
#else
    this->_file = std::freopen(this->_filename.c_str(), this->_mode, this->_file);
#endif
    return this->_file ? retV : false;
}

bool XML_Level::writeToString(std::string &string)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLPrinter stream;

    this->write(doc);
    doc.Print(&stream);
    string = stream.CStr();
    return true;
}

XML_Level::XML_Level(const char *filename, const XML_Level::Mode mode) : _file(NULL), _mode(mode)
{
    if (filename || mode != XML_Level::NONE)
    {
	this->_filename = filename + std::string(".map");
	this->header.name = filename;
	this->_file =
#ifdef _WIN32
	::_fsopen(this->_filename.c_str(), mode, _SH_DENYNO);
#else
	std::fopen(this->_filename.c_str(), mode);
#endif
    }
}

XML_Level::~XML_Level(void)
{
	if (this->_file)
		fclose(this->_file);
}

// XML_Level::IXmlNode

XML_Level::IXmlNode::~IXmlNode(void) {}

// XML_Level::Header

XML_Level::Header::Header(void) : version(NULL) {}

XML_Level::Version *XML_Level::Header::parseVersion(const tinyxml2::XMLNode *node) const
{
	std::istringstream v;
	int t[3];
	std::string s = node->ToText()->Value();

	std::memset(t, 0, sizeof(t));
	for (int i = 0, j = 0; s.at(0) >= '0' && s.at(0) <= '9'; s = s.substr(j, std::string::npos), i++)
	{
		v.str(s);
		v >> t[i];
		if (static_cast<size_t>(j = s.find('.')) == std::string::npos)
			break;
		j++;
	}
	return new XML_Level::Version(t[0], t[1], t[2]);
}

void XML_Level::Header::read(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLElement *p;
	tinyxml2::XMLElement *elem;
	tinyxml2::XMLNode *node;

	if ((p = doc.FirstChildElement(TAG_HEADER)))
	{
		if ((elem = p->FirstChildElement(TAG_BUILDER)) && (node = elem->FirstChild()))
			this->builder = node->ToText()->Value();
		if ((elem = p->FirstChildElement(TAG_BUILDTIME)) && (node = elem->FirstChild()))
			this->buildTime = static_cast<time_t>(::atoll(node->ToText()->Value()));
		if ((elem = p->FirstChildElement(TAG_VERSION)) && (node = elem->FirstChild()))
			this->version = this->parseVersion(node);
	}
}

void XML_Level::Header::getBuildData(void)
{
	char *builder_ = NULL;

#ifdef _WIN32
	size_t s = 0;

	_dupenv_s(&builder_, &s, "USERNAME");
#else
	builder_ = std::getenv("USER");
#endif
	this->builder = (builder_ && std::strlen(builder_)) ? builder_ : "(NULL)";
	this->buildTime = std::time(NULL);
}

void XML_Level::Header::write(tinyxml2::XMLDocument &doc)
{
	std::ostringstream v;

	this->getBuildData();
	tinyxml2::XMLElement *header_ = doc.NewElement(TAG_HEADER);
	tinyxml2::XMLElement *builder_ = doc.NewElement(TAG_BUILDER);
	tinyxml2::XMLElement *buildTime_ = doc.NewElement(TAG_BUILDTIME);
	tinyxml2::XMLElement *version_ = doc.NewElement(TAG_VERSION);
	builder_->SetText(this->builder.c_str());
	v << static_cast<long long int>(this->buildTime);
	buildTime_->SetText(v.str().c_str());
	v.str("");
	v << std::setfill('0') << std::setw(3) << static_cast<int>(this->version ? this->version->major : 0) << '.'
	    << std::setw(3) << static_cast<int>(this->version ? this->version->minor : 0) << '.'
	    << std::setw(3) << static_cast<int>(this->version ? this->version->revision : 0);
	version_->SetText(v.str().c_str());
	header_->InsertEndChild(builder_);
	header_->InsertEndChild(buildTime_);
	header_->InsertEndChild(version_);
	doc.InsertEndChild(header_);
}

XML_Level::Header::~Header(void)
{
	if (this->version)
		delete this->version;
}

// XML_Level::Version

XML_Level::Version::Version(unsigned char major, unsigned char minor, unsigned char revision) : major(major), minor(minor), revision(revision) {}

// XML_Level::ReferenceIndex

void XML_Level::ReferenceIndex::read(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLElement *p;
	tinyxml2::XMLElement *elem;

	if ((p = doc.FirstChildElement(TAG_REFINDEX)))
	{
		for (elem = p->FirstChildElement(TAG_REFERENCE); elem; elem = elem->NextSiblingElement(TAG_REFERENCE))
		{
			ReferenceIndex::MapRType map;
			const char *name = elem->Attribute(ATTR_REFNAME);

			if (name)
			{
			    elem->DeleteAttribute(ATTR_REFNAME);
			    for (const tinyxml2::XMLAttribute *attr = elem->FirstAttribute(); attr; attr = elem->FirstAttribute())
			    {
				map.insert(ReferenceIndex::MapRPairType(attr->Name(), attr->Value()));
				elem->DeleteAttribute(attr->Name());
			    }
			    this->c.insert(ReferenceIndex::MapPairType(name, map));
			}
		}
	}
}

void XML_Level::ReferenceIndex::write(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLElement *refIndex_ = doc.NewElement(TAG_REFINDEX);
	for (ReferenceIndex::MapType::const_iterator it = this->c.begin(); it != this->c.end(); ++it)
	{
		tinyxml2::XMLElement *ref_ = doc.NewElement(TAG_REFERENCE);

		ref_->SetAttribute(ATTR_REFNAME, it->first.c_str());
		for (ReferenceIndex::MapRType::const_iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			ref_->SetAttribute(it1->first.c_str(), it1->second.c_str());
		refIndex_->InsertEndChild(ref_);
	}
	doc.InsertEndChild(refIndex_);
}

// XML_Level::Content

void XML_Level::Content::read(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLElement *p;
	tinyxml2::XMLElement *elem;

	if ((p = doc.FirstChildElement(TAG_CONTENT)))
	{
		for (elem = p->FirstChildElement(TAG_CONTENTOBJ); elem; elem = elem->NextSiblingElement(TAG_CONTENTOBJ))
		{
			Content::MapType map;

			for (const tinyxml2::XMLAttribute *attr = elem->FirstAttribute(); attr; attr = elem->FirstAttribute())
			{
				map.insert(Content::MapPairType(attr->Name(), attr->Value()));
				elem->DeleteAttribute(attr->Name());
			}
			this->c.push_back(map);
		}
	}
}

void XML_Level::Content::write(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLElement *content_ = doc.NewElement(TAG_CONTENT);
	for (Content::ListType::const_iterator it = this->c.begin(); it != this->c.end(); ++it)
	{
		tinyxml2::XMLElement *obj_ = doc.NewElement(TAG_CONTENTOBJ);

		for (Content::MapType::const_iterator it1 = it->begin(); it1 != it->end(); ++it1)
			obj_->SetAttribute(it1->first.c_str(), it1->second.c_str());
		content_->InsertEndChild(obj_);
	}
	doc.InsertEndChild(content_);
}
