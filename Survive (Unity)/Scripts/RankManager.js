/*Saves and orders the high scores, theres a max of 10 high scores at 1 time, they contain score, the time survived and the name of the Player.
The class Score will be serialized into the xml file from GameMain, so it also needs to have the default constructor declared*/

#pragma strict

class Score
{	
	var score : int;
	var time : String;
	var name : String;
	
	function Score(score : int, time : String, name : String)
	{
		this.score = score;
		this.time = time;
		this.name = name;
	}
	
	function Score()
	{
	
	}
}

class RankManager
{
	private var scores : List.<Score>;
	
	function RankManager(scores : List.<Score>)
	{
		this.scores = scores;
		this.scores.Sort(compare);
	}
	
	function getScore(index : int)
	{
		if(index < scores.Count)
			return scores[index];
		else
			return null;
	}
	
	function addScore(score : int, time : String, name : String)
 	{
		scores.Add(Score(score,time,name));
		scores.Sort(compare);
		if(scores.Count > 10)
			scores.RemoveAt(scores.Count-1);			
 	}
	
	function getScores()
	{
		return scores;
	}
	
	function compare(a : Score, b : Score)
	{
		if(a.score <= b.score)
			return 1;
		else 
			return -1;
	}
}