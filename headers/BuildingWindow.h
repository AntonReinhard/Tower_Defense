#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"

class Building;

class BuildingWindow
	: public Window, public ButtonObject
{
public:
	BuildingWindow(SDL_Rect dim, Building* building);
	~BuildingWindow();

	void render() override;
	void on_button_press(int button_id) override;
	
	void demolish_building() const;
	void upgrade_building() const;

	Button* get_demolish_button() const;
	Button* get_upgrade_button() const;
	CoordinatesInDouble get_button_offset() const;
	Building* get_building() const;


private:
	Button* mDemolish_button;
	Button* mUpgrade_button;
	CoordinatesInDouble mButton_offset;

	//the textures can't be immediately deleted, so they are reused instead
	std::map<RESOURCETYPES, LTexture*> mText;
	LTexture* mHeadline;

protected:
	Building* mBuilding;
};
