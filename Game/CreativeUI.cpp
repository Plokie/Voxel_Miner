#include "CreativeUI.h"

#include "../Engine/UI/TextInput.h"
#include "ItemIcon.h"
#include "../Engine/Engine.h"
#include "PlayerData.h"
#include "Inventory.h"
#include "InventoryUI.h"

void CreativeUI::ClearList() {
	for(auto& slot : _spawnedSlots) {
		delete slot;
		slot = nullptr;
	}
	_spawnedSlots.clear();
	for(auto& icon : _spawnedItemIcons) {
		delete icon;
		icon = nullptr;
	}
	_spawnedItemIcons.clear();
}

void CreativeUI::DrawList(vector<pair<unsigned short, InventoryItem::Type>>& vec) {
	const float spacing = 5.f;
	int i = 0;
	for(auto& item : vec) {
		int x = i % 10;
		int y = i / 10;

		Button* button = new Button("Baloo");
		button->Init(pDevice);
		button->SetRect({ 0.3f,.3f,.3f,1.f });
		button->SetDimensions({ 50.f, 50.f });
		button->SetParent(this);
		button->SetAnchor(0.f, 0.f);
		button->SetPivot(0.f, 0.f);
		button->SetDepth(14.f);
		button->SetPosition(
			x * (50.f + spacing) + 0.f,
			y * (50.f + spacing) + 80.f
		);
		button->SetText("");

		_spawnedSlots.push_back(button);

		ItemIcon* icon = new ItemIcon(item.first, item.second);
		icon->Init(pDevice);
		icon->SetDimensions({ 45.f, 45.f });
		icon->SetParent(button);
		icon->SetAnchor(0.5f, 0.5f);
		icon->SetPivot(.5f, .5f);
		icon->SetDepth(15.f);

		_spawnedItemIcons.push_back(icon);


		button->AddListener([=] {
			Engine::Get()->GetCurrentScene()->GetObject3D<PlayerData>("PlayerData")->GetInventory()->AddItem(item.first, item.second);
			Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI")->HardReloadIcons();
		});

		i++;
	}
}

void CreativeUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);

	TextInput* textInput = MakeTextInput("Search");
	//textInput->SetPosition(100.f, 100.f);
	//textInput
	textInput->AddListener([this, textInput] {
		this->LoadSearch(textInput->GetText());
	});

	for(int i = 0; i < CAT_COUNT; i++) {
		Button* testButton = MakeButton(ItemCategory::categoryName[(ITEM_CATEGORY)i]);
		testButton->SetDimensions({88.f, 35.f});
		testButton->SetPosition((88.f + 5.f) * i, 40.f);

		testButton->AddListener([=] {
			this->LoadCategory((ITEM_CATEGORY)i);
		});
	}



	LoadCategory(CAT_NATURE);
	
}

void CreativeUI::Close() {
	TableInterface::Close();
}

bool CompareSearch(const string& search, const string& name) {
	string _search = search;
	string _name = name;

	transform(_search.begin(), _search.end(), _search.begin(), [](unsigned char c) {return tolower(c); });
	transform(_name.begin(), _name.end(), _name.begin(), [](unsigned char c) {return tolower(c); });

	return _name.find(_search) != std::string::npos;
}

void CreativeUI::LoadSearch(const string& search) {
	ClearList();

	vector<pair<unsigned short, InventoryItem::Type>> items = {};

	for(int i = 0; i < BLOCK_COUNT; i++) {
		if(items.size() >= 3 * 10) break;

		const Block& blockDef = BlockDef::GetDef((BlockID)i);
		if(CompareSearch(search, blockDef.GetName())) {
			items.push_back({ i, InventoryItem::BLOCK });
		}
	}

	for(int i = 0; i < ITEM_COUNT; i++) {
		if(items.size() >= 3 * 10) break;

		const Item& itemDef = ItemDef::Get((ItemID)i);
		if(CompareSearch(search, itemDef.GetName())) {
			items.push_back({ i, InventoryItem::ITEM });
		}
	}

	DrawList(items);
}

void CreativeUI::LoadCategory(ITEM_CATEGORY itemCategory) {
	ClearList();

	vector<pair<unsigned short, InventoryItem::Type>> items = {};

	for(int i = 0; i < BLOCK_COUNT; i++) {
		const Block& blockDef = BlockDef::GetDef((BlockID)i);
		if(blockDef.GetCategory() == itemCategory) {
			items.push_back({ i, InventoryItem::BLOCK });
		}
	}

	for(int i = 0; i < ITEM_COUNT; i++) {
		const Item& itemDef = ItemDef::Get((ItemID)i);
		if(itemDef.GetCategory() == itemCategory) {
			items.push_back({ i, InventoryItem::ITEM });
		}
	}

	DrawList(items);
}

void CreativeUI::Start() {
}

void CreativeUI::Update(const float dt) {
	TableInterface::Update(dt);
}

void CreativeUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);
}
