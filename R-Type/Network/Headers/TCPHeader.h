#ifndef _TCPHEADER_H_
# define _TCPHEADER_H_

#define TCP_HEADER_LENGTH 12

class TCPHeader
{
public:
	TCPHeader();
	~TCPHeader();

	int getMsgID() const;
	int getProcedureCode() const;
	unsigned int getDataLength() const;

	void setMsgID(int);
	void setProcedureCode(int);
	void setDataLength(unsigned int);

	void setFromData(char *);
	void setDataFromHeader(char *) const;

private:
	int _msgID;
	int _procedureCode;
	unsigned int _dataLength;
};

#endif