package com.company.tcp_server;

import java.io.IOException;
import java.net.*;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Dany on 21/10/2016.
 */
public class UDPManager extends Thread
{
    private final NotificationHandler notificationHandler;
    private final int TIME_TO_UPDATE = 5000, MAX_ALLOWED_TIMEOUT = 3000, TIME_TO_SEND_TO_CLIENT = 60000 ;
    private MulticastSocket multicastSocket;
    private int udpPort = 6000, tcpPort;
    private InetAddress group;
    private ConcurrentHashMap<Integer,TCPServerInformation> tcpServersInformation = new ConcurrentHashMap<Integer,TCPServerInformation>();

    public UDPManager(int __tcpPort, String __hostname, NotificationHandler __notificationHandler)
    {
        notificationHandler = __notificationHandler;
        tcpPort = __tcpPort;
        tcpServersInformation.put(__tcpPort,new TCPServerInformation(__hostname,__tcpPort));

        try {
            group = InetAddress.getByName("225.0.0.0");

            try{
                multicastSocket = new MulticastSocket(udpPort);
                ////System.out.println("Binded tcp server to UDP port: " + udpPort);

            }catch(IOException e)
            {
                ////System.out.println("Exception binding UDP port: " + e);
            }

            multicastSocket.joinGroup(group);
            multicastSocket.setTimeToLive(1);

            new Thread()
            {
                public void run()
                {
                    while(true)
                    {
                        try
                        {
                            sleep(TIME_TO_UPDATE);
                            tcpServersInformation.get(tcpPort).updateLoad(TCPServerLoad.getServerConnectionCount());
                            byte[] buf = tcpServersInformation.get(tcpPort).toBytes();
                            DatagramPacket msgOut = new DatagramPacket(buf, buf.length, group, udpPort);
                            multicastSocket.send(msgOut);
                        } catch (Exception e)
                        {
                            ////System.out.println("Exception sending to multicast or sleep: " + e);
                            break;
                        }
                    }
                }
            }.start();
        } catch(Exception e) {
            ////System.out.println("Exception creating socket: " + e);
        }
        start();
    }

    public void run()
    {
        long time = System.currentTimeMillis();
        while(true)
        {
            byte[] inBuf = new byte[8 * 1024];
            DatagramPacket data= new DatagramPacket(inBuf, inBuf.length);
            try
            {
                multicastSocket.receive(data);
                parseAndUpdateData(new String(inBuf,0,data.getLength()));

                //////System.out.println("Message recieved from multicast: " + new String(inBuf,0,data.getLength()));
                if(System.currentTimeMillis() - time >= TIME_TO_SEND_TO_CLIENT)
                {
                    time = System.currentTimeMillis();
                    //////System.out.println("Sending load to clients");
                    checkForTimedOutTCPs();
                    notificationHandler.sendLoadToClients(informationToString());
                }

            } catch (IOException e)
            {
                //////System.out.println("Exception recieving in multicast: " + e);
                break;
            }
        }
    }

    public void checkForTimedOutTCPs()
    {
        for(Integer key : tcpServersInformation.keySet())
        {
            if(System.currentTimeMillis() - tcpServersInformation.get(key).getLastUpdate() > MAX_ALLOWED_TIMEOUT)
            {
                tcpServersInformation.remove(key);
                //////System.out.println("Removed a tcp element");
            }
        }
    }

    public String informationToString()
    {
        String data = "type:notification_load , server_list: " + tcpServersInformation.size();
        int i=0;
        for(Integer key : tcpServersInformation.keySet())
        {
            TCPServerInformation inf = tcpServersInformation.get(key);
            data = data + ", server_" + i + "_hostname:" + inf.getHostname();
            data = data + ", server_" + i + "_port: " + inf.getTcpPort();
            data = data + ", server_" +i + "_load: " + inf.getLoad();
            i++;
        }
        return data;
    }

    public void exitUDPManager()
    {
        try
        {
            multicastSocket.leaveGroup(group);
            ////System.out.println("leaving multicast group");
        }catch(IOException e)
        {
            ////System.out.println("Error leaving multicast group: " + e);
        }
    }

    public void parseAndUpdateData(String data)
    {
        int tcpPort,load;
        String hostname;

        tcpPort = Integer.parseInt(data.substring(0,data.indexOf("-")));
        hostname = data.substring(data.indexOf("-")+1,data.lastIndexOf("-"));
        load = Integer.parseInt(data.substring(data.lastIndexOf("-")+1));
       // ////System.out.println("TCP port: " + tcpPort + " hostname: " + hostname + " load: " + load);
        if(tcpServersInformation.containsKey(tcpPort))
        {
            tcpServersInformation.get(tcpPort).updateLoad(load);
            //////System.out.println("Updating tcp server: " + tcpPort);
        }
        else
        {
            tcpServersInformation.put(tcpPort, new TCPServerInformation(hostname,tcpPort));
        }
    }

    public class TCPServerInformation
    {
        private int load, tcpPort;
        private String hostname;
        private long lastUpdate;

        public TCPServerInformation(String __hostname, int __tcpPort)
        {
            tcpPort = __tcpPort;
            hostname = __hostname;
            lastUpdate = System.currentTimeMillis();
        }
        public byte[] toBytes()
        {
            return (Integer.toString(tcpPort) + "-" + hostname + "-" + Integer.toString(load)).getBytes();
        }

        public void updateLoad(int __load)
        {
            load = __load;
            lastUpdate = System.currentTimeMillis();
        }

        public int getLoad()
        {
            return load;
        }

        public int getTcpPort()
        {
            return tcpPort;
        }
        public String getHostname()
        {
            return hostname;
        }
        public long getLastUpdate()
        {
            return lastUpdate;
        }
    }
}
