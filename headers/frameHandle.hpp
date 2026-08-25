#ifndef FRAMEHANDLE_HPP
#define FRAMEHANDLE_HPP

#include "renders.hpp"
#include "house.hpp"

HOUSE::HOUSE housePlaceholder;

void UpdatePre(){

}

void Update(){

}

void UpdatePost(){

}

void Render(){
    RENDER::RenderHouse(housePlaceholder);
}

#endif