#include <iostream>
#include "TCPMessageFactory.h"

TCPMessageFactory *TCPMessageFactory::_instance = 0;

TCPMessageFactory::TCPMessageFactory()
{
}

int TCPMessageFactory::SendMsg(std::shared_ptr<Session> session, const std::string &msg, int procedureCode)
{
	TCPFrame *frame = new TCPFrame();
	char *data = new char[TCP_HEADER_LENGTH + (msg.size() + 1)];
	frame->getHeader().setDataLength(msg.size() + 1);
	frame->getHeader().setProcedureCode(procedureCode);
	frame->getHeader().setMsgID(this->_currentID);

	this->incrementID();

	frame->setData(data);
	frame->getHeader().setDataFromHeader(frame->getData());
	std::memcpy(&data[TCP_HEADER_LENGTH], msg.c_str(), msg.size() + 1);

	int id = frame->getHeader().getMsgID();
	session.get()->pushTCPToSendFrame(frame);
	return id;
}

int TCPMessageFactory::SendFile(std::shared_ptr<Session> session, const std::string &file_path, const std::string &remote_path, int proc)
{
	std::string *file = nullptr;
	try
	{
		file = FileManager::readFile(file_path);
	}
	catch (std::exception &)
	{
		std::cerr << "File not found!" << std::endl;
		return 0;
	}
	if (!file)
		return 0;

	file->insert(0, remote_path + '\0');

	TCPFrame *frame = new TCPFrame();
	char *header = new char[TCP_HEADER_LENGTH];
	frame->getHeader().setMsgID(_currentID);
	frame->getHeader().setDataLength(file->size());
	frame->getHeader().setProcedureCode(proc);
	frame->getHeader().setDataFromHeader(header);
	file->insert(0, header, TCP_HEADER_LENGTH);
	delete []header;

	char *entire_frame = new char[TCP_HEADER_LENGTH + file->size()];
	file->copy(entire_frame, TCP_HEADER_LENGTH + file->size());
	delete file;

	frame->setData(entire_frame);

	int id = frame->getHeader().getMsgID();
	session.get()->pushTCPToSendFrame(frame);

	this->incrementID();
	return id;
}