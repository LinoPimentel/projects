/*The reason on why this exists is explained on the Attack.js header comment*/

#pragma strict

//declare each power up here so we don't need to use int / char codes directly as power up
enum typeOfPowerUp
{
	Armor,
	SpeedBuff,
	Health,
	Energy,
	ExtraLife
}

//same as above but for obstacles
enum typeOfObstacle
{
	Rock1x1
}

//same as above for enemies
enum typeOfEnemy
{
	GreenKnight,
	Ogre,
	Skeleton
}

//for attacks
enum typeOfAttack
{
	DefaultBullet,
	Bolt,
	Fire,
	Ice,
	Quake
}

enum typeOfScript
{
	Enemy,
	Obstacle,
	PowerUp
}