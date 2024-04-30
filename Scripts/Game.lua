-- This is the frontend interface for game-related events

Game = {
	
}


--------

function Game:GetBlockAt(x,y,z)
	--CallGameFunction("GetBlockAt", x, y, z)
end

function Game:SetBlockAt(x,y,z,id)
	CallGameFunction("SetBlockAt", x, y, z, id)
end