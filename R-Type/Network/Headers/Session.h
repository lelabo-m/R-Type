#ifndef _SESSION_H_
# define _SESSION_H_

#include <queue>
#include <memory>
#include <mutex>
#include <set>

#include "NetUser.h"
#include "TCPFrame.h"
#include "UDPFrame.h"

class SessionManager;

class Session
{
public:
	enum ConnectionState
	{
		CONNECTED,
		DISCONNECTED
	};

	static unsigned int IDPool;

public:
	Session(SessionManager &manager, NetUser *);
	Session(SessionManager &manager, unsigned int id);
	~Session();

#ifdef _WIN32
	SOCKET getSocket();
#else
	int getSocket();
#endif

	std::string getIP() const;
	TCPFrame *getTCPReceivedFrame();
	void pushTCPReceivedFrame(TCPFrame*);
	unsigned int countTCPReceivedFrame() const;
	void clearTCPReceivedFrame();

	TCPFrame *getTCPToSendFrame();
	void pushTCPToSendFrame(TCPFrame*);
	unsigned int countTCPToSendFrame() const;
	void clearTCPToSendFrame();

	UDPFrame *getUDPReceivedFrame();
	void pushUDPReceivedFrame(UDPFrame*);
	unsigned int countUDPReceivedFrame() const;
	void clearUDPReceivedFrame();

	UDPFrame *getUDPToSendFrame();
	void pushUDPToSendFrame(UDPFrame*);
	unsigned int countUDPToSendFrame() const;
	void clearUDPToSendFrame();

	ConnectionState getSessionState() const;
	void setSessionState(ConnectionState);

	unsigned int getSessionID() const;

	NetUser *getNetUser();

	void notifyTCPSent(int);
	bool isTCPMessageSent(int);

public:
	std::mutex UDPSending;
	std::mutex UDPReceiving;

protected:
	unsigned int _sessionID;
	NetUser *_netUser;

	std::queue<TCPFrame*> _receivedTCPQueue;
	std::queue<TCPFrame*> _toSendTCPQueue;
	std::set<int> _tcpSent;
	std::mutex _tcpMutex;

	std::queue<UDPFrame*> _receivedUDPQueue;
	std::queue<UDPFrame*> _toSendUDPQueue;

	ConnectionState _sessionState;

	std::mutex _receivedTCPQueue_mutex;
	std::mutex _toSendTCPQueue_mutex;
	std::mutex _receivedUDPQueue_mutex;
	std::mutex _toSendUDPQueue_mutex;

	SessionManager &_manager;
};

#endif
