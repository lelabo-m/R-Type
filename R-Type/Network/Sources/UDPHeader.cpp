#include <stdio.h>
#include <cstring>
#include "UDPHeader.h"

UDPHeader::UDPHeader()
{
}


UDPHeader::~UDPHeader()
{
}

void UDPHeader::setMagicNumber(char magic)
{
	_magicNumber = magic;
}

void UDPHeader::setSessionID(unsigned int id)
{
	_sessionID = id;
}

void UDPHeader::setNOPE(char nope)
{
	_nope = nope;
}

void UDPHeader::setTime(int time)
{
	_time = time;
}

char UDPHeader::getMagicNumber() const
{
	return _magicNumber;
}

unsigned int UDPHeader::getSessionID() const
{
	return _sessionID;
}

char UDPHeader::getNOPE() const
{
	return _nope;
}

int UDPHeader::getTime() const
{
	return _time;
}

void UDPHeader::setFromData(char *data)
{
	this->_magicNumber = data[0];
	this->_sessionID = *((unsigned int*)(&data[1]));
	this->_nope = data[5];
	this->_time = *((int*)(&data[6]));
}

void UDPHeader::setDataFromHeader(char *data) const
{
	unsigned int shift = 0;

	std::memcpy(&data[shift], &_magicNumber, sizeof(_magicNumber));
	shift += sizeof(_magicNumber);
	std::memcpy(&data[shift], &_sessionID, sizeof(_sessionID));
	shift += sizeof(_sessionID);
	std::memcpy(&data[shift], &_nope, sizeof(_nope));
	shift += sizeof(_nope);
	std::memcpy(&data[shift], &_time, sizeof(_time));
}
