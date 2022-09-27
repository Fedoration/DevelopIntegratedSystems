#pragma once

#include "Message.h"
#include <chrono>
using namespace chrono;


class Session
{
public:
	int id;
	string name;
	queue<Message> messages;
	system_clock::time_point lastActiveTime;

	CCriticalSection cs;
	Session(int _id, string _name)
		:id(_id), name(_name)
	{
		lastActiveTime = system_clock::now();
	}

	void add(Message& m);
	void send(CSocket& s);
	bool IsInactive();
	void UpdateLastActive();

};

