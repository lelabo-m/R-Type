#ifndef _TCPFRAME_H_
# define _TCPFRAME_H_

#include "TCPHeader.h"

class TCPFrame
{
public:
	TCPFrame();
	~TCPFrame();
	char *getData();
	const char *getMessage();
	void setData(char *data);
	void setFrameFromBuffer(char*);
	TCPHeader &getHeader();
private:
	TCPHeader _header;
	char *_data;
};

#endif