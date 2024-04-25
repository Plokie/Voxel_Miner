testVar = { x=1, y=2, z=3 }
pi = 3.141592654

--engine = {}
--function engine:set

function Vector3(Px,Py,Pz)
	return {x=Px,y=Py,z=Pz}
end

function newObject3D(nameArg)
	local object = {
		name = nameArg,
		position = {x=0,y=0,z=0},
		rotation = {x=0,y=0,z=0}
	}

	function object:testFunc()
		testVar = testVar + 1
	end

	function object:SetPosition(posX, posY, posZ)
		self.position = Vector3(posX,posY,posZ)
		Engine_SetObject3DPos(self.name, posX, posY, posZ)
	end
	function object:Move(x,y,z)
		self.position.x = self.position.x + x
		self.position.y = self.position.y + y
		self.position.z = self.position.z + z
		Engine_MoveObject3DPos(self.name, x,y,z)
	end
	function object:SetRotation(x,y,z)
		self.rotation = Vector3(x,y,z)
		Engine_SetObject3DRot(self.name, x, y, z)
	end
	function object:Rotate(x,y,z)
		self.rotation.x = self.rotation.x + x
		self.rotation.y = self.rotation.y + y
		self.rotation.z = self.rotation.z + z
		Engine_RotObject3DRot(self.name, x,y,z)
	end

	function object:AddModel(modelName)
		Engine_AddObject3DModel(self.name, modelName)
	end

	Engine_CreateObject3D(object.name)

	return object
end


function testFunction()
	return 100
end

function OnSceneLoad(sceneName)
	if (sceneName == "game") then
		TestObject = newObject3D("testname")
		TestObject:AddModel("cube")
		TestObject:SetPosition(0,5,0)
		TestObject:Rotate(0, 2*pi, 0)
	end
end


