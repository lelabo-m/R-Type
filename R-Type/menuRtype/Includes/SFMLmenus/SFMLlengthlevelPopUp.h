#pragma once
#include "SFMLbasePopUp.h"
#include "SFMLinputLauncher.h"
#include "SFMLbuttonLauncher.h"

class SFMLInputPopUp : public SFMLbasePopUp
{
public:
	SFMLinputLauncher		*_input;
	SFMLbuttonLauncher		*_cancel;
	SFMLbuttonLauncher		*_confirm;

public:
	SFMLInputPopUp(const std::string &, const std::string &, int);
	virtual ~SFMLInputPopUp();

private:
	void	initPopUp(const std::string &, const std::string &, int);
	void	confirmInput(SFMLshape *);
	void	Cancel(SFMLshape *);
};

