/*Simple script to make camera always follow player at a set distance so that player is always at the center of the screen*/

#pragma strict

 var cameraTarget : Transform;
 var myTransform : Transform;
 var cameraHeight : Vector3;
 
 function Start ()
 {
 	myTransform.position = cameraTarget.position + cameraHeight;
 }
 
 function Update () 
 {
	var pos : Vector3;
	pos = cameraTarget.position + cameraHeight;
	pos -= myTransform.position;
	if(pos.x != 0 || pos.y != 0)
		myTransform.Translate(pos*Time.deltaTime*10);
 }