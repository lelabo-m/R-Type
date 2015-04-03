#include <iostream>
#include "ClientManager.h"
#include "MapDownloader.h"
#include "FileSystem.hpp"
#include "SFMLbuttonevents.h"
#include "SFMLlevelscoreMenu.h"
#include "SFMLmapscoreSelectorMenu.h"
#include "SFMLsolomapSelectorMenu.h"
#include "SFMLeditor.h"
#include "SFMLplaymapSelectorMenu.h"
#include "databaseSFML.h"

SFMLbuttonevents::SFMLbuttonevents()
{}

SFMLbuttonevents::~SFMLbuttonevents()
{}

void	SFMLbuttonevents::littleButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("littleButtonHover"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(195, 165, 0, 255));
}

void	SFMLbuttonevents::littleButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("littleButton"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(169, 237, 255, 255));
}

void	SFMLbuttonevents::littleButtonClick(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("littleButtonClick"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(229, 204, 0, 255));
}

void	SFMLbuttonevents::bigButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("bigButtonHover"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(195, 165, 0, 255));
}

void	SFMLbuttonevents::bigButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("bigButton"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(169, 237, 255, 255));
}

void	SFMLbuttonevents::bigButtonClick(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbutton		*buttonPtr;

	button->_background.setTexture(db->getTexture("bigButtonClick"));

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		buttonPtr->_text.setColor(sf::Color(229, 204, 0, 255));
}

void	SFMLbuttonevents::mapselectorButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("mapselectorButtonHover"));
}

void	SFMLbuttonevents::mapselectorButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("mapselectorButton"));
}

void	SFMLbuttonevents::mapselectorButtonClick(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("mapselectorButtonClick"));
}

void	SFMLbuttonevents::submenuButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("submenuButtonHover"));
}

void	SFMLbuttonevents::submenuButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("submenuButton"));
}

void	SFMLbuttonevents::submenuButtonClick(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("submenuButtonClick"));
}

void	SFMLbuttonevents::editorButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("menueditorButtonHover"));
}

void	SFMLbuttonevents::editorButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("menueditorButton"));
}


void	SFMLbuttonevents::optionCancel(SFMLshape *button)
{
	bigButtonUnhover(button);
}

void	SFMLbuttonevents::levelScore(SFMLshape *button)
{
	SFMLbutton		*buttonPtr;

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
	{
		SFMLlevelscoreMenu levelScore(buttonPtr->_text.getString());

		mapselectorButtonUnhover(button);
		levelScore.launch();
	}
}

void	SFMLbuttonevents::submitLevel(SFMLshape *button)
{
	SFMLbutton		*buttonPtr;

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
		throw::ReturnToMenu(buttonPtr->_text.getString().toAnsiString().c_str());
}

void SFMLbuttonevents::upArrowHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("upArrowHover"));
}

void SFMLbuttonevents::upArrowUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("upArrow"));
}

void SFMLbuttonevents::downArrowHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("downArrowHover"));
}

void SFMLbuttonevents::downArrowUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("downArrow"));
}

void SFMLbuttonevents::playButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("playButtonHover"));
}

void SFMLbuttonevents::playButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("playButton"));
}

void SFMLbuttonevents::pauseButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("pauseButtonHover"));
}

void SFMLbuttonevents::pauseButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("pauseButton"));
}

void SFMLbuttonevents::stopButtonHover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("stopButtonHover"));
}

void SFMLbuttonevents::stopButtonUnhover(SFMLshape *button)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	button->_background.setTexture(db->getTexture("stopButton"));
}

void SFMLbuttonevents::monsterButtonHover(SFMLshape *sender)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sender->_background.setTexture(db->getTexture("monsterButtonHover"));
}

void SFMLbuttonevents::monsterButtonUnhover(SFMLshape *sender)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sender->_background.setTexture(db->getTexture("monsterButton"));
}
