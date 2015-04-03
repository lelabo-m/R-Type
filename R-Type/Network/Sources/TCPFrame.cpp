#include <cstring>
#include "TCPFrame.h"


TCPFrame::TCPFrame()
{
	_data = NULL;
}

TCPFrame::~TCPFrame()
{
	if (_data)
		delete[] _data;
}

char *TCPFrame::getData()
{
	return _data;
}

const char *TCPFrame::getMessage()
{
	return &_data[TCP_HEADER_LENGTH];
}

void TCPFrame::setData(char *data)
{
	_data = data;
}

void TCPFrame::setFrameFromBuffer(char *data)
{
	_header.setFromData(data);
	_data = new char[_header.getDataLength() + TCP_HEADER_LENGTH];
	_header.setDataFromHeader(_data);
	std::memcpy(&_data[TCP_HEADER_LENGTH], data, _header.getDataLength());
}

TCPHeader &TCPFrame::getHeader()
{
	return _header;
}
