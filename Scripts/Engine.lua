testVar = { x=1, y=2, z=3 }
pi = 3.141592654

Engine = {
	elapsedTime = 0.0
}
function Engine:LoadTexture(path, name)
	Engine_LoadTexture(path, name)
end

function Engine:LoadMesh(indices, vertices, name)
	Engine_LoadMesh(indices, vertices, name)
end


indices = {0,2,1, 1,2,3, 0,1,2, 1,3,2}

--pos, uv, normal
vertices = {
	{-1.0,-1.0,0.0,		0.0,1.0,	0.0,0.0,1.0},
	{1.0,-1.0,0.0,		1.0,1.0,	0.0,0.0,1.0},
	{-1.0,1.0,0.0,		0.0,0.0,	0.0,0.0,1.0},

	{1.0,1.0,0.0,		1.0,0.0,	0.0,0.0,1.0},
}

Engine:LoadMesh(indices, vertices, "LuaTestMesh")
Engine:LoadTexture("Data\\Textures\\lua.dds", "LuaTestTexture")

function Vector3(Px,Py,Pz)
	return {x=Px,y=Py,z=Pz}
end

function newObject3D(nameArg)
	local object = {
		name = nameArg,
		position = {x=0,y=0,z=0},
		rotation = {x=0,y=0,z=0},
		created = false
	}

	function object:testFunc()
		testVar = testVar + 1
	end

	function object:SetPosition(x, y, z)
		self.position = Vector3(x,y,z)
		Engine_SetObject3DPos(self.name, x, y, z)
	end
	function object:Move(x,y,z)
		self.position.x = self.position.x + x
		self.position.y = self.position.y + y
		self.position.z = self.position.z + z
		Engine_SetObject3DPos(self.name, self.position.x, self.position.y, self.position.z)
	end
	function object:SetRotation(x,y,z)
		self.rotation = Vector3(x,y,z)
		Engine_SetObject3DRot(self.name, x, y, z)
	end
	function object:Rotate(x,y,z)
		self.rotation.x = self.rotation.x + x
		self.rotation.y = self.rotation.y + y
		self.rotation.z = self.rotation.z + z
		Engine_SetObject3DRot(self.name, self.rotation.x, self.rotation.y, self.rotation.z)
	end

	function object:AddModel(modelName)
		Engine_AddObject3DModel(self.name, modelName)
	end
	function object:SetModelTexture(idx, textureName)
		Engine_SetModelTexture(self.name, idx, textureName)
	end

	function object:Create()
		self.created = true
		Engine_CreateObject3D(self.name)
	end
	

	return object
end

SinObject = newObject3D("SinObject")
CosObject = newObject3D("CosObject")
TestObject = newObject3D("TestObject")

function Update(deltaTime)
	if SinObject.created then
		SinObject:Rotate(0, deltaTime, 0)
		SinObject:SetPosition(0, math.sin(Engine.elapsedTime) * 5.0 + 5.0, 0)
	end
	if CosObject.created then
		CosObject:SetPosition(math.cos(Engine.elapsedTime) * 5.0, 5.0, 0)
		CosObject:Rotate(deltaTime, 0, 0)
	end
	if TestObject.created then
		TestObject:Rotate(0, deltaTime * 0.5, 0)
	end
end

function OnSceneLoad(sceneName)
	if (sceneName == "game") then
		SinObject:Create()
		SinObject:AddModel("cube")

		CosObject:Create()
		CosObject:AddModel("cube")

		TestObject:Create()
		TestObject:Move(0, 2, 0)
		TestObject:AddModel("LuaTestMesh")
		TestObject:SetModelTexture(0, "LuaTestTexture")
	end
end


