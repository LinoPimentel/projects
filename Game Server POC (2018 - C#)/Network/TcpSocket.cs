using System;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;
using System.Runtime.InteropServices;

namespace KongregateGameServer.Network
{
    class TcpSocket : ISocketInterface
    {
        TcpClient tcpClient = null;
        NetworkStream stream = null;
        SemaphoreSlim writeSemaphore = null;

        public TcpSocket(TcpClient tcp_client)
        {
            Console.WriteLine("Accepted new TCP client");
            tcpClient = tcp_client;
            stream = tcpClient.GetStream();

            writeSemaphore = new SemaphoreSlim(1, 1);
            Task.Run(() => ReadStream());
        }

        public void PostWrite(Packet packet)
        {
            Task.Run(
                async () =>
                {
                    await writeSemaphore.WaitAsync();
                    try
                    { 
                        await stream.WriteAsync(packet.payload, 0, packet.size);
                    }catch(Exception e)
                    {
                        Console.WriteLine("Canceled write with exception: {0}", e.Message);
                    }
                    writeSemaphore.Release();
                });
        }

        private async void ReadStream()
        {
            byte[] buffer = new byte[256];
            try
            {
                while(true)
                {
                    int packetSize = 0;
                    //Read the packet length
                    while(packetSize < sizeof(short))
                        packetSize += await stream.ReadAsync(buffer, packetSize, sizeof(short) - packetSize);
                    packetSize = 0;
                    for (int i = sizeof(short) - 1; i >= 0; i--)
                        packetSize = (packetSize >> 8) | buffer[i];

                    //Reads the packet data until length is read
                    int bytesRead = 0;
                    while(bytesRead < packetSize)                    
                        bytesRead += await stream.ReadAsync(buffer, bytesRead, packetSize - bytesRead);

                    PacketProvider.ParsePacketData(buffer, (short)packetSize);
                }
                
            }catch(Exception e)
            {
                Console.WriteLine("Connection closed. Exception {0}", e.Message);
            }
        }                         
    }
}
