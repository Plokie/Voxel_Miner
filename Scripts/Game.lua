-- This is the frontend interface for game-related functions

Game = {
	
}


--------

function Game:GetBlockAt(x,y,z)
	return CallGameFunction("GetBlockAt", x, y, z)
end

function Game:SetBlockAt(x,y,z,id)
	CallGameFunction("SetBlockAt", x, y, z, id)
end