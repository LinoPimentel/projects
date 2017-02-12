package com.company.tcp_server;

import com.company.Notification;
import com.company.rmi_server.CallbackInterface;

import java.io.PrintWriter;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Dany on 20/10/2016.
 */
public class NotificationHandler extends UnicastRemoteObject implements CallbackInterface
{
    private ConcurrentHashMap<Long,PrintWriter> onlineUserPrintWriters;
    private ConcurrentHashMap<Long,PrintWriter> temporaryUserPrintWriters;

    public void sendLoadToClients(final String serverLoadInf)
    {
        new Thread()
        {
            public void run()
            {
                sendToUsersFromList(serverLoadInf,onlineUserPrintWriters);
                sendToUsersFromList(serverLoadInf,temporaryUserPrintWriters);
            }
        }.start();
    }

    private void sendToUsersFromList(String serverLoadInf, ConcurrentHashMap<Long,PrintWriter> printWriters)
    {
        //System.out.println("sendToUsersFromList()");

        Set<Long> set = printWriters.keySet();
        for(Long key : set)
        {
            try {
                PrintWriter writer = printWriters.get(key);
                synchronized (writer)
                {
                    writer.println(serverLoadInf);
                    //System.out.println("Sent notification + " + n.message);
                }
            }catch(NullPointerException e)
            {
                // will enter here if between the time it checked if key existed and read its value, the key was removed
                continue;
            }
        }
    }

    public ArrayList<Long> pushNotification(ArrayList<Notification> notifies) throws java.rmi.RemoteException
    {
        /*System.out.println("pushNotification()");

        System.out.println("onlineUserPrintWriters is ");
        for(Long userid : onlineUserPrintWriters.keySet())
        {
            System.out.println("user id " + userid);
        }*/

        ArrayList<Long> toReturn = new ArrayList<>(); //Will return notifications that it did NOT send

        while (notifies != null && !notifies.isEmpty() && notifies.size() > 0)
        {
            Notification n = notifies.remove(0);

            //System.out.println("Iterating, " + n);

            if(!onlineUserPrintWriters.containsKey(n.userId))
            {
                toReturn.add(n.messageId);
            }

            //System.out.println("for user: " + onlineUserPrintWriters.containsKey(n.userId));
            try {
                PrintWriter writer = onlineUserPrintWriters.get(n.userId);
                synchronized (writer)
                {
                    writer.println(n.message);
                    //System.out.println("Sent notification to user: " + n.userId + " msg: " + n.message);
                }
            }catch(NullPointerException e)
            {
                // will enter here if between the time it checked if key existed and read its value, the key was removed
                toReturn.add(n.messageId);
                continue;
            }
        }
        return toReturn;
    }

    public NotificationHandler(ConcurrentHashMap<Long,PrintWriter> __onlineUsers, ConcurrentHashMap<Long,PrintWriter> __temporaryUserPrintWriters) throws RemoteException
    {
        onlineUserPrintWriters = __onlineUsers;
        temporaryUserPrintWriters = __temporaryUserPrintWriters;
    }
}
