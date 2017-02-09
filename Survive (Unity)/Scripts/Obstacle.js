/*Simple obstacle class that creates the object on screen with given sprite and assigns it a collider to use physics with.
*/

#pragma strict

class Obstacle extends MapObject
{
	var obstacleType : typeOfObstacle;
	var rend : SpriteRenderer;

	
	function Initialize(sprite : String, mapRef, type : typeOfObstacle)
	{
		this.mapRef = mapRef;
		obstacleType = type;
		rend.sprite = Resources.Load.<Sprite>(sprite);
		
		var obstacleCollider : PolygonCollider2D = gameObject.AddComponent.<PolygonCollider2D>(); // assigns the collider to detect collisions
		obstacleCollider.isTrigger = true; // if we assign the polygonCollider last, it will auto adjust to sprite size	
		return gameObject;
	}
}