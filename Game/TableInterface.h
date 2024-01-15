#pragma once

#include <vector>
#include "../Engine/Object2D.h"
#include "Blocks.h"
//#include "../Engine/UI/Button.h"
//#include "../Engine/UI/HorizontalLayoutRect.h"
#include "InventoryItem.h"

class HorizontalLayoutRect;
class Button;
class UIRect;
class Inventory;
class InventoryUI;
class ItemIcon;
class BlockData;
class Label;
class PlayerData;
class TextInput;

struct InterfaceContext {
	PlayerData* playerData;
	BlockData* blockData;
	Vector3Int tablePosition;
	BlockID blockID = BlockID::AIR;

	InterfaceContext(PlayerData* playerData, BlockData* blockData, Vector3Int tablePosition, BlockID blockID) {
		this->playerData = playerData;
		this->blockData = blockData;
		this->tablePosition = tablePosition;
		this->blockID = blockID;
	}

	InterfaceContext(PlayerData* playerData, BlockID blockID) {
		this->playerData = playerData;
		this->blockData = nullptr;
		this->tablePosition = Vector3Int(0,0,0);
		this->blockID = blockID;
	}
};

class TableInterface : public Object2D {
protected:
	vector<Button*> _spawnedSlots;
	vector<ItemIcon*> _spawnedItemIcons;
	vector<UIRect*> _spawnedUIRects;
	vector<Label*> _spawnedLabels;
	vector<TextInput*> _spawnedTextInputs;
	vector<Button*> _spawnedButtons;
	BlockData* blockData;
	InterfaceContext recentContext = InterfaceContext(nullptr, nullptr, Vector3Int(0,0,0), (BlockID)0);
	InventoryUI* invUI;
	map<tuple<int, int>, InventoryItem*> itemExistsHash;

	map<tuple<int, int>, Button*> _indexedSlots;

	Button* MakeSlot(int idx, int idy);
	Label* MakeLabel(const string& text);
	TextInput* MakeTextInput(const string& defaultText);
	Button* MakeButton(const string& text, const XMFLOAT4& textCol = { 1.f, 1.f, 1.f, 1.f }, const XMFLOAT4& bgCol = {.3f, .3f, .3f, 1.f});
	ItemIcon* MakeItemIcon(InventoryItem* invItem);
	void ComputeItemExistsHash();

	// If an item exists at position idx,idy. Create it as child of parent.
	bool TryMakeItemIcon(Button* parent, int idx, int idy);
public:
	void ReleaseHeldItem();
	bool EraseIcon(ItemIcon* icon);
	void ReloadIcons();
	void HardReload();
	void PushIcon(ItemIcon* itemIcon);

	const InterfaceContext& GetRecentInterfaceContext() const { return recentContext; }

	virtual void Open(InterfaceContext ctx);
	virtual void Close();

	void Start() override = 0;
	void Update(const float dt) override;
	void Draw(SpriteBatch* sb) override;
};