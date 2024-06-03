#pragma once

namespace statistic{
    inline unsigned int get_num_players(){
        return 4;
    }

    inline unsigned int get_num_cards(){
        return 52;
    }

    inline unsigned int get_num_rounds(){
        return get_num_cards() / get_num_players();
    }

    inline std::string get_card_name(unsigned int card_ID){
        unsigned int division = (card_ID - 1) / 13;
        unsigned int remainder = card_ID % 13;
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
}


