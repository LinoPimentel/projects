/*This is the root class of Obstacle, Player, Enemy, PowerUp as it contains some functions all of them need, mainly the function to move an object.
Function to change one's health, flicker the sprite and destroy the object.
*/

#pragma strict

public class MapObject extends MonoBehaviour
{
	protected var health : int;
	protected var maxHealth : int = 100;
	
	var worldPos : Transform;
	var mapRef : Map;
	var pos : Vector2;
	var walkSpeed : float;
	
	protected var flickerAmmount : int;
	private var flickerTime : float;
	private var nextFlickerTime : float;
	var bumpSound : AudioClip;
	protected var animationManager : AnimationManager;
	protected var facingDirection : Vector2;
	protected var isInvulnerable : boolean;
	protected var isImobile : boolean;
	protected var armor : int = 0;
	
	function toggleInvulnerable()
	{
		isInvulnerable = !isInvulnerable;
	}
	
	function toggleImobile()
	{
		isImobile = !isImobile;
	}
	
	function changeHealth(ammount : int)
	{	
		if(ammount < 0 && !isInvulnerable) // if we took damage
		{
			if(gameObject.tag == "Player")
				AudioSource.PlayClipAtPoint(bumpSound,worldPos.position,mapRef.gameMain.sfx/100);
			setFlickering(9,0.05);
			if(armor > 0)
			{
				ammount = ammount * (((100-armor)/200) + 0.5);
				armor += ammount / 3.0; //remember, amount is negative!
				if(armor < 0)
					armor = 0;
			}
			health += ammount; // apply damage
			if(health <= 0) // check if enemy died
			{
				health = 0;
				if(gameObject.tag == "Enemy")
					(this as Enemy).destroyEnemy();
			}
		}
		else if(ammount > 0) // applies heal, this is here because healing bypasses invulnerable
		{
			health += ammount;
			if(health > maxHealth)
				health = maxHealth;		
		}			
	}	
	
	function flickerSprite()
	{
		if(nextFlickerTime <= Time.time)
		{
			animationManager.toggleRendering();
			flickerAmmount--;
			nextFlickerTime = Time.time + flickerTime;
		}		
	}
	
	function setFlickering(ammount : int, interval : float)
	{
		if(flickerAmmount == 0)
		{
			if(ammount % 2 == 1)
				ammount++;
			flickerAmmount = ammount;
			flickerTime = interval;
			nextFlickerTime = Time.time;
			if(gameObject.tag == "Player") // if its player make him invulnerable for few miliseconds
			{
				isInvulnerable = true;
				mapRef.setElapsedTimeListener(Time.time+(ammount*interval),toggleInvulnerable,this);
			}		
			flickerSprite();
		}
	}
	
	function getHealth()
	{
		return health;
	}

	function setArmor(armor : int)
	{
		if(armor > 100)
			armor = 100;
		this.armor = armor;
	}

	function getArmor()
	{
		return armor;
	}
	
	function DestroySelf() 
	{
		//notify the map to remove this from it's list
		mapRef.removeFromList(this);
		//call this to destroy the object the script is attached to
		Destroy(gameObject);
	}
	
	// generates and loads all the data necessary for animations and sprites
	function generateAnimationData(sprite : String, updateColliderOnSpriteChange : boolean)
	{
		this.animationManager = new AnimationManager(sprite, gameObject.GetComponent(SpriteRenderer), updateColliderOnSpriteChange, gameObject);
	}
		
	//function to move in direction taking into account the speed and deltaTime (delta time takes into account the frames)
	function moveObject(direction : Vector2, layerMask : LayerMask, objectTransform : Transform, circleRadius : float, speed : float)
	{
		if(direction == Vector2.zero)
			return;
		var walkTo : Vector2 = direction * Time.deltaTime * speed;	
		facingDirection = animationManager.updateWalkAnimation(walkTo.normalized);
		//casts a circleCast with radius of the furthest away Y point. 
		//We also apply a position offset in the circle to make it so that it detects more to the top than to the bottom
		var pos : Vector3 = objectTransform.position;
		pos.y += 0.1;
		
		var rayHit : RaycastHit2D = Physics2D.CircleCast(pos,circleRadius,direction,2,layerMask);
		if(rayHit.collider != null)
		{
			if(rayHit.distance > walkTo.magnitude)
				objectTransform.Translate(walkTo);	
			else
			{			
				//changes direction if we're in a dead end and checks if we can move in that new direction
				direction = (rayHit.normal+direction).normalized;	
				rayHit = Physics2D.CircleCast(pos,circleRadius,direction,2,layerMask);
				walkTo = direction*Time.deltaTime*speed;
				if(rayHit.collider == null)
				{
					objectTransform.Translate(walkTo);
					return 0; // return 0 for knockback, only place that needs to know
				}
				else if(rayHit.distance > walkTo.magnitude)				
					objectTransform.Translate(walkTo);			
				else				
					objectTransform.Translate(rayHit.normal*Time.deltaTime*0.3);
					//this still needs to exist because by using Translate to move our object, sometimes it will move inside a collider and would be locked without this
			}		
		}
		else
			objectTransform.Translate(walkTo);
		return 1;
	}
}