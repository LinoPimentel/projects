/*This is the class in charge of recieving all the player input and replying accordingly.
There is only 1 instance of this class and it's assigned to the player object.
Both this and the enemy, even though both use attacks, they have their own launch attack function. The reason for this is to save
extra time needed to check if we're on a player or on an enemy, since they have different behaviours.
This class is also in charge of playing the sound effects related to the player, like attack sounds and getting hit.
*/

#pragma strict

public class Player extends MapObject
{
	private var currentScore : int;	
	private var layerMask : LayerMask;	
	private var attackList : Attack[];	
	private var selectedAttack : Attack;
	private var selectedAttackIndex : int;
	private var walkDirection : Vector2;
	private var inAtkAnimation : boolean;
	private var mana : int;
	private var manaRegen : int;
	private var manaRegenSpeed : float;
	private var previousSpeed : int;
	var playingDeathSound : boolean = false; 
	var castDistance : int;
	var playInterface : PlayInterface;


	function Start () 
	{
		walkDirection = new Vector2(0,0);
		//loads the attack list
		attackList = new Attack[5];
		attackList[0] = new Attack(typeOfAttack.DefaultBullet,this,"Images/magic_attack",0.2);
		attackList[1] = new Attack(typeOfAttack.Bolt,this,"Images/magic_attack/bolt",0.1);
		attackList[2] = new Attack(typeOfAttack.Ice,this,"Images/magic_attack/ice",0.1);
		attackList[3] = new Attack(typeOfAttack.Fire,this,"Images/magic_attack/fire",0.1);
		attackList[4] = new Attack(typeOfAttack.Quake,this,"Images/magic_attack/quake",0.1);
		selectedAttackIndex = 1;
		castDistance = attackList[selectedAttackIndex].range;
		layerMask =  1<<8;
		generateAnimationData("Images/player",false); // only passes the folder where all the animations are at
		facingDirection = Vector2.right;
		inAtkAnimation = false;
		health = 100;
		mana = 100;
		manaRegen = 5;
		manaRegenSpeed = 5;
		mapRef.setElapsedTimeListener(Time.time+manaRegenSpeed,regenMana,this);
		currentScore = 0;
		isInvulnerable = false;
		setFlickering(17,0.1);
		isImobile = false;
		playInterface = GameObject.Find("Interface").GetComponent(PlayInterface);
	}
	
	function Update () 
	{
		if(!Time.timeScale)
			return;		
		if(flickerAmmount > 0)
			flickerSprite();
		
		if(health <= 0)
		{
			if(!playingDeathSound)
			{
				AudioSource.PlayClipAtPoint(Resources.Load.<AudioClip>("Sounds/death"),worldPos.position,1f);
				playingDeathSound = true;
			}
			var done = animationManager.updateDeadAnimation();
			if(done == 1 ) // finished running dead animation, pause game
				Time.timeScale = 0;
			return;
		}

		//check if the attack is valid
		if(playInterface.showAttackRadius)
		{
			var mousePos : Vector3 = Camera.main.ScreenToWorldPoint(Input.mousePosition);
			mousePos.z = 0;
			var distance : float = Vector2.Distance(mousePos,worldPos.position);
			if(distance <= castDistance) // we are in cast range
			{ 
				var rayHit : RaycastHit2D = Physics2D.Raycast(worldPos.position,mousePos-worldPos.position,distance,1<<8);
				if(rayHit.collider != null)
					playInterface.attackRadiusValid = false;
				else
					playInterface.attackRadiusValid = true;
			}
			else
				playInterface.attackRadiusValid = false;
		}

		if(inAtkAnimation || (Input.GetMouseButtonDown(1) && mana > 0) || Input.GetMouseButton(0))	
		{		
			if(Input.GetMouseButtonDown(1))
			{
				if(!playInterface.showAttackRadius)
					return;

				mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
				mousePos.z = 0;
				distance = Vector2.Distance(mousePos,worldPos.position);
				if(distance <= castDistance) // we are in cast range
				{ 
					rayHit = Physics2D.Raycast(worldPos.position,mousePos-worldPos.position,distance,1<<8);
					if(rayHit.collider != null)
					{
						playInterface.attackRadiusValid = false;
						return; // we dont have line of sight
					}
					playInterface.attackRadiusValid = true;				
					selectedAttack = attackList[selectedAttackIndex];
				}
				else return; // ignore mouse event
			}
			else if(Input.GetMouseButton(0))
				selectedAttack = attackList[0];
			launchAttack(selectedAttack);	
			return;
		}	
		var wheel : float = Input.GetAxis("Mouse ScrollWheel");
		if(wheel > 0)
		{
			selectedAttackIndex = (selectedAttackIndex+1) % 5;
			if(selectedAttackIndex == 0)
				selectedAttackIndex++;
			castDistance = attackList[selectedAttackIndex].range;
		}
		else if(wheel < 0)
		{
			selectedAttackIndex--;
			if(selectedAttackIndex == 0)
				selectedAttackIndex = 4;
			castDistance = attackList[selectedAttackIndex].range;
		}
		
		//runs all of the ifs instead of breaking midway to check for multiple key presses to allow diagonal movement
		walkDirection = Vector2.zero;
		if(Input.GetKey(KeyCode.S))
			walkDirection = walkDirection + Vector2.down;
		if(Input.GetKey(KeyCode.A))
			walkDirection = walkDirection + Vector2.left;
		if(Input.GetKey(KeyCode.D))
			walkDirection = walkDirection + Vector2.right;
		if(Input.GetKey(KeyCode.W))
			walkDirection = walkDirection + Vector2.up;
			
		if(!isImobile && walkDirection != Vector2.zero)		
			moveObject(walkDirection,layerMask,worldPos,0.2,walkSpeed);
		else
			facingDirection = animationManager.updateIdleAnimation(facingDirection);	
	}

	function getSelectedAttackName()
	{
		switch(selectedAttackIndex)
		{
			case 1 : return "Bolt";
			case 2 : return "Ice";
			case 3 : return "Fire";
			case 4 : return "Quake";
		}
	}

	function getSelectedAttackIndex()
	{
		return selectedAttackIndex;
	}
	
	function getScore()
	{
		return currentScore;
	}
	
	function addToScore(ammount : int)
	{	
		currentScore += ammount;
	}
	
	function getMana()
	{
		return mana;
	}
	
	function changeMana(ammount : int)
	{
		mana += ammount;
		if(mana < 0)
			mana = 0;
		else if(mana > 100)
			mana = 100;
	}
	
	function regenMana()
	{
		changeMana(manaRegen);
		mapRef.setElapsedTimeListener(Time.time+manaRegenSpeed,regenMana,this);
	}
	
	function launchAttack(atk : Attack)
	{
		//gets direction on mouse position
		var mousePos : Vector3 = Camera.main.ScreenToWorldPoint(Input.mousePosition);
		mousePos.z = 0;
		mousePos = mousePos - worldPos.position;
		
		var dir : Vector2;
		dir = animationManager.updateAttackAnimation(mousePos);
		if(dir != Vector2.zero) // makes sure to not launch the bullet until the animation ends
		{
			facingDirection = dir;
			inAtkAnimation = true;
			return;
		}
		inAtkAnimation = false;
		//instantiates selected attack
		var bulletInstance : GameObject;
		if(atk.startInMousePos) // for special attacks
		{
			mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
			mousePos.z = 0;
			bulletInstance = Instantiate(Resources.Load("Prefabs/BulletPrefab"),mousePos, worldPos.rotation);
		}
		else // fore default attacks
		{
			var offset : Vector2 = Vector2(facingDirection.x*0.2,facingDirection.y*0.2);
			bulletInstance = Instantiate(Resources.Load("Prefabs/BulletPrefab"),worldPos.position+offset, worldPos.rotation);
		}
		var bullet : Bullet;
		bullet = bulletInstance.GetComponent(Bullet);
		//initializes the bullet, do any function calls to the bullet script here
		bullet.Initialize(atk.speed, mousePos.normalized , this, atk.sprite, atk.power, atk.startInMousePos, atk.animationInterval);
		AudioSource.PlayClipAtPoint(atk.audioClip,worldPos.position,mapRef.gameMain.sfx/100); // change the volume here to be the one on game main
		changeMana(atk.manaCost);
	}
	
	// function to change the movement speed of the player
	public function applySpeedChange(spdValue : int, duration : float)
	{
		previousSpeed = walkSpeed;
		walkSpeed = walkSpeed + spdValue;
		mapRef.setElapsedTimeListener(Time.time+duration,revertSpeed,this);		
	}
	
	function revertSpeed()
	{
		walkSpeed = previousSpeed;
	}
	
	function OnTriggerEnter2D(collider : Collider2D)
	{
		if(!isInvulnerable)
			if(collider.gameObject.tag == "Enemy") // knock back function
			{	
				var vec : Vector2;
				vec.x -= facingDirection.x;
				vec.y -= facingDirection.y;
				var distance : int = 20;
				do
				{	
					var moved = moveObject(vec,layerMask,worldPos,0.2,distance);
					distance -= 5;
				}while(moved == 0 && distance > 0);
				changeHealth(-10);
				isImobile = true;
				mapRef.setElapsedTimeListener(Time.time+0.3,toggleImobile,this.gameObject);
				setFlickering(10,0.1);
			}
	}
}