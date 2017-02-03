/*Every power up created in field will have an instance of this class. It's in charge of applying it's own effect when player steps on it.
NOTE: The reason we use enum to initialize and run certain effects is explained under Atttack.js script.
*/

#pragma strict

class PowerUp extends MapObject
{		
	var powerUpType : typeOfPowerUp;	
	private var endTime : float;
	
	function applyPowerUp(playerScript : Player)
	{
		//behaviour of power up depends on it's type
		
		if(powerUpType == typeOfPowerUp.SpeedBuff)
			playerScript.applySpeedChange(2,10);
		else if(powerUpType == typeOfPowerUp.Armor)
			playerScript.setArmor(playerScript.getArmor() + 50);
		else if(powerUpType == typeOfPowerUp.Health)
			playerScript.changeHealth(50);
		else if(powerUpType == typeOfPowerUp.Energy)
			playerScript.changeMana(50);
		else if(powerUpType == typeOfPowerUp.ExtraLife)
			GameObject.Find("Interface").GetComponent(PlayInterface).addLife();
			
		DestroySelf(); // Maybe put this inside each if depending if we want some power ups to not vanish after use -- naahh
		mapRef.numOfPowerUps--;
	}
	
	//triggers when some other collider enters this object's collider
	function OnTriggerEnter2D(collider : Collider2D)
	{
		if(collider.gameObject.tag == "Player") // makes sure it was the player that entered the collider
		{
			var playerScript : Player = collider.gameObject.GetComponent.<Player>();	
			applyPowerUp(playerScript);
		}
	}
	
	function Initialize(sprite : String, mapRef, type : typeOfPowerUp, duration : float)
	{
		this.mapRef = mapRef;
		powerUpType = type;
		endTime = Time.time+duration;
		
		gameObject.GetComponent(SpriteRenderer).sprite = Resources.Load.<Sprite>(sprite);
		
		var enemyCollider : PolygonCollider2D = gameObject.AddComponent.<PolygonCollider2D>(); // assigns the collider to detect collisions
		enemyCollider.isTrigger = true; // if we assign the polygonCollider last, it will auto adjust to sprite size
		
		if(duration)
			this.mapRef.setElapsedTimeListener(endTime,DestroySelf,this);	// sets power up to destroy itself after this time	
		return gameObject;
	}
	
	function OnGUI()
	{
		var screenPos : Vector2 = Camera.main.WorldToScreenPoint(worldPos.position);
		screenPos.y -= 12;
		screenPos.x -= 12;
		GUI.skin.box.padding.top = -1;
		GUI.skin.box.alignment = TextAnchor.UpperCenter;
		var time : int = endTime-Time.time;
		GUI.Box(Rect(screenPos.x,Screen.height-screenPos.y,30,15),time.ToString());
	}
}