class Node {
public:
    int depth;
    bool player;
    Board board;
    int value;
    vector<Node> children;
    int state;
  
    Node(int depth, bool player, Board board, int state, int value = 0) : depth(depth), player(player), board(board), state(state), value(value) {}

    int get_state() const{
        return state;
    }

    void create_children() {
        int value_child;
        int dist1, dist2;
        if (depth > 0 && abs(value) != MAX) {
            if (player == 0) {
                
                if (board.player1.get_number_walls() > 0) {
                    bool possible;
                    vector<int> close_walls = board.player2.get_close_walls();
                    for (int state : close_walls) {
                        possible = true;
                        for (int state_imp : board.grid.get_list_states_impossible()) {
                            if (state_imp == state) {
                                possible = false;
                                break;
                            }
                        }
                        if (!possible) continue;
                        
                        if (!board.is_possible_wall(state, &dist1, &dist2)) continue;
                        Board board_child = board;
                        board_child.grid.add_wall(state, player);
                        board_child.update_valid_next_pos(0);
                        board_child.update_valid_next_pos(1);
                        value_child = dist2 - dist1;
                        board_child.player1.dec_wall();
                        children.push_back(*(new Node(depth - 1, !player, board_child, state + numberGridSquares - 1, value_child)));
                    }
                }
                for (int state : board.player1.get_valid_next_pos()) { // player
                    Board board_child = board;
                    board_child.moove_player(0, state);
                    if (state / GRID_SIZE == 0) {
                        children.push_back(*(new Node(depth - 1, !player, board_child, state, MAX)));
                    }
                    else {
                        value_child = real_val(board_child);
                        children.push_back(*(new Node(depth - 1, !player, board_child, state, value_child)));
                    }
                }
            }
            else {
                
                if (board.player2.get_number_walls() > 0) {
                    bool possible;
                    vector<int> close_walls = board.player2.get_close_walls();
                    for (int state : close_walls) {
                        possible = true;
                        for (int state_imp : board.grid.get_list_states_impossible()) {
                            if (state_imp == state) {
                                possible = false;
                                break;
                            }
                        }
                        if (!possible) continue;
                        if (!board.is_possible_wall(state, &dist1, &dist2)) continue;
                        Board board_child = board;
                        board_child.grid.add_wall(state, player);
                        board_child.update_valid_next_pos(1);
                        board_child.update_valid_next_pos(0);
                        value_child = dist2 - dist1;
                        board_child.player2.dec_wall();
                        children.push_back(*(new Node(depth - 1, !player, board_child, state + numberGridSquares - 1, value_child)));
                    }
                }
                for (int state : board.player2.get_valid_next_pos()) { // player
                    Board board_child = board;
                    board_child.moove_player(1, state);
                    if (state / GRID_SIZE == GRID_SIZE - 1) {
                        children.push_back(*(new Node(depth - 1, !player, board_child, state, -MAX)));
                    }
                    else {
                        value_child = real_val(board_child);
                        children.push_back(*(new Node(depth - 1, !player, board_child, state, value_child)));
                    }
                }
            }
        }
    }
    int real_val(Board board) {
        int dist1 = board.shortest_path(1);
        int dist2 = board.shortest_path(0);
        return dist1 - dist2;
    }
};

int minmax(Node node, bool player, int alpha, int beta) {
    int best_value, val;
    if (node.depth == 0 || abs(node.value) > MAX / 2)
        return node.value;
    node.create_children();
    if (player == 0) {
        best_value = MIN + 1000 * node.depth;
        for (Node child : node.children) {
            val = minmax(child, !player, alpha, beta);
            best_value = max(best_value, val);
            alpha = max(alpha, val);
            if (beta <= alpha) 
                break;
        }
    }
    else {
        best_value = MAX - 1000 * node.depth;
        for (Node child : node.children) {
            val = minmax(child, !player, alpha, beta);
            best_value = min(best_value, val);
            beta = min(beta, val);
            if (beta <= alpha)
                break;
                
        }
    }
    return best_value;
}