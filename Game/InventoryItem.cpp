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