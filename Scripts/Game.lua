-- This is the frontend interface for game-related functions
-- This should NOT be edited

Game = {
	
}


--------

function Game:GetBlockAt(x,y,z)
	return CallGameFunction("GetBlockAt", x, y, z)
end

function Game:SetBlockAt(x,y,z,id)
	CallGameFunction("SetBlockAt", x, y, z, id)
end

function Game:CreateDroppedItem(x,y,z,type,id,amt)
	CallGameFunction("CreateDroppedItem", x, y, z, type, id, amt)
end