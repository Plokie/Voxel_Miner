testVar = { x=1, y=2, z=3 }


--engine = {}
--function engine:set

function newObject3D()
	local object = {}

	function object:testFunc()
		testVar = testVar + 1
	end
	return object
end



TestObject = newObject3D()