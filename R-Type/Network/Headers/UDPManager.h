#ifndef _UDPMANAGER_H_
# define _UDPMANAGER_H_

#include <condition_variable>
#include <mutex>
#include <thread>
#include "SessionManager.h"
#include "UDPSocket.h"

class UDPManager
{
public:
	UDPManager(SessionManager &, unsigned short port, bool doBind = true, bool threads = true);
	virtual ~UDPManager();
	void shutdown();
	void thread_send_unlock();

	static std::condition_variable NotifyUDPSender;


protected:
	void thread_recvLoop();
	void thread_sendLoop();
	void launchThreads();

protected:
	UDPFramePool _sendPool;
	UDPFramePool _recvPool;
	bool _running;
	unsigned short _port;
	SessionManager &_sessionManager;
	UDPSocket _ownSock;
	std::thread _recvThread;
	std::thread _sendThread;

	std::mutex _sendMutex;
};

#endif