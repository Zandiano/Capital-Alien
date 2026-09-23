#include "game_logic.hpp"
#include "game_renderer.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include <iostream>

Game mainGame;

void Init(){
    Init(mainGame, 40, 2);

    InitPlayer(mainGame, Constructor(1, "Zandiano", RED));
}

void UpdatePre(){

}

void Update(){
    switch(GetKeyPressed()){
        case KEY_Z:
            NextPlayer(mainGame);
            break;
        case KEY_X:
            NextRound(mainGame);
            break;
        case KEY_M:
            AddMoney(GetPlayer(mainGame), 20);
            break;
    }        
}

void UpdatePost(){

}

void Render3D(){
    RenderHouse(mainGame);
}

void Render2D(){
    RenderName(mainGame);
    RenderRound(mainGame);
    RenderMoney(mainGame);
}

void Debug(){
    std::cout << "Player: " << GetName(GetPlayer(mainGame)) << std::endl;
    std::cout << "House num: " << to_string(GetPos(GetPlayer(mainGame))) << std::endl;
}