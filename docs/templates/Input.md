# Input

Hamburger Engine's Input system uses (SDL2's ScanCode)[https://wiki.libsdl.org/SDL2/SDL_Scancode] for keyboard input and (SDL_GameControllerButton)[https://wiki.libsdl.org/SDL2/SDL_GameControllerButton] for controller input meaning adding input is as simple as finding the button you want and doing this:

# justPressed

```cpp
// keyboar
if (Input::justPressed(SDL_SCANCODE_RETURN))
{
    std::cout << "ENTER/RETURN PRESSED" << std::endl;
}

// controllr
if (Input::isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_A))
{
    std::cout << "Ughhh A button pressed" << std::endl;
}
```        

But before you add any input stuff make sure to add:
```cpp
#include "../engine/Input.h"
```
to your defines

and in your state and add:
```cpp
Input::UpdateKeyStates();
```
to your update function!

# justReleased

```cpp
// keyboar
if (Input::justReleased(SDL_SCANCODE_RETURN))
{
    std::cout << "RETURN/ENTER RELEASED" << std::endl;
}

// controllr
if (Input::isControllerButtonJustReleased(SDL_CONTROLLER_BUTTON_A))
{
    std::cout << "Ughhh A button pressed" << std::endl;
}
```        

But before you add any input stuff make sure to add:
```cpp
#include "../engine/Input.h"
```
to your defines

and in your state and add:
```cpp
Input::UpdateKeyStates();
```
to your update function!

avaliable stuff:

```cpp
// controller:
isControllerButtonJustPressed()
isControllerButtonJustReleased()
isControllerButtonPressed()
getControllerAxis()

// keyboar
justReleased()
justPressed()
pressed()
```