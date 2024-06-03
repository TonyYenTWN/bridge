// STL

// project specific
#include "src/process_manager.h"

int main()
{
    process_manager_class process_manager;
    process_manager.start_game();
    auto players = process_manager.assign_players();
    process_manager.draw_cards();
    process_manager.receive_cards(players);
    process_manager.bid_start();
    process_manager.bid_ace(players);
    process_manager.play_game(players);
    process_manager.end_game();
    return 0;
}
