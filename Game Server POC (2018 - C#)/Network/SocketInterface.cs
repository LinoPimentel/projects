using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KongregateGameServer.Network
{
    interface ISocketInterface
    {
        void PostWrite(Packet packet);
    }
}
