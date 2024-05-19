#pragma once

// STL
#include <vector>

class player_class{
    private:
        unsigned int ID;
        unsigned int num_win = 0;
        unsigned int need_win = 0;
        std::vector <unsigned int> cards;

    public:
        unsigned int get_ID(){
            return this->ID;
        }

        unsigned int get_cards_size(){
            return this->cards.size();
        }

        std::string get_card_name(unsigned int card_ID){
            unsigned int division = this->cards[card_ID] / 13;
            unsigned int remainder = this->cards[card_ID] % 13 + 1;
            std::string shape;

            switch(division){
                case 0:
                    shape = "Flower";
                    break;
                case 1:
                    shape = "Square";
                    break;
                case 2:
                    shape = "Heart ";
                    break;
                case 3:
                    shape = "Spade ";
                    break;
            }

            return shape + " " + std::to_string(remainder);
        }

        void ID_set(unsigned int input){
            this->ID = input;
        }

        void num_win_add(){
            this->num_win += 1;
        }

        void cards_set(std::vector <unsigned int> &input){
            this->cards = input;
        }

        void card_release(unsigned int input){
            this->cards.erase(this->cards.begin() + input);
        }
};
