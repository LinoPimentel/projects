using System.Threading.Tasks;

namespace KongregateGameServer.Network
{
    class NetworkListener
    {
        TCP tcpListner = null;
        WebsocketListener websocketListener = null;

        public NetworkListener()
        {
            tcpListner = new TCP("127.0.0.1", 7000);
            //websocketListener = new WebsocketListener("https://ireallyneedtogetadomainrightnow.cf", 7778);
        }

        public void StartListeners()
        {
            tcpListner?.Start();
            websocketListener?.Start();
        }

        public void StopListeners()
        {
            tcpListner?.Stop();
            websocketListener?.Stop();
        }

        public static void RunTests(ISocketInterface socket)
        {
            Task.Run(() =>
            {
                socket.PostWrite(PacketProvider.BuildPacketPayload(EPackets.eKeepAlive));
                socket.PostWrite(PacketProvider.BuildPacketPayload(EPackets.eLogInRequest, 12324489));
                socket.PostWrite(PacketProvider.BuildPacketPayload(EPackets.eLogInResponse, (byte)244));
                socket.PostWrite(PacketProvider.BuildPacketPayload(EPackets.ePlayerInfo, 41234, "username"));
            });
        }
    }
}
