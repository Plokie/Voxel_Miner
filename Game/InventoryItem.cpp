#include "InventoryItem.h"
#include "Items.h"
#include "Blocks.h"


Vector2Int InventoryItem::GetUVPos() const
{
    if(type == ITEM) {
        const Item& def = ItemDef::Get((ItemID)ID);
        return { def.UVx(), def.UVy() };
    }
    else {
        const Block& def = BlockDef::GetDef((BlockID)ID);
        return { def.GetTopUVidx(), def.GetTopUVidy() };
    }
}
