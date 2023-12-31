#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"


/**
 * @brief evaluate the state
 * 
 * @return int 
 */

static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};



int State::countThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W], int kingRow, int kingCol){
  int threats = 0;
  threats += State::countPawnThreats(oppn_board, self_board, kingRow, kingCol);
  threats += State::countKnightThreats(oppn_board, self_board, kingRow, kingCol);
  threats += State::countBishopThreats(oppn_board, self_board, kingRow, kingCol);
  threats += State::countRookThreats(oppn_board, self_board, kingRow, kingCol);
  threats += State::countQueenThreats(oppn_board, self_board, kingRow, kingCol);
  return threats;
}

int State::countPawnThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W],int kingRow, int kingCol){
  int threats = 0;

  for(int i=-1;i<=1;i++){
    for(int j=-1;j<=1;j++){
      if(i==0 && j==0) continue;
      int row = kingRow + i;
      int col = kingCol + j;
      if(row >= 0 && row < BOARD_H && col >= 0 && col < BOARD_W){
        if((oppn_board[row][col]) == '1') threats++;
      }
    }
  }

  return threats;
}

int State::countKnightThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W], int kingRow, int kingCol){
  int threats = 0;

  int dir_x[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
  int dir_y[8] = {1, 2, 2, 1, -1, -2, -2, -1};

  for(int i=0;i<8;i++){
    int row = kingRow + dir_x[i];
    int col = kingCol + dir_y[i];
    if(row >= 0 && row < BOARD_H && col >= 0 && col < BOARD_W){
      if((oppn_board[row][col]) == '3') threats++;
    }
  }

  return threats;
}

int State::countBishopThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W], int kingRow, int kingCol){
  int threats = 0;

  for(int i=4;i<8;i++){
    for(int j=0;j<5;j++){
      int row = kingRow + move_table_rook_bishop[i][j][0];
      int col = kingCol + move_table_rook_bishop[i][j][1];
      if(row >= 0 && row < BOARD_H && col >= 0 && col < BOARD_W){
        if(oppn_board[row][col] != '4' && oppn_board[row][col] != '0')break;
        else if(self_board[row][col] != '0') break;
        if((oppn_board[row][col]) == '4') threats++;
      }
    }
  }

  return threats;
}

int State::countRookThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W], int kingRow, int kingCol){
  int threats = 0;
  bool pathBlocked[4] = {false, false, false, false}; //up, down, right, left

  for(int i=0;i<4;i++){
    for(int j=0;j<5;j++){
      int row = kingRow + move_table_rook_bishop[i][j][0];
      int col = kingCol + move_table_rook_bishop[i][j][1];
      if(row >= 0 && row < BOARD_H && col >= 0 && col < BOARD_W){
        if(oppn_board[row][col] != '2' && oppn_board[row][col] != '0')break;
        else if(self_board[row][col] != '0') break;
        else if((oppn_board[row][col]) == '2') threats++;
      }
    }
  }

  return threats;
}

int State::countQueenThreats(char oppn_board[BOARD_H][BOARD_W], char self_board[BOARD_H][BOARD_W], int kingRow, int kingCol){
  int threats = 0;

  for(int i=0;i<8;i++){
    for(int j=0;j<5;j++){
      int row = kingRow + move_table_rook_bishop[i][j][0];
      int col = kingCol + move_table_rook_bishop[i][j][1];
      if(row >= 0 && row < BOARD_H && col >= 0 && col < BOARD_W){
        if(oppn_board[row][col] != '5' && oppn_board[row][col] != '0')break;
        else if(self_board[row][col] != '0') break;
        if((oppn_board[row][col]) == '5') threats++;
      }
    }
  }

  return threats;
}

bool State::isUndevelopedPiece(int now_piece, int row, int col, bool self){
  switch(now_piece){
    case '1': //pawn
      if(this->player == 0){
        if(row == 4) return true;
      }
      else if(row == 1) return true;
      break;
    case '2': //rook
      if(this->player == 0){
        if(row == 5 && col == 0) return true;
      }
      else if(row == 0 && col == 4) return true;
      break;
    case '3': //knight
      if(this->player == 0){
        if(row == 5 && col == 1) return true;
      }
      else if(row == 0 && col == 3) return true;
      break;
    case '4': //bishop
      if(this->player == 0){
        if(row == 5 && col == 2) return true;
      }
      else if(row == 0 && col == 2) return true;
      break;
    case '5': //queen
      if(this->player == 0){
        if(row == 5 && col == 3) return true;
      }
      else if(row == 0 && col == 1) return true;
      break;
    defalut:
      return false;
  }
}

int State::evaluate(int player){
  // [TODO] design your own evaluation function
  int score = 0;
  //int piececount = 0;
  int kingRow, kingCol;
  //int self_undev = 0, oppn_undev = 0;
  auto self_board = this->board.board[player];
  auto oppn_board = this->board.board[1 - player];
  int now_piece;
  for(int i=0;i<BOARD_H;i++){
    for(int j=0;j<BOARD_W;j++){
      if((now_piece=self_board[i][j])){
        //piececount++;
        //if(isUndevelopedPiece(now_piece, i, j, true)) self_undev++;
        switch(now_piece){
          case 1: //pawn
            score += 1;
            break;
          case 2: //rook
            score += 5;
            break;
          case 3: //knight
            score += 3;
            break;
          case 4: //bishop
            score += 3;
            break;
          case 5: //queen
            score += 9;
            break;
          case 6: //king
            score += 90;
            kingRow = i;
            kingCol = j;
            break;
          default:
            score += 0;
        }
      }
      if((now_piece=oppn_board[i][j])){
        //if(isUndevelopedPiece(now_piece, i, j, false)) oppn_undev++;
        switch(now_piece){
          case 1: //pawn
            score -= 1;
            break;
          case 2: //rook
            score -= 5;
            break;
          case 3: //knight
            score -= 3;
            break;
          case 4: //bishop
            score -= 3;
            break;
          case 5: //queen
            score -= 9;
            break;
          case 6: //king
            score -= 90;
            break;
          default:
            score += 0;
        }
      }
    }
  }

  // if(piececount > 6){ //centerControl
  //   int centerControl = 0;
  //   for(int i=1;i<=4;i++){
  //     for(int j=1;j<=3;j++){
  //       if((now_piece=self_board[i][j])) centerControl++;
  //       else if((now_piece=oppn_board[i][j])) centerControl--;
  //     }
  //   }
  //   score += centerControl;
  // }
  // else{ //kingSafety
    //int kingSafety = 0;
    int threats = State::countThreats(oppn_board, self_board, kingRow, kingCol);
    //kingSafety -= threats;
    //score = score - self_undev*2 + oppn_undev*2;
    score -= threats * 10;
  // }

  return score;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}




/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i++){
    for(int j=0; j<BOARD_W; j++){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


// const char piece_table[2][7][5] = {
//   {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
//   {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
// };

/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}