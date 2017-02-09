package com.company.tcp_server;

import com.company.rmi_server.RMIServerMethods;

import java.io.*;

import java.rmi.*;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Dany on 10/14/2016.
 */
public class TCPServer {

    private static ConcurrentHashMap<Long,PrintWriter> onlineUserPrintWriters = new ConcurrentHashMap<>();
    private static ConcurrentHashMap<Long,PrintWriter> temporaryUserPrintWriters = new ConcurrentHashMap<>();
    private static RMIServerMethods rmiServerMethods = null;
    private static NotificationHandler notificationHandle = null;

    public static RMIServerMethods getRmiServerMethods()
    {
        synchronized (rmiServerMethods)
        {
            return rmiServerMethods;
        }
    }

    private void refreshRmiServerMethods(String binderIp,String binderPort)
    {
        synchronized (rmiServerMethods)
        {
            try{
                rmiServerMethods = (RMIServerMethods)Naming.lookup("rmi://"+binderIp + ":" + binderPort + "/inverseAuction");
                rmiServerMethods.subscribeNotificationCallback("callback",notificationHandle);
               // System.out.println("Re-found to rmi server");
            }catch(Exception e)
            {
                //System.out.println("Exception looking up rmi server");
            }
        }
    }

    public TCPServer(final String binderIp, final String binderPort, int tcpPort)
    {
        while(true)
        try {
            rmiServerMethods = (RMIServerMethods)Naming.lookup("rmi://"+binderIp + ":" + binderPort + "/inverseAuction");
            notificationHandle = new NotificationHandler(onlineUserPrintWriters,temporaryUserPrintWriters);
            rmiServerMethods.subscribeNotificationCallback("testCallback",notificationHandle);
            //Thread to check if rmiserver goes down
            new Thread()
            {
                public void run()
                    {
                        while(true)
                        {
                            try
                            {
                                rmiServerMethods.keepAlive();
                                try{
                                    sleep(500);
                                }catch(InterruptedException e1){}
                            } catch (RemoteException e)
                            {
                                //System.out.println("RMI server went down, reconnecting soon");
                                refreshRmiServerMethods(binderIp,binderPort);
                            }
                        }
                }
            }.start();
            //System.out.println("Found rmi server");
            ArrayList<Integer> a = new ArrayList<>();


            //System.out.println("Successfully subscribed notification callback to rmi server");

            TCPConnectionService tcpConnectionService = new TCPConnectionService(onlineUserPrintWriters,temporaryUserPrintWriters,tcpPort);
            UDPManager udpManager = new UDPManager(tcpConnectionService.getConnectedPort(), "KeepAliveCheck",notificationHandle);
            System.out.println("TCP ready on port: " + tcpPort);
            while(true)
            {
                if(!tcpConnectionService.isConnectionAlive())
                    break;
            }
            udpManager.exitUDPManager();
            tcpConnectionService.exitTCPService();

            // change this later to prepare for MAX_CLIENTS reached
        } catch (Exception e) {
            //e.printStackTrace();
            //System.out.println("Exception 1 : " + e);
        }
    }

    public static void main(String args[])
    {
        String binderIp, binderPort;
        int tcpPort = 3000;
        if(args.length >= 2)
        {
            binderPort = args[1];
            binderIp = args[0];
            if(args.length == 3)
                tcpPort = Integer.parseInt(args[2]);
        }
        else
        {
            System.out.println("Usage: <binderIP> <binderPort> <tcpPort> \ntcpPort is optional");
            return;
        }
        new TCPServer(binderIp,binderPort,tcpPort);
    }
}