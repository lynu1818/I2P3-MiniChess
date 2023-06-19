#include <cstdlib>
#include <algorithm>
#include <climits>

#include "../state/state.hpp"
#include "./minimax.hpp"


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move Minimax::get_move(State *state, int depth, int player){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  int maximizing_value = Minimax::minimax(state, depth, player);
  return state->prev_move;
}

int Minimax::minimax(State *state, int depth, int maximizingPlayer){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  
  if(depth == 0 || state->game_state == WIN){
    return state->evaluate(maximizingPlayer);
  }
  if(maximizingPlayer){
    int bestValue = INT_MIN;
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = minimax(next_state, depth-1, maximizingPlayer);
      if(val > bestValue && depth == 4) state->prev_move = next_state->prev_move;
      bestValue = std::max(bestValue, val);
    }
    return bestValue;
  }
  else{
    int bestValue = INT_MAX;
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = minimax(next_state, depth-1, maximizingPlayer);
      if(val < bestValue && depth == 4) state->prev_move = next_state->prev_move;
      bestValue = std::min(bestValue, val);
    }
    return bestValue;
  }
}