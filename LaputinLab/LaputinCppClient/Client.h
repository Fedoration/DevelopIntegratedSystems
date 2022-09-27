#pragma once

class Client
{
private:
	int id;
	void ProcessMessages();
	void PrintCLIMenu();
	int ClientRunner();

public:
	Client();
	~Client() {};

};

