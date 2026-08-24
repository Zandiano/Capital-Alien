#ifndef HOUSE_HPP
#define HOUSE_HPP

#include <string>
#include "player.hpp"
#include "raylib.h"

namespace HOUSE{
    class HOUSE{
        private:
        std::string name;
        std::string description;
        Color color = RED;
        Plr::Player* owner = 0;

        public:
        HOUSE(std::string name, std::string description, Color color){
            this->name = name;
            this->description = description;
            this->color = color;
        }

        std::string GetName(){
            return name;
        }

        std::string GetDescription(){
            return description;
        }

        Color GetColor(){
            return color;
        }

        Plr::Player* GetOwner(){
            return owner;
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
    };
}

#endif