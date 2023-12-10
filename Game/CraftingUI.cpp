#include "CraftingUI.h"

#include "../Engine/Engine.h"
#include "Crafting.h"
#include "ItemIcon.h"
#include "../Graphics/Graphics.h"
#include "Inventory.h"
#include "../Engine/UI/Button.h"
#include "InventoryUI.h"

void CraftingUI::DeleteCategoryObjects() {
	for(auto& ptr : _spawnedButtons) {
		delete ptr;
		ptr = nullptr;
	}
	_spawnedButtons.clear();

	for(auto& ptr : _spawnedItemIcons) {
		delete ptr;
		ptr = nullptr;
	}
	_spawnedItemIcons.clear();
}

void CraftingUI::DeleteRecipeObjects()
{
	for(auto& ptr : _spawnedRecipeIcons) {
		delete ptr;
		ptr = nullptr;
	}
	_spawnedRecipeIcons.clear();

	if(craftButton) delete craftButton;
}

CraftingUI::CraftingUI() {
	SetTexture("white");
	SetColour(0.5f, 0.5f, 0.5f, 1.f); // debug colour
	//tabs = HorizontalLayoutRect({ 1.f,1.f,1.f,1.f });

}

CraftingUI::~CraftingUI(){
	for(auto& buttonPtr : _spawnedTabs) {
		delete buttonPtr;
		buttonPtr = nullptr;
	}
	_spawnedTabs.clear();

	DeleteCategoryObjects();
	DeleteRecipeObjects();
}

void CraftingUI::Create() {
	//tabs.Init(Graphics::Get()->GetDevice());
	//tabs.SetDimensions({550.f, 50.f});
	//tabs.SetAnchor(0.5f, 0.f);
	//tabs.SetPivot(0.5f, 0.f);
	//tabs.SetParent(this);
	//tabs.SetDepth(12.f);

	for(const Category& category : Crafting::categories) {
		int index = static_cast<int>(_spawnedTabs.size());

		Button* tab = new Button(L"Data\\Fonts\\Baloo2.spritefont");
		tab->Init(Graphics::Get()->GetDevice());
		tab->SetDimensions({ 100.f, 30.f });
		tab->SetDepth(13.f);
		tab->SetParent(this);
		tab->SetPivot(0.0f, 0.f);
		tab->SetAnchor(0.0f, 0.0f);
		tab->SetText(category.name);
		tab->SetPosition(110.f * index, 0.f);
		tab->SetBgColour(0.4f, 0.4f, 0.4f, 1.0f);
		tab->SetTextColour(1.f, 1.f, 1.f, 1.f);

		_spawnedTabs.push_back(tab);
		//_spawnedTabs[tab] = &category;

		tab->AddListener([this, &category] {
			this->LoadCategory(&category);
		});
	}

	recipeDisplayBg = new UIRect("white", {0.4f, 0.4f, 0.4f, 1.0f});
	recipeDisplayBg->Init(Graphics::Get()->GetDevice());
	recipeDisplayBg->SetAnchor(1.f, 0.f);
	recipeDisplayBg->SetPivot(0.5f, 0.5f);
	recipeDisplayBg->SetParent(this);
	recipeDisplayBg->SetPosition(-100.f, 150.f);
	recipeDisplayBg->SetDimensions({ 200.f, 200.f });
	recipeDisplayBg->SetDepth(12.f);

	for(auto& tab : _spawnedTabs) {
		tab->SetText(tab->GetText());
	}
}

void CraftingUI::LoadCategory(const Category* category) {
	DeleteCategoryObjects();

	int index = 0;
	for(const auto& recipeArray : category->recipes) {
		//todo: make other recipes within array (scroll up/down thing to variations)
		//if(recipeArray.size() == 0) continue;
		//for(int i = 0; i < ; i++) {
		int variationNum = 0;
		for(const string& recipeName : recipeArray) {

			const Recipe& recipe = Recipe::recipes[recipeName];

			Button* button = new Button(L"Data\\Fonts\\Baloo2.spritefont");
			button->Init(Graphics::Get()->GetDevice());
			button->SetRect({ 0.3f,.3f,.3f,1.f });
			button->SetDimensions({ 50.f, 50.f });
			button->SetParent(this);
			button->SetAnchor(0.f, 0.f);
			button->SetPivot(0.f, 0.f);
			button->SetDepth(14.f);
			button->SetPosition(55.f * index, 50.f + (50.f * variationNum));
			button->SetText("");

			_spawnedButtons.push_back(button);


			//ItemIcon* icon = new UIRect("pfp");

			ItemIcon* icon = new ItemIcon(recipe.result);
			icon->Init(Graphics::Get()->GetDevice());
			icon->SetDimensions({ 45.f, 45.f });
			icon->SetParent(button);
			icon->SetAnchor(0.5f, 0.5f);
			icon->SetPivot(.5f, .5f);
			icon->SetDepth(15.f);
			
			_spawnedItemIcons.push_back(icon);

			button->AddListener([this, recipe] {
				this->SelectRecipe(recipe);

			});

			variationNum++;
		}
		index++;
	}
}

void CraftingUI::SelectRecipe(const Recipe& recipe) {
	DeleteRecipeObjects();

	if(inventory == nullptr) {
		inventory = (Inventory*)Engine::Get()->GetScene("game")->GetObject3D("Inventory");
	}

	craftButton = new Button(L"Data\\Fonts\\Baloo2.spritefont");
	craftButton->Init(Graphics::Get()->GetDevice());
	craftButton->SetDimensions({ 150.f, 30.f });
	craftButton->SetParent(recipeDisplayBg);
	craftButton->SetAnchor(0.5f, 1.f);
	craftButton->SetPivot(0.5f, 0.5f);
	craftButton->SetDepth(14.f);
	craftButton->SetPosition(0.f, -30.f);
	craftButton->SetText("Craft");

	if(inventory->CanCraft(recipe)) {
		craftButton->SetTextColour(1.f,1.f,1.f,1.f);
		craftButton->SetBgColour(0.5f,.5f,.5f,1.f );

		craftButton->AddListener([recipe] {
			((Inventory*)Engine::Get()->GetScene("game")->GetObject3D("Inventory"))->TryCraft(recipe);
			InventoryUI* invUI = (InventoryUI*)Engine::Get()->GetScene("game")->GetObject2D("invUI");
			invUI->Close();
			invUI->Open();
		});
	}
	else {
		craftButton->SetTextColour(.5f, .5f, .5f, 1.f);
		craftButton->SetBgColour(0.3f, .3f, .3f, .6f);
	}


	int index = 0;
	for(const RecipeComponent& component : recipe.input) {
		//InventoryItem invitem = InventoryItem(component.type, component.ID, 0, 0, component.amount);
		ItemIcon* icon = new ItemIcon(component);
		icon->Init(Graphics::Get()->GetDevice());
		icon->SetDimensions({ 45.f, 45.f });
		icon->SetParent(recipeDisplayBg);
		icon->SetAnchor(0.0f, 0.0f);
		icon->SetPivot(.0f, .0f);
		icon->SetDepth(15.f);

		icon->SetPosition(45.f * index, 0.f);

		_spawnedRecipeIcons.push_back(icon);
		index++;
	}
}

void CraftingUI::Update(const float dt) {
	for(auto& btn : _spawnedTabs) {
		btn->Update(dt);
	}

	for(auto& ptr : _spawnedButtons) {
		ptr->Update(dt);
	}

	for(auto& ptr : _spawnedItemIcons) {
		ptr->Update(dt);
	}

	for(auto& ptr : _spawnedRecipeIcons) {
		ptr->Update(dt);
	}

	if(craftButton) craftButton->Update(dt);
}

void CraftingUI::Draw(SpriteBatch* spriteBatch) {
	UIRect::Draw(spriteBatch);
	recipeDisplayBg->Draw(spriteBatch);
	for(auto& ptr : _spawnedRecipeIcons) {
		ptr->Draw(spriteBatch);
	}
	if(craftButton) {
		craftButton->Draw(spriteBatch);
	}

	//tabs.Draw(spriteBatch);
	for(auto& btn : _spawnedTabs) {
		btn->Draw(spriteBatch);
	}

	for(auto& ptr : _spawnedButtons) {
		ptr->Draw(spriteBatch);
	}

	for(auto& ptr : _spawnedItemIcons) {
		ptr->Draw(spriteBatch);
	}

}
