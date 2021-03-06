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

using namespace std;
// global var
int color_count;
map<char, int> color2int;
map<int, char> int2color;
int countAssignment = 0;

//Run the code on mac: c++ -std=c++11 -stdlib=libc++ p1_1021.cpp && ./a.out

class var{
public:
  int i;
  int j;
  int color;
  vector<int> domain; // vector of available color
  bool is_source;

  var(){
  }

  var(int i, int j, bool is_source){
    this->i = i;
    this->j = j;
    this->color = 0;
    this->is_source = is_source;
  }

  var(int i, int j, int color, bool is_source){
    this->i = i;
    this->j = j;
    this->color = color;
    this->is_source = is_source;
  }
};

struct LessThanBySize
{
  bool operator()(const var& lhs, const var& rhs) const
  {
    return lhs.domain.size() < rhs.domain.size();
  }
};

void convert(vector<vector<var>> result){
  for(int i = 0; i < result.size(); i++){
    for(int j = 0; j < result.size(); j++){
      cout<<int2color[result[i][j].color];
      if(result[i][j].color == 0){
        cout << "_";
      }
    }
    cout << endl;
  }
}

void printVector(vector<vector<var> > v){
  for(int i = 0; i < v.size(); i++){
    for(int j = 0; j < v[0].size(); j++){
      cout << v[i][j].color;
    }
    cout << endl;
  }
}

vector<vector<var>> forward_checking(vector<vector<var>> assignment){
	for(int x = 0; x < assignment.size(); x++){
		for(int y = 0; y < assignment[0].size(); y++){
      if(assignment[x][y].color == 0){
        //Preprocess
        map<int, int> fill_dict;
        vector<var> fill_counter;
        // int fill_counter = 0;
        int edge_counter = 0;

        // printVector(assignment);

        if (y-1 >= 0){//up
          if(assignment[x][y-1].color != 0){
            if (fill_dict.find(assignment[x][y-1].color) == fill_dict.end()){
              fill_dict[assignment[x][y-1].color] = 1;
            }else{
              fill_dict[assignment[x][y-1].color] += 1;
            }
            // fill_counter++;
            fill_counter.push_back(assignment[x][y-1]);
          }
        }else{
          edge_counter ++;
        }
        if ((y+1 < assignment.size())){
          if (assignment[x][y+1].color != 0){
            if (fill_dict.find(assignment[x][y+1].color) == fill_dict.end()){
              fill_dict[assignment[x][y+1].color] = 1;
            }else{
              fill_dict[assignment[x][y+1].color] += 1;
            }
            // fill_counter++;
            fill_counter.push_back(assignment[x][y+1]);
          }
        }else{
          edge_counter ++;
        }

        if ((x-1 >= 0)){
          if(assignment[x-1][y].color != 0){
            if (fill_dict.find(assignment[x-1][y].color) == fill_dict.end()){
              fill_dict[assignment[x-1][y].color] = 1;
            }else{
              fill_dict[assignment[x-1][y].color] += 1;
            }
            // fill_counter++;
            fill_counter.push_back(assignment[x-1][y]);
          }
        }else{
          edge_counter ++;
        }

        if ((x+1 < assignment.size())){
          if(assignment[x+1][y].color != 0){
            if (fill_dict.find(assignment[x+1][y].color) == fill_dict.end()){
              fill_dict[assignment[x+1][y].color] = 1;
            }else{
              fill_dict[assignment[x+1][y].color] += 1;
            }
            // fill_counter++;
            fill_counter.push_back(assignment[x+1][y]);
          }
        }else{
          edge_counter++;
        }

        if (edge_counter == 2){//at the corner
          //if fill_counter == 2, domain = all
          if (fill_counter.size() == 1){
            vector<int> curr_domain = {fill_counter[0].color};
            assignment[x][y].domain = curr_domain;
          }
        }else if (edge_counter == 1){
          if (fill_counter.size() == 2){
            if (fill_counter[0].color == fill_counter[1].color){
              vector<int> curr_domain = {fill_counter[0].color};
              assignment[x][y].domain = curr_domain;
            }else{
              vector<int> curr_domain = {fill_counter[0].color, fill_counter[1].color};
              assignment[x][y].domain = curr_domain;
            }
          }else if (fill_counter.size() == 3){
            if (fill_dict.size() == 3 || fill_dict.size() == 1){//three different color, fail
              return vector<vector<var>>();
            }
            vector<int> curr_domain = {fill_dict[fill_counter[0].color] == 2?fill_counter[0].color:fill_counter[1].color};
          }
        }else{//in the middle
          if (fill_counter.size() == 3){
            vector<int> curr_domain;
            for (map<int,int>::iterator it=fill_dict.begin(); it!=fill_dict.end(); ++it){
              curr_domain.push_back(it->first);
            }
            assignment[x][y].domain = curr_domain;
          }else if (fill_counter.size() == 4){
            if (fill_dict.size() == 1){//three different color, fail
              return vector<vector<var>>();
            }else{
              if(fill_dict.size() == 3){
                vector<int> curr_domain;
                for(int i = 0; i < 3; i++){
                  if(fill_dict[fill_counter[i].color] == 2){
                    curr_domain = {fill_counter[i].color};
                    break;
                  }
                }
                assignment[x][y].domain = curr_domain;
              }else{
                vector<int> curr_domain;
                for (map<int,int>::iterator it=fill_dict.begin(); it!=fill_dict.end(); ++it){
                  curr_domain.push_back(it->first);
                }
                assignment[x][y].domain = curr_domain;
              }
            }
          }
        }
      }
		}
	}
  return assignment;
}


int checkConstraints(vector<vector<var>> assignment, int x, int y){
  //check up
  map<int, int> fill_dict;
  int fill_counter = 0;
  int edge_counter = 0;

  if (y-1 >= 0){
    if(assignment[x][y-1].color != 0){
      if (fill_dict.find(assignment[x][y-1].color) == fill_dict.end()){
        fill_dict[assignment[x][y-1].color] = 1;
      }else{
        fill_dict[assignment[x][y-1].color] += 1;
      }
      fill_counter++;
    }
  }else{
    edge_counter ++;
  }
  if ((y+1 < assignment.size())){
    if (assignment[x][y+1].color != 0){
      if (fill_dict.find(assignment[x][y+1].color) == fill_dict.end()){
        fill_dict[assignment[x][y+1].color] = 1;
      }else{
        fill_dict[assignment[x][y+1].color] += 1;
      }
      fill_counter++;
    }
  }else{
    edge_counter ++;
  }

  if ((x-1 >= 0)){
    if(assignment[x-1][y].color != 0){
      if (fill_dict.find(assignment[x-1][y].color) == fill_dict.end()){
        fill_dict[assignment[x-1][y].color] = 1;
      }else{
        fill_dict[assignment[x-1][y].color] += 1;
      }
      fill_counter++;
    }
  }else{
    edge_counter ++;
  }

  if ((x+1 < assignment.size())){
    if(assignment[x+1][y].color != 0){
      if (fill_dict.find(assignment[x+1][y].color) == fill_dict.end()){
        fill_dict[assignment[x+1][y].color] = 1;
      }else{
        fill_dict[assignment[x+1][y].color] += 1;
      }
      fill_counter++;
    }
  }else{
    edge_counter++;
  }

  if(!assignment[x][y].is_source){ // is not source
    if(edge_counter == 0){
      if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && (fill_dict[assignment[x][y].color] == 1 || fill_dict[assignment[x][y].color] == 2))){
          return 0;
        }
      }
      else if(fill_counter == 4){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 2)) {
          return 0;
        }
      }
    }
    else if(edge_counter == 1){
      if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && (fill_dict[assignment[x][y].color] == 1 || fill_dict[assignment[x][y].color] == 2))){
          return 0;
        }
      }
      else if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 2)) {
          return 0;
        }
      }
    }
    else{ // edge_counter == 2
      if(fill_counter == 1){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 1)){
          return 0;
        }
      }
      else if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 2)) {
          return 0;
        }
      }
    }
  }
  else{ // is source
    if(edge_counter == 0){
      if(fill_counter == 4){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 1)){
          return 0;
        }
      }
    }
    else if(edge_counter == 1){
      if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 1)){
          return 0;
        }
      }
    }
    else{ // edge_counter == 2
      if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y].color) != fill_dict.end() && fill_dict[assignment[x][y].color] == 1)){
          return 0;
        }
      }
    }
  }
  return 1;
}

vector<vector<var>> Recursive_backtracking(vector<vector<var>> assignment){
  countAssignment++;
  // printVector(assignment);
  // cout<<endl;
  //#1 Check if the assignment is completed => no zero in the 2d array
  // if completed, return
  int flag = 1;

  for(int i = 0; i < assignment.size(); i++){
    for(int j = 0; j < assignment[0].size(); j++){
      if(assignment[i][j].color == 0){
        flag = 0;
        break;
      }
    }
  }  

  if(flag){
    return assignment;
  }

  vector<vector<var>> new_assignment = forward_checking(assignment);

  if(new_assignment.empty()){
    return vector<vector<var>>();
  }
  else{
    assignment = new_assignment;
  }

  int min_domain = color_count+1;
  var var_cur;
  for(int i = 0; i < assignment.size(); i++){
    for(int j = 0; j < assignment[0].size(); j++){
      if(assignment[i][j].is_source == 0 && assignment[i][j].domain.size() != 0 && assignment[i][j].domain.size() < min_domain){
        var_cur = assignment[i][j];
        min_domain = assignment[i][j].domain.size();
      }
    }
  }
  // while(!var_list.empty()){//if the var_list is not empty, the maze is not looped throughly
    //if vlaue is consistent with assignment given constraints
    //check if it's a source
    int x = var_cur.i;
    int y = var_cur.j;
    int avail_domain = var_cur.domain.size();
    // for(int i = 0; i < avail_domain)
    // For each value in color_count
    for (int i = 0; i < avail_domain; i++){
      int curr_domain = var_cur.domain[i];
      assignment[x][y].color = curr_domain;//assign current color to assignment
      //Check Constraints
      if (!checkConstraints(assignment, x, y)){
        assignment[x][y].color = 0;
        continue;
      }
      if(x+1 < assignment.size() && assignment[x+1][y].color){
        if(!checkConstraints(assignment, x+1, y)){
          assignment[x][y].color = 0;
          continue;
        }
      }
      if(y+1 < assignment.size() && assignment[x][y+1].color){
        if(!checkConstraints(assignment, x, y+1)){
          assignment[x][y].color = 0;
          continue;
        }
      }
      if(x-1 >= 0 && assignment[x-1][y].color){
        if(!checkConstraints(assignment, x-1, y)){
          assignment[x][y].color = 0;
          continue;
        }
      }
      if(y-1 >= 0 && assignment[x][y-1].color){
        if(!checkConstraints(assignment, x, y-1)){
          assignment[x][y].color = 0;
          continue;
        }
      }
      //else pass constraints test
      assignment[x][y].domain = vector<int>();
      vector<vector<var>> ret = Recursive_backtracking(assignment);
      if (!ret.empty()) return ret;
    }
  // }
  return vector<vector<var>>();
}

int main()
{
  clock_t t1,t2;
  t1=clock();

  color_count = 0;//count the number of color
  ifstream input("input77.txt");
  string line;
  vector<vector<var> > assignment;
  // vector<var> var_list;
  int line_count = 0;

  //Read the graph and preprocess
  while(getline(input, line)){
    vector<var> temp;
    for(int i = 0; i < line.size(); i++){
      if(isupper(line[i])){
        if(color2int.find(line[i]) == color2int.end()){//current color is not in the map yet
          color_count ++;//Counter ++
          color2int[line[i]] = color_count; //add current color to map
          int2color[color_count] = line[i];
          var current_var(line_count, i, color_count, 1);
          temp.push_back(current_var); //push the int representation of current color into temp
        }
        else{
          var current_var(line_count, i, color2int[line[i]], 1);
          temp.push_back(current_var);
        }
      }
      else{
      	var current_var(line_count, i, 0, 0);
        temp.push_back(current_var);
        var x(line_count, i, 0);
      }

    }
    line_count ++;
    assignment.push_back(temp);
  }

  for(int i = 0; i < assignment.size(); i++){
  	for(int j = 0; j < assignment[0].size(); j++){
  		for(int m = 1; m <= color_count; m++){
        if(!assignment[i][j].is_source){
  			 assignment[i][j].domain.push_back(m);
        }
  		}
  	}
  }

  convert(assignment);
  cout << endl;
  //Process the "assignment" first
  vector<vector<var>> result = Recursive_backtracking(assignment);
  if(result.empty()){
    cout <<"yyyyyy"<<endl;
  }
  convert(result);

  cout << countAssignment<<endl;
  t2=clock();
  float diff ((float)t2-(float)t1);
  cout<<diff<<endl;
  return 0;
}
