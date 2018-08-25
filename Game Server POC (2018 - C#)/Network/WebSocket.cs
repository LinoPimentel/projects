using System;
using System.Threading.Tasks;
using System.Net.WebSockets;
using System.Threading;

namespace KongregateGameServer.Network
{
    class WS : ISocketInterface
    {
        SemaphoreSlim writeSemaphore = null;
        WebSocket webSocket;

        public WS(WebSocketContext web_socket)
        {
            webSocket = web_socket.WebSocket;
            Console.WriteLine("Accepted new WS client");
            writeSemaphore = new SemaphoreSlim(1, 1);
            Task.Run(() => ReadStream());
        }

        public void PostWrite(Packet packet)
        {
            ArraySegment<byte> array = new ArraySegment<byte>(packet.payload, sizeof(short), packet.size - sizeof(short));
            Task.Run(
                async () =>
                {
                    await writeSemaphore.WaitAsync();
                    try
                    {
                        await webSocket.SendAsync(array, WebSocketMessageType.Binary, true, CancellationToken.None);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Canceled websocket write with exception: {0}", e.Message);
                    }
                    writeSemaphore.Release();
                });
        }

        private async void ReadStream()
        {
            byte[] buffer = new byte[256];
            ArraySegment<byte> array = new ArraySegment<byte>(buffer, 0, 256);
            
            try
            {
                while (true)
                {
                    WebSocketReceiveResult result = await webSocket.ReceiveAsync(array, CancellationToken.None);
                    if(result.MessageType == WebSocketMessageType.Binary)
                        PacketProvider.ParsePacketData(buffer, (short)result.Count);
                }

            }
            catch (Exception e)
            {
                Console.WriteLine("WS closed. Exception {0}", e.Message);
            }
        }
    }
}
