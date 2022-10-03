import threading
from dataclasses import dataclass
import socket, struct, time
from msg import *


def process_messages():
	while True:
		m = Message.SendMessage(MR_BROKER, MT_GETDATA)
		if m.Header.Type == MT_DATA:
			print(f"User[{m.Header.From}]: {m.Data}")
		
		elif m.Header.Type == MT_GETUSERS:
		
			if m.Header.Size == 0 :
				print("So sad, there's no one here\n")
			else:
				print("Actual users:\n", m.Data)
		
		else:
			time.sleep(2)


def print_cli_menu():
	print(
"""
1. Send message to all
2. Send direct message
3. Show all actual users
0. Leave
-----------------
"""
	)


def client_runner():
	while True:
		print_cli_menu()
		choice = input("Ur choice: ")
		if choice == '1':
			Message.SendMessage(MR_ALL, MT_DATA, input("Type ur message: "))
		
		elif choice == '2':
			id = None
			try:
				id = int(input("Input users's id: "))
			except Exception as e:
				print("Incorrect user's id")
			if id:
				Message.SendMessage(id, MT_DATA, input("Type ur message: "))


		elif choice == '3':
			Message.SendMessage(MR_BROKER, MT_GETUSERS)
		
		elif choice == '0':
			Message.SendMessage(MR_BROKER, MT_EXIT)
			break
		
		else:
			print("Something went wrong...")


def main():
	Message.SendMessage(MR_BROKER, MT_INIT)
	t = threading.Thread(target=process_messages, daemon=True)
	t.start()
	client_runner()
	

if __name__ == "__main__":
	main()