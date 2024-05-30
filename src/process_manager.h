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

        void bid_start(){
            std::filesystem::create_directories("files/.bid");
            writer_class writer;
            if(this->real_player_ID == 0){
                // record first next player
                std::string filename = "files/.bid/player_next.txt";
                unsigned int next_player_ID;
                if(this->real_player_ID == this->num_real_player - 1){
                    next_player_ID = 0;
                }
                else{
                    next_player_ID = this->real_player_ID + 1;
                }
                writer.write_one_line(std::to_string(next_player_ID), filename);

                // record initial bid level
                filename = "files/.bid/bid_level.csv";
                std::vector <std::vector <std::string>> data(2);
                data[0] = std::vector <std::string> (2);
                data[0][0] = "type";
                data[0][1] = "0";
                data[1] = std::vector <std::string> (2);
                data[1][0] = "number";
                data[1][1] = "0";
                writer.write_csv(data, filename);

                // record players that have passed
                filename = "files/.bid/passed.txt";
                writer.write_one_line("0", filename);

                // record whether the loop should end
                filename = "files/.bid/end_bid_signal.txt";

                // set end signal to false
                filename = "files/.bid/end_bid_signal.txt";
                writer.write_one_line("0", filename);
            }
        }

        void bid_ace(std::vector <player_class> &players){
            // Main loop for bidding
            while(true){
                reader_class reader;
                writer_class writer;

                // Check if there are still other players eligible for bidding
                std::string filename = "files/.bid/passed.txt";
                unsigned int passed_player = stoi(reader.read_one_line(filename));

                filename = "files/.bid/bid_level.csv";
                auto bid_now = reader.read_csv(filename);
                unsigned int num_now = (unsigned int) stoi(bid_now["number"][0]);
                unsigned int type_now = (unsigned int) stoi(bid_now["type"][0]);
                unsigned int passed_min;
                if(num_now == 0 && type_now == 0){
                    passed_min = this->num_real_player;
                }
                else{
                    passed_min = this->num_real_player - 1;
                }

                if(passed_player == passed_min){
                    std::cout << "All other players have passed!";
                    std::cout << "\n";
                    std::cout << "\n";

                    // Send end signal to other players
                    filename = "files/.bid/end_bid_signal.txt";
                    writer.write_one_line("1", filename);

                    // Record the player getting bet
                    filename = "files/.bid/bet_player_real.txt";
                    writer.write_one_line(std::to_string(this->real_player_ID), filename);
                    std::filesystem::create_directories("files/.game");
                    filename = "files/.game/turn_player_virtual.txt";
                    writer.write_one_line(std::to_string(players[0].get_ID()), filename);
                    filename = "files/.game/accumulated_player_virtual.txt";
                    writer.write_one_line(std::to_string(0), filename);
                    filename = "files/.game/current_round.txt";
                    writer.write_one_line(std::to_string(1), filename);

                    // Update the ace to 1 no king if no one bids
                    if(num_now == 0 && type_now == 0){
                        filename = "files/.bid/bid_level.csv";
                        std::vector <std::vector <std::string>> data(2);
                        data[0] = std::vector <std::string> (2);
                        data[0][0] = "type";
                        data[0][1] = "5";
                        data[1] = std::vector <std::string> (2);
                        data[1][0] = "number";
                        data[1][1] = "1";
                        writer.write_csv(data, filename);
                    }

                    return;
                }

                // Check if the loop should end
                filename = "files/.bid/end_bid_signal.txt";
                bool end_flag = (bool) stoi(reader.read_one_line(filename));
                if(end_flag){
                    break;
                }

                // Check if it is the real player's turn
                filename = "files/.bid/player_next.txt";
                unsigned int next_player = stoi(reader.read_one_line(filename));
                if(next_player != this->real_player_ID){
                    continue;
                }

                // Bidding process
                std::cout << "Players now bid for ace.";
                std::cout << "\n";

                // Show cards for default virtual player
                std::cout << "Cards of virtual player " + std::to_string(players[0].get_ID()) + ":";
                std::cout << "\n";
                for(unsigned int card_iter = 0; card_iter < players[0].get_cards_size(); ++ card_iter){
                    std::cout << "Card " << card_iter + 1 << ":" << players[0].get_card_name(card_iter);
                    std::cout << "\n";
                }
                std::cout << "\n";

                filename = "files/.bid/bid_level.csv";
                auto prev_bid = reader.read_csv(filename);
                unsigned int num_min = (unsigned int) stoi(prev_bid["number"][0]);
                unsigned int type_min = (unsigned int) stoi(prev_bid["type"][0]);
                unsigned int num_bid;
                unsigned int type_bid;
                // Bid submission
                bool pass_flag = 0;
                while(true){
                    std::cout << "Which ace number do you want to bid for ?";
                    std::cout << "\n";
                    std::cout << "0) Pass";
                    std::cout << "\n";
                    for(unsigned int num_iter = 1; num_iter <= 7; ++ num_iter){
                        if(num_iter < num_min){
                            continue;
                        }
                        if(num_iter == num_min && type_min == 5){
                            continue;
                        }

                        std::cout << num_iter << ") " << num_iter;
                        std::cout << "\n";
                    }
                    std::cout << "Press one of the options to continue. ";
                    std::cin >> num_bid;
                    std::cout << "\n";

                    std::vector <std::string> type_ID(5);
                    if(num_bid != 0){
                        std::cout << "Which ace type do you want to bid for ?";
                        std::cout << "\n";
                        type_ID[0] = "Flower";
                        type_ID[1] = "Square";
                        type_ID[2] = "Heart";
                        type_ID[3] = "Spade";
                        type_ID[4] = "No King";
                        for(unsigned int type_iter = 0; type_iter < 5; ++ type_iter){
                            if(num_bid == num_min){
                                if(type_iter < type_min){
                                    continue;
                                }
                            }

                            std::cout << type_iter + 1 << ") " << type_ID[type_iter];
                            std::cout << "\n";
                        }
                        std::cout << "Press one of the options to continue. ";
                        std::cin >> type_bid;
                        std::cout << "\n";
                    }
                    else{
                        pass_flag = 1;
                    }

                    bool break_flag;
                    std::cout << "Your bid choice is: ";
                    if(num_bid != 0){
                        std::cout << num_bid << " " << type_ID[type_bid - 1];
                        std::cout << "\n";
                    }
                    else{
                        std::cout << "Pass";
                        std::cout << "\n";
                    }
                    std::cout << "Confirmed?";
                    std::cout << "\n";
                    std::cout << "0) No";
                    std::cout << "\n";
                    std::cout << "1) Yes";
                    std::cout << "\n";
                    std::cout << "Press one of the options to continue. ";
                    std::cin >> break_flag;
                    std::cout << "\n";
                    if(break_flag){
                        break;
                    }
                }

                // Rewrite current bid level
                if(!pass_flag){
                    filename = "files/.bid/bid_level.csv";
                    std::vector <std::vector <std::string>> data(2);
                    data[0] = std::vector <std::string> (2);
                    data[0][0] = "type";
                    data[0][1] = std::to_string(type_bid);
                    data[1] = std::vector <std::string> (2);
                    data[1][0] = "number";
                    data[1][1] = std::to_string(num_bid);
                    writer.write_csv(data, filename);
                }
                else{
                    passed_player += 1;
                    filename = "files/.bid/passed.txt";
                    writer.write_one_line(std::to_string(passed_player), filename);
                }

                // Update next player
                filename = "files/.bid/player_next.txt";
                unsigned int next_player_ID;
                if(this->real_player_ID == this->num_real_player - 1){
                    next_player_ID = 0;
                }
                else{
                    next_player_ID = this->real_player_ID + 1;
                }
                writer.write_one_line(std::to_string(next_player_ID), filename);
            }
        }

        void play_game(std::vector <player_class> &players){
            reader_class reader;
            writer_class writer;

            // Read ace type and number
            std::string filename = "files/.bid/bid_level.csv";
            auto ace_bid = reader.read_csv(filename);
            unsigned int ace_num = (unsigned int) stoi(ace_bid["number"][0]);
            unsigned int ace_type = (unsigned int) stoi(ace_bid["type"][0]);

            bool break_flag = 0;
            while(true){
                // End loop when all rounds are performed
                if(break_flag){
                    break;
                }

                for(unsigned int player_iter = 0; player_iter < players.size(); ++ player_iter){
                    // Check if all rounds are played
                    filename = "files/.game/current_round.txt";
                    unsigned int current_round = (unsigned int) stoi(reader.read_one_line(filename));
                    // End loop when all rounds are performed
                    if(current_round > statistic::get_num_rounds()){
                        break_flag = 1;
                        break;
                    }

                    // Check if already all players played the round
                    filename = "files/.game/accumulated_player_virtual.txt";
                    unsigned int accumulated_player = (unsigned int) stoi(reader.read_one_line(filename));
                    if(accumulated_player == statistic::get_num_players()){
                        // Check if current virtual player is the winner

                        break;
                    }

                    // Check if it is the virtual player's turn
                    filename = "files/.game/turn_player_virtual.txt";
                    unsigned int turn_player = (unsigned int) stoi(reader.read_one_line(filename));
                    if(turn_player != players[player_iter].get_ID()){
                        continue;
                    }

                    // Play the card and update current winner
                    std::cout << "Round: " << current_round;
                    std::cout << "\n";
                    if(accumulated_player == 0){
                        // First virtual player has no constraint on the card type they can play
                        std::cout << "Available cards of virtual player " + std::to_string(players[player_iter].get_ID()) + ":";
                        std::cout << "\n";
                        for(unsigned int card_iter = 0; card_iter < players[player_iter].get_cards_size(); ++ card_iter){
                            if(players[player_iter].get_card_status(card_iter) == 0){
                                std::cout << "Card " << card_iter + 1 << ": " << players[player_iter].get_card_name(card_iter);
                                std::cout << "\n";
                            }
                        }
                        std::cout << "Choose the card you want to play...";
                        unsigned int card_chosen;
                        std::cin >> card_chosen;
                        card_chosen -= 1;
                        players[player_iter].update_card_status(card_chosen, current_round);
                        std::cout << "\n";

                        // Update card type of current round
                        filename = "files/.game/type_current_round.txt";
                        unsigned int type_current_round = players[player_iter].get_card_type(card_chosen);
                        type_current_round += 1;
                        writer.write_one_line(std::to_string(type_current_round), filename);
                    }
                    else{
                        // Other virtual players must follow the card type of first player unless they run out of the card type
                        bool type_constrained_flag = 0;
                        filename = "files/.game/type_current_round.txt";
                        unsigned int type_current_round = (unsigned int) stoi(reader.read_one_line(filename));
                        unsigned int start_ace_ID = 0;
                        for(unsigned int card_iter = 0; card_iter < players[player_iter].get_cards_size(); ++ card_iter){
                            unsigned int card_type = players[player_iter].get_card_type(card_iter);
                            card_type += 1;

                            if(card_type == type_current_round){
                                type_constrained_flag = 1;
                                start_ace_ID = card_iter;
                                break;
                            }
                        }

                        std::cout << "Available cards of virtual player " + std::to_string(players[player_iter].get_ID()) + ":";
                        std::cout << "\n";
                        for(unsigned int card_iter = start_ace_ID; card_iter < players[player_iter].get_cards_size(); ++ card_iter){
                            if(players[player_iter].get_card_status(card_iter) != 0){
                                continue;
                            }

                            // Break loop if no more same card type of the round
                            if(type_constrained_flag){
                                unsigned int card_type = players[player_iter].get_card_type(card_iter);
                                card_type += 1;
                                if(card_type != type_current_round){
                                    break;
                                }
                            }

                            std::cout << "Card " << card_iter + 1 << ": " << players[player_iter].get_card_name(card_iter);
                            std::cout << "\n";
                        }
                        std::cout << "Choose the card you want to play...";
                        unsigned int card_chosen;
                        std::cin >> card_chosen;
                        card_chosen -= 1;
                        players[player_iter].update_card_status(card_chosen, current_round);
                        std::cout << "\n";
                    }

                    // Update players played, next player and (possibly) rounds played
                    // The following order is designed to avoid race conditions!!
                    if(accumulated_player + 1 == statistic::get_num_players()){
                        // Update winner of the round

                        // Check if current virtual player is the winner

                    }

                    // Update accumulated players played in the round
                    filename = "files/.game/accumulated_player_virtual.txt";
                    accumulated_player += 1;
                    writer.write_one_line(std::to_string(accumulated_player), filename);

                    if(accumulated_player == statistic::get_num_players()){
                        // If round ends
                        // Set winner of this round as first player of next round

                        // Reset accumulayed player to 0 for other players to play the next round
                        filename = "files/.game/accumulated_player_virtual.txt";
                        accumulated_player = 0;
                        writer.write_one_line(std::to_string(accumulated_player), filename);

                        // Update current round
                        filename = "files/.game/current_round.txt";
                        current_round += 1;
                        writer.write_one_line(std::to_string(current_round), filename);
                        if(current_round > statistic::get_num_rounds()){
                            break_flag = 1;
                            break;
                        }
                    }
                    else{
                        // Current round continues
                        // Update next player
                        filename = "files/.game/turn_player_virtual.txt";
                        unsigned int next_player_ID;
                        if(turn_player == statistic::get_num_players() - 1){
                            next_player_ID = 0;
                        }
                        else{
                            next_player_ID = turn_player + 1;
                        }
                        writer.write_one_line(std::to_string(next_player_ID), filename);
                    }
                }
            }
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
