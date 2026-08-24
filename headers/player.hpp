#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

namespace Plr{
    class Player{
        private:
        std::string name;
        uint32_t money;
        bool ownings[40];

        public:
        Player(std::string name, uint32_t money){
            this->name = name;
            this->money = money;
            memset(this->ownings, false, sizeof(this->ownings));
        }

        bool AddMoney(uint32_t value){
            this->money += value;
            return true; 
        }

        bool RemoveMoney(uint32_t value){
            this->money -= value;
            return true;
        }

        bool AddOwning(uint8_t index){
            if(index < 0 || index > sizeof(ownings)){
                std::cout << "Erro ao adicionar propriedade (Fora do array)" << std::endl;
                return false;
            }
            this->ownings[index] = true;
            return true;
        }

        bool RemoveOwning(uint8_t index){
            if(index < 0 || index > sizeof(ownings)){
                std::cout << "Erro ao adicionar propriedade (Fora do array)" << std::endl;
                return false;
            }
            this->ownings[index] = false;
            return true;
        }
    };
}

#endif