/*Every attack once launched creates an instance of this class. This is the class that is actually attatched to a game object that we
will see moving in the Screen. It's also in charge of identifying who it collided with and calling the function to deal damage.
*/

#pragma strict

public class Bullet extends MonoBehaviour
{
	private var direction : Vector2;
	private var speed : float;
	private var owner;
	private var elapsedTime : float;
	private var animationManager : AnimationManager;
	private var power: int;
	private var destroyOnAnimationEnd : boolean;
	private var animationInterval : float;
	private var collidersHit : ArrayList;

	function Start () 
	{
		
		collidersHit = new ArrayList();
	}

	function Update () 
	{
		var val : int = animationManager.updateBulletAnimation(animationInterval);
		if(destroyOnAnimationEnd && val == 0)
			Destroy(gameObject,animationInterval);
		gameObject.transform.Translate(direction * Time.deltaTime * speed);
		elapsedTime += Time.deltaTime;
		if(elapsedTime > 10)
			Destroy(gameObject);
	}
	
	function DestroySelf()
	{
		//run anything we need before destroying bullet
		Destroy(gameObject);
	}
	
	function HitObstacle()
	{
		//in case we want to add any extra effects on hitting obstacle
		DestroySelf();
	}
	
	function Initialize(spd : float, dir : Vector2, own, sprite : String, power : int, destroyOnAnimationEnd : boolean,animationInterval : float)
	{
		this.power = power;
		speed = spd;
		direction = dir;
		owner = own;
		this.destroyOnAnimationEnd = destroyOnAnimationEnd;
		this.animationInterval = animationInterval;
		
		this.animationManager = new AnimationManager(sprite, gameObject.GetComponent(SpriteRenderer), true, gameObject);
		
		var bulletCollider : PolygonCollider2D = gameObject.AddComponent.<PolygonCollider2D>(); // assigns the collider to detect collisions
		bulletCollider.isTrigger = true; // if we assign the polygonCollider last, it will auto adjust to sprite size
	}
	
	function OnTriggerEnter2D(collider : Collider2D)
	{
		if(collider.gameObject.tag == "Obstacle")
		{
			//applies an offset to not destroy the bullet if he's on the upper edge of an obstacle
			var pos : Vector3 = gameObject.transform.position;
			pos.y += 0.1;
			var layer : LayerMask = 1<<8;
			var rayHit : RaycastHit2D = Physics2D.Raycast(pos,direction,2,layer);
			if(rayHit != null)			
				if(collider == rayHit.collider)
					if(!destroyOnAnimationEnd)
						DestroySelf();			
		}
		if(owner.GetType().Name == "Player") // checks who owns the bullet
		{
			if(collider.gameObject.tag == "Enemy") // checks if it collided with an enemy
			{
				if(!collidersHit.Contains(collider))
				{	
					collidersHit.Add(collider);
					var enemyScript : Enemy = collider.gameObject.GetComponent.<Enemy>();
					enemyScript.changeHealth(-power);
				}
				if(!destroyOnAnimationEnd)
					DestroySelf();
			}
		}
		else if(owner.GetType().Name == "Enemy")
		{
			if(collider.gameObject.tag == "Player") // checks if it collided with an enemy
			{
				if(!collidersHit.Contains(collider))
				{	
					collidersHit.Add(collider);
					var playerScript  : Player = collider.gameObject.GetComponent.<Player>();
					playerScript.changeHealth(-power);
				}
				if(!destroyOnAnimationEnd)
					DestroySelf();
			}
		}	
	}
}