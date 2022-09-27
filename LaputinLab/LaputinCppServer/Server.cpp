#include "pch.h"
#include "Server.h"


void Server::ProcessClient(SOCKET hSock)
{
	CSocket s;
	s.Attach(hSock);
	Message m;
	int code = m.receive(s);
	cout << "to: " << m.header.to << " from: " << m.header.from << " type: " << m.header.type << " code: " << code << endl;
	switch (code)
	{
	case MT_INIT:
	{
		auto session = make_shared<Session>(++maxID, m.data);
		sessions[session->id] = session;
		Message::send(s, session->id, MR_BROKER, MT_INIT);
		cout << "User [" << session->id << "] has joined" << endl;
		break;
	}
	case MT_EXIT:
	{
		sessions.erase(m.header.from);
		Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
		cout << "User [" << m.header.from << "] has left" << endl;
		break;
	}
	case MT_GETDATA:
	{
		auto iSession = sessions.find(m.header.from);
		if (iSession != sessions.end())
		{
			iSession->second->send(s);
			iSession->second->UpdateLastActive();
		}
		break;
	}
	case MT_GETUSERS:
	{
		string users = "";
		auto iSession = sessions.find(m.header.from);
		if (iSession != sessions.end())
		{
			for (auto& [id, session] : sessions)
			{
				if (id != m.header.from)
				{
					users.append(to_string(session->id));
					users.append("\n");
				}
			}
			Message ms = Message::send(m.header.from, MT_GETUSERS, users);
			iSession->second->add(ms);
		}
		break;
	}
	default:
	{
		auto iSessionFrom = sessions.find(m.header.from);
		if (iSessionFrom != sessions.end())
		{
			iSessionFrom->second->UpdateLastActive();
			auto iSessionTo = sessions.find(m.header.to);
			if (iSessionTo != sessions.end())
			{
				iSessionTo->second->add(m);
			}
			else if (m.header.to == MR_ALL)
			{
				for (auto& [id, session] : sessions)
				{
					if (id != m.header.from)
						session->add(m);
				}
			}
		}
		break;
	}
	}
}

void Server::CheckActivity()
{
	while (true)
	{
		for (auto& [id, session] : sessions)
		{
			if (session->IsInactive())
			{
				cout << "User [" << id << "] has been inactive for a long period" << endl;
				sessions.erase(id);
				break;
			}
		}
		Sleep(1000);
	}
}

Server::Server()
{
	thread checkClientsConnection(&Server::CheckActivity, this);
	checkClientsConnection.detach();

	AfxSocketInit();
	CSocket server;
	server.Create(12345);
	while (true)
	{
		if (!(server.Listen()))
			break;
		CSocket s;
		server.Accept(s);
		thread t(&Server::ProcessClient, this, s.Detach());
		t.detach();
	}
}