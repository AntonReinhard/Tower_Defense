#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "LayerHandler.h"

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, Renderable* texture_to_render_on, std::string upgrade_section, int button_id) : Button(button_name, dim, obj, texture_to_render_on, button_id), mUpgrade_section(upgrade_section)
{

}

UpgradeButton::~UpgradeButton()
{
	
}



std::string UpgradeButton::get_upgrade_section()
{
	return mUpgrade_section;
}

void UpgradeButton::render()
{
	Button::render();
}



