/*This is the script in charge of all the main menu, this includes showing the menu, allowing to change options, showing high scores, exiting game,
starting new game, playing the mainmenu bgm.
*/

#pragma strict
import System.Xml;
import System.Xml.Serialization;

enum View
{
	MainMenu,
	About,
	Options,
	HighScores,
}

var headerGuiStyle : GUIStyle;
var buttonStyle : GUIStyle;
var leftButton : GUIStyle;
var rightButton : GUIStyle;
var textStyle : GUIStyle;
var sliderBar : GUIStyle;
var sliderHand : GUIStyle;
var boxStyle : GUIStyle;
var audioSrc : AudioSource;

private var title : Texture;
private var background2 : Texture;
private var view : View;

private var gameMain : GameMain;

function getRectangleByPercentages(x : float, y : float, width : float, height : float)
{
	return Rect(Screen.width*x,Screen.height*y,Screen.width*width,Screen.height*height);
}

function Start () 
{
	title = Resources.Load.<Texture>("Interface/title");
	background2 = Resources.Load.<Texture>("Interface/background2");
	gameMain = GameObject.Find("GamePlayData").GetComponent(GameMain);
	buttonStyle = GUIStyle("Button"); // this is the default style, "buttonStyle" can be edited in unity editor too
	view = View.MainMenu;
}

function Update () 
{
	if(audioSrc.volume != (gameMain.bgm/100)*0.2)
		audioSrc.volume = (gameMain.bgm/100)*0.2;	
}

function OnGUI()
{
	GUI.DrawTexture(getRectangleByPercentages(0.3,0,0.4,0.2),title);
	GUI.Box(getRectangleByPercentages(0.4,0.05,0.2,0.15),"Survive",headerGuiStyle); 
	GUI.DrawTexture(getRectangleByPercentages(0.35,0.2,0.3,0.75),background2);
	if(view == View.MainMenu)
	{
		if(GUI.Button(getRectangleByPercentages(0.4,0.3,0.2,0.05),"Start Game"))	
			Application.LoadLevel("Scenes/PlayScene"); // changes scene	
		if(GUI.Button(getRectangleByPercentages(0.4,0.4,0.2,0.05),"Options"))	
			view = View.Options;
		if(GUI.Button(getRectangleByPercentages(0.4,0.5,0.2,0.05),"High Scores"))	
			view = View.HighScores;
		if(GUI.Button(getRectangleByPercentages(0.4,0.6,0.2,0.05),"About/Instructions"))	
			view = View.About;
		if(GUI.Button(getRectangleByPercentages(0.4,0.7,0.2,0.05),"Quit Game"))	
		{
			gameMain.saveData();
			Application.Quit();
		}
		return;
	}
	else if(view == View.About)
	{
		GUI.Box(getRectangleByPercentages(0.37,0.27,0.26,0.5),
		"This game was made using Unity free version\nUse W,A,S,D to move;\nLeft Mouse Button to Shoot;\n" +
		"Right Mouse Button for special attack;\nScroll Wheel to change special attack;\n"+
		"\nSpace to show cast range and affected area\nESC to Pause game\n\n" +
		"Monsters have a chance of dropping power ups and will try to find their way to kill you!\n\nHave fun an try to survive for as long as you can!!"+
		"\n\nStudents of LEI: David Martins, Lino Pereira, Luis Cruz",boxStyle);
	}
	else if(view == View.HighScores)
	{
		GUI.Box(getRectangleByPercentages(0.4,0.3,0.15,0.13),"High Scores",boxStyle);
 		for(var i : int =0 ; i < 10 ; i++)
		{
 			var score : Score = gameMain.rankManager.getScore(i);
 			if(score)
				GUI.Box(getRectangleByPercentages(0.42,0.35+i*0.05,0.20,0.13),score.time + "\t" + score.score+ "\t" + score.name,boxStyle);
			else
				GUI.Box(getRectangleByPercentages(0.42,0.35+i*0.05,0.20,0.13),"--\\--\t----------\t---------",boxStyle);
		}
	}
	else if(view == View.Options)
	{
		GUI.Box(getRectangleByPercentages(0.38,0.3,0.1,0.08),"Map Size : ",textStyle);
		gameMain.mapSize = createOptions(Screen.width*0.48,Screen.height*0.3, ["Small","Medium","Large"],gameMain.mapSize-1) + 1;
		GUI.Box(getRectangleByPercentages(0.38,0.4,0.1,0.08),"Difficulty : ",textStyle);
		gameMain.difficulty = createOptions(Screen.width*0.48,Screen.height*0.4, ["Easy","Normal","Hard"],gameMain.difficulty-1) + 1;
		GUI.Box(getRectangleByPercentages(0.38,0.5,0.13,0.08),"PowerUp Ratio : ",textStyle);
		gameMain.powerUps = createOptions(Screen.width*0.505,Screen.height*0.5, ["Few","Frequent","Abundant"],gameMain.powerUps-1) + 1;
		GUI.Box(getRectangleByPercentages(0.38,0.6,0.1,0.08),"Lives : ",textStyle);
		gameMain.lives = createOptions(Screen.width*0.48,Screen.height*0.6, ["0","1","2","3","4","5"],gameMain.lives-1) + 1;
		GUI.Box(getRectangleByPercentages(0.38,0.7,0.1,0.08),"BGM : ",textStyle);
		gameMain.bgm = GUI.HorizontalSlider(getRectangleByPercentages(0.48,0.7,0.1,0.03),gameMain.bgm,0,100,sliderBar,sliderHand);
		GUI.Box(getRectangleByPercentages(0.38,0.8,0.1,0.08),"SFX : ",textStyle);	
		gameMain.sfx = GUI.HorizontalSlider(getRectangleByPercentages(0.48,0.8,0.1,0.03),gameMain.sfx,0,100,sliderBar,sliderHand);
	}
	if(GUI.Button(getRectangleByPercentages(0.575,0.85,0.05,0.05),"Back",buttonStyle))	
			view = View.MainMenu;
}

function createOptions(x : float, y : float, options : String[], selected : int)
{
	if(GUI.Button(Rect(x,y,20,20),"",leftButton))	
		selected = (selected+options.Length-1)%options.Length;		
	GUI.Box(Rect(x+40,y,60,30),options[selected],textStyle);
	if(GUI.Button(Rect(x+120,y,20,20),"",rightButton))
		selected = (selected+1)%options.Length;	
	return selected;	
}

