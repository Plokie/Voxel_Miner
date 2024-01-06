#include "InventoryItem.h"
#include "Items.h"
#include "Blocks.h"


Vector2Int InventoryItem::GetUVPos(BLOCK_FACE_TEX face_tex) const
{
    if(type == ITEM) {
        const Item& def = ItemDef::Get((ItemID)ID);
        return { def.UVx(), def.UVy() };
    }
    else {
        const Block& def = BlockDef::GetDef((BlockID)ID);

        switch(face_tex) {
        case FT_TOP: return { def.GetTopUVidx(), def.GetTopUVidy() };
        case FT_BOTTOM: return { def.GetBottUVidx(), def.GetBottUVidy() };
        default: return { def.GetSideUVidx(), def.GetSideUVidy() };
        }
    }
}

const int InventoryItem::GetMaxStack()
{
    if(type == ITEM) {
        const Item& def = ItemDef::Get((ItemID)ID);
        return def.GetMaxStack();
    }
    return MAX_STACK;
    //const Block& def= BlockDef::GetDef((BlockID)ID);
}

bool InventoryItem::IsSameItemAs(const InventoryItem& other)
{
    return this->type == other.type && this->ID == other.ID;
}

bool InventoryItem::IsSameItemAs(const InventoryItem* other)
{
    return this->type == other->type && this->ID == other->ID;
}

bool InventoryItem::Is(BlockID blockID)
{
    return blockID==ID && type==BLOCK;
}

bool InventoryItem::Is(ItemID itemID)
{
    return itemID==ID && type==ITEM;
}


nlohmann::json InventoryItem::Serialize()
{
    //string str = "";
    nlohmann::json json = {};
    json["type"] = type;
    json["ID"] = ID;
    json["amount"] = amount;
    json["posX"] = posX;
    json["posY"] = posY;

    return json;
    //return string();
}

InventoryItem* InventoryItem::Deserialize(nlohmann::json json)
{
    InventoryItem* item = new InventoryItem();
    item->type = json["type"];
    item->ID = json["ID"];
    item->amount = json["amount"];
    item->posX = json["posX"];
    item->posY = json["posY"];

    return item;
}