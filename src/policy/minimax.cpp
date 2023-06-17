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
Move Minimax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  int maximizing_value = Minimax::minimax(state, depth, true);
  for(State *next_state: state->legal_next_states){
    if(next_state->evaluate() == maximizing_value) return next_state->prev_move;
  }
  auto actions = state->legal_actions;
  return actions[0];
}

int Minimax::minimax(State *state, int depth, bool maximizingPlayer){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  //push all possible legal next states
  // for(Move actions: state->legal_actions){
  //   State* tmp = state->next_state(actions);
  //   tmp->prev_move = actions;
  //   state->legal_next_states.push_back(tmp);
  // }
  
  if(depth == 0 || state->game_state == WIN){
    //state->value = state->evaluate();
    return state->evaluate();
  }
  if(maximizingPlayer){
    int bestValue = INT_MIN;
    //state->value = INT_MIN;
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = minimax(next_state, depth-1, false);
      bestValue = std::max(bestValue, val);
      if(val >= bestValue) state->prev_move = next_state->prev_move;
    }
    // for(State *next_state: state->legal_next_states){
    //   //state->value = std::max(state->value, minimax(next_state, depth-1, false));
    // }
    //return state->value;
    return bestValue;
  }
  else{
    int bestValue = INT_MAX;
    //state->value = INT_MAX;
    for(Move actions: state->legal_actions){
      State *next_state = state->next_state(actions);
      next_state->prev_move = actions;
      int val = minimax(next_state, depth-1, true);
      bestValue = std::min(bestValue, val);
      if(val <= bestValue) state->prev_move = next_state->prev_move;
    }
    // for(State *next_state: state->legal_next_states){
    //   //state->value = std::min(state->value, minimax(next_state, depth-1, true));
    // }
    return bestValue;
    //return state->value;
  }
}