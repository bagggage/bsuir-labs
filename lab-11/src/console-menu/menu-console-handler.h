#pragma once

#include "menu.h"

void drawMenu(const Menu* menu);

/*Returns:
0 - succsses
1 - exit item choosed
*/
int handleMenuInteraction(const Menu* menu);

//Returns number of choosen menu item
int handleMenuInput(const Menu* menu);