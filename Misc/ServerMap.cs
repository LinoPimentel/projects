using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Remoting;
using UnityEditor.VersionControl;
using Static;

namespace Server
{   
    public class ServerMap : MonoBehaviour 
    {

    	//*************************** MAP TILE CLASS
    	private class MapTile
    	{
    		/// <summary>
    		/// Contains all the flags for this tile written with bitmasks
    		/// </summary>
    		private char flags; 
    		/// <summary>
    		/// List of all the MapObjects in the tile
    		/// </summary>
    		private LinkedList<MapObject> objectsInTile;

    		public MapTile(char flags)
    		{
    			this.flags = flags;
    			objectsInTile = new LinkedList<MapObject>();
    		}

    		/// <summary>
    		/// Checks if tile is walkable
    		/// </summary>
    		/// <returns><c>true</c>, if walkable, <c>false</c> otherwise.</returns>
    		public bool isWalkable()
    		{
    			return (flags&1)==1 ? true : false;
    		}

    		/// <summary>
            /// Gets a List of of objects of type <T> inside that tile (this list in c# is continous in memory so can access with index). T is constrained by the class MapObject
    		/// </summary>
    		/// <returns>The list of Ts</returns>
            public List<T> getAllOfType<T>() where T : MapObject
    		{
    			if(objectsInTile.Count <= 0)
    				return null;

    			List<T> returnList = new List<T>();
    			LinkedListNode<MapObject> current = objectsInTile.First;
    			for(int i=0;i<objectsInTile.Count;i++)
    			{
                    if (current.Value is T)
                        returnList.Add((T)current.Value);	                
    				current = current.Next;
    			}
    			return returnList;
    		}

    		/// <summary>
    		/// Gets the first object of type <T>. T is constrained by the class MapObject
    		/// </summary>
    		/// <returns>The linkedListNode</returns>
            public T getFirstOfType<T>() where T : MapObject
    		{
    			if(objectsInTile.Count <= 0)
    				return null;
    			LinkedListNode<MapObject> current = objectsInTile.First;
    			for(int i=0;i<objectsInTile.Count;i++)
    			{
                    if(current.Value is T)
                        return (T)current.Value;
    				current = current.Next;
    			}
                return null;
    		}

    		/// <summary>
    		/// Adds the object to tile and returns the node
    		/// </summary>
    		/// <returns>The LinkedListNode to the object added</returns>
    		/// <param name="obj">The object to add</param>
    		public LinkedListNode<MapObject> addObjectToTile(MapObject obj)
    		{
    			return objectsInTile.AddLast(obj);
    		}
    		/// <summary>
    		/// Adds the LinkedListNode to the tile list
    		/// </summary>
    		/// <param name="node">The node to add</param>
    		public void addObjectToTile(LinkedListNode<MapObject> node)
    		{
    			objectsInTile.AddLast(node);
    		}

    		/// <summary>
    		/// Removes the LinkedListNode from the list.
    		/// </summary>
    		/// <returns><c>true</c>, if node from tile was removed, <c>false</c> otherwise.</returns>
    		/// <param name="node">The node to remove</param>
    		public bool removeNodeFromTile(LinkedListNode<MapObject> node)
    		{
    			try
    			{
    				objectsInTile.Remove(node);
    			}catch(Exception e)
    			{
    				return false;
    			}
    			return true;
    		}
    	}

    	//********************* MAP CLASS
    	Monster[] monsterArray;
    	Player player; // this will be an array later on
    	MapTile[] mapTiles;


        public bool isTileWalkable(int tile)
        {
            return mapTiles[tile].isWalkable();
        }

    	/// <summary>
    	/// Checks if its possible to move from <from> to <to>
    	/// </summary>
    	/// <returns><c>true</c>, if possible <c>false</c> otherwise.</returns>
    	/// <param name="from">Move from</param>
    	/// <param name="to">Move to</param>
    	private bool canMoveFromTo(Coords from, Coords to)
    	{
    		if(to.x > Statics.mapSizeX || to.x < 0 || to.y > Statics.mapSizeY || to.y < 0)
    			return false;
    		//needs to run pathfinder to check here too
    		return true; // decide on what this will have later
    	}

    	/// <summary>
    	/// If the coordinates are valid it will add the MapObject to the tile
    	/// </summary>
    	/// <returns>The node of the object after adding</returns>
    	/// <param name="tileCords">The tile to add in</param>
    	/// <param name="obj">The object to add</param>
    	public LinkedListNode<MapObject> addObjectToTile(Coords tileCords, MapObject obj)
    	{
    		return mapTiles[tileCords.x+tileCords.y*Statics.mapSizeY].addObjectToTile(obj);
    	}

    	/// <summary>
    	/// Moves the player to coords
    	/// </summary>
    	/// <returns><c>true</c>, if player to was moved, <c>false</c> otherwise.</returns>
    	/// <param name="p">the player instance (later chance to ID).</param>
    	/// <param name="to">The coordinates to move to</param>
        public bool movePlayerTo(int id, Coords to) // make use of id later
    	{
            if(!canMoveFromTo(player.Coordinates, to) || !mapTiles[Statics.coordsToInt(to)].isWalkable())
    			return false;
            if(to.x != player.Coordinates.x || to.y != player.Coordinates.y)
                player.changeCords(to);
    		return true; 
    	}

        public void updateDirection(int id, char newDir)
        {
            if (id == 0) // change this to go on player array later on
                player.stats.direction = newDir;             
        }

        //skills, disabled for now, untested
        /*
        public bool castSkill(MapObject src, int skillId, int skillLvl)
        {
            if (src is Player) 
            {
                List<Monster> monsterList;
                List<int> tileList = Statics.skills[skillId].getTilesInRange(((Player)src).stats.direction, Statics.coordsToInt(src.Coordinates)); // change the skill part to call the right skill later
                for (int k = 0; k < tileList.Count; k++)
                {
                    markTileForSec(tileList[k],3); // delete later, this is only for testing purposes
                    monsterList = mapTiles[tileList[k]].getAllOfType<Monster>(); // gets all the monsters in that tile
                    if (monsterList != null)
                    {
                        for (int i = 0; i < monsterList.Count; i++)
                            monsterList[i].changeHealth(-((Player)src).stats.attack);
                    }
                }
            }     
            return true;
        }

        public bool castSkillAtTile(int id, int skillId, int skillLvl, int center)
    	{
    		// runs checks if skill can be used here, for now just assume yes

            List<Monster> monsterList;
            List<int> tileList = Statics.skills[skillId].getTilesInRange(player.stats.direction, center); // change the skill part to call the right skill later
            for (int k = 0; k < tileList.Count; k++)
            {
                markTileForSec(tileList[k],3); // delete later, this is only for testing purposes
                monsterList = mapTiles[tileList[k]].getAllOfType<Monster>(); // gets all the monsters in that tile
                if (monsterList != null)
                {
                    for (int i = 0; i < monsterList.Count; i++)
                        monsterList[i].changeHealth(-player.stats.attack);
                }
            }		   
    		return true;
    	}*/

    	// Use this for initialization
    	void Start () 
    	{
            GameObject aux; // delete later
    		int numOfMonsters = 1;
    		monsterArray = new Monster[numOfMonsters];
    		mapTiles = new MapTile[Statics.mapSizeX*Statics.mapSizeY];
            int[] generatedMap = new MapGenerator().getMap();
            for (int i = 0; i < Statics.mapSizeX * Statics.mapSizeY; i++)
            {
                mapTiles[i] = new MapTile((char)generatedMap[i]); 
                if (mapTiles[i].isWalkable())
                {
                    aux = new GameObject();
                    aux.transform.parent = gameObject.transform;
                    aux.AddComponent<SpriteRenderer>().sprite = Resources.Load<Sprite>("Square");
                    aux.GetComponent<SpriteRenderer>().sortingLayerName = "Map";
                    aux.transform.position = Statics.mapToWorldCords(i);
                }
            }
            Pathfinder.loadMap(ref generatedMap, Statics.mapSizeX, Statics.mapSizeY); // update this later so that map can change
            player = new Player(this, new Coords(Statics.mapSizeX/2,Statics.mapSizeY/2));
            Parser.spawnPlayer(player.Coordinates);

    		//tests
    		for(int i = 0;i<numOfMonsters;i++)
                monsterArray[i] = new Monster(this,new Coords(Statics.mapSizeX/2-1,Statics.mapSizeY/2));		
    	}
    	
        
    	// Update is called once per frame
    	void Update () 
    	{
    		// run monster AI here
            monsterArray[0].runAI();
    	}


        // unity only
        private void markTileForSec(int tile, float time)
        {
            //delete this part after testing
            GameObject visual = new GameObject();
            visual.transform.position = Statics.mapToWorldCords(tile);
            visual.AddComponent<SpriteRenderer>().sprite = Resources.Load<Sprite>("ice");
            Destroy(visual, time);
        }
    }
}