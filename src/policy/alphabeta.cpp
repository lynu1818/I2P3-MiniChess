#include <cstdlib>
#include <algorithm>

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
  auto next_state = state->legal_next_states.begin();
  for(;next_state != state->legal_next_states.end();next_state++){
    if((*next_state)->value == maximizing_value){
      return (*next_state)->prev_move;
    }
    else continue;
  }
  // auto actions = state->legal_actions;
  // return actions[(rand()+depth)%actions.size()];
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
  if(depth == 0 || state->game_state == WIN ||state->game_state == DRAW){
    return state->evaluate();
  }
  if(maximizingPlayer){
    state->value = INT32_MIN;
    auto next_state = state->legal_next_states.begin();
    for(;next_state != state->legal_next_states.end();next_state++){
        state->value = std::max(state->value, alphabeta(*next_state, depth-1, alpha, beta, false));
        alpha = std::max(alpha, state->value);
        if(alpha >= beta) break;
    }
    return state->value;
  }
  else{
    state->value = INT32_MAX;
    auto next_state = state->legal_next_states.begin();
    for(;next_state != state->legal_next_states.end();next_state++){
        state->value = std::min(state->value, alphabeta(*next_state, depth-1, alpha, beta, true));
        beta = std::min(beta, state->value);
        if(beta <= alpha) break;
    }
    return state->value;
  }
}