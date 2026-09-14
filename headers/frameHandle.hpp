#ifndef FRAMEHANDLE_HPP
#define FRAMEHANDLE_HPP

#include "renders.hpp"
#include "house.hpp"
#include "player.hpp"
#include "tabletop.hpp"

TBL::Game *mainGame = new TBL::Game;

void Init(){
    TBL::Init(*mainGame, 40, 2);

    TBL::InitPlayer(*mainGame, PLR::Constructor("Zandiano", red));
}

void UpdatePre(){

}

void Update(){
    switch(GetKeyPressed()){
        case KEY_Z:
            TBL::NextPlayer(*mainGame);

        case KEY_X:
            TBL::NextRound(*mainGame);
    }        
}

void UpdatePost(){

}

void Render3D(){
    RENDER::RenderHouse(*mainGame);
}

void Render2D(){
    RENDER::RenderName(*mainGame);
    RENDER::RenderRound(*mainGame);
}

void Debug(){
    std::cout << "Player: " << GetName(*GetPlayer(*mainGame)) << std::endl;
    std::cout << "House num: " << to_string(GetPos(*GetPlayer(*mainGame))) << std::endl;
}

#endif