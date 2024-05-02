--
-- This is the scripting frontend to be communicated with
--	NOT TO BE EDITED
--
--
--
--
--


pi = 3.141592654

Engine = {
	elapsedTime = 0.0,
	currentScene = "",
	SubscribeEvent = {

	}
}
function Engine:LoadTexture(path, name)
	Engine_LoadTexture(path, name)
end

function Engine:LoadMesh(indices, vertices, name)
	Engine_LoadMesh(indices, vertices, name)
end

function Vector3(Px,Py,Pz)
	return {x=Px,y=Py,z=Pz}
end



function Engine:NewObject3D(nameArg)
	local object = {
		name = nameArg,
		position = {x=0,y=0,z=0},
		rotation = {x=0,y=0,z=0},
		created = false
	}

	-- Sets the position of the object
	function object:SetPosition(x, y, z)
		self.position = Vector3(x,y,z)
		Engine_SetObject3DPos(self.name, x, y, z)
	end
	-- Translates the object
	function object:Move(x,y,z)
		self.position.x = self.position.x + x
		self.position.y = self.position.y + y
		self.position.z = self.position.z + z
		Engine_SetObject3DPos(self.name, self.position.x, self.position.y, self.position.z)
	end
	-- Sets the rotation of the object
	function object:SetRotation(x,y,z)
		self.rotation = Vector3(x,y,z)
		Engine_SetObject3DRot(self.name, x, y, z)
	end
	-- Rotates the object
	function object:Rotate(x,y,z)
		self.rotation.x = self.rotation.x + x
		self.rotation.y = self.rotation.y + y
		self.rotation.z = self.rotation.z + z
		Engine_SetObject3DRot(self.name, self.rotation.x, self.rotation.y, self.rotation.z)
	end

	-- Adds a model to the object with a mesh
	function object:AddModel(modelName)
		Engine_AddObject3DModel(self.name, modelName)
	end
	-- Sets the texture resource of a model within the object
	function object:SetModelTexture(idx, textureName)
		Engine_SetModelTexture(self.name, idx, textureName)
	end

	-- Creates the object
	function object:Create()
		if(self.created == false) then
			Engine_CreateObject3D(self.name)
			self.created = true
		end
	end
	

	return object
end

-- Gets an objects within the active scene
function Engine:GetObject3D(name)
	local object = Engine:NewObject3D(name)
	success, px, py, pz, rx, ry, rz = Engine_GetObject3D(name)

	if(success) then
		object.position.x = px
		object.position.y = py
		object.position.z = pz
		object.rotation.x = rx
		object.rotation.y = ry
		object.rotation.z = rz
		object.created = true
	else
		object.name = "ERROR"
		object.position.x = 0
		object.position.y = 0
		object.position.z = 0
		object.rotation.x = 0
		object.rotation.y = 0
		object.rotation.z = 0
		object.created = false
	end
	

	return object
end

-- Called by c++ to handle events
function Backend_EventCall(eventName, ...)
	if(Engine.SubscribeEvent[eventName] ~= nil) then
		Engine.SubscribeEvent[eventName](...)
	end
end

function Engine:UnsubscribeEvent(eventName)
	Engine.SubscribeEvent[eventName] = nil
end