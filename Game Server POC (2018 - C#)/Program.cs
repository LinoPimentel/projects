using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KongregateGameServer
{
    class Program
    {
        static void Main(string[] args)
        {
            Network.NetworkListener networkListener = new Network.NetworkListener();
            networkListener.StartListeners();
            Console.ReadKey();
            networkListener.StopListeners();
            Console.ReadKey();
        }
    }
}
