using System;
using System.Threading.Tasks;
using System.Net;
using System.Net.WebSockets;

namespace KongregateGameServer.Network
{  
    class WebsocketListener
    {
        HttpListener httpListener = null;

        public WebsocketListener(string domain, int port)
        {
            httpListener = new HttpListener();
            httpListener.Prefixes.Add(domain + ":" + port + "/");
        }

        public void Start()
        {
            Console.WriteLine("Starting websocket listener");
            httpListener.Start();
            Task.Run(() => AcceptHttpClient());
        }

        public void Stop()
        {
            httpListener.Stop();
            Console.WriteLine("Stoped websocket listener\n");
        }

        private async void AcceptHttpClient()
        {
            while(true)
            {
                try
                {
                    Console.WriteLine("Waiting for websocket client....");
                    HttpListenerContext httpContext = await httpListener.GetContextAsync();
                    WebSocketContext webSocketContext = await httpContext.AcceptWebSocketAsync(subProtocol: null);
                    WS webSocket = new WS(webSocketContext);
                    NetworkListener.RunTests(webSocket);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Webosocket Closed. Exception: {0}", e.Message);
                    return;
                }
            }
        }
    }
}
