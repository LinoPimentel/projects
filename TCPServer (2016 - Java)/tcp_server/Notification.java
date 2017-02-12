package com.company;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by Dany on 18/10/2016.
 */
public class Notification implements Serializable
{
    public String message;
    public long userId, messageId, auctionId;

    public Notification(String __message, long __userId, long __messageId, long __auctionId)
    {
        auctionId = __auctionId;
        message = __message;
        userId = __userId;
        messageId = __messageId;

        //System.out.println("Creating notification id " + messageId + " for user " + userId);
    }

    public String getMessage()
    {
        Map map = StaticMethods.parseDataToMap(message);
        return map.toString();
    }

    @Override
    public String toString()
    {
        return "notification id " + messageId + " for user " + userId;
    }
}
