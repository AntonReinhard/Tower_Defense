#pragma once
#include "ShowMoreButton.h"
#include "Text.h"
#include "UpgradeButton.h"

class BigUpgrade
{
public:
    BigUpgrade(const std::string& obj_name, const std::string& upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button);
    ~BigUpgrade();

    void shift(int v); //how many pixels it is shifted in y-direction

    ShowMoreButton* get_show_more_button() const;
    UpgradeButton* get_big_upgrade_button() const;
    Text* get_upgrade_name() const;
    bool is_upgrade_description_shown() const;
    bool is_shifted_down() const;
    void set_shifted_down(bool v);
    void set_upgrade_description_shown(bool v) const;

private:
    UpgradeButton* mBig_upgrade_button;
    ShowMoreButton* mShow_more_button;

    Text* mUpgrade_name;
    Text* mUpgrade_description;

    bool mShifted_down; //true if a show-more-button from a other upgrade, that has a smaller y-Value(coord) is clicked, so this upgrade is shifted down, so the description can be shown
};
