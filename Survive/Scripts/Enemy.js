/*
This class represents an instance of an enemy. It continuosly updates its position, using the doAIBehavior() function.
If the enemy is far away from the player it will enter an idle mode, moving randomly. If it's close, it will try to pathfind to the player and attack them.
Power-ups are also created here, when the enemy dies.
NOTE: The reason we use enums to initialize values is explained under Attack.js header comment
*/

#pragma strict

class Enemy extends MapObject
{
	var enemyType : typeOfEnemy;
	
	private var lastAttackTime : float = 0;
	private var attack : Attack = new Attack(typeOfAttack.DefaultBullet,this,"Images/magic_attack", 0.2);
	private var player : GameObject;
	private var inAtkAnimation : boolean;
	private var showHealthBar : boolean;
	private var isMoving : boolean;
	private var movingStartTime : float;
	private var moveDirection : Vector2;
	private var movementScheduled : boolean;
	private var attackPower : int;

	private var lastPathfindingTime = 0;
	private var path : Array;
	private var pathPos : int = 0;

	function Start ()
	{
		facingDirection = Vector2.down;
		inAtkAnimation = false;
		showHealthBar = false;
		isImobile = false;
		isInvulnerable = false;
		isMoving = false;
		movementScheduled = false;
	}

	function Update () 
	{
		if(!Time.timeScale)
			return;
		if(flickerAmmount > 0)
			flickerSprite();	
		animationManager.updateIdleAnimation(facingDirection);
		doAIBehavior();
	}
		
	function doAIBehavior()
	{
		var playerPos : Vector2 = new Vector2(player.transform.position.x, player.transform.position.y);
		var selfPos : Vector2 = new Vector2(transform.position.x, transform.position.y);
		
		if(Vector2.Distance(selfPos, playerPos) > 30) // If we are very far away from the player, dont do anything
			return;

		var raycastToPlayer = Physics2D.CircleCast(worldPos.position, 0.5, player.transform.position-worldPos.position, (player.transform.position-worldPos.position).magnitude, 1<<8);
		
		if(Time.time - lastAttackTime > 5 && Random.Range(0, 10) < 1 && Vector2.Distance(selfPos, playerPos) < 14 && !isMoving && raycastToPlayer.collider == null)
		{
			//Launch attack
			if(enemyType == typeOfEnemy.GreenKnight && Random.Range(0, 10) > 5 && Vector2.Distance(selfPos, playerPos) < 10)
			{
				var rand : int = Mathf.Floor(Random.Range(0, 4));
				if(rand == 0)
					attack = new Attack(typeOfAttack.Fire,this,"Images/magic_attack/fire", 0.2);
				if(rand == 1)
					attack = new Attack(typeOfAttack.Ice,this,"Images/magic_attack/ice", 0.2);
				if(rand == 2)
					attack = new Attack(typeOfAttack.Quake,this,"Images/magic_attack/quake", 0.2);
				if(rand == 3)
					attack = new Attack(typeOfAttack.Bolt,this,"Images/magic_attack/bolt", 0.2);
			}
			else
				attack = new Attack(typeOfAttack.DefaultBullet,this,"Images/magic_attack", 0.2);


			launchAttack(selfPos, playerPos);
			lastAttackTime = Time.time;
		}

		if(Vector2.Distance(selfPos, playerPos) > 12)
		{
			//If we are away from the player, just move around randomly
			if(!isMoving && !movementScheduled && ((Time.time - movingStartTime) > (4 + Random.Range(0, 4))))
			{
				//Schedule new movement
				movementScheduled = true;
				mapRef.setElapsedTimeListener(Time.time + Random.Range(1, 2), setIdleMoving, this.gameObject);
			}

			if(isMoving && ((Time.time - movingStartTime) > 1.5))
			{
				//End the movement
				movementScheduled = false;
				isMoving = false;
			}
		
			if(isMoving)
				moveEnemyToDirection(moveDirection);
		}
		else
		{
			if(Vector2.Distance(selfPos, playerPos) < 4 && raycastToPlayer.collider == null)
			{
				//We are very close to the player, stop and attack
				path = null;
			}
			else if((Time.time - lastPathfindingTime) > (3 + Random.Range(0, 2)))
			{
				//We are close to the player, try to pathfind to them
				var playerPosInt : Vector2 = new Vector2(Mathf.Floor(player.transform.position.x), Mathf.Floor(player.transform.position.y));
				var selfPosInt : Vector2 = new Vector2(Mathf.Floor(transform.position.x), Mathf.Floor(transform.position.y));

				var pathfinder : Pathfinder = new Pathfinder(mapRef.map, selfPosInt, playerPosInt);
				path = pathfinder.calculatePath();

				if(path.Count == 0) //unable to find a path
					path = null; 
				
				if(path != null)
				{
					pathPos = 1;

					//print the path for debug
					/*Debug.Log("path.Count is " + path.Count);
					for(var i : int = 0;  i < path.Count; i++)
					{
						Debug.Log("path [" + i + "]: " + (path[i] as int[])[0]+" , " + (path[i] as int[])[1]);
					}*/
				}
				else
					isMoving = false;

				lastPathfindingTime = Time.time;
			}

			if(path != null)
				moveAlongPath();
			else
				isMoving = false;
		}
	}

	function moveAlongPath()
	{
		if(pathPos >= path.Count)
			return;
		
		var selfPos : Vector2 = new Vector2(transform.position.x, transform.position.y);
		var nextPos : Vector2 = new Vector2((path[pathPos] as int[])[0], (path[pathPos] as int[])[1]);
		var movementDirection : Vector2 = nextPos - selfPos;

		movementDirection.Normalize();
		moveEnemyToDirection(movementDirection);

		//Debug.Log("we are at "+selfPos.x+", "+selfPos.y+", moving to " + nextPos.x+", "+nextPos.y +". pathPos is " + pathPos);

		if(Vector2.Distance(selfPos, nextPos) < 0.1)
		{
			pathPos++;
			//Debug.Log("increasing pathPos to " + pathPos);
		}
	}

	function moveToPosition(pos : Vector2)
	{
		var selfPos : Vector2 = new Vector2(transform.position.x, transform.position.y);
		var movementDirection : Vector2 = pos - selfPos;
		movementDirection.Normalize();
		moveEnemyToDirection(movementDirection);
	}

	function setIdleMoving()
	{
		var mapXPos : int = Mathf.Floor(transform.position.x);
		var mapYPos : int = Mathf.Floor(transform.position.y);

		//Calculate possible directions to move to
		var moveDirections : Array = [];

		if(!mapRef.isBlockAt(mapXPos, mapYPos+1))
			moveDirections.Add(new Vector2(0, 1));

		if(!mapRef.isBlockAt(mapXPos, mapYPos-1))
			moveDirections.Add(new Vector2(0, -1));

		if(!mapRef.isBlockAt(mapXPos+1, mapYPos))
			moveDirections.Add(new Vector2(1, 0));

		if(!mapRef.isBlockAt(mapXPos-1, mapYPos))
			moveDirections.Add(new Vector2(-1, 0));

		if(moveDirections.Count > 0)
		{
			movingStartTime = Time.time;
			isMoving = true;
			moveDirection = moveDirections[Random.Range(0, moveDirections.Count)];
		}
		else
		{
			//Nowhere to move to..
			movementScheduled = false;
			isMoving = false;
		}
	}

	function moveEnemyToDirection(direction : Vector2)
	{
		moveObject(direction, 1<<8, transform, 0.2, walkSpeed);
	}

	function launchAttack(selfPos : Vector2, playerPos : Vector2)
	{
		//Calculate the direction vector and create the bullet
		var directionVector = playerPos - selfPos;
		directionVector.Normalize();

		facingDirection = animationManager.updateAttackAnimation(directionVector);
		
		//instantiates selected attack
		var bulletInstance : GameObject;
		if(attack.startInMousePos)
		{
			bulletInstance = Instantiate(Resources.Load("Prefabs/BulletPrefab"),playerPos, worldPos.rotation);
		}
		else
		{
			var offset : Vector2 = Vector2(facingDirection.x*0.2,facingDirection.y*0.2);
			bulletInstance = Instantiate(Resources.Load("Prefabs/BulletPrefab"),worldPos.position+offset, worldPos.rotation);
		}

		var bullet : Bullet;
		bullet = bulletInstance.GetComponent(Bullet);
		bullet.Initialize(attack.speed, directionVector , this, attack.sprite, attack.power+attackPower, attack.startInMousePos, attack.animationInterval);
	}
	
	public function destroyEnemy()
	{
		mapRef.addScoreToPlayer(1000);
		mapRef.numOfEnemies--;

		if(Random.Range(0, 10) > 4 || enemyType == typeOfEnemy.GreenKnight)
		{
			//Generate a power-up here
			var powerup : PowerUp = mapRef.InstantiateMapObject(transform.position,typeOfScript.PowerUp);
			mapRef.objectList.Add(powerup);
			var rand = Random.Range(0, 10);
			if(rand > 9)
				powerup.Initialize("Images/Items/speed",mapRef,typeOfPowerUp.SpeedBuff, Mathf.Floor(Random.Range(10, 30)));
			else if(rand > 8)
				powerup.Initialize("Images/Items/life",mapRef,typeOfPowerUp.ExtraLife, Mathf.Floor(Random.Range(10, 20)));
			else if(rand > 7)
				powerup.Initialize("Images/Items/energy",mapRef,typeOfPowerUp.Energy, Mathf.Floor(Random.Range(10, 30)));
			else if(rand > 5)
				powerup.Initialize("Images/Items/armor",mapRef,typeOfPowerUp.Armor, Mathf.Floor(Random.Range(10, 30)));
			else 
				powerup.Initialize("Images/Items/health",mapRef,typeOfPowerUp.Health, Mathf.Floor(Random.Range(10, 50)));

			mapRef.numOfPowerUps++;
		}

		DestroySelf();
	}
	
	function Initialize(sprite : String, mapRef, type : typeOfEnemy, player : GameObject)
	{
		this.mapRef = mapRef;
		this.player = player;
		enemyType = type;

		if(enemyType == typeOfEnemy.GreenKnight)
		{
			maxHealth = 250;
			walkSpeed = 0.8;
			attackPower = 40;
		}
		else if(enemyType == typeOfEnemy.Ogre)
		{
			maxHealth = 80;
			walkSpeed = 1.3;
			attackPower = 10;
		}
		else if(enemyType == typeOfEnemy.Skeleton)
		{
			maxHealth = 160;
			walkSpeed = 1;
			attackPower = 20;
		}

		health = maxHealth;
		
		generateAnimationData(sprite, false);
					
		var enemyCollider : PolygonCollider2D = gameObject.AddComponent.<PolygonCollider2D>(); // assigns the collider to detect collisions
		enemyCollider.isTrigger = true; // if we assign the polygonCollider last, it will auto adjust to sprite size	
		return gameObject;
	}
	
	function OnGUI()
	{
		if(showHealthBar)
		{
			var screenPos : Vector3 = Camera.main.WorldToScreenPoint(worldPos.position);
			//Debug.Log(screenPos);
			GUI.DrawTexture(Rect(screenPos.x-11,Screen.height-screenPos.y+20,25*health/maxHealth,8),Resources.Load.<Texture>("Interface/barFillHealth"));
		}
	}
	
	function OnBecameVisible()
	{
		showHealthBar = true;
	}
	
	function OnBecameInvisible()
	{
		showHealthBar = false;
	}
	
}
