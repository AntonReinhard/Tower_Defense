#include "BigUpgrade.h"
#include "ConfigFile.h"

BigUpgrade::BigUpgrade(std::string obj_name, std::string upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button) : mBig_upgrade_button(big_upgrade_button), mShow_more_button(show_more_button)
{
	mShifted_down = false;

	SDL_Color text_color = { 0,0,0,0 };
	const auto x_difference = 130; // x difference between bigupgradebutton and the shown upgradename

	auto dim_of_upgrade_name_texture = big_upgrade_button->get_dimension();
	dim_of_upgrade_name_texture.x -= x_difference;
	dim_of_upgrade_name_texture.w = 0;
	dim_of_upgrade_name_texture.h = 0;

	std::string upgrade_name;
	upgrade_name.assign(gConfig_file->value(obj_name + "/upgrade" + upgrade_section, "name"));
	mUpgrade_name = new Text(upgrade_name, dim_of_upgrade_name_texture, WINDOWBUTTONS, text_color, mBig_upgrade_button->get_window());

	const auto y_difference = 30; // y difference between upgradename and upgradedescription
	auto dim_of_upgrade_description = dim_of_upgrade_name_texture;
	dim_of_upgrade_description.y += y_difference;

	std::string upgrade_description;
	upgrade_description.assign(gConfig_file->value(obj_name + "/upgrade" + upgrade_section, "description"));
	mUpgrade_description = new Text(upgrade_description, dim_of_upgrade_description, WINDOWBUTTONS, text_color, mBig_upgrade_button->get_window());
	mUpgrade_description->set_rendering_enabled(false);
}

BigUpgrade::~BigUpgrade()
{
	delete mBig_upgrade_button;
	delete mShow_more_button;
	delete mUpgrade_name;
	delete mUpgrade_description;
}

void BigUpgrade::shift(int v)
{
	mBig_upgrade_button->add_y_dimension(v);
	mBig_upgrade_button->set_clickable_space(mBig_upgrade_button->get_dimension());
	mShow_more_button->add_y_dimension(v);
	mShow_more_button->set_clickable_space(mShow_more_button->get_dimension());
	get_upgrade_name()->add_y_dim(v);
	
	if (v > 0) mShifted_down = true;
	else mShifted_down = false;
}

UpgradeButton* BigUpgrade::get_big_upgrade_button()
{
	return mBig_upgrade_button;
}

ShowMoreButton* BigUpgrade::get_show_more_button()
{
	return mShow_more_button;
}

Text* BigUpgrade::get_upgrade_name()
{
	return mUpgrade_name;
}

bool BigUpgrade::is_upgrade_description_shown()
{
	return mUpgrade_description->is_rendering_enabled();
}

bool BigUpgrade::is_shifted_down()
{
	return mShifted_down;
}

void BigUpgrade::set_shifted_down(bool v)
{
	mShifted_down = v;
}

void BigUpgrade::set_upgrade_description_shown(bool v)
{
	mUpgrade_description->set_rendering_enabled(v);
}



 