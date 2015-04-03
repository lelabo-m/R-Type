#include "InputPopupValue.h"

InputPopupValue *InputPopupValue::inst = NULL;

InputPopupValue::InputPopupValue()
{
}

InputPopupValue::~InputPopupValue()
{
}

InputPopupValue *InputPopupValue::getInst()
{
	if (inst == NULL)
		inst = new InputPopupValue();
	return inst;
}

void InputPopupValue::deleteInst()
{
	if (inst)
	{
		delete inst;
	}
	inst = NULL;
}

const std::string 		&InputPopupValue::getValue() const
{
	return this->_valueInput;
}

void					InputPopupValue::setValue(const std::string &name)
{
	this->_valueInput = name;
}