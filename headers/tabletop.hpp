#ifndef TABLETOP_HPP
#define TABLETOP_HPP

#include "player.hpp"
#include "house.hpp"

namespace TBL{
    class TABLETOP{
        private:
        Plr::Player players[6];
        Plr::Player* podium[3] = {0};
        HOUSE::HOUSE houses[40];

        public:
        TABLETOP(int numOfPlayers){
            for(int i = 0; i < numOfPlayers; i++){
                std::string name = "Plr n" + i;
                this->players[i] = Plr::Player(name, 0);
            }
        }
    };
}

#endif