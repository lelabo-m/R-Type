#include "SFMLstartEditorPopUp.h"
#include "SFMLlengthlevelPopUp.h"
#include "SFMLbuttonevents.h"
#include "SFMLplaymapSelectorMenu.h"
#include "SFMLeditor.h"
#include "InputPopupValue.h"
#include "MapSelected.h"

SFMLstartEditorPopUp::SFMLstartEditorPopUp()
{
	this->_menuName = "StartEditorPopUp";
	this->initPopUp();
}

SFMLstartEditorPopUp::~SFMLstartEditorPopUp()
{
	delete	_emptyMap;
	delete	_listMap;
	delete	_cancel;
}

void	SFMLstartEditorPopUp::initPopUp()
{
	SFMLbasicText	title("Select a map", sf::Vector2<int>(540, 75));

	this->_cancel	= new SFMLbigButton("Cancel", sf::Vector2<int>(350, 762));
	this->_listMap	= new SFMLbuttonLauncher(sf::Vector2<int>(549, 270), "Existing map");
	this->_emptyMap = new SFMLbuttonLauncher(sf::Vector2<int>(549, 400), "Empty map");
	_cancel->_funcRelease	= std::bind(&SFMLstartEditorPopUp::Cancel, this, _cancel);
	_emptyMap->_funcRelease = std::bind(&SFMLstartEditorPopUp::editorMode, this, _emptyMap);
	_listMap->_funcRelease	= std::bind(&SFMLstartEditorPopUp::mapSelectorEditor, this, _listMap);

	this->_forms.push_back(_listMap);
	this->_forms.push_back(_emptyMap);
	this->_texts.push_back(title);

	this->_eventManager.add(_listMap);
	this->_eventManager.add(_emptyMap);

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void	SFMLstartEditorPopUp::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
	if (this->_client->CurrentMapDownload)
	{
		if (_client->CurrentMapDownload->getDownloadState() == _client->CurrentMapDownload->FINISHED)
		{
			SFMLeditor	editor(_mapEditorName);

			_menuManager->goTo("Editor");
			editor.launch();
			delete this->_client->CurrentMapDownload;
			this->_client->CurrentMapDownload = nullptr;
		}
	}
}

void	SFMLstartEditorPopUp::updateDynamicContents(const sf::Event &)
{
}


void 	SFMLstartEditorPopUp::editorMode(SFMLshape *)
{
	SFMLInputPopUp				popUp("R-Type editor", "Define the level name", 20);
	InputPopupValue				*inputPopup = InputPopupValue::getInst();

	_menuManager->goTo("InputPopUp");
	popUp.launch();

	std::string		level(inputPopup->getValue());
	if (level != "")
	{
		std::replace(level.begin(), level.end(), ';', ' ');
		std::replace(level.begin(), level.end(), ',', ' ');

		_mapEditorName = level;
		_client->askMap("");
		_client->CurrentMapDownload = new MapDownloader("");
	}
}

void SFMLstartEditorPopUp::mapSelectorEditor(SFMLshape *)
{
	SFMLplaymapSelectorMenu selectorMenu;
	MapSelected	*mapSelected = MapSelected::getInst();

	_menuManager->goTo("Mapselector");
	selectorMenu.launch();

	std::string		level(mapSelected->getMap());
	if (level != "")
	{
		level = FileSystem::FileName(level);

		_mapEditorName = level;
		_client->askMap(level + ".map");
		_client->CurrentMapDownload = new MapDownloader(level + ".map");
	}
}

void SFMLstartEditorPopUp::Cancel(SFMLshape *)
{
	_menuManager->goTo("Play");
}