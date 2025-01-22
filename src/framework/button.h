//
//  button.h
//  ComputerGraphics
//
//  Created by Pol Obrador on 22/1/25.
//
#pragma once

#include "main/includes.h"


#include "src/framework/image.h"
#include <vector>


class Button{
public:
    std::vector<int> coordinates;
    Image image;
    
    
    bool IsMouseInside(std::vector<int> click);
    
};




