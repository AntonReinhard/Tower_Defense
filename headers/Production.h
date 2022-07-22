#pragma once

#include "Constants.h"

#include <array>

class Building;

// a structure storing what the building produces/consumes
class Production : public std::array<PRODUCTIONSTATE, RESOURCES_TOTAL> {
public:
    Production();
    explicit Production(Building& building);

    //initializes the map and sets everything to "none"
    void initialize();

    //updates the map in case the building got upgraded etc.
    void update(Building& building);
};
