#include <cstring>
#include "TCPManagerClient.h"
#include "Values.h"
#include "NetClientManager.h"

TCPManagerClient::TCPManagerClient(SessionManager &session, unsigned short port, NetClientManager &clientManager) : TCPManager(session, port), _clientManager(clientManager)
{
}

TCPManagerClient::~TCPManagerClient()
{
}

void TCPManagerClient::otherNotification()
{
	if (FD_ISSET(this->_ownSock.getSocket(), &this->_wrfs) && _clientManager.getOwnSessionID() != 0)
	{
		try
		{
			TCPFrame *frame;
			frame = _sessionManager.getSession(_clientManager.getOwnSessionID())->getTCPToSendFrame();
			if (frame)
			{
				_ownSock.sendToServer(frame->getData(), frame->getHeader().getDataLength() + TCP_HEADER_LENGTH);
				_clientManager.getOwnSession()->notifyTCPSent(frame->getHeader().getMsgID());
				delete frame;
			}
		}
		catch (std::runtime_error &e)
		{
			_clientManager.setConnectionState(NetClientManager::DISCONNECTED);
			throw e;
		}
	}
}

void TCPManagerClient::ownReadNotified()
{
	try
	{
		if (_clientManager.getOwnSessionID() != 0)
		{
			char tcp_header[TCP_HEADER_LENGTH];
			TCPFrame *frame = new TCPFrame();

			_ownSock.recvFromServer(tcp_header, TCP_HEADER_LENGTH);
			frame->getHeader().setFromData(tcp_header);

			char *full_frame = new char[TCP_HEADER_LENGTH + frame->getHeader().getDataLength()];
			std::memcpy(full_frame, tcp_header, TCP_HEADER_LENGTH);
			_ownSock.recvFromServer(&full_frame[TCP_HEADER_LENGTH], frame->getHeader().getDataLength());
			frame->setData(full_frame);
			_sessionManager.getSession(_clientManager.getOwnSessionID()).get()->pushTCPReceivedFrame(frame);
		}
		else
		{
			char tcp_header[TCP_HEADER_LENGTH];
			TCPFrame *frame = new TCPFrame();

			_ownSock.recvFromServer(tcp_header, TCP_HEADER_LENGTH);
			frame->getHeader().setFromData(tcp_header);

			char *full_frame = new char[TCP_HEADER_LENGTH + frame->getHeader().getDataLength()];
			std::memcpy(full_frame, tcp_header, TCP_HEADER_LENGTH);
			_ownSock.recvFromServer(&full_frame[TCP_HEADER_LENGTH], frame->getHeader().getDataLength());
			frame->setData(full_frame);
			if (frame->getHeader().getProcedureCode() == Protocole::Server::ACCEPT)
			{
				unsigned int sessionID = *(int*)(&frame->getData()[TCP_HEADER_LENGTH]);
				_sessionManager.createNewSession(sessionID);
				_clientManager.setOwnSessionID(sessionID);
				_clientManager.setConnectionState(NetClientManager::CONNECTED);
				*_clientManager.getOwnSession().get()->getNetUser()->getSockaddr() = _ownSock.getSockaddrIn();
			}
			else
			{
				throw std::runtime_error("Server refused");
			}
			delete frame;
		}
	}
	catch (std::runtime_error &e)
	{
		_clientManager.setConnectionState(NetClientManager::DISCONNECTED);
		throw e;
	}
}
