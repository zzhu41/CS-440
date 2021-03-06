#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <stack>
#include <queue>
#include <math.h>
#include <map>
#include <time.h>
using namespace std;

// class Piece{
// public:
//   int x; 
//   int y;
//   int what; //"0" stands for blank "1" stands for black piece "2" stands for white piece
//   // constructor
//   Piece(int x, int y, int what){
//     this->x = x;
//     this->y = y;
//     this->what = what; 
    
//   }

// };
int count_alphabeta, count_minimax;
int orig_i, orig_j, new_i, new_j;
int black_count, white_count;
void printVector(vector<vector<int> > v){
  for(int i = 0; i < v.size(); i++){
    for(int j = 0; j < v[0].size(); j++){
      cout << v[i][j];
    }
    cout << endl;
  }
}
int tell_win(vector<vector<int>> board){
	if(black_count == 0 || white_count == 0){
		cout << "one color has nothing remain" << endl;
		if(black_count == 0){
			cout<<"white wins"<<endl;
			return 2;
		}
		else if(white_count == 0){
			cout<<"black wins"<<endl;
			return 1;
		}
	}
	for(int i =0;i<10;i++){
		if(board[0][i] == 2){
			cout << "white reach the first line, white wins" << endl;
			return 2;
		}
		if(board[4][i] == 1){
			cout << "black reach the last line, black wins" << endl;
			return 1;
		}
	}
	return 0;
}
double alpha_beta(vector<vector<int>> board, int current_depth,int max_depth, int start_color, int current_color, int remaining_black, int remaining_white, int strategy,double a,double b){
    count_alphabeta ++;
    if(current_depth == max_depth){
    	if(start_color == 1){ // if own is black
    		if(strategy == 0){
    			double cur_heuristic = 2*(remaining_black) + ((double) rand() / (RAND_MAX));
    			// cout << cur_heuristic << endl;
    			return cur_heuristic;
    		}
    		else if(strategy == 1){
    			double cur_heuristic = 2*(30 - remaining_white) + ((double) rand() / (RAND_MAX));
    			//cout << cur_heuristic << endl;
    			return cur_heuristic;
    		}
    		else if(strategy == 2){
    			double cur_heuristic = (remaining_black)+ 10*(16-remaining_white)+ ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}
    		else if(strategy == 3){
    			double cur_heuristic = (remaining_black) + 2*(30 - remaining_white) + ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}
    	}
    	else{ // if own is white
    		if(strategy == 0){
    			double cur_heuristic = 2*(remaining_white) + ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}
    		else if(strategy == 1){
    			double cur_heuristic = 2*(30 - remaining_black) + ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}	
    		else if(strategy == 2){
    			double cur_heuristic = (remaining_white) + 10*(16-remaining_black) + ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}
    		else if(strategy == 3){
    			double cur_heuristic = (remaining_white) + 2*(30 - remaining_black) + ((double) rand() / (RAND_MAX));
    			return cur_heuristic;
    		}
    	}
    }
    else{
    	if(start_color == current_color){ // is max
    		double max_heuristic = INT_MIN;
	    	if(current_color == 1){ // current move is black
	    		// go through whole board to find black piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					if(i+1==4){
	    						// cout << __LINE__ << endl;
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i+1;
    								if(j-1 >= 0){
    									new_j = j-1;
    								}
    								else{
    									new_j = j+1;
    								}
    							}
	    						return INT_MAX;
	    					}
    						// go three direction
    						// left down
    						if(j-1>=0){
    							if(board[i+1][j-1] == 2){ // if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
	    							}

	    							board[i][j] = 1;
	    							board[i+1][j-1] = 2;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							} 
    							else if(board[i+1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 0;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}
    						// right down
    						if(j+1<=9){
    							if(board[i+1][j+1] == 2){// if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 2;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    							else if(board[i+1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 0;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}

    						// dowm
    						if(board[i+1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i+1][j] = 1;
    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 1;
    							board[i+1][j] = 0;
    							a = max(a, max_heuristic);
	    						if(b <= a){
	    							break;
	    						}
							}
	    				}
	    			}
	    		}
	    	}
	    	else{ // current move is white
	    		  // go through whole board to find white piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if white
	    				if(board[i][j] == 2){
	    					// detect win
	    					if(i-1 == 0){
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i-1;
    								if(j-1 >= 0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					// cout << __LINE__ << endl;
		    					return INT_MAX;
		    				}
    						// go three direction
    						// left up
    						if(j-1>=0){
    							if(board[i-1][j-1] == 1){ // if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 1;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							} 
    							else if(board[i-1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 0;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}
    						// right up
    						if(j+1<=9){
    							if(board[i-1][j+1] == 1){// if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 1;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    							else if(board[i-1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 0;
	    							a = max(a, max_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}

    						// up
    						if(board[i-1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i-1][j] = 2;
    							double new_max_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 2;
    							board[i-1][j] = 0;
    							a = max(a, max_heuristic);
	    						if(b <= a){
	    							break;
	    						}
							}
	    				}
	    			}
	    		}
	    	}
	    	return max_heuristic;
	    }

	    else{ // is min
	    	double min_heuristic = INT_MAX;
	    	if(current_color == 1){ // current move is black
	    		// go through whole board to find black piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					// detect win
	    					if(i+1 == 4){
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i+1;
    								if(j-1>=0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					// cout << __LINE__ << endl;
		    					return INT_MIN;
		    				}
    						// go three direction
    						// left down
    						if(j-1>=0){
    							if(board[i+1][j-1] == 2){ // if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 2;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							} 
    							else if(board[i+1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 0;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}
    						// right down
    						if(j+1<=9){
    							if(board[i+1][j+1] == 2){// if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 2;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    							else if(board[i+1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}	
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 0;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}

    						// dowm
    						if(board[i+1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i+1][j] = 1;
    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy,a,b);
	    						if(new_min_heuristic <= min_heuristic){
	    							min_heuristic = new_min_heuristic;
	    							if(current_depth == 0){
	    								// cout << __LINE__ << endl;
		    							orig_i = i;
		    							orig_j = j;
		    							new_i = i+1;
		    							new_j = j;
	    							}
	    						}
    							board[i][j] = 1;
    							board[i+1][j] = 0;
    							b = min(b, min_heuristic);
	    						if(b <= a){
	    							break;
	    						}
	    					}
	    				}
	    			}
	    		}
	    	}
	    	else{ // current move is white
	    		  // go through whole board to find white piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if white
	    				if(board[i][j] == 2){
	    					// detect if win
	    					if(i-1==0){
		    					// cout << __LINE__ << endl;
		    					if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i-1;
    								if(j-1>=0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					return INT_MIN;
		    				}
    						// go three direction
    						// left up
    						if(j-1>=0){
    							if(board[i-1][j-1] == 1){ // if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 1;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							} 
    							else if(board[i-1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 0;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}
    						// right up
    						if(j+1<=9){
    							if(board[i-1][j+1] == 1){// if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 1;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    							else if(board[i-1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 0;
	    							b = min(b, min_heuristic);
	    							if(b <= a){
	    								break;
	    							}
    							}
    						}

    						// up
    						if(board[i-1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i-1][j] = 2;
    							double new_min_heuristic = alpha_beta(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy,a,b);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 2;
    							board[i-1][j] = 0;
    							b = min(b, min_heuristic);
	    						if(b <= a){
	    							break;
	    						}
	    					}
	    				}
	    			}
	    		}
	    	}
	    	return min_heuristic;
	    }
    }
}
//offensive 2*(30 - number_of_opponent_pieces_remaining) + random().
// strategy = 0 defensive ,  = 1 offensive ,
double minimax_defensive(vector<vector<int>> board, int current_depth,int max_depth, int start_color, int current_color, int remaining_black, int remaining_white, int strategy){
    count_minimax++;
    if(current_depth == max_depth){
    	if(start_color == 1){ // if own is black
    		if(strategy == 0){
    			return 2*(remaining_black) + ((double) rand() / (RAND_MAX));
    		}
    		else if(strategy == 1){
    			return 2*(30 - remaining_white) + ((double) rand() / (RAND_MAX));
    		}
    	}
    	else{ // if own is white
    			if(strategy == 0){
    			return 2*(remaining_white) + ((double) rand() / (RAND_MAX));
    		}
    		else if(strategy == 1){
    			return 2*(30 - remaining_black) + ((double) rand() / (RAND_MAX));
    		}	
    	}
    }
    else{
    	if(start_color == current_color){ // is max
    		double max_heuristic = INT_MIN;
	    	if(current_color == 1){ // current move is black
	    		// go through whole board to find black piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					if(i+1==4){
	    						// cout << __LINE__ << endl;
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i+1;
    								if(j-1 >= 0){
    									new_j = j-1;
    								}
    								else{
    									new_j = j+1;
    								}
    							}
	    						return INT_MAX;
	    					}
    						// go three direction
    						// left down
    						if(j-1>=0){
    							if(board[i+1][j-1] == 2){ // if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
	    							}

	    							board[i][j] = 1;
	    							board[i+1][j-1] = 2;
    							} 
    							else if(board[i+1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
		    							}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 0;
    							}
    						}
    						// right down
    						if(j+1<=9){
    							if(board[i+1][j+1] == 2){// if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 2;
    							}
    							else if(board[i+1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 0;
    							}
    						}

    						// dowm
    						if(board[i+1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i+1][j] = 1;
    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 1;
    							board[i+1][j] = 0;
							}
	    				}
	    			}
	    		}
	    	}
	    	else{ // current move is white
	    		  // go through whole board to find white piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if white
	    				if(board[i][j] == 2){
	    					// detect win
	    					if(i-1 == 0){
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i-1;
    								if(j-1 >= 0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					// cout << __LINE__ << endl;
		    					return INT_MAX;
		    				}
    						// go three direction
    						// left up
    						if(j-1>=0){
    							if(board[i-1][j-1] == 1){ // if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 1;
    							} 
    							else if(board[i-1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 0;
    							}
    						}
    						// right up
    						if(j+1<=9){
    							if(board[i-1][j+1] == 1){// if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
    									return INT_MAX;
    								}
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 1;
    							}
    							else if(board[i-1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 0;
    							}
    						}

    						// up
    						if(board[i-1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i-1][j] = 2;
    							double new_max_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_max_heuristic >= max_heuristic){
	    								max_heuristic = new_max_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 2;
    							board[i-1][j] = 0;
							}
	    				}
	    			}
	    		}
	    	}
	    	return max_heuristic;
	    }

	    else{ // is min
	    	double min_heuristic = INT_MAX;
	    	if(current_color == 1){ // current move is black
	    		// go through whole board to find black piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					// detect win
	    					if(i+1 == 4){
	    						if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i+1;
    								if(j-1>=0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					// cout << __LINE__ << endl;
		    					return INT_MIN;
		    				}
    						// go three direction
    						// left down
    						if(j-1>=0){
    							if(board[i+1][j-1] == 2){ // if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 2;
    							} 
    							else if(board[i+1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j-1] = 1;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j-1] = 0;
    							}
    						}
    						// right down
    						if(j+1<=9){
    							if(board[i+1][j+1] == 2){// if white, eat
    								if(remaining_white-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white-1,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 2;
    							}
    							else if(board[i+1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i+1][j+1] = 1;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i+1;
		    								new_j = j+1;
	    								}	
	    							}
	    							board[i][j] = 1;
	    							board[i+1][j+1] = 0;
    							}
    						}

    						// dowm
    						if(board[i+1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i+1][j] = 1;
    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color+1, remaining_black, remaining_white,strategy);
	    						if(new_min_heuristic <= min_heuristic){
	    							min_heuristic = new_min_heuristic;
	    							if(current_depth == 0){
	    								// cout << __LINE__ << endl;
		    							orig_i = i;
		    							orig_j = j;
		    							new_i = i+1;
		    							new_j = j;
	    							}
	    						}
    							board[i][j] = 1;
    							board[i+1][j] = 0;
	    					}
	    				}
	    			}
	    		}
	    	}
	    	else{ // current move is white
	    		  // go through whole board to find white piece
	    		for(int i=0;i<5;i++){
	    			for(int j=0;j<10;j++){
	    				// if white
	    				if(board[i][j] == 2){
	    					// detect if win
	    					if(i-1==0){
		    					// cout << __LINE__ << endl;
		    					if(current_depth == 0){
									// cout << __LINE__ << endl;
    								orig_i = i;
    								orig_j = j;
    								new_i = i-1;
    								if(j-1>=0)
    									new_j = j-1;
    								else
    									new_j = j+1;
								}
		    					return INT_MIN;
		    				}
    						// go three direction
    						// left up
    						if(j-1>=0){
    							if(board[i-1][j-1] == 1){ // if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 1;
    							} 
    							else if(board[i-1][j-1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j-1] = 2;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j-1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j-1] = 0;
    							}
    						}
    						// right up
    						if(j+1<=9){
    							if(board[i-1][j+1] == 1){// if black, eat
    								if(remaining_black-1 == 0){
    									if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
    									return INT_MIN;
    								}
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black-1, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 1;
    							}
    							else if(board[i-1][j+1] == 0){ // if empty
    								board[i][j] = 0;
	    							board[i-1][j+1] = 2;
	    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j+1;
	    								}
	    							}
	    							board[i][j] = 2;
	    							board[i-1][j+1] = 0;
    							}
    						}

    						// up
    						if(board[i-1][j] == 0){ // if empty
								board[i][j] = 0;
    							board[i-1][j] = 2;
    							double new_min_heuristic = minimax_defensive(board, current_depth+1, max_depth, start_color, current_color-1, remaining_black, remaining_white,strategy);
	    							if(new_min_heuristic <= min_heuristic){
	    								min_heuristic = new_min_heuristic;
	    								if(current_depth == 0){
	    									// cout << __LINE__ << endl;
		    								orig_i = i;
		    								orig_j = j;
		    								new_i = i-1;
		    								new_j = j;
	    								}
	    							}
    							board[i][j] = 2;
    							board[i-1][j] = 0;
	    					}
	    				}
	    			}
	    		}
	    	}
	    	return min_heuristic;
	    }
    }
}



int main()
{
	srand (time(NULL));
	vector<vector<int> > board_start;
	for(int i = 0;i<5;i++){
		vector<int> temp;
		for(int j = 0; j<10; j++){
			if(i == 0 || i ==1){
				temp.push_back(1); //black
			}
			else if(i == 3 || i ==4){
				temp.push_back(2); //white
			}
			else{
				temp.push_back(0);
			}
		}
		board_start.push_back(temp);
	}
	int black_win = 0;
	int white_win = 0;
	for(int l = 0; l < 10; l++){
		vector<vector<int>> board = board_start;
		int win = 0;
		black_count = 16;
		white_count = 16;
		int start = 0;//black even, white odd
		count_alphabeta = 0;
		// strategy: 0 defensive_1, 1 offensive_1, 2 ???_2
		int alpha_beta_black, alpha_beta_white = 0;
		while(!win){
			if(start%2 == 0){//black's turn
				// cout<<"black's turn"<<endl;
				// minimax_defensive(board,0,3,1,1,black_count,white_count,1);
				alpha_beta(board,0,3,1,1,black_count,white_count,3,INT_MIN,INT_MAX);
				alpha_beta_black += count_alphabeta;
				count_alphabeta = 0;
				board[orig_i][orig_j] = 0;
				// cout<<"black's turn"<<endl;
				// cout <<"ij" << orig_i <<" "<<orig_j <<endl;
				// cout <<"newij" << new_i <<" "<<new_j <<endl;
				if(board[new_i][new_j] == 2){
					white_count --;
					// cout <<"white_count reduce:" << white_count <<endl;
				}
				board[new_i][new_j] = 1;
			}
			if(start%2 == 1){//white's turn
				//minimax_defensive(board,0,4,2,2,black_count,white_count,0);
				// cout<<"white's turn"<<endl;
				alpha_beta(board,0,3,2,2,black_count,white_count,2,INT_MIN,INT_MAX);
				alpha_beta_white += count_alphabeta;
				count_alphabeta = 0;
				board[orig_i][orig_j] = 0;
				// cout<<"white's turn"<<endl;
				// cout <<"ij" << orig_i <<" "<<orig_j <<endl;
				// cout <<"newij" << new_i <<" "<<new_j <<endl;
				if(board[new_i][new_j] == 1){
					black_count --;
					// cout <<"black_count reduce:" << black_count <<endl;
				}
				board[new_i][new_j] = 2;
			}

			win = tell_win(board);
			start++;
			// printVector(board);
			// cout<<endl;
		}
		printVector(board);
		cout << "Final white color: "<<white_count << endl;
		cout << "Final black color: "<<black_count << endl;
		cout<< "Black Alpha-Beta node expanded: " << alpha_beta_black<<endl;
		cout<< "White Alpha-Beta node expanded: " << alpha_beta_white<<endl;
		// cout<< "Minimax node count: " << count_minimax<<endl;
		cout << endl;
		if(win == 1){
			black_win ++;
		}
		else{
			white_win ++;
		}
	}
	cout << "Black win: " << black_win << endl;
	cout << "White win: " << white_win << endl;
}