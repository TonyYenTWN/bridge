#pragma once

// STL
#include <vector>

class player_class{
    private:
        unsigned int ID;
        unsigned int num_win = 0;
        unsigned int need_win = 0;
        std::vector <unsigned int> cards;
        std::vector <unsigned int> status;

    public:
        unsigned int get_ID(){
            return this->ID;
        }

        unsigned int get_cards_size(){
            return this->cards.size();
        }

        std::string get_card_name(unsigned int card_ID){
            unsigned int division = (this->cards[card_ID] - 1) / 13;
            unsigned int remainder = this->cards[card_ID] % 13;
            std::string shape;
            std::string number;

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

            if(remainder == 0){
                number = "A";
            }
            else{
                number = std::to_string(remainder + 1);
            }

            return shape + " " + number;
        }

        unsigned int get_card_ID(unsigned int card_ID){
            return this->cards[card_ID];
        }

        unsigned int get_card_type(unsigned int card_ID){
            return (this->cards[card_ID] - 1) / 13;
        }

        unsigned int get_card_status(unsigned int card_ID){
            return(this->status[card_ID]);
        }

        void update_card_status(unsigned int card_ID, unsigned int value){
            this->status[card_ID] = value;
        }

        void ID_set(unsigned int input){
            this->ID = input;
        }

        void num_win_add(){
            this->num_win += 1;
        }

        void cards_set(std::vector <unsigned int> &input){
            this->cards = input;
            this->status.reserve(input.size());
            for(unsigned int card_iter = 0; card_iter < input.size(); ++ card_iter){
                this->status.push_back(0);
            }
        }

        void card_release(unsigned int input){
            this->cards.erase(this->cards.begin() + input);
        }
};
