#include "Production.h"
#include "IndustrialBuilding.h"

Production::Production()
{
    initialize();
}

Production::Production(Building& building)
{
    initialize();
    update(building);
}

void Production::initialize()
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        (*this)[RESOURCETYPES(i)] = NONE;
    }
}

void Production::update(Building& building)
{
    auto maintenance = building.get_maintenance();
    auto produce = building.get_produce();

    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        //consuming has priority over the others
        //if a building is both consuming and producing a resource it will still get it delivered
        if (maintenance.get_resource(RESOURCETYPES(i)) != 0) {	//needs this resource
            (*this)[RESOURCETYPES(i)] = CONSUMING;
        }
        else if (produce.get_resource(RESOURCETYPES(i)) != 0) {
            (*this)[RESOURCETYPES(i)] = PRODUCING;
        }
        else {
            (*this)[RESOURCETYPES(i)] = NONE;
        }
    }
}