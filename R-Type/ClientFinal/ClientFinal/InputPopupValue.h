#ifndef	_INPUTPOPUPVALUE_H
#define	_INPUTPOPUPVALUE_H

#include <string>

class InputPopupValue
{
private:
	std::string				_valueInput;
	static InputPopupValue	*inst;

private:
	const InputPopupValue &operator=(const InputPopupValue &) { return *this; };
	const InputPopupValue &operator=(const InputPopupValue &&) { return *this; };
	InputPopupValue();
	~InputPopupValue();

public:
	static InputPopupValue	*getInst();
	static void				deleteInst();
	const std::string 		&getValue() const;
	void					setValue(const std::string &);
};

#endif