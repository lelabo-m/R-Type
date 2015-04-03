// DEBUG
#include <iostream>

#include <cstring>
#include "Values.h"
#include "TCPManagerServer.h"

TCPManagerServer::TCPManagerServer(SessionManager &session, unsigned short port) : TCPManager(session, port)
{
}

TCPManagerServer::~TCPManagerServer()
{
}

void TCPManagerServer::otherNotification()
{
	for (auto it = _sockets.begin(); it != _sockets.end(); it++)
	{
		Session *session = (*it).get();

		if (FD_ISSET(session->getSocket(), &this->_rdfs))
		{
			TCPFrame *frame = new TCPFrame();
			try
			{
				char tcp_header[TCP_HEADER_LENGTH];

				_ownSock.recvTCP(session->getSocket(), tcp_header, TCP_HEADER_LENGTH);
				frame->getHeader().setFromData(tcp_header);

				char *full_frame = new char[TCP_HEADER_LENGTH + frame->getHeader().getDataLength()];
				std::memcpy(full_frame, tcp_header, TCP_HEADER_LENGTH);
				_ownSock.recvTCP(session->getSocket(), &full_frame[TCP_HEADER_LENGTH], frame->getHeader().getDataLength());
				frame->setData(full_frame);
				session->pushTCPReceivedFrame(frame);

				// DEBUG
				std::cout << "MSG_ID : " <<
					frame->getHeader().getMsgID()
					<< " DATA_LENGTH: " <<
					frame->getHeader().getDataLength()
					<< " PROCEDURE_CODE: " <<
					frame->getHeader().getProcedureCode()
					<< std::endl;
			}
			catch (std::exception &e)
			{
				(void)e;
				session->setSessionState(Session::DISCONNECTED);
				// DEBUG
				std::cout << "SESSION_DISCONNECTED" << std::endl;
				this->refreshSessions();
				delete frame;
				return;
			}
			return;
		}
		if (FD_ISSET(session->getSocket(), &this->_wrfs))
		{
			TCPFrame *frame;

			if ((frame = session->getTCPToSendFrame()) != NULL)
			{
				try
				{
					_ownSock.sendTCP(session->getSocket(), frame->getData(), frame->getHeader().getDataLength() + TCP_HEADER_LENGTH);
					session->notifyTCPSent(frame->getHeader().getMsgID());
					delete frame;
				}
				catch (std::exception &e)
				{
					(void)e;
					session->setSessionState(Session::DISCONNECTED);
					delete frame;
					this->refreshSessions();
					return;
				}
			}
		}
	}
}

void TCPManagerServer::ownReadNotified()
{
	try
	{
		NetUser *new_user = _ownSock.acceptClient();
		new_user->getSockaddr()->sin_port = htons(_port);
		_sessionManager.createNewSession(new_user);
		unsigned int new_id = Session::IDPool;
		sendSessionID(new_id);
		this->refreshSessions();
	}
	catch (std::runtime_error &e)
	{
		(void)e;
	}
}

void TCPManagerServer::sendSessionID(unsigned int id)
{
	TCPFrame *frame = new TCPFrame();

	frame->getHeader().setDataLength(sizeof(unsigned int));
	frame->getHeader().setMsgID(0);
	frame->getHeader().setProcedureCode(Protocole::Server::ACCEPT);

	char *data = new char[TCP_HEADER_LENGTH + sizeof(unsigned int)];
	frame->getHeader().setDataFromHeader(data);
	std::memcpy(&data[TCP_HEADER_LENGTH], &id, sizeof(unsigned int));
	frame->setData(data);
	_sessionManager.getSession(id).get()->pushTCPToSendFrame(frame);
}
