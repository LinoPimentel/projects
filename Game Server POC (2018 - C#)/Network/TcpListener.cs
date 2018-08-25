using System.Net;
using System.Threading;
using System.Threading.Tasks;
using System.Net.Sockets;
using System;
using System.Collections.Generic;

namespace KongregateGameServer.Network
{
    class TCP
    {
        TcpListener tcpListener = null;

        public TCP(string localAddr, int port)
        {
            tcpListener = new TcpListener(IPAddress.Parse(localAddr),port);
        }

        public void Start()
        {
            Console.WriteLine("Starting TCP socket at enpoint: {0}", tcpListener.LocalEndpoint);
            try
            { 
                tcpListener.Start();
                Task.Run(() => AcceptClients());
            }
            catch(Exception e)
            {
                Console.WriteLine("Failed to start tcp listener with exception: {0}", e.Message);
            }
        }

        public void Stop()
        {
            tcpListener.Stop();
            Console.WriteLine("Stoped TCP socket\n");
        }

        private async void AcceptClients()
        {
            while(true)
            {
                try
                { 
                    Console.WriteLine("Waiting for tcp client....");
                    TcpSocket tcpSocket = new TcpSocket(await tcpListener.AcceptTcpClientAsync());
                    NetworkListener.RunTests(tcpSocket);    
                }
                catch(Exception e)
                {
                    Console.WriteLine("Tcp socket Closed. Exception: {0}",e.Message);
                    return;
                }
            }
        }        
    }
}
