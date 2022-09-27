#include "pch.h"
#include "Session.h"
using namespace chrono;

void Session::add(Message& m)
{
	CSingleLock lock(&cs, TRUE);
	messages.push(m);
}

void Session::send(CSocket& s)
{
	CSingleLock lock(&cs, TRUE);
	if (messages.empty())
	{
		Message::send(s, id, MR_BROKER, MT_NODATA);
	}
	else
	{
		messages.front().send(s);
		messages.pop();
	}
}

bool Session::IsInactive()
{
	auto currTime = system_clock::now();
	duration<double> d = currTime - lastActiveTime;
	seconds inactiveInterval = duration_cast<seconds>(d);
	if (inactiveInterval > seconds(30)) { return true; } else { return false; }

}

void Session::UpdateLastActive()
{
	CSingleLock lock(&cs, TRUE);
	lastActiveTime = system_clock::now();
}
