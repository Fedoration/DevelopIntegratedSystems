using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;


namespace LaputinSharpClient
{
    class Program
    {
        static void ProcessMessages()
        {
            while (true)
            {
                var m = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_GETDATA);
                switch (m.header.type)
                {
                    case MessageTypes.MT_DATA:
                        Console.WriteLine("User [" + m.header.from.ToString() + "]:" + m.data);
                        break;
                    case MessageTypes.MT_GETUSERS:
                        if (m.data.Length == 0)
                            Console.WriteLine("So sad, there's no one here\n");
                        else
                        {
                            Console.WriteLine("Actual users:\n");
                            Console.WriteLine(m.data);
                        }
                        break;
                    default:
                        Thread.Sleep(2000);
                        break;
                }
            }
        }
        static void PrintCLIMenu()
        {
            Console.WriteLine("1. Send message to all");
            Console.WriteLine("2. Send direct message");
            Console.WriteLine("3. Show all actual users");
            Console.WriteLine("0. Leave");
            Console.WriteLine("-----------------");
        }

        static int ClientRunner()
        {
            while (true)
            {
                PrintCLIMenu();
                Console.Write("Ur choice: ");
                var s = Console.ReadLine();
                var isNumber = int.TryParse(s, out int num);
                if (isNumber == true)
                {
                    switch (num)
                    {
                        case 1:
                            {
                                Console.Write("Type ur message: ");
                                var text = Console.ReadLine();
                                if (text is not null)
                                {
                                    Message.send(MessageRecipients.MR_ALL, MessageTypes.MT_DATA, text);
                                }
                                break;
                            }
                        case 2:
                            {
                                Console.Write("Input user's id: ");
                                var client = Console.ReadLine();
                                var isNumber_ = int.TryParse(client, out int id);
                                Console.Write("Type ur message: ");
                                var text = Console.ReadLine();
                                if (isNumber_ && (text is not null))
                                {
                                    Message.send((MessageRecipients)id, MessageTypes.MT_DATA, text);
                                }
                                break;
                            }
                        case 3:
                            {
                                Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_GETUSERS);
                                break;
                            }
                        case 0:
                            {
                                Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_EXIT);
                                return 0;
                            }
                        default:
                            {
                                Console.WriteLine("Something went wrong...");
                                break;
                            }
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            Thread t = new Thread(ProcessMessages);
            t.IsBackground = true;
            t.Start();

            var m = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_INIT);
            int status = ClientRunner();
        }
    }
}
