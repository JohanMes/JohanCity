#ifndef MENU_H
#define MENU_H

#include "JohanEngine\Dialogs.h"
#include "Tile.h"

enum MenuType {
	mtNone,
	mtRoad,
	mtBulldoze,
	mtPowerPlant,
	mtPowerLine,
	mtResidential,
	mtCommercial,
	mtIndustrial,
};

class Menu {
	public:
		Menu();
		~Menu();
		
		Window* buttons;
		Window* info;
		Label* moneylabel;
		Label* populationlabel;
		Label* R;
		Label* C;
		Label* I;
		MenuType selection;
		
		void CreateButtons();
		void CreateInfo();
		void SetR(int value); // 0 = min, 1000 = max
		void SetC(int value); // 0 = min, 1000 = max
		void SetI(int value); // 0 = min, 1000 = max
		void SetMoney(int amount);
		void SetPopulation(int value);
};

void BulldozeClick(Component* sender);
void ZoningClick(Component* sender);
void RoadClick(Component* sender);
void PowerClick(Component* sender);

#endif
