// Header for drawing cards
#pragma once

// STL
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <time.h>
#include <vector>

// Project specific
#include "src/statistic.h"
#include "src/rw.h"

class drawer_class{
    private:
        unsigned int num_card = get_num_cards();
        unsigned int num_players = get_num_players();
        std::vector <unsigned int> drawn_cards;

        template <typename vec_type>
        std::vector <unsigned int> cards_sort(vec_type v){
            std::vector <std::size_t> w(v.size());
            std::iota(begin(w), end(w), 0);
            std::sort(begin(w), end(w), [&v](std::size_t i, std::size_t j) { return v[i] < v[j]; });
            std::vector <unsigned int> r(w.size());
            for (std::size_t n, i = 0; i < w.size(); i += n){
                n = 1;
                while (i + n < w.size() && v[w[i]] == v[w[i+n]]) ++n;
                for (std::size_t k = 0; k < n; ++ k)
                {
                     r[w[i+k]] = i + k + 1;      // random order
                }
            }

            return r;
        };

        std::vector <unsigned int> rand_generate(){
            std::srand(std::time(NULL));

            std::vector <unsigned int> result;
            result.reserve(this->num_card);
            for(unsigned int card_iter = 0; card_iter < this->num_card; ++ card_iter){
                result.push_back(std::rand());
            }

            return result;
        };

        void draw_cards(){
            this->drawn_cards = this->cards_sort(this->rand_generate());
        }

        void give_cards(){
            unsigned int num_card_player = this->num_card / num_players;
            std::vector <std::vector <std::string>> drawn_cards_player(num_players);
            std::vector <std::string> initial_status;
            initial_status.reserve(num_card_player + 1);
            initial_status.push_back("status");
            for(unsigned int card_iter = 0; card_iter < num_card_player; ++ card_iter){
                initial_status.push_back("-1");
            }
            std::vector <std::vector <std::string>> write_file(2);
            write_file[1] = initial_status;

            for(unsigned int player_iter = 0; player_iter < num_players; ++ player_iter){
                unsigned int start_ID = player_iter * num_card_player;
                unsigned int end_ID = start_ID + num_card_player;

                std::vector <unsigned int> temp;
                temp.reserve(num_card_player);
                for(unsigned int card_iter = start_ID; card_iter < end_ID; ++ card_iter){
                    temp.push_back(this->drawn_cards[card_iter]);
                }
                std::sort(temp.begin(), temp.end());

                drawn_cards_player[player_iter].reserve(num_card_player + 1);
                drawn_cards_player[player_iter].push_back("cards");
                for(unsigned int card_iter = 0; card_iter < num_card_player; ++ card_iter){
                    drawn_cards_player[player_iter].push_back(std::to_string(temp[card_iter]));
                }

                // write the drawn results to csv files
                write_file[0] = drawn_cards_player[player_iter];
                std::string filename = "files/cards_player_" + std::to_string(player_iter) + ".csv";
                writer_class writer;
                writer.write_csv(write_file, filename);
            }
        };

    public:
        void act(){
            this->draw_cards();
            this->give_cards();
        }
};
