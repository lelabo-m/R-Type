// DEBUG
#include <iostream>

#include <cstring>
#include <chrono>
#include <ctime>
#include "TCPManager.h"
#include "NetClientManager.h"
#include "NetServerManager.h"

TCPManager::TCPManager(SessionManager &sessionManager, unsigned short port) :
_port(port),
_ownSock(port),
_sessionManager(sessionManager)
{
}

TCPManager::~TCPManager()
{
}

void TCPManager::connect(const std::string &ip)
{
	if (_ownSock.connectTo(ip) == false)
		throw std::runtime_error("Failed to connect");
	_type = CLIENT;
	_running = true;
	_tcpThread = std::thread(&TCPManager::thread_selectLoop, this);
}

void TCPManager::host()
{
	try
	{
		_ownSock.host();
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
	_type = SERVER;
	_running = true;
	_tcpThread = std::thread(&TCPManager::thread_selectLoop, this);
}

void TCPManager::shutdown()
{
	if (_running == false)
		return;
	_running = false;
	_tcpThread.join();
}

void TCPManager::thread_selectLoop()
{
	struct timeval tv;
	int ret = 0;
	this->setFS();

	while (_running)
	{
		tv.tv_usec = 0;
		tv.tv_sec = 5;
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		try
		{
			while ((ret = select(_maxSock + 1, &this->_rdfs, &this->_wrfs, NULL, &tv)) > 0)
			{
				if (FD_ISSET(this->_ownSock.getSocket(), &this->_rdfs))
					ownReadNotified();
				else
					otherNotification();

				this->setFS();

				if (clock(tv, start))
					break;

			}
		}
		catch (std::runtime_error &e)
		{
			(void)e;
			return;
		}
		_sessionManager.eraseDisconnectedList();
		this->refreshSessions();
		this->setFS();
	}
	_ownSock.shutdown();
}

bool TCPManager::clock(struct timeval &tv, std::chrono::high_resolution_clock::time_point &start)
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	tv.tv_usec -= (static_cast<long>(time) / 10000);
	if (tv.tv_usec < 0)
	{
		int a = tv.tv_usec * -1;
		int shift = a / 1000000;
		if ((double)a / 1000000.0f >(double)shift)
			shift++;
		tv.tv_usec = 100000 - (a % 100000);
		tv.tv_sec -= shift;
		if (tv.tv_sec < 0)
		{
			return true;
		}
	}

	start = std::chrono::high_resolution_clock::now();
	if (time < CYCLE_TIME)
		std::this_thread::sleep_for(std::chrono::nanoseconds(CYCLE_TIME - time));
	return false;
}

void TCPManager::setFS()
{
	FD_ZERO(&this->_rdfs);
	FD_ZERO(&this->_wrfs);

	FD_SET(this->_ownSock.getSocket(), &this->_rdfs);
	FD_SET(this->_ownSock.getSocket(), &this->_wrfs);

	_maxSock = this->_ownSock.getSocket();
	if (_type == SERVER)
	{
		for (auto it = _sockets.begin(); it != _sockets.end(); it++)
		{
			if ((int)(*it).get()->getSocket() > _maxSock)
				_maxSock = (*it).get()->getSocket();
			FD_SET((*it).get()->getSocket(), &this->_rdfs);
			FD_SET((*it).get()->getSocket(), &this->_wrfs);
		}
	}
}

void TCPManager::refreshSessions()
{
	_sockets = _sessionManager.getAllSession();
}