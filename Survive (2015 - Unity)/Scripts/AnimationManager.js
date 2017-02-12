/*This class has 1 main objective. Running sprite animations. There are 4 possible animations in this class, walking, idle, attacking, and bullet Animations.
Call the function for the animation you want every time you want to check if sprite should change
*/

#pragma strict

class AnimationManager
{
	private var walkSpriteAnimations : Sprite[];
	private var deadSpriteAnimations : Sprite[];
	private var atkSpriteAnimations : Sprite[];
	private var idleAnimationSpeed : float;
	private var walkAnimationSpeed : float;
	private var atkAnimationSpeed : float;	
	private var deadAnimationSpeed : float;	
	
	private var nextWalkSprite : int;
	private var nextIdleSprite : int;
	private var nextAtkSprite : int;
	private var nextDeadSprite : int;
	
	private var nextSpriteChangeTimeWalk : float;
	private var nextSpriteChangeTimeIdle : float;
	private var nextSpriteChangeTimeAtk : float;	
	private var nextSpriteChangeTimeDead : float;

	
	private var updateCollider : boolean;
	private var renderer : SpriteRenderer;
	private var gameObject : GameObject;	
				
	function AnimationManager(folder : String, spriteRenderer : SpriteRenderer, updateCollider : boolean, gameObject : GameObject)
	{
		renderer = spriteRenderer;
		
		walkSpriteAnimations = Resources.LoadAll.<Sprite>(folder + "/walk");
		if(walkSpriteAnimations.Length)		
		{
			walkAnimationSpeed = 0.4/(walkSpriteAnimations.Length / 3); // 3 is the number of sides for animations
			idleAnimationSpeed = walkAnimationSpeed*5;
			nextIdleSprite = 0;
			nextWalkSprite = 0;	
			renderer.sprite = walkSpriteAnimations[0];
		}
		atkSpriteAnimations = Resources.LoadAll.<Sprite>(folder + "/atk");
		if(atkSpriteAnimations.Length)
		{
			atkAnimationSpeed = 0.2/(atkSpriteAnimations.Length / 3);
			nextAtkSprite = 0;
		}
		else // use the walking animation as atk animation if no atk is present
		{
			atkSpriteAnimations =  walkSpriteAnimations;
			atkAnimationSpeed = walkAnimationSpeed/2;
			nextAtkSprite = 0;
		}
		
		deadSpriteAnimations = Resources.LoadAll.<Sprite>(folder + "/dead");
		if(deadSpriteAnimations.Length)
		{
			deadAnimationSpeed = deadSpriteAnimations.Length/3;
			nextDeadSprite = 0;
		}
				
		nextSpriteChangeTimeWalk = 0;
		nextSpriteChangeTimeIdle = 0;
		this.updateCollider = updateCollider;			
		this.gameObject = gameObject;
	}
	
	function toggleRendering()
	{
		if(renderer != null)
			renderer.enabled = !renderer.enabled;
		else
			Debug.Log("was null");
	}
	
	private function getFacingDirectionFromVector(vec : Vector2)
	{
		var angle : float = Vector2.Angle(Vector2.right,vec); // this only returns the abosolute distance as a 0-180 angle
		gameObject.transform.localScale.x = 3;
		if(angle >= 45 && angle < 135) 
		{
			if(vec.y >= 0)
				return 1; // facing up
			else
				return 2; // facing down
		}
		else if(angle >=0 && angle < 45) // this angle will always be facing right no matter if vector is pointing up or down		
			return 0; // facing right
		// last angle would be between 135 and 180, this is also always facing right no matter if vec is pointing up or down
		gameObject.transform.localScale.x = -3; // invert scale to show as left
		return 3; // facing left		
	}
	
	private function getFacingDirectionToVector(direction : int)
	{
		switch(direction)
		{
			case 0: return Vector2.right;break;
			case 1: return Vector2.up;break;
			case 2: return Vector2.down;break;
			case 3: return Vector2.left;break;
		}
	}
	
	function updateBulletAnimation(interval : float)
	{
		if(Time.time > nextSpriteChangeTimeWalk + interval*2) 			
			nextSpriteChangeTimeWalk = Time.time;
			
		if(!walkSpriteAnimations.Length)
			return 1;
		if(Time.time >= nextSpriteChangeTimeWalk)
		{
			updateSpriteRenderer(walkSpriteAnimations[nextWalkSprite]);		
			nextWalkSprite = (nextWalkSprite+1)%(walkSpriteAnimations.Length);
			nextSpriteChangeTimeWalk = Time.time + interval;
			return nextWalkSprite;
		}
		return 1;
	}
	
	function updateDeadAnimation()
	{
		if(Time.time > nextSpriteChangeTimeDead + deadAnimationSpeed*2) 			
			nextSpriteChangeTimeDead = Time.time;
			
		if(!deadSpriteAnimations.Length)
			return;
		if(Time.time >= nextSpriteChangeTimeDead)
		{
			updateSpriteRenderer(deadSpriteAnimations[nextDeadSprite]);		
			nextDeadSprite = (nextDeadSprite+1)%(deadSpriteAnimations.Length);
			nextSpriteChangeTimeDead = Time.time + deadAnimationSpeed/2;
			if(nextDeadSprite == 0)
				return 1;
		}
	}
	
	function updateWalkAnimation(walkDirection : Vector2)
	{
		// if current time is greater than the previous time + double the animation speed, the most likely we're at animation start so reset time
		if(Time.time > nextSpriteChangeTimeWalk + walkAnimationSpeed*2) 	
		{	
			nextSpriteChangeTimeWalk = Time.time;
			nextSpriteChangeTimeAtk = 0;
			nextSpriteChangeTimeIdle = 0;
		}
		var direction : int = getFacingDirectionFromVector(walkDirection);
		if(!walkSpriteAnimations.Length)
			return getFacingDirectionToVector(direction);
		if(Time.time >= nextSpriteChangeTimeWalk)
		{
			var sprDir = direction;
			if(sprDir == 3)
				sprDir = 0;
			updateSpriteRenderer(walkSpriteAnimations[sprDir*(walkSpriteAnimations.Length/3) + nextWalkSprite]);		
			nextWalkSprite = (nextWalkSprite+1)%(walkSpriteAnimations.Length/3);
			nextSpriteChangeTimeWalk = Time.time + walkAnimationSpeed;
		}
		return getFacingDirectionToVector(direction);
	}
	
	function updateIdleAnimation(walkDirection : Vector2)
	{
		if(Time.time > nextSpriteChangeTimeIdle + idleAnimationSpeed) 
		{	
			nextSpriteChangeTimeWalk = Time.time;
			nextSpriteChangeTimeAtk = 0;
			nextSpriteChangeTimeIdle = 0;
		}	
			
		var direction : int = getFacingDirectionFromVector(walkDirection);
		if(!walkSpriteAnimations.Length)
			return getFacingDirectionToVector(direction);			
		if(Time.time >= nextSpriteChangeTimeIdle)
		{
			var sprDir = direction;
			if(sprDir == 3)
				sprDir = 0;
			updateSpriteRenderer(walkSpriteAnimations[sprDir*(walkSpriteAnimations.Length/3) + nextIdleSprite]);		
			nextIdleSprite = (nextIdleSprite+1)%(walkSpriteAnimations.Length/3);
			nextSpriteChangeTimeIdle = Time.time + idleAnimationSpeed;
		}
		return getFacingDirectionToVector(direction);
	}
	
	function updateAttackAnimation(atkDirection : Vector2)
	{
		if(Time.time > nextSpriteChangeTimeAtk + atkAnimationSpeed) 
		{	
			nextSpriteChangeTimeWalk = Time.time;
			nextSpriteChangeTimeAtk = 0;
			nextSpriteChangeTimeIdle = 0;
		}
			
		var direction : int = getFacingDirectionFromVector(atkDirection);
		if(!atkSpriteAnimations.Length)
			return Vector2.zero;		
		if(Time.time >= nextSpriteChangeTimeAtk)
		{
			var sprDir = direction;
			if(sprDir == 3)
				sprDir = 0;
			updateSpriteRenderer(atkSpriteAnimations[sprDir*(atkSpriteAnimations.Length/3) + nextAtkSprite]);		
			nextAtkSprite = (nextAtkSprite+1)%(atkSpriteAnimations.Length/3);
			if(!nextAtkSprite)
				return Vector2.zero; // returns 0 to say animation ended
			nextSpriteChangeTimeAtk = Time.time + atkAnimationSpeed;
		}
		return getFacingDirectionToVector(direction);
	}
	
	private function updateSpriteRenderer(sprite : Sprite)
	{
		renderer.sprite = sprite;	// update the sprite		
		if(updateCollider) // if we changed direction redo our collider
		{
			UnityEngine.Object.Destroy(gameObject.GetComponent.<PolygonCollider2D>());
			var col : PolygonCollider2D = gameObject.AddComponent.<PolygonCollider2D>();
			col.isTrigger = true;
		}
	}
}