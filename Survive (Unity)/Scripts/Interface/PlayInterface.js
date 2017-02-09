/*This is the script in charge of showing the interface during the game. This includes lives, time, health, energy etc.
As well as the play game bgm, and the menu to pause and continue game, as well as game over.
*/

#pragma strict

class PlayInterface extends MonoBehaviour
{
	var headerGuiStyle : GUIStyle;
	var guiStyle : GUIStyle;
	
	private var lives : int;
	private var background : Texture;
	private var background2 : Texture;
	private var title : Texture;
	private var barBackground : Texture;
	private var healthBar : Texture;
	private var manaBar : Texture;
	private var armorBar : Texture;
	private var darken : Texture;
	private var skull : Texture;
	private var heart : Texture;
	private var blackHeart : Texture;
	private var circle : Texture;
	private var attackRadiusCircle : Texture;
	private var attackRadiusCircleRed : Texture;

	private var attack_bolt : Texture;
	private var attack_ice : Texture;
	private var attack_fire : Texture;
	private var attack_quake : Texture;

	private var square : Texture;
	
	private var startTime : float;
	private var timeScale : float;
	private var maxLives : int = 5;
	private var showRange : boolean = false;
	private var showRangeTime : float = 0;
	private var audioClip : AudioClip;
	private var audioSource : AudioSource;
	private var typingName : boolean = false;
	private var playerName : String;
	var showAttackRadius : boolean = false;
	var attackRadiusValid : boolean = false;
	var player : Player;
	var mapRef : Map;
	
	function Start () 
	{
		background = Resources.Load.<Texture>("Interface/background");
		background2 = Resources.Load.<Texture>("Interface/background2");
		title = Resources.Load.<Texture>("Interface/title");
		barBackground = Resources.Load.<Texture>("Interface/barBackground");
		healthBar = Resources.Load.<Texture>("Interface/barFillHealth");
		manaBar = Resources.Load.<Texture>("Interface/barFillMana");
		armorBar = Resources.Load.<Texture>("Interface/barFillArmor");
		darken = Resources.Load.<Texture>("Interface/darkenOnPause");
		skull = Resources.Load.<Texture>("Interface/skull");
		heart = Resources.Load.<Texture>("Interface/heart");
		blackHeart = Resources.Load.<Texture>("Interface/blackHeart");
		circle = Resources.Load.<Texture>("Interface/circle");
		attackRadiusCircle = Resources.Load.<Texture>("Interface/attackRadiusCircle");
		attackRadiusCircleRed = Resources.Load.<Texture>("Interface/attackRadiusCircle_red");

		attack_bolt = Resources.Load.<Texture>("Interface/attack_bolt");
		attack_ice = Resources.Load.<Texture>("Interface/attack_ice");
		attack_fire = Resources.Load.<Texture>("Interface/attack_fire");
		attack_quake = Resources.Load.<Texture>("Interface/attack_quake");

		square = Resources.Load.<Texture>("Interface/square");

		if(Random.Range(0,50)%2) // randomly selects the background music for the stage
			audioClip = Resources.Load.<AudioClip>("Sounds/stage1");
		else
			audioClip = Resources.Load.<AudioClip>("Sounds/stage2");
		audioSource = gameObject.GetComponent(AudioSource);
		audioSource.clip = audioClip;
		audioSource.Play();
		audioSource.volume = (mapRef.gameMain.bgm/100)*0.3;
		startTime = Time.time;
		lives = mapRef.gameMain.lives;
		lives = mapRef.gameMain.lives-1;
		playerName = "name";
	}

	function Update () 
	{
		if(Input.GetKeyDown(KeyCode.Escape))
		{
			if(Time.timeScale != 0)
			{
				timeScale = Time.timeScale;
				Time.timeScale = 0;
				audioSource.Pause();
			}
			else
			{
				Time.timeScale = timeScale;
				audioSource.Play();
			}
		}
		if(player.getHealth() == 0)		
			audioSource.Pause();
		if(Input.GetKeyDown(KeyCode.Space))
		{
			if(showRange == false)
			{
				showRange = true;
				showAttackRadius = true;
				showRangeTime = Time.time;
			}
			else
			{
				deactivateRange();
			}

		}
		if(Input.GetMouseButtonUp(1) || (Time.time - showRangeTime) > 4)
		{
			deactivateRange();
		}
	}
	
	function deactivateRange()
	{
		showRangeTime = float.MaxValue;
		showRange = false;
		showAttackRadius = false;
	}
	
	function addLife()
	{
		if(lives <5)
			lives++;
	}

	function getStartTime()
	{
		return startTime;
	}
	
	private function getTime()
	{
		var minutes : int = (Time.time-startTime)/60;
		var seconds : int = (Time.time-startTime)%60;
		var sMin = minutes.ToString();
		var sSec = seconds.ToString();
		if(minutes<10)
			sMin = "0" + minutes.ToString();
		if(seconds<10)
			sSec = "0" + seconds.ToString();
		return sMin + ":" + sSec;
	}
	
	function OnGUI()
	{
		// main window group
		GUI.BeginGroup(Rect(Screen.width-345,0,380,340)); // only need to change this one to change position of the rest
		{ // brackets are not needed but they help knowing what is going on
			GUI.DrawTexture(Rect(64,30,290,310),background);
			GUI.DrawTexture(Rect(50,0,290,70),title);
			GUI.Box(Rect(68,24,250,40),"Survive",headerGuiStyle); 
			GUI.Box(Rect(80,70,60,30),"Time : " + getTime(), guiStyle); // time
			GUI.Box(Rect(80,100,100,30),"Score : " + player.getScore(),guiStyle); // score
			GUI.Box(Rect(80,130,50,30),"Lives : ",guiStyle); // lives
			for(var i :int = 0 ; i < lives ;i++)
				GUI.DrawTexture(Rect(140+ i*28,127,25,25),heart);
			GUI.BeginGroup(Rect(80,160,290,50)); // group for health bar
			{
				GUI.Box(Rect(0,0,60,30),"Health : ", guiStyle);
				GUI.DrawTexture(Rect(65,-3,160,25),barBackground);
				GUI.DrawTexture(Rect(73,2,145*player.getHealth()/100,13.8),healthBar);
			}
			GUI.EndGroup();
			GUI.BeginGroup(Rect(80,190,290,50)); // group for mana bar
			{
				GUI.Box(Rect(0,0,60,30),"Energy : ", guiStyle);
				GUI.DrawTexture(Rect(65,-3,160,25),barBackground);
				GUI.DrawTexture(Rect(73,2,145*player.getMana()/100,13.8),manaBar);
			}
			GUI.EndGroup();
			GUI.BeginGroup(Rect(80,220,290,50)); // group for armor bar
			{
				GUI.Box(Rect(0,0,60,30),"Armor : ", guiStyle);
				GUI.DrawTexture(Rect(65,-3,160,25),barBackground);
				GUI.DrawTexture(Rect(73,2,145*player.getArmor()/100,13.8),armorBar);
			}
			GUI.EndGroup();
			GUI.BeginGroup(Rect(80,230,290,70)); // group for special attacks
			{			
				GUI.Box(Rect(0,35,60,30),"Attacks : ", guiStyle);
				GUI.DrawTexture(Rect(10+60, 20,46,46),attack_bolt);
				GUI.DrawTexture(Rect(56+60, 20,46,46),attack_ice);
				GUI.DrawTexture(Rect(102+60, 20,46,46),attack_fire);
				GUI.DrawTexture(Rect(148+60, 20,46,46),attack_quake);
				GUI.DrawTexture(Rect(10+60 + 46 * (player.getSelectedAttackIndex()-1), 20,46,46),square);
			}
			GUI.EndGroup();
			GUI.Box(Rect(80,300,50,30),"Speed : \t" + player.walkSpeed,guiStyle); // speed 
		}
		GUI.EndGroup();
		if(showRange)
			GUI.DrawTexture(Rect(Screen.width/2-51*player.castDistance,Screen.height/2-51*player.castDistance,103*player.castDistance,103*player.castDistance),circle);
		if(showAttackRadius)
			if(attackRadiusValid)
				GUI.DrawTexture(Rect(Input.mousePosition.x-25,Screen.height-Input.mousePosition.y-25,50,50),attackRadiusCircle);
			else
				GUI.DrawTexture(Rect(Input.mousePosition.x-25,Screen.height-Input.mousePosition.y-25,50,50),attackRadiusCircleRed);
		
		if(!Time.timeScale) // if game time is paused
		{
			GUI.DrawTexture(Rect(0,0,Screen.width,Screen.height),darken);
			if(player.getHealth() == 0) // if player is dead
			{
				GUI.BeginGroup(Rect((Screen.width/2)-100,(Screen.height/2)-70,200,300));
				{
					GUI.DrawTexture(Rect(0,15,200,200),background2);
					GUI.DrawTexture(Rect(50,0,100,30),skull);
					GUI.Box(Rect(60,35,80,40),"You Died",headerGuiStyle);
					for(var p :int = 0 ; p < maxLives ;p++) // draws the hearts
						if(p >= lives)
							GUI.DrawTexture(Rect(32+ p*28,72,25,25),blackHeart);
						else
							GUI.DrawTexture(Rect(32+ p*28,72,25,25),heart);
					if(lives > 0) // only show continue button if we have lives
					{
						if(GUI.Button(Rect(60,100,80,40),"Continue"))
						{
							player.changeHealth(100);
							player.setFlickering(17,0.1);
							Time.timeScale = 1;
							audioSource.Play();
							player.playingDeathSound = false;
							lives--;
						}
					}
					else	
						GUI.Box(Rect(60,100,80,40),"Game Over",headerGuiStyle);
					if(!typingName)
					{
						if(GUI.Button(Rect(60,150,80,40),"Quit Game"))
						{
							typingName = true;
							audioSource.clip = Resources.Load.<AudioClip>("Sounds/gameOver");
							audioSource.loop = false;
							audioSource.Play();
						}
					}
					else
					{
						playerName = GUI.TextField(Rect(40,145,115,25),playerName,15);
						if(GUI.Button(Rect(125,172,60,25),"Quit"))
	 					{
							if(playerName == "name")
								playerName = "Unknown";
							GameObject.Find("GamePlayData").GetComponent(GameMain).rankManager.addScore(player.getScore(),getTime(),playerName);
							Time.timeScale = 1;
							Application.LoadLevel("MainMenu");
						}
					}
				}
				GUI.EndGroup();
			}
			else // if player paused game
			{
				GUI.BeginGroup(Rect((Screen.width/2)-100,(Screen.height/2)-70,200,190));
				{
					GUI.DrawTexture(Rect(0,15,200,140),background2);
					GUI.DrawTexture(Rect(50,0,100,30),skull);
					GUI.Box(Rect(60,35,80,40),"PAUSED",headerGuiStyle);
					if(typingName)
					{
						playerName = GUI.TextField(Rect(40,80,115,25),playerName,15);
						if(GUI.Button(Rect(125,115,60,25),"Submit"))
						{
							if(playerName == "name")
								playerName = "Unknown";
							GameObject.Find("GamePlayData").GetComponent(GameMain).rankManager.addScore(player.getScore(),getTime(),playerName);
							Time.timeScale = 1;
							Application.LoadLevel("MainMenu");
						}
					}
					else
					{
						if(GUI.Button(Rect(60,85,80,40),"Quit Game"))
						{
							typingName = true;
							audioSource.clip = Resources.Load.<AudioClip>("Sounds/gameOver");
							audioSource.loop = false;
							audioSource.Play();
						}
					}
				}
				GUI.EndGroup();
			}
		}
	}
}











