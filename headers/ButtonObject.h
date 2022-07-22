#pragma once

class Button;

class ButtonObject
{
public:
    ButtonObject();
    virtual ~ButtonObject() = default;

    //is called when the button is pressed. Button_ID is from the button to note which button was pressed
    virtual void on_button_press(int button_id, Button& button) = 0;
};
