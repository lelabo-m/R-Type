#include "UDPManager.h"

std::condition_variable UDPManager::NotifyUDPSender;

UDPManager::UDPManager(SessionManager &session, unsigned short port, bool doBind, bool threads) :
_running(true),
_port(port),
_sessionManager(session),
_ownSock(port, doBind)
{
	if (threads)
		launchThreads();
}

void UDPManager::launchThreads()
{
	_recvThread = std::thread(&UDPManager::thread_recvLoop, this);
	_sendThread = std::thread(&UDPManager::thread_sendLoop, this);
}

UDPManager::~UDPManager()
{
}

void UDPManager::shutdown()
{
	_ownSock.shutdown_udp();
	_running = false;
	this->NotifyUDPSender.notify_one();
	if (_recvThread.joinable())
		_recvThread.join();
	if (_sendThread.joinable())
		_sendThread.join();
}

void UDPManager::thread_recvLoop()
{
	while (_running)
	{
		UDPFrame *new_frame = _recvPool.createUDPFrame();

		try
		{
			_ownSock.recv(new_frame->getData(), UDP_HEADER_LENGTH + UDP_FRAME_CONTENT_LENGTH);

			new_frame->getHeader().setFromData(new_frame->getData());
			Session *ses = _sessionManager.getSessionPtr(new_frame->getHeader().getSessionID());
			if (ses)
			{
				if (ses->UDPReceiving.try_lock())
				{
					ses->pushUDPReceivedFrame(new_frame);
					ses->UDPReceiving.unlock();
				}
			}
		}
		catch (std::runtime_error &e)
		{
			(void)e;
		}
	}
}

void UDPManager::thread_sendLoop()
{
	UDPFrame *frame;
	Session *ses;

	std::unique_lock<std::mutex> lock(_sendMutex);
	while (_running)
	{
		if (_sessionManager.UDPSlotEmpty() == false)
		{
			ses = _sessionManager.getSessionPtr(_sessionManager.getNextNotifiedSessionUDP());
			if (ses)
			{
				if (ses->UDPSending.try_lock())
				{
					try
					{
						frame = ses->getUDPToSendFrame();
						_ownSock.send(ses->getNetUser()->getSockaddr(),
							frame->getData(), UDP_HEADER_LENGTH + UDP_FRAME_CONTENT_LENGTH);
						frame->release();
						ses->UDPSending.unlock();
					}
					catch (std::runtime_error &e)
					{
						(void)e;
						ses->UDPSending.unlock();
					}

				}
			}
		}
		else
		{
			NotifyUDPSender.wait(lock);
		}
	}
}

void UDPManager::thread_send_unlock()
{
	NotifyUDPSender.notify_one();
}
