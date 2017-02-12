/**
 * Created by Dany on 10/14/2016.
 */
package com.company;

import com.company.rmi_server.RMIServerMethods;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import static java.lang.System.exit;

public class StaticMethods
{

    public enum RmiReply
    {
        USER_ID,
        REPLY_TO_CLIENT
    };

    public enum CallingFrom
    {
        TCP,
        TOMCAT
    };

    public enum Types
    {
        LOGIN,
        REGISTER,
        LOGOUT,
        CREATE_AUCTION,
        SEARCH_AUCTION,
        DETAIL_AUCTION,
        MY_AUCTIONS,
        ALL_AUCTIONS,
        BID,
        MESSAGE,
        ONLINE_USERS,
        EDIT_AUCTION,
        CANCEL_AUCTION,
        BAN_USER
    };

    public enum RMItoTomcatMap
    {
        TYPE,
        SUCCESS,
        ITEMS,
        AUCTIONS,
        USERS
    };



    public static Map parseDataToMap(String data)
    {
        // data = data.trim();
        String[] fields = data.split(",");
        Map<String, String> map = new HashMap<String, String>();

        for (String s : fields) {
            map.put(s.substring(0, s.indexOf(":")).trim(), s.substring(s.indexOf(":") + 1).trim());
            //System.out.println("'" + s.substring(0,s.indexOf(":")).trim() + "' and '" + s.substring(s.indexOf(":")+1) + "'");
        }
        return map;
    }
/*
    HashMap<String, HashMap> selects = new HashMap<String, HashMap>();

for(Map.Entry<String, HashMap> entry : selects.entrySet()) {
    String key = entry.getKey();
    HashMap value = entry.getValue();

    // do what you have to do here
    // In your case, an other loop.
}*/
public static String readString()
{
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    try
    {
        return br.readLine();
    }
    catch (IOException e)
    {
        e.printStackTrace();
        exit(-1);
    }
    return null;
}
    public static int readInt()
    {
        while(true)
        {
            try
            {
                return Integer.parseInt(readString());
            }
            catch(NumberFormatException e)
            {
                System.out.print("Não é um valor inteiro! Tente Novamente: ");
            }
        }
    }
    public static float readFloat()
    {
        while(true)
        {
            try
            {
                return Float.parseFloat(readString());
            }
            catch(NumberFormatException e)
            {
                System.out.print("Não é um valor float! Tente Novamente: ");
            }
        }
    }

    public static String parseMapToData(Map map)
    {
        if(!map.containsKey("type"))
            return "Type doesnt exist";

        String data = "type: " + (String)map.get("type");
        map.remove("type");
        for(Map.Entry<String,String> entry : (Set<Map.Entry>)map.entrySet())
        {
            String key = entry.getKey();
            String value = entry.getValue();
            data = data +  " , " + key + ": " + value;
        }
        //System.out.println(data);
        return data;
    }
    public static long parseDateToLong(String date)
    {
        date = date.substring(0,date.lastIndexOf("-")) + ":" + date.substring(date.lastIndexOf("-")+1) + ":00";
        java.sql.Timestamp ts = java.sql.Timestamp.valueOf(date);
        return ts.getTime();
    }

    public static Map<RmiReply,Object> callRMIMethod(RMIServerMethods rmiServer, Map<String,String> map, int userId, long timestamp) throws RemoteException
    {
        //item list, login, status, register, create action,  search auction, detail auction, my auctions, bid
        //edit auction, message, online users,
        if(map.containsKey("type"))
        {
            switch (map.get("type")) // add other cases here
            {
                case "login":
                {
                    if (map.containsKey("username") && map.containsKey("password"))
                        return rmiServer.login(map.get("username"), map.get("password"), CallingFrom.TCP);
                    break;
                }
                case "register":
                {
                    if(map.containsKey("username") && map.containsKey("password"))
                        return rmiServer.register(map.get("username"), map.get("password"), CallingFrom.TCP);
                    break;
                }
                case "create_auction":
                {
                    String addDetails = "";
                    if(map.containsKey("additionalDetails"))
                        addDetails = map.get("additionalDetails");
                    if(map.containsKey("code") && map.containsKey("title") && map.containsKey("description") && map.containsKey("amount") && map.containsKey("deadline"))
                        return rmiServer.createAuction(userId,map.get("code"), map.get("title"), map.get("description"), addDetails, Float.parseFloat(map.get("amount")), parseDateToLong(map.get("deadline")),timestamp, CallingFrom.TCP);
                    break;
                }
                case "search_auction":
                {
                    if(map.containsKey("code"))
                        return rmiServer.getAuctionsByProductCode(map.get("code"), CallingFrom.TCP);
                    break;
                }
                case "detail_auction":
                {
                    if(map.containsKey("id"))
                        return  rmiServer.getAuctionDetails(Integer.parseInt(map.get("id")), CallingFrom.TCP);
                    break;
                }
                case "my_auctions":
                {
                        return rmiServer.getAuctionsByUserActivity(userId, CallingFrom.TCP);
                }
                case "edit_auction":
                {
                    if(map.containsKey("id"))
                    {
                        int a = Integer.parseInt(map.remove("id"));
                        return rmiServer.editAuction(a, map,timestamp, CallingFrom.TCP);
                    }
                    break;
                }
                case "bid":
                {
                    if(map.containsKey("id") && map.containsKey("amount"))
                        return rmiServer.bidAuction(userId, Integer.parseInt(map.get("id")), Float.parseFloat(map.get("amount")),timestamp, CallingFrom.TCP);
                }break;
                case "message":
                {
                    if(map.containsKey("id") && map.containsKey("text"))
                        return rmiServer.addMessage(userId, Integer.parseInt(map.get("id")), map.get("text"),timestamp, CallingFrom.TCP);
                }break;
                case "online_users":
                {
                    return rmiServer.getLoggedUsers(CallingFrom.TCP);
                }
                case "all_auctions":
                {
                    return rmiServer.getAllAuctions(CallingFrom.TCP);
                }
                case "logout":
                {
                    return rmiServer.logout(userId,CallingFrom.TCP);
                }
                default:
                {return null;}
            }
        }
        return null;
    }

    public static Map<StaticMethods.Types, Object> getResultMap(Map<StaticMethods.RmiReply,Object> map)
    {
        return (Map<StaticMethods.Types, Object>) map.get(StaticMethods.RmiReply.REPLY_TO_CLIENT);
    }
}