/*
This class is responsible for creating the map with obstacles and enemies, and will also continuosly add more enemies as the game progresses.
We use a simple bidimesional array to store the map obstacles, as it makes generating enemies, and their pathfinding, much easier to implement.
There is a method in this class that allows us to register a time event and it will call it after X time has passed.
We use this to register any ammount of events we want and only do the elapsed time check on 1 single Event.
*/

#pragma strict

import System.Collections.Generic;
		
																							
class Map extends MonoBehaviour
{
	var size : Vector2;

	var map : Array;
	
	private var mapSize : int;
	private var ratio : int = 1;
	private var elapsedTimeEventList : List.<TimeEvent>;
 	var objectList : List.<MapObject>; // this doesnt exist in normal javascript. This is part of windows libraries for the sake of better code
	var numOfPowerUps : int;
	var numOfObstacles : int;
	var numOfEnemies : int;

	var lastEnemyGenerationTime = 0;

	var initialNumOfEnemies : int;
    var gameMain : GameMain;
    var playInterface : PlayInterface;
	
	class TimeEvent
	{
		var time : float;
		var func : function();
		var obj;
		
		function TimeEvent(t : float , f, obj)
		{
			time = t;
			func = f;
			this.obj = obj;
		}
	}
	
	//Start only runs once when script is first loaded
	function Start () 
	{
		// initializes all the variables we'll need
		numOfPowerUps = 0;
		numOfObstacles = 0;
		numOfEnemies = 0;
		objectList = new List.<MapObject>();
		objectList.Add(GameObject.Find("Player").GetComponent(Player));
		gameMain = GameObject.Find("GamePlayData").GetComponent(GameMain);
		playInterface = GameObject.Find("Interface").GetComponent(PlayInterface);
		mapSize = (gameMain.mapSize+1)*20;


		//Debug.Log("mapSize is " + mapSize);
		
		//instantiate our background
		var background : GameObject = Instantiate(Resources.Load("Prefabs/background"),Vector3((mapSize+5)/2,(mapSize+5)/2,0), Quaternion.identity);
		background.transform.localScale.x = mapSize+40;
		background.transform.localScale.y = mapSize+20;
		var backgroundMaterial : Material = background.GetComponent(MeshRenderer).material;
		backgroundMaterial.mainTextureScale = Vector2(mapSize/2+5,mapSize/2+5);
		
		//register an event from any class extending from MapObject with following example < mapRef.setElapsedTimeListener(Time.time+duration,funcToCall,this.gameObject); >
		//If you destroy an object it'll check if it was destroyed before calling the function, so no errors
		elapsedTimeEventList = new List.<TimeEvent>();
		
		var noiseScale : float = 0.01;
		var perlinRand : float = Random.Range(0, 10000);

		//Create the map array
		map = new Array(mapSize);
		for(var x : int = 0; x < mapSize; x++)
		{
			map[x] = new Array(mapSize);
			for(var y : int = 0; y < mapSize; y++)
			{
				//Generate the map using perlin noise
				var perlinX : float = x/(mapSize*1.0*noiseScale) + 0.01 + perlinRand;
				var perlinY : float = y/(mapSize*1.0*noiseScale) + 0.01 + perlinRand;
				var noise : float = Mathf.PerlinNoise(perlinX, perlinY); //Make some noise!
				var block : int = noise > 0.56 ? 1 : 0;
				if(x == 0 || y == 0 || x == mapSize-1 || y == mapSize-1) // Blocks at the edge of the map
					block = 1;
				(map[x] as Array)[y] = block;

				if(block)
				{
					//Create the obstacle
					var obstacle : Obstacle = InstantiateMapObject(new Vector2(x*ratio + ratio/2, y*ratio + ratio/2), typeOfScript.Obstacle);
					objectList.Add(obstacle);
					obstacle.Initialize("Images/rock",this,typeOfObstacle.Rock1x1);
					numOfObstacles++;
				}
			}
		}

		//Find somewhere in the center to put the player in
		var done : boolean = false;
		for(x  = (mapSize/2)-2; x < mapSize; x++)
		{
			for(y = (mapSize/2)-2; y < mapSize; y++)
			{
				if(!isBlockAt(x, y))
				{
					//Ok, its here
					objectList[0].gameObject.transform.position = new Vector3(x*ratio + ratio/2, y*ratio + ratio/2, 0);
					done = true;
					break;
				}
			}
			if(done)
				break;
		}

		//Generate some enemies
		for(x = 0; x < mapSize; x++)
		{
			for(y = 0; y < mapSize; y++)
			{
				if(!isBlockAt(x, y) && Mathf.Floor(Random.Range(0, 45)) == 1 && dist2D(x, y, objectList[0].gameObject.transform.position.x, objectList[0].gameObject.transform.position.y) > 6) 
				{
					var enemy : Enemy = InstantiateMapObject(new Vector2(x*ratio + ratio/2, y*ratio + ratio/2),typeOfScript.Enemy);
					objectList.Add(enemy);
					var rand = Random.Range(0, 10);
					if(rand > 8)
						enemy.Initialize("Images/green_knight",this,typeOfEnemy.GreenKnight, objectList[0].gameObject);
					else if(rand > 4)
						enemy.Initialize("Images/ogre",this,typeOfEnemy.Ogre, objectList[0].gameObject);
					else
						enemy.Initialize("Images/skeleton",this,typeOfEnemy.Skeleton, objectList[0].gameObject);

					numOfEnemies++;
				}
			}
		}

		initialNumOfEnemies = numOfEnemies;	
	}

	//runs every frame in unity
	function Update () 
	{
		checkElapsedTimeEvents();

		//Generate more enemies (and power-ups ?)
		if(Time.time-playInterface.getStartTime() > 30 && Time.time-lastEnemyGenerationTime > 15 && numOfEnemies < initialNumOfEnemies * 1.5)
		{
			lastEnemyGenerationTime = Time.time;
			//Debug.Log("numOfEnemies: " + numOfEnemies + ", increasing");
			for(var x = 0; x < mapSize; x++)
			{
				for(var y = 0; y < mapSize; y++)
				{
					if(!isBlockAt(x, y) && Mathf.Floor(Random.Range(0, 140)) == 1 && dist2D(x, y, objectList[0].gameObject.transform.position.x, objectList[0].gameObject.transform.position.y) > 16) 
					{
						var enemy : Enemy = InstantiateMapObject(new Vector2(x*ratio + ratio/2, y*ratio + ratio/2),typeOfScript.Enemy);
						objectList.Add(enemy);
						var rand = Random.Range(0, 10);
						if(rand > 8)
							enemy.Initialize("Images/green_knight",this,typeOfEnemy.GreenKnight, objectList[0].gameObject);
						else if(rand > 4)
							enemy.Initialize("Images/ogre",this,typeOfEnemy.Ogre, objectList[0].gameObject);
						else
							enemy.Initialize("Images/skeleton",this,typeOfEnemy.Skeleton, objectList[0].gameObject);

						numOfEnemies++;

						if(numOfEnemies > initialNumOfEnemies * 1.5)
						{
							//Debug.Log("numOfEnemies: " + numOfEnemies);
							return;
						}
					}
				}
			}
			//Debug.Log("numOfEnemies: " + numOfEnemies);
		}
	}
	
	function isBlockAt(x : int, y : int)
	{
		if(x > mapSize || y > mapSize || x < 0 || y < 0)
			return 0;
		return (map[x] as Array)[y];
	}

	function dist2D(x1 : float, y1 : float, x2 : float, y2 : float)
	{
		return Mathf.Sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	}

	function comparer(a : TimeEvent, b : TimeEvent)
	{
		if(a.time < b.time)
			return -1;
		else
			return 1;
	}
	
	function setElapsedTimeListener(timeToTrigger : float, func, obj)
	{
		elapsedTimeEventList.Add(new TimeEvent(timeToTrigger,func, obj));
		elapsedTimeEventList.Sort(comparer);
	}
	
	private function checkElapsedTimeEvents()
	{
		var time : float = Time.time;
		while(elapsedTimeEventList.Count && time >= elapsedTimeEventList[0].time)
		{
			if(elapsedTimeEventList[0].obj != null)	// checks if object still exists before calling the function				
				elapsedTimeEventList[0].func();		
			elapsedTimeEventList.RemoveAt(0);
		}		
	}
	
	//called when an object was destroyed
	function removeFromList(script : MapObject)
	{
		if(script.gameObject.name.Contains("Po")) // we will only have 3 types of map objects, "Enemy" "Player" "PowerUp", so only need to check first 2 letters
			numOfPowerUps--;
		else if(script.gameObject.name.Contains("O")) // for obstacles
			numOfObstacles--;
		else if(script.gameObject.name.Contains("E")) // for enemies
			numOfEnemies--;
		objectList.Remove(script);
	}
				
	function addScoreToPlayer(ammount : int)
	{
		(objectList[0] as Player).addToScore(ammount);
	}
	/*************************   Instantiation of Obstacles/Enemies/PowerUps bellow   **********************/	
	function InstantiateMapObject(position : Vector2, type : typeOfScript)
	{
		var objectInstance : GameObject; // assigns the script component
		var script : MapObject;
		
		if(type == typeOfScript.Obstacle)
		{
			objectInstance = Instantiate(Resources.Load("Prefabs/ObstaclePrefab"),position, Quaternion.identity);
			numOfObstacles++;
			script = objectInstance.GetComponent(Obstacle);
		}
		else if(type == typeOfScript.Enemy)
		{
			objectInstance = Instantiate(Resources.Load("Prefabs/EnemyPrefab"),position, Quaternion.identity);
			numOfEnemies++;
			script = objectInstance.GetComponent(Enemy);
		}
		else
		{
			objectInstance = Instantiate(Resources.Load("Prefabs/PowerUpPrefab"),position, Quaternion.identity);
			numOfPowerUps++;
			script = objectInstance.GetComponent(PowerUp);
		}
		return script;
	}
	
	
}
