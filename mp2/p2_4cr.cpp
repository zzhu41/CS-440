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
	if(black_count < 3 || white_count < 3){
		if(black_count < 3){
			cout<<"white wins by eating n-2 black"<<endl;
			return 2;
		}
		else if(white_count < 3){
			cout<<"black wins by eating n-2 white"<<endl;
			return 1;
		}
	}
	int black_reach_end = 0;
	int white_reach_end = 0;
	for(int i =0;i<8;i++){
		if(board[0][i] == 2){
			white_reach_end ++ ;
		}
		if(board[7][i] == 1){
			black_reach_end ++ ;
		}
	}
	if(black_reach_end == 3){
		cout<<"black wins by reaching end"<<endl;
		return 1;
	}
	else if(white_reach_end == 3){
		cout<<"white wins by reaching end"<<endl;
		return 2;
	}
	return 0;
}

// double cal_heuristic_white(vector<vector<int>> board){
// 	double dis = 0;
// 	int count = 0;
// 	for(int i=0;i<8;i++){
// 		if(board[0][i] == 2){
// 			count ++;
// 		}
// 	}
// 	return count* 5;
// }

double cal_heuristic_white(vector<vector<int>> board){
	int dis = 0;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(board[i][j] == 2){
				dis += pow(12,i); // 6
			}
		}
	}
	return dis;
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
    			double cur_heuristic = (remaining_black) + 10*(30 - remaining_white)+ ((double) rand() / (RAND_MAX));
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
    			double cur_heuristic = cal_heuristic_white(board) + 2*(30 - remaining_white) +(remaining_black);
    			return cur_heuristic;
    		}
    	}
    }
    else{
    	if(start_color == current_color){ // is max
    		double max_heuristic = INT_MIN;
	    	if(current_color == 1){ // current move is black
	    		// go through whole board to find black piece
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if black
	    				if(board[i][j] == 1 && i+1 <= 7){
	    					if(i+1==7){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if white
	    				if(board[i][j] == 2 && i-1 >= 0){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if black
	    				if(board[i][j] == 1 && i+1 <= 7){
	    					// detect win
	    					if(i+1 == 7){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if white
	    				if(board[i][j] == 2 && i-1 >= 0){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					if(i+1==7){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
	    				// if black
	    				if(board[i][j] == 1){
	    					// detect win
	    					if(i+1 == 7){
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
    						if(j+1<=7){
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
	    		for(int i=0;i<8;i++){
	    			for(int j=0;j<8;j++){
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
    						if(j+1<=7){
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
	for(int i = 0;i<8;i++){
		vector<int> temp;
		for(int j = 0; j<8; j++){
			if(i == 0 || i ==1){
				temp.push_back(1); //black
			}
			else if(i == 6 || i ==7){
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
	double TOTAL_TIME_BLACK = 0;
	double TOTAL_TIME_WHITE = 0;
	int TOTAL_EXPEND_BLACK = 0;
	int TOTAL_EXPEND_WHITE = 0;
	for(int l = 0; l < 2; l++){
		clock_t t1,t2;
		float time_black_total = 0;
		float time_white_total = 0;
		vector<vector<int>> board = board_start;
		int win = 0;
		black_count = 16;
		white_count = 16;
		int start = 0;//black even, white odd
		count_alphabeta = 0;
		// strategy: 0 defensive_1, 1 offensive_1, 2 ???_2
		int alpha_beta_black = 0;
		int alpha_beta_white = 0;
		int white_move_count = 0;
		int black_move_count = 0;
		while(!win){
			if(start%2 == 0){//black's turn
				t1=clock();
				black_move_count ++;
				// cout<<"black's turn"<<endl;
				//minimax_defensive(board,0,4,1,1,black_count,white_count,1);
				alpha_beta(board,0,4,1,1,black_count,white_count,1,INT_MIN,INT_MAX);
				//alpha_beta_black += count_alphabeta;
				//cout << "count minimax: " << count_minimax << endl;
				alpha_beta_black += count_minimax;
				//cout << "alpha beta black: " << alpha_beta_black << endl;
				count_minimax = 0;
				board[orig_i][orig_j] = 0;
				// cout<<"black's turn"<<endl;
				// cout <<"ij" << orig_i <<" "<<orig_j <<endl;
				// cout <<"newij" << new_i <<" "<<new_j <<endl;
				if(board[new_i][new_j] == 2){
					white_count --;
					// cout <<"white_count reduce:" << white_count <<endl;
				}
				board[new_i][new_j] = 1;
				t2=clock();
				time_black_total += ((float)t2-(float)t1);

			}
			if(start%2 == 1){//white's turn
				t1=clock();
				white_move_count ++;
				//minimax_defensive(board,0,4,2,2,black_count,white_count,0);
				// cout<<"white's turn"<<endl;
				alpha_beta(board,0,4,2,2,black_count,white_count,3,INT_MIN,INT_MAX);
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
				t2=clock();
				time_white_total += ((float)t2-(float)t1);
			}

			win = tell_win(board);
			start++;
			// printVector(board);
			// cout<<endl;
		}
		cout << "Final white color: "<<white_count << endl;
		cout << "Final black color: "<<black_count << endl;
		cout<< "Black node expanded: " << alpha_beta_black<<endl;
		cout<<black_move_count<<endl;
		cout<<white_move_count<<endl;
		cout<< "average number of black nodes expanded per move: "<<(float)alpha_beta_black/black_move_count<<endl;
		cout<< "total time of black "<< time_black_total/1000000<<endl;
		cout<< "average amount of time to make a move " <<(float)time_black_total/(1000000*black_move_count)<<" second"<<endl;
		cout<< "White node expanded: " << alpha_beta_white<<endl;
		cout<< "average number of white nodes expanded per move: "<<alpha_beta_white/white_move_count<<endl;
		cout<< "total time of white "<< (float)time_white_total/1000000<<endl;
		cout<< "average amount of time to make a move " <<(float)time_white_total/(1000000*white_move_count)<<" second"<<endl;
		// cout<< "Minimax node count: " << count_minimax<<endl;
		cout << endl;
		if(win == 1){
			black_win ++;
		}
		else{
			white_win ++;
		}
		TOTAL_EXPEND_BLACK += (float)alpha_beta_black/black_move_count;
		TOTAL_EXPEND_WHITE += (float)alpha_beta_white/white_move_count;
		TOTAL_TIME_BLACK += (float)time_black_total/(1000000*black_move_count);
		TOTAL_TIME_WHITE += (float)time_white_total/(1000000*white_move_count);
	}
	cout << "Black win: " << black_win << endl;
	cout << "White win: " << white_win << endl;
	cout<<endl;
	cout<< "average number of black nodes expanded per move: "<<(float)TOTAL_EXPEND_BLACK/10<<endl;
	cout<< "average amount of time to make a move " <<(float)TOTAL_TIME_BLACK/10<<" second"<<endl;
	cout<< "average number of white nodes expanded per move: "<<(float)TOTAL_EXPEND_WHITE/10<<endl;
	cout<< "average amount of time to make a move " <<(float)TOTAL_TIME_WHITE/10<<" second"<<endl;
}