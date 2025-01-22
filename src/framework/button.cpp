//
//  button.cpp
//  ComputerGraphics
//
//  Created by Pol Obrador on 22/1/25.
//

#include "button.h"

bool Button::IsMouseInside(std::vector<int> click){
    if (coordinates[0] + 32 > click[0] > coordinates[0] && coordinates[1] + 32 > click[1] > coordinates[1] ){
        return true;
    }else{
        return false;
    }
}
