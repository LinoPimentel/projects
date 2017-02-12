/*This is the class in charge of saving and loading all the game configurations as well as scores.
When we exit the game it serializes all the data into a xml file, and it loads that same file when we start the game
NOTE: the GameData class needs to have the default constructor declared to be able to be serialized into a file*/

#pragma strict

@XmlRoot("Options")
class GameData
{
	var difficulty;
	var bgm;
	var sfx;
	var lives;
	var mapSize;
	var powerUps;
	@XmlArray("Scores")
	@XmlArrayItem("Score")
	var scores : List.<Score>;
	
	public function Save()
	{
		var serializer : XmlSerializer = new XmlSerializer(GameData);
		var stream : System.IO.Stream = new System.IO.FileStream("Config.xml", System.IO.FileMode.Create);
		serializer.Serialize(stream,this);
		stream.Close();
	}
	
	public static function Load(): GameData
 	{
 		if(!System.IO.File.Exists("Config.xml"))
 			return null;
 		var serializer : XmlSerializer = new XmlSerializer(GameData);
 		var stream : System.IO.Stream = new System.IO.FileStream("Config.xml", System.IO.FileMode.Open);
 		var result : GameData = serializer.Deserialize(stream) as GameData;
 		stream.Close();
 		return result;
 	}
	
	public function GameData()	
	{
	
	}
	
	public function GameData(mapSize, lives, difficulty, bgm, sfx, powerUps, scores)
	{
		this.mapSize = mapSize;
		this.lives = lives;
		this.difficulty = difficulty;
		this.bgm = bgm;
		this.sfx = sfx;
		this.powerUps = powerUps;
		this.scores = scores;
	}
}

class GameMain extends MonoBehaviour
{
	static var instance : GameMain;
	var difficulty : int = 1;
	var bgm : float = 40;
	var sfx : float = 30;
	var lives : int = 3;
	var mapSize : int = 4;
	var powerUps : int = 1;
	var rankManager : RankManager;
	
	function Start () 
	{
		if(instance)
			GameObject.DestroyImmediate(gameObject);
		else
		{
			DontDestroyOnLoad(gameObject); // makes it so that this object and its children will not be destroyed on scene change
			instance = this;
		}
		var data : GameData = new GameData().Load();
		if(!data)
		{
			rankManager = RankManager(new List.<Score>());
			return;
		}
		rankManager = RankManager(data.scores);
		difficulty = data.difficulty;
		bgm = data.bgm;
		sfx = data.sfx;
		lives = data.lives;
		mapSize = data.mapSize;
		powerUps = data.powerUps;
	}
	
	function saveData()
	{
		var data : GameData = new GameData(mapSize,lives,difficulty,bgm,sfx,powerUps,rankManager.getScores());
		data.Save();
	}
}






