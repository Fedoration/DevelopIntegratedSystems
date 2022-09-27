#include "pch.h"
#include "Client.h"
#include <cstdlib>
#include "../LaputinCppServer/Message.cpp"
#include "MyUtils.h"

void Client::ProcessMessages()
{
	while (true)
	{
		Message m = Message::send(MR_BROKER, MT_GETDATA);
		switch (m.header.type)
		{
		case MT_DATA:
			cout << "User [" << m.header.from << "]: " << m.data << endl;
			break;
		case MT_GETUSERS:
			if (m.data.size() == 0)
				cout << "So sad, there's no one here\n";
			else
				cout << "Actual users :\n" << m.data << endl;
			break;
		default:
			Sleep(2000);
			break;
		}
	}
}

void Client::PrintCLIMenu()
{
	cout << "\n1. Send message to all\n" <<
		"2. Send direct message\n" <<
		"3. Show all actual users\n" <<
		"0. Leave \n" <<
		"----------------\n";
}

int Client::ClientRunner()
{
	while (true)
	{
		PrintCLIMenu();
		int choice = GetCorrectNumber("Ur choice: ", 0, 3);
		switch (choice)
		{
		case 1:
		{
			cout << "Type ur message: ";
			string str;
			cin >> ws;
			getline(cin, str);
			Message::send(MR_ALL, MT_DATA, str);
			break;
		}
		case 2:
		{
			int id = GetCorrectNumber("Input user's id: ", 101, 10000);

			cout << "Type ur message: ";
			string str;
			cin >> ws;
			getline(cin, str);
			Message::send(id, MT_DATA, str);
			break;
		}
		case 3:
		{
			Message m = Message::send(MR_BROKER, MT_GETUSERS);
			break;
		}
		case 0:
		{
			Message::send(MR_BROKER, MT_EXIT);
			return 0;
			break;
		}
		default:
		{
			cout << "Incorrect number" << endl;
		}
		}
	}
}

Client::Client()
{
	AfxSocketInit();
	thread t(&Client::ProcessMessages, this);
	t.detach();

	Message m = Message::send(MR_BROKER, MT_INIT);
	this->id = m.clientID;

	int state = ClientRunner();
}