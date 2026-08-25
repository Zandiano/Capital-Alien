#ifndef HOUSE_HPP
#define HOUSE_HPP                                                                                                                                                                                                                                                                       

#include <string>
#include "player.hpp"
#include "raylib.h"

namespace HOUSE{
    class HOUSE{
        private:
        std::string name = "None";
        std::string description = "None";
        Color color = RED;
        Plr::Player* owner = 0;
        uint64_t revenue;


        public:
        HOUSE(){
            this->name = "name";
            this->description = "description";
            this->color = RED;
            this->revenue = 0;
        }

        HOUSE(std::string name, std::string description, Color color, uint64_t revenue){
            this->name = name;
            this->description = description;
            this->color = color;
            this->revenue = revenue;
        }

        std::string GetName(){
            return this->name;
        }

        std::string GetDescription(){
            return this->description;
        }

        Color GetColor(){
            return this->color;
        }

        Plr::Player* GetOwner(){
            return this->owner;
        }

        uint64_t GetRevenue(){
            return this->revenue;
        }

        void SetName(std::string newName){
            this->name = newName;
        }

        void SetDescription(std::string newDesc){
            this->description = newDesc;
        }

        void SetColor(Color newColor){
            this->color = newColor;
        }

        void SetOwner(Plr::Player* player){
            this->owner = player;
        }

        void SetRevenue(uint64_t newRevenue){
            this->revenue = newRevenue;
        }
    };
}

#endif