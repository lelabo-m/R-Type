#ifndef _UDPHEADER_H_
# define _UDPHEADER_H_

# define UDP_HEADER_LENGTH 10

class UDPHeader
{
public:
	UDPHeader();
	~UDPHeader();

	void setMagicNumber(char magic);
	void setSessionID(unsigned int id);
	void setNOPE(char nope);
	void setTime(int time);

	char getMagicNumber() const;
	unsigned int getSessionID() const;
	char getNOPE() const;
	int getTime() const;

	void setFromData(char *);
	void setDataFromHeader(char *) const;

protected:
	char _magicNumber;
	unsigned int _sessionID;
	char _nope;
	int _time;
};

#endif