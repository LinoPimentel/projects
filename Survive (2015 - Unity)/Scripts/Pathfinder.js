/*
Implementation of the A* pathfinding algorithm.
*/
#pragma strict

import System.Collections.Generic;

class Pathfinder
{
	
	var world : Array;
	var pathStart : Vector2;
	var pathEnd : Vector2;
	
	// shortcuts for speed
	var	abs = Mathf.Abs;
	var	max = Mathf.Max;
	var	pow = Mathf.Pow;
	var	sqrt = Mathf.Sqrt;

	var worldWidth : int;
	var worldHeight : int;
	var worldSize : int;

	var distanceFunction = ManhattanDistance;

	function Pathfinder(world : Array, pathStart : Vector2, pathEnd : Vector2)
	{
		this.world = world;
		this.pathStart = pathStart;
		this.pathEnd = pathEnd;
		this.worldWidth = (world[0] as Array).length;
		this.worldHeight = world.length;
		this.worldSize = worldWidth * worldHeight;
	}

	function ManhattanDistance(Point : Vector2, Goal : Vector2)
	{	// linear movement - no diagonals - just cardinal directions (NSEW)
		return abs(Point.x - Goal.x) + abs(Point.y - Goal.y);
	}

	// Neighbours functions, used by findNeighbours function
	// to locate adjacent available cells that aren't blocked

	// Returns every available North, South, East or West
	// cell that is empty. No diagonals,
	// unless distanceFunction function is not Manhattan
	function Neighbours(x : int, y : int)
	{
		var	N = y - 1;
		var S = y + 1;
		var E = x + 1;
		var W = x - 1;
		var myN = N > -1 && canWalkHere(x, N);
		var myS = S < worldHeight && canWalkHere(x, S);
		var myE = E < worldWidth && canWalkHere(E, y);
		var myW = W > -1 && canWalkHere(W, y);
		var result : Array = [];
		if(myN)
			result.push(Vector2(x,N));
		if(myE)
			result.push(Vector2(E,y));
		if(myS)
			result.push(Vector2(x,S));
		if(myW)
			result.push(Vector2(W,y));
		return result;
	}

	// returns boolean value (world cell is available and open)
	function canWalkHere(x : int, y : int)
	{
		if(x == pathEnd.x && y == pathEnd.y)
			return 1;
		return ((world[x] != null) && ((world[x] as Array)[y] != null) && ((world[x] as Array)[y] == 0));
	}

	class Node
	{
		var Parent : Node;
		// array index of this Node in the world linear array
		var val : int;
		// the location coordinates of this Node
		var x : int;
		var y : int;
		// the heuristic estimated cost
		// of an entire path using this node
		var f : int = 0;
		// the distanceFunction cost to get
		// from the starting point to this node
		var g : int = 0;
		var point : Vector2;

		function Node(Parent : Node, Point : Vector2, worldWidth : int)
		{
			this.Parent = Parent;
			val = Point.x + (Point.y * worldWidth);
			x = Point.x;
			y = Point.y;
			this.point = Point;
		}

	}

	// Path function, executes AStar algorithm operations
	function calculatePath()
	{
		
		// create Nodes from the Start and End x,y coordinates
		var	mypathStart : Node = new Node(null, pathStart, worldWidth);
		var mypathEnd : Node = new Node(null, pathEnd, worldWidth);
		// create an array that will contain all world cells
		var AStar = new Array(worldSize);
		// list of currently open Nodes
		var Open : List.<Node> = new List.<Node>();
		Open.Add(mypathStart);
		// list of closed Nodes
		var Closed : Array = [];
		// list of the final output array
		var result : Array = [];
		// reference to a Node (that is nearby)
		var myNeighbours : Array;
		// reference to a Node (that we are considering now)
		var myNode : Node;
		// reference to a Node (that starts a path in question)
		var myPath : Node;
		// temp integer variables used in the calculations
		var length : int;
		var max : int;
		var min : int;
		var i : int;
		var j : int;
		// iterate through the open list until none are left
		
		var zxc : int = 5;
		while(true)
		{
			length = Open.Count;
			if(length == 0)
				break;
					
			max = worldSize;
			min = -1;
			for(i = 0; i < length; i++)
			{
				if((Open[i] as Node).f < max)
				{
					max = (Open[i] as Node).f;
					min = i;
				}
			}

			// grab the next node and remove it from Open array
			myNode = (Open.GetRange(min, 1) as List.<Node>)[0];
			Open.RemoveAt(min);
			// is it the destination node?
			if(myNode.val == mypathEnd.val)
			{
				myPath = Closed[Closed.push(myNode) - 1];
				do
				{
					result.push([myPath.x, myPath.y]); //
					myPath = myPath.Parent;
					if(!myPath)
						break;
				}while (true);
				// clear the working arrays
				AStar = [];
				Closed = [];
				Open = new List.<Node>();
				// we want to return start to finish
				result.reverse();
			}
			else // not the destination
			{
				// find which nearby nodes are walkable
				myNeighbours = Neighbours(myNode.x, myNode.y);
				// test each one that hasn't been tried already
				i = 0;
				for(j = myNeighbours.length; i < j; i++)
				{
					myPath = new Node(myNode, myNeighbours[i], worldWidth);
					if (!AStar[myPath.val])
					{
						// estimated cost of this particular route so far
						myPath.g = myNode.g + distanceFunction((myNeighbours as Array)[i], myNode.point);
						// estimated cost of entire guessed route to the destination
						myPath.f = myPath.g + distanceFunction((myNeighbours as Array)[i], mypathEnd.point);
						// remember this new path for testing above
						Open.Add(myPath);
						// mark this node in the world graph as visited
						AStar[myPath.val] = true;
					}
				}
				// remember this route as having no more untested options
				Closed.push(myNode);
			}
		} // keep iterating until the Open list is empty
		return result;
	}
}