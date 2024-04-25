testVar = { x=1, y=2, z=3 }

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

	function object:SetPosition(pos)
		position = pos

	end

	Engine_CreateObject3D(object.name)

	return object
end


function testFunction()
	return 100
end

function Root_OnSceneLoad(sceneName)
	if (sceneName == "game") then
		TestObject = newObject3D("testname")
		TestObject.SetPosition(Vector3(0,5,0))
	end
end


