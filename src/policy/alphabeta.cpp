#include <cstdlib>
#include <algorithm>
#include <climits>

#include "../state/state.hpp"
#include "./alphabeta.hpp"


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move Alphabeta::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  int maximizing_value = Alphabeta::alphabeta(state, depth, INT32_MIN, INT32_MAX, true);
  return state->prev_move;
}

int Alphabeta::alphabeta(State *state, int depth, int alpha, int beta, bool maximizingPlayer){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  //push all possible legal next states
  auto actions = state->legal_actions.begin();
  for(;actions != state->legal_actions.end();actions++){
    State* tmp = state->next_state(*actions);
    tmp->prev_move = (*actions);
    state->legal_next_states.push_back(tmp);
  }

  if(depth == 0 || state->game_state == WIN){
    return state->evaluate();
  }
  if(maximizingPlayer){
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = alphabeta(next_state, depth-1, alpha, beta, false);
      //if(val >= bestValue && depth == 3) state->prev_move = next_state->prev_move;
      if(alpha < val && depth == 4) state->prev_move = next_state->prev_move;
      alpha = std::max(alpha, val);
      if(alpha >= beta) break;
    }
    return alpha;
  }
  else{
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = alphabeta(next_state, depth-1, alpha, beta, true);
      //if(val <= bestValue && depth == 3) state->prev_move = next_state->prev_move;
      if(beta > val && depth == 4) state->prev_move = next_state->prev_move;
      beta = std::min(beta, val);
      if(beta <= alpha) break;
    }
    return beta;
  }
}