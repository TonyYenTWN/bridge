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
}


