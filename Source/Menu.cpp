#include "JohanEngine\Interface.h"
#include "Menu.h"
#include "resource.h"

Menu::Menu() {
	selection = mtNone;
	
	CreateButtons();
	CreateInfo();
	
	// Show all by default
	buttons->Show(1);
	info->Show(1);
}

Menu::~Menu() {
	// ui stuff is deleted by engine
}

void Menu::CreateButtons() {
	// Create button list
	buttons = new Window(10,10,20,70 * 7 + 10,"");
	
	// Bulldoze
	Button* bulldozebutton = new Button(10,10,80,60,"Bulldoze");
	bulldozebutton->hotcolor = float4(0.7,0.2,0,1);
	bulldozebutton->backcolor = 0.8 * bulldozebutton->hotcolor;
	bulldozebutton->OnClick = BulldozeClick;
	buttons->AddChild(bulldozebutton);
	
	// Add some zoning
	Button* r1button = new Button(10,80,80,60,"Residential");
	r1button->hotcolor = float4(0,1,0,1);
	r1button->backcolor = 0.8 * r1button->hotcolor;
	r1button->OnClick = ZoningClick;
	buttons->AddChild(r1button);
	
	Button* cbutton = new Button(10,150,80,60,"Commercial");
	cbutton->hotcolor = float4(0,0,1,1);
	cbutton->backcolor = 0.8 * cbutton->hotcolor;
	cbutton->OnClick = ZoningClick;
	buttons->AddChild(cbutton);
	
	Button* ibutton = new Button(10,220,80,60,"Industrial");
	ibutton->hotcolor = float4(0.72,0.5,0.05,1);
	ibutton->backcolor = 0.8 * ibutton->hotcolor;
	ibutton->OnClick = ZoningClick;
	buttons->AddChild(ibutton);
	
	// And a basic road option
	Button* roadbutton = new Button(10,290,80,60,"Road");
	roadbutton->hotcolor = float4(0.1,0.1,0.1,1);
	roadbutton->backcolor = 0.8 * roadbutton->hotcolor;
	roadbutton->OnClick = RoadClick;
	buttons->AddChild(roadbutton);
	
	// Add power related stuff
	Button* powerbutton = new Button(10,360,80,60,"Power Plant");
	powerbutton->hotcolor = float4(1,1,0,1);
	powerbutton->backcolor = 0.8 * powerbutton->hotcolor;
	powerbutton->OnClick = PowerClick;
	buttons->AddChild(powerbutton);
	
	Button* powerlinebutton = new Button(10,430,80,60,"Power Line");
	powerlinebutton->hotcolor = float4(1,0,1,1);
	powerlinebutton->backcolor = 0.8 * powerlinebutton->hotcolor;
	powerlinebutton->OnClick = PowerClick;
	buttons->AddChild(powerlinebutton);
	
	ui->AddComponent(buttons);
}
void Menu::CreateInfo() {
	
	// Create properties window
	info = new Window(10,768 - 150,80,140,"RCI");
	
	// Create R, C, I bars
	R = new Label(5,18,20,60,"",false);
	R->backcolor = float4(0,1,0,1);
	info->AddChild(R);
	
	C = new Label(30,18,20,60,"",false);
	C->backcolor = float4(0,0,1,1);
	info->AddChild(C);
	
	I = new Label(55,18,20,60,"",false);
	I->backcolor = float4(0.72,0.5,0.05,1);
	info->AddChild(I);
	
	// Create money label
	moneylabel = new Label(5,85,70,20,"$0",true);
	info->AddChild(moneylabel);
	
	// Create pop label
	populationlabel = new Label(5,115,70,20,"0",true);
	info->AddChild(populationlabel);
	
	ui->AddComponent(info);
}
void Menu::SetMoney(int amount) {
	char buffer[32];
	snprintf(buffer,32,"$%d",amount);
	moneylabel->SetCaption(buffer);
}
void Menu::SetPopulation(int amount) {
	char buffer[32];
	snprintf(buffer,32,"%d",amount);
	populationlabel->SetCaption(buffer);
}
void Menu::SetR(int value) {
	int newheight = std::min(value,200) / 200.0f * 60.0f; // 60 is max height, 200 is max showable value
	int newtop = 18 + 60 - newheight; // 18 is default height
	R->SetRect(R->left,newtop,R->width,newheight);
}
void Menu::SetC(int value) {
	int newheight = std::min(value,200) / 200.0f * 60.0f; // 60 is max height, 200 is max showable value
	int newtop = 18 + 60 - newheight; // 18 is default height
	C->SetRect(C->left,newtop,C->width,newheight);
}
void Menu::SetI(int value) {
	float newheight = std::min(value,200) / 200.0f * 60.0f; // 60 is max height, 200 is max showable value
	int newtop = 18 + 60 - newheight; // 18 is default height
	I->SetRect(I->left,newtop,I->width,newheight);
}
void BulldozeClick(Component* sender) {
	menu->selection = mtBulldoze;
}
void ZoningClick(Component* sender) {
	Button* button = (Button*)sender;
	const char* caption = button->GetCaption();
	
	if(!strcmp(caption,"Residential")) {
		menu->selection = mtResidential;		
	} else if(!strcmp(caption,"Commercial")) {
		menu->selection = mtCommercial;		
	} else if(!strcmp(caption,"Industrial")) {
		menu->selection = mtIndustrial;		
	}
}
void RoadClick(Component* sender) {
	menu->selection = mtRoad;
}
void PowerClick(Component* sender) {
	Button* button = (Button*)sender;
	const char* caption = button->GetCaption();
	
	if(!strcmp(caption,"Power Plant")) {
		menu->selection = mtPowerPlant;		
	} else if(!strcmp(caption,"Power Line")) {
		menu->selection = mtPowerLine;		
	}
}
