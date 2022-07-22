#include "BigUpgrade.h"
#include "ConfigFile.h"

BigUpgrade::BigUpgrade(const std::string& obj_name, const std::string& upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button)
    : mBig_upgrade_button(big_upgrade_button)
    , mShow_more_button(show_more_button)
{
    mShifted_down = false;

    const SDL_Color text_color = { 0,0,0,0 };
    const auto x_difference = 130; // x difference between big upgrade button and the shown upgrade name

    auto dim_of_upgrade_name_texture = mBig_upgrade_button->get_dimension();
    dim_of_upgrade_name_texture.x -= x_difference;
    dim_of_upgrade_name_texture.w = 0;
    dim_of_upgrade_name_texture.h = 0;

    std::string upgrade_name;
    upgrade_name.assign(gConfig_file->value(obj_name + "/upgrade" + upgrade_section, "name"));
    mUpgrade_name = std::make_shared<Text>(upgrade_name, dim_of_upgrade_name_texture, WINDOWCONTENT, text_color, true);

    const auto y_difference = 30; // y difference between upgrade name and upgrade description
    auto dim_of_upgrade_description = dim_of_upgrade_name_texture;
    dim_of_upgrade_description.y += y_difference;

    std::string upgrade_description;
    upgrade_description.assign(gConfig_file->value(obj_name + "/upgrade" + upgrade_section, "description"));
    mUpgrade_description = std::make_shared<Text>(upgrade_description, dim_of_upgrade_description, WINDOWCONTENT, text_color, true);
    mUpgrade_description->set_rendering_enabled(false);
}

BigUpgrade::~BigUpgrade()
{
    delete mBig_upgrade_button;
    delete mShow_more_button;
}

void BigUpgrade::shift(int v)
{
    mBig_upgrade_button->add_y_dimension(v);
    mBig_upgrade_button->set_clickable_space(mBig_upgrade_button->get_dimension());
    mShow_more_button->add_y_dimension(v);
    mShow_more_button->set_clickable_space(mShow_more_button->get_dimension());

    const auto dim = get_upgrade_name()->get_dimensions();
    get_upgrade_name()->set_position(dim.x, dim.y + v);
    
    if (v > 0) mShifted_down = true;
    else mShifted_down = false;
}

UpgradeButton* BigUpgrade::get_big_upgrade_button() const
{
    return mBig_upgrade_button;
}

ShowMoreButton* BigUpgrade::get_show_more_button() const
{
    return mShow_more_button;
}

std::shared_ptr<Text> BigUpgrade::get_upgrade_name() const
{
    return mUpgrade_name;
}

bool BigUpgrade::is_upgrade_description_shown() const
{
    return mUpgrade_description->is_rendering_enabled();
}

bool BigUpgrade::is_shifted_down() const
{
    return mShifted_down;
}

void BigUpgrade::set_shifted_down(const bool v)
{
    mShifted_down = v;
}

void BigUpgrade::set_upgrade_description_shown(const bool v) const
{
    mUpgrade_description->set_rendering_enabled(v);
}
 