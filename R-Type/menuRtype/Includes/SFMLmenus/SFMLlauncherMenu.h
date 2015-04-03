#pragma once
#include "SFMLbasePopUp.h"
#include "SFMLinputLauncher.h"
#include "SFMLbuttonLauncher.h"

class SFMLlauncherMenu :
	public SFMLbasePopUp
{
private:
	SFMLinputLauncher	*_ip;
	SFMLbuttonLauncher	*_connect;
	SFMLbuttonLauncher	*_cancel;
	bool				_isConnected;

public:
	SFMLlauncherMenu();
	virtual ~SFMLlauncherMenu();

private:
	void			initBaseLauncher();
	void			initLauncher();
	void			connect(SFMLshape *);
	virtual void	executeRequests();
	virtual void	cancel(SFMLshape *);
};

