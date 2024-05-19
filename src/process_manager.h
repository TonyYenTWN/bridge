// Header for process manager
#pragma once

// STL
#include <filesystem>
#include <iostream>

// Project specific
#include "src/draw.h"
#include "src/player.h"
#include "src/rw.h"
#include "src/statistic.h"

inline int file_count(std::string dirname){
    auto file_iter = std::filesystem::directory_iterator(dirname);
    unsigned int num_file = 0;
    for(auto &entry : file_iter){
        if(entry.is_regular_file()){
            ++ num_file;
        }
    }

    return num_file;
}

class process_manager_class{
    private:
        unsigned int num_real_player;
        unsigned int real_player_ID;
        unsigned int ace_type;
        unsigned int current_round = 0;

    public:
        int get_real_player_ID(){
            return this->real_player_ID;
        }

        int get_num_real_player(){
            return this->num_real_player;
        }

        void start_game(){
            std::filesystem::create_directories("files");

            // Players first initalization
            std::filesystem::create_directories("files/.init");
            std::cout << "Who are you?";
            std::cout << "\n";
            std::cout << "0) Main player (Card server)";
            std::cout << "\n";
            std::cout << "1) Ordinary player";
            std::cout << "\n";
            std::cout << "Press one of the options to continue. ";

            unsigned int player_ID;
            std::cin >> player_ID;
            std::cout << "\n";

            if(player_ID == 0){
                this->real_player_ID = 0;

                std::cout << "How many players are in this game?";
                std::cout << "\n";
                std::cout << "1) One (test mode)";
                std::cout << "\n";
                std::cout << "2) Two";
                std::cout << "\n";
                std::cout << "4) Four";
                std::cout << "\n";
                std::cout << "Press one of the options to continue. ";

                std::cin >> this->num_real_player;
                std::cout << "\n";

                // Create init file
                std::string filename = "files/.init/player_0_entered.txt";
                writer_class writer;
                writer.write_blank(filename);

                // Check if all players are in
                std::cout << "Waiting for other players to enter...";
                std::cout << "\n";
                while(true){
                    if(file_count("files/.init") == this->num_real_player){
                        std::cout << "All players have entered!";
                        std::cout << "\n";
                        std::cout << "\n";

                        filename = "files/.init/all_entered.txt";
                        writer.write_one_line(std::to_string(this->num_real_player), filename);
                        break;
                    }
                }

                // Check if all other players know all players are in
                while(true){
                    if(file_count("files/.init") == 2 * this->num_real_player){
                        // Delete initialization directory
                        std::filesystem::remove_all("files/.init");
                        break;
                    }
                }
            }
            else{
                std::cout << "Which ordinary player are you?";
                std::cout << "\n";
                std::cout << "1) Player number one";
                std::cout << "\n";
                std::cout << "2) Player number two";
                std::cout << "\n";
                std::cout << "3) Player number three";
                std::cout << "\n";
                std::cout << "Press one of the options to continue. ";

                std::cin >> this->real_player_ID;
                std::cout << "\n";

                // Create init file
                std::string filename = "files/.init/player_" + std::to_string(this->real_player_ID) + "_entered.txt";
                writer_class writer;
                writer.write_blank(filename);

                // Check if all players are in
                std::cout << "Waiting for other players to enter...";
                std::cout << "\n";
                reader_class reader;
                while(true){
                    filename = "files/.init/all_entered.txt";
                    if(std::filesystem::exists(filename)){
                        std::cout << "All players have entered!";
                        std::cout << "\n";
                        std::cout << "\n";

                        // Store number of total players
                        this->num_real_player = std::stod(reader.read_one_line(filename));

                        // Confirm player knows all other players have entered
                        filename = "files/.init/player_" + std::to_string(this->real_player_ID) + "_confirmed.txt";
                        writer.write_blank(filename);
                        break;
                    }
                }
            }
        }

        std::vector <player_class> assign_players(){
            std::vector <unsigned int> assign_ID;
            if(this->num_real_player == 1){
                assign_ID = std::vector <unsigned int> (4);
                assign_ID[0] = 0;
                assign_ID[1] = 1;
                assign_ID[2] = 2;
                assign_ID[3] = 3;
            }
            else if(this->num_real_player == 2){
                assign_ID = std::vector <unsigned int> (2);
                assign_ID[0] = this->real_player_ID;
                assign_ID[1] = assign_ID[0] + 2;
            }
            else if(this->num_real_player == 4){
                assign_ID = std::vector <unsigned int> (1);
                assign_ID[0] = this->real_player_ID;
            }

            std::vector <player_class> output;
            output.reserve(assign_ID.size());
            std::cout << "Assigning virtual players to the player...";
            std::cout << "\n";
            for(unsigned int player_iter = 0; player_iter < assign_ID.size(); ++ player_iter){
                player_class player_temp;
                player_temp.ID_set(assign_ID[player_iter]);
                std::cout << "Virtual player " + std::to_string(assign_ID[player_iter]) + " is assigned to the player!";
                std::cout << "\n";

                output.push_back(player_temp);
            }
            std::cout << "\n";

            return output;
        }

        void draw_cards(){
            if(this->real_player_ID != 0){
                return;
            }

            std::cout << "Cards are shuffled and drawn for the players.";
            std::cout << "\n";
            std::cout << "\n";
            drawer_class drawer;
            drawer.act();
        }

        void receive_cards(std::vector <player_class> &players){
            reader_class reader;
            std::cout << "Virtual players are receiving cards...";
            std::cout << "\n";
            for(unsigned int player_iter = 0; player_iter < players.size(); ++ player_iter){
                std::string filename = "files/cards_player_" + std::to_string(players[player_iter].get_ID()) + ".csv";;

                auto card_data = reader.read_csv(filename);
                std::vector <unsigned int> cards;
                cards.reserve(card_data["cards"].size());
                for(unsigned int card_iter = 0; card_iter < card_data["cards"].size(); ++ card_iter){
                    cards.push_back(std::stod(card_data["cards"][card_iter]));
                }

                players[player_iter].cards_set(cards);

                std::cout << "Virtual player " + std::to_string(players[player_iter].get_ID()) + " received their cards!";
                std::cout << "\n";
            }

            std::cout << "\n";
        }

        void bid_ace(std::vector <player_class> &players){
            std::cout << "Players now bid for ace.";
            std::cout << "\n";
            std::filesystem::create_directories("files/.bid");

            // Show cards for default virtual player
            std::cout << "Cards of virtual player " + std::to_string(players[0].get_ID()) + ":";
            std::cout << "\n";
            for(unsigned int card_iter = 0; card_iter < players[0].get_cards_size(); ++ card_iter){
                std::cout << players[0].get_card_name(card_iter);
                std::cout << "\n";
            }
            std::cout << "\n";

            // Place bids
        }

        void end_game(){
            if(this->real_player_ID != 0){
                std::cout << "Game finished!";
                std::cout << "\n";
                return;
            }

            std::cout << "Game finished! Delete game files?";
            std::cout << "\n";
            std::cout << "0) No";
            std::cout << "\n";
            std::cout << "1) Yes";
            std::cout << "\n";
            std::cout << "Press one of the options to continue. ";

            bool delete_flag;
            std::cin >> delete_flag;
            if(delete_flag){
                std::filesystem::remove_all("files");
            }
        }
};
