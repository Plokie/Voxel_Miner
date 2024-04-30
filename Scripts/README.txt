=====================================
HOW TO USE THE SCRIPTING API
=====================================


Introduction_________________________

The lua scripts allow a user to affect some elements of the game without changing or building the source code of the project. 
There are multiple features that allow some modification and reactions to in-game events.




Script structure_____________________

Game.lua and Engine.lua are considered backend scripts. Should only be used to interact with the backend via lua. 
I would not recommend editing these scripts unless you know what you're doing!

Main.lua is the entry point and script that is called first. For this reason, Game.lua and Engine.lua should be required at the top of Main.lua; In that order.

require("Scripts\\Game.lua")
require("Scripts\\Engine.lua")


Automatically called functions____________________________

There are some predefined functions that should be used by the script frontend

::	OnSceneLoad(sceneName)
Called whenever a scene is loaded via the engine. The sceneName passed is the name of the scene which was just opened.
Notable scene names:
"titlemain" - The first scene loaded at the title screen
"game" - The scene opened when the world is loaded / unpaused
"pause" - The scene opened when the game is paused

::	Update(deltaTime)
Called once per frame, passing deltaTime as a parameter. deltaTime is the time taken to render the previous frame. Should be used for movement calculations over time.


Engine class_______________________________________

There are some functions that the scripter can call that relate to some general functions of the engine

::	Engine:LoadTexture(path, name)
		Registers a texture to the engine's resource collection.
		
		path :
			The path to the texture wanting to be registered. Should be a .dds texture.

		name :
			Unique name identifier to refer to the texture thereforth


::	Engine:LoadMesh(indices, vertices, name)
		Registers a mesh to the engine's resource collection. 

		indices : 
			An array of integers which tie together the array of vertices

		vertices : 
			An array of vertices
			A vertex is structured as such
			{pos.x, pos.y, pos.z,		uv.x, uv.y,			normal.x,normal.y,normal.z}

		name :
			Unique name identifier to refer to the mesh thereforth





Object 3D Class______________________________

There are some classes provided by the script structure that should be used

::	Object3D

	Created by calling `Engine:NewObject3D(name)`

	::	SetPosition(x,y,z)

	::	Move(x,y,z)

	::	SetRotation(x,y,z)

	::	Rotate(x,y,z)

	::	AddModel(modelName)
		Adds a model to the object, with modelName being the name of the mesh being added

	::	SetModelTexture(idx, textureName)
		Sets the texture resource of a model.
		
		idx :
			The index of the model in the Object3D

		textureName :
			The unique name identifer of the texture



Game functions_______________________________

There are some callable functions that allow manipulation / probing of the game world

Game:GetBlock(x,y,z)



Events_______________________________________

An event is an identifier that can have a function assigned to it. The event is that called during the user's gameplay at different moments.

An event can be subscribed to by assigning to the event table like so:
Engine.SubscribeEvent["EventName"] = function(...) end

The events defined are:

"TestEvent" : 
	No arguments

	An example debug event, called when the 'G' key is pressed

"Player.Jump" :
	No arguments
	
	Called when the player presses the Jump key (Usually space bar)

"Player.Hurt" :
	Args:
		hurtAmt - The damage taken
		cause - The reason for the damage

	Called when the player is hurt

"ChunkLoaded" :
	Args:
		cx, cy, cz - 3 float arguments of the chunk's index in chunk-based coordinates