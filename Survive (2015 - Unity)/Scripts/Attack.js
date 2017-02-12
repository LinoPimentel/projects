/*Every attack has his own instance of this class with the variables it needs.
Currently this class uses an enum to know which initial values the attack should have according to type.
The other 2 options would be, having a class for each attack, or load values from a File. 

Note: After extensive research on which one games usually adopt, the second one is the prefered type due to not having to recompile the code every time
we want to add an attack, since the compile time of a game is usually very big. It also makes it so anyone can create an attack, and not just programmers.
Given that, we chose to keep the enum version due to this being a small scale project, as there were other things that needed atention.
This same system is used for obstacles, power ups and enemies. And for all of those we should be loading from a file instead.
*/

#pragma strict


public class Attack
{
	var speed : float;
	var sprite : String;
	var power : int;
	var startInMousePos : boolean;
	var animationInterval : float;
	var manaCost : int;
	var range : int;
	var audioClip : AudioClip;
	
	public function Attack(type : typeOfAttack, owner : MapObject, sprite : String, animationInterval : float)
	{
		this.animationInterval = animationInterval;
		this.sprite = sprite;
		switch(type)
		{
			case typeOfAttack.DefaultBullet:
				this.speed = 8;
				this.power = 15;
				manaCost = 0;
				startInMousePos = false;
				audioClip = Resources.Load.<AudioClip>("Sounds/default");
				break;
			case typeOfAttack.Fire:
				speed = 0;
				power = 30;
				manaCost = -20;
				range = 7;
				startInMousePos = true;
				audioClip = Resources.Load.<AudioClip>("Sounds/fire");
				break;
			case typeOfAttack.Ice:
				speed = 0;
				power = 35;
				range = 8;
				manaCost = -35;
				startInMousePos = true;
				audioClip = Resources.Load.<AudioClip>("Sounds/ice");
				break;
			case typeOfAttack.Bolt:
				speed = 0;
				power = 15;
				range = 9;
				manaCost = -15;
				startInMousePos = true;
				audioClip = Resources.Load.<AudioClip>("Sounds/bolt");
				break;
			case typeOfAttack.Quake:
				speed = 0;
				power = 50;
				range = 5;
				manaCost = -20;
				startInMousePos = true;
				audioClip = Resources.Load.<AudioClip>("Sounds/quake");
			break;
		}
	}
}