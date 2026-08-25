#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

namespace Plr{
    class Player{
        private:
        std::string name = "None";
        uint64_t money = 0;
        bool ownings[40] = {false};

        public:
        Player(){
            this->name = "None";
            this->money = 0;
            memset(this->ownings, false, sizeof(this->ownings));
        }

        Player(std::string name, uint64_t money){
            this->name = name;
            this->money = money;
            memset(this->ownings, false, sizeof(this->ownings));
        }

        std::string GetName(){
            return this->name;
        }

        uint64_t GetMoney(){
            return this->money;
        }

        bool GetOwning(uint8_t index){
            return this->ownings[index];
        }

        void SetName(std::string newName){
            this->name = newName;
        }

        bool AddMoney(uint64_t value){
            this->money += value;
            return true; 
        }

        bool RemoveMoney(uint64_t value){
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