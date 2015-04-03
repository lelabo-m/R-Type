#include "SFMLlengthlevelPopUp.h"
#include "SFMLbasicText.h"
#include "SFMLbuttonevents.h"
#include "InputPopupValue.h"

SFMLInputPopUp::SFMLInputPopUp(const std::string &title, const std::string &subTitle, int lengthMax)
{
	this->_menuName = "InputPopUp";
	this->initPopUp(title, subTitle, lengthMax);
}

SFMLInputPopUp::~SFMLInputPopUp()
{
	delete 	_input;
	delete 	_confirm;
	delete 	_cancel;
}

void	SFMLInputPopUp::initPopUp(const std::string &titleTxt, const std::string &subTitleTxt, int lengthMax)
{
	SFMLbasicText	title(titleTxt, sf::Vector2<int>(520, 75), 30);
	SFMLbasicText	subtitle(subTitleTxt, sf::Vector2<int>(460, 210), 20);

	title.setPosition(sf::Vector2<float>(640 - (title.getLocalBounds().width / 2), 75));
	subtitle.setPosition(sf::Vector2<float>(640 - (subtitle.getLocalBounds().width / 2), 210));
	this->_input = new SFMLinputLauncher(sf::Vector2<int>(430, 255), lengthMax);
	this->_confirm = new SFMLbuttonLauncher(sf::Vector2<int>(430, 572), "Confirm");
	this->_cancel = new SFMLbuttonLauncher(sf::Vector2<int>(668, 572), "Cancel");

	this->_cancel->_funcRelease = std::bind(&SFMLInputPopUp::Cancel, this, _cancel);
	this->_confirm->_funcRelease = std::bind(&SFMLInputPopUp::confirmInput, this, _confirm);
	this->_input->_funcValidInput = std::bind(&SFMLInputPopUp::confirmInput, this, _confirm);

	this->_forms.push_back(_input);
	this->_forms.push_back(_confirm);
	this->_forms.push_back(_cancel);

	this->_eventManager.add(_input);
	this->_eventManager.add(_confirm);
	this->_eventManager.add(_cancel);

	this->_texts.push_back(title);
	this->_texts.push_back(subtitle);
}

void	SFMLInputPopUp::confirmInput(SFMLshape *)
{
	InputPopupValue		*inputPopup = InputPopupValue::getInst();

	inputPopup->setValue(_input->getMessage());
	this->_menuManager->goToPrevious();
}

void	SFMLInputPopUp::Cancel(SFMLshape *)
{
	InputPopupValue		*inputPopup = InputPopupValue::getInst();

	inputPopup->setValue("");
	this->_menuManager->goToPrevious();
}
