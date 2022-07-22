#include "DemolishTool.h"
#include "MouseHandler.h"
#include "DemolishToolMouseItem.h"

DemolishTool::DemolishTool(Level* level, SDL_Point coords) : MenuItem("demolishtool", level, coords, WINDOWCONTENT, WINDOWCONTENT)
{

}

DemolishTool::~DemolishTool()
{
    
}

void DemolishTool::render()
{
    MenuItem::render();
}

void DemolishTool::on_click(int mouse_x, int mouse_y)
{
    gMouse_handler->set_item_on_mouse(new DemolishToolMouseItem(mName_of_object, mSprite, mLevel, WINDOWCONTENT));
}
