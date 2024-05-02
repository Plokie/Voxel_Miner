
--
-- This is a template example script and can be overwritten
--
--	Some of the scripting engine's functionality is demonstrated here
--  and can be referred to later to figure out how to use stuff
--
--


-- Implements the Game and Engine functions
-- Should always be at the top, in this order
require("Scripts\\Game")
require("Scripts\\Engine")

-- Create some test mesh data
indices = {0,2,1, 1,2,3, 0,1,2, 1,3,2}
vertices = {
	--pos,				uv,			normal
	{-1.0,-1.0,0.0,		0.0,1.0,	0.0,0.0,1.0},
	{1.0,-1.0,0.0,		1.0,1.0,	0.0,0.0,1.0},
	{-1.0,1.0,0.0,		0.0,0.0,	0.0,0.0,1.0},
	{1.0,1.0,0.0,		1.0,0.0,	0.0,0.0,1.0},
}

-- Pushes the mesh and texture to the engine to be loaded as resources
Engine:LoadMesh(indices, vertices, "LuaTestMesh")
Engine:LoadTexture("Data\\Textures\\lua.dds", "LuaTestTexture")

-- Initialises some objects to be used later
SinObject = Engine:NewObject3D("SinObject")
CosObject = Engine:NewObject3D("CosObject")
TestObject = Engine:NewObject3D("TestObject")
-- Dont forget to :Create() objects when theyre needed!!

loaded=false

-- Called whenever a scene is loaded.
-- Use the sceneName to execute when desired
function OnSceneLoad(sceneName)
	if (sceneName == "game") then
		SinObject:Create() -- Creating an object here makes sure it is added to the correct scene
		SinObject:AddModel("cube")

		CosObject:Create()
		CosObject:AddModel("cube")

		TestObject:Create()
		TestObject:SetPosition(0, 2, 0)
		TestObject:AddModel("LuaTestMesh")
		TestObject:SetModelTexture(0, "LuaTestTexture")

		
	end
end

-- Called once per frame. deltaTime is the time taken to render the previous frame in seconds.
function Update(deltaTime)
	if (Engine.currentScene == "game") then
		if SinObject.created then
			SinObject:Rotate(0, deltaTime, 0)
			SinObject:SetPosition(0, math.sin(Engine.elapsedTime) * 5.0 + 5.0, 0)
		end

		if CosObject.created then
			CosObject:SetPosition(math.cos(Engine.elapsedTime) * 5.0, 5.0, 0)
			CosObject:Rotate(deltaTime, 0, 0)
		end

		test = Engine:GetObject3D("TestObject")
		if test.created then
			test:Rotate(0, deltaTime * 0.5, 0)
		end
	end
end

-- Example of subscribing to an event
-- See README for a list of events and the arguments they pass
-- TestEvent is an example debug event that is called when the 'G' key is pressed
Engine.SubscribeEvent["TestEvent"] = function()
	SinObject:SetModelTexture(0, "LuaTestTexture")
end

Engine.SubscribeEvent["Player.Loaded"] = function()
	loaded = true
end

Engine.SubscribeEvent["Player.Use"] = function(x,y,z,itemid)
	if (itemid == 21) then -- if the player is holding a stick
		local id = Game:GetBlockAt(x,y,z)
		Game:SetBlockAt(x,y+1,z,id)
		Game:SetBlockAt(x,y,z,1)
	end

	if (itemid == 4) then -- amethyst
		local pc = Engine:GetObject3D("PlayerController")
		if pc.created then
			local fwd = pc:Forward()
			Game:CreateDroppedItem(pc.position.x + fwd.x, pc.position.y + fwd.y, pc.position.z + fwd.z, 0, 28, 1)
		end
	end
end

-- Created a blackstone platform within the chunk index 1,1,1
Engine.SubscribeEvent["ChunkLoaded"] = function(cx,cy,cz)
	if(cx==1 and cy==1 and cz==1) then
		for x=0,15 do
			for z=0,15 do
				Game:SetBlockAt(cx*16 + x, cy*16, cz*16 + z, 5)
			end
		end

		Game:SetBlockAt(cx*16, cy*16 + 1, cz*16, 48)

	end
end