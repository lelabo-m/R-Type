#include <stdio.h>
#include <cstring>
#include "TCPHeader.h"

TCPHeader::TCPHeader()
{
}

TCPHeader::~TCPHeader()
{
}

int TCPHeader::getMsgID() const
{
	return _msgID;
}

int TCPHeader::getProcedureCode() const
{
	return _procedureCode;
}

unsigned int TCPHeader::getDataLength() const
{
	return _dataLength;
}

void TCPHeader::setMsgID(int msgID)
{
	_msgID = msgID;
}

void TCPHeader::setProcedureCode(int procedureCode)
{
	_procedureCode = procedureCode;
}

void TCPHeader::setDataLength(unsigned int dataLength)
{
	_dataLength = dataLength;
}

void TCPHeader::setFromData(char *data)
{
	this->_msgID = *((int*)(&data[0]));
	this->_procedureCode = *((int*)(&data[4]));
	this->_dataLength = *((unsigned int*)(&data[8]));
}

void TCPHeader::setDataFromHeader(char *header) const
{
	unsigned int i = 0;

	std::memcpy(&header[i], &_msgID, sizeof(_msgID));
	i += sizeof(_msgID);
	std::memcpy(&header[i], &_procedureCode, sizeof(_procedureCode));
	i += sizeof(_procedureCode);
	std::memcpy(&header[i], &_dataLength, sizeof(_dataLength));
}