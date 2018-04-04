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
//1.
using namespace std;
// global var
int countAssignment = 0;
int color_count;
map<pair<int, int>, bool> sourceDict;//a dict of source
map<char, int> color2int;
map<int, char> int2color;

//Run the code on mac: c++ -std=c++11 -stdlib=libc++ gbf.cpp && ./a.out

class var{
public:
  int i;
  int j;
  int color;

  var(int i, int j){
    this->i = i;
    this->j = j;
  }
};

void convert(vector<vector<int>> result){
  for(int i = 0; i < result.size(); i++){
    for(int j = 0; j < result.size(); j++){
      cout<<int2color[result[i][j]];
      if(result[i][j] == 0){
        cout << "_";
      }
    }
    cout << endl;
  }
}

void printVector(vector<vector<int> > v){
  for(int i = 0; i < v.size(); i++){
    for(int j = 0; j < v[0].size(); j++){
      cout << v[i][j];
    }
    cout << endl;
  }
}

int checkConstraints(vector<vector<int>> assignment, int x, int y, map<pair<int, int>, bool> sourceDict){
  //check up
  map<int, int> fill_dict;
  int fill_counter = 0;
  int edge_counter = 0;

  // printVector(assignment);

  if (y-1 >= 0){
    if(assignment[x][y-1] != 0){
      if (fill_dict.find(assignment[x][y-1]) == fill_dict.end()){
        fill_dict[assignment[x][y-1]] = 1;
      }else{
        fill_dict[assignment[x][y-1]] += 1;
      }
      // cout << "a" << endl;
      fill_counter++;
    }
  }else{
    edge_counter ++;
  }
  if ((y+1 < assignment.size())){
    if (assignment[x][y+1] != 0){
      if (fill_dict.find(assignment[x][y+1]) == fill_dict.end()){
        fill_dict[assignment[x][y+1]] = 1;
      }else{
        fill_dict[assignment[x][y+1]] += 1;
      }
      fill_counter++;
      // cout << "b" << endl;
    }
  }else{
    edge_counter ++;
  }

  if ((x-1 >= 0)){
    if(assignment[x-1][y] != 0){
      if (fill_dict.find(assignment[x-1][y]) == fill_dict.end()){
        fill_dict[assignment[x-1][y]] = 1;
      }else{
        fill_dict[assignment[x-1][y]] += 1;
      }
      fill_counter++;
      // cout << "c" << endl;
    }
  }else{
    edge_counter ++;
  }

  if ((x+1 < assignment.size())){
    if(assignment[x+1][y] != 0){
      if (fill_dict.find(assignment[x+1][y]) == fill_dict.end()){
        fill_dict[assignment[x+1][y]] = 1;
      }else{
        fill_dict[assignment[x+1][y]] += 1;
      }
      fill_counter++;
      // cout << "d" << endl;
    }
  }else{
    edge_counter++;
  }
  // for (map<int,int>::iterator it=fill_dict.begin(); it!=fill_dict.end(); ++it){
  //   if (it->second > 2)  return 0;//fail
  //   fill_counter += it->second;
  // }
  // if (!sourceDict[make_pair(x, y)]){
  //   if (fill_counter == 3-edge_counter && !(fill_dict.find(assignment[x][y]) != fill_dict.end() && (fill_dict[assignment[x][y]] == 1 || fill_dict[assignment[x][y]] == 2))) return 0;//have three different color around the point
  //   if (fill_counter == 4-edge_counter && (fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] != 2)) return 0;
  // }
  // if (sourceDict[make_pair(x, y)] && fill_counter == 4 && (fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] != 1)) return 0;
  //
  // return 1;
  if(!sourceDict[make_pair(x, y)]){
    if(edge_counter == 0){
      if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && (fill_dict[assignment[x][y]] == 1 || fill_dict[assignment[x][y]] == 2))){
          return 0;
        }
      }
      else if(fill_counter == 4){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 2)) {
          return 0;
        }
      }
    }
    else if(edge_counter == 1){
      if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && (fill_dict[assignment[x][y]] == 1 || fill_dict[assignment[x][y]] == 2))){
          return 0;
        }
      }
      else if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 2)) {
          return 0;
        }
      }
    }
    else{ // edge_counter == 2
      if(fill_counter == 1){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 1)){
          return 0;
        }
      }
      else if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 2)) {
          return 0;
        }
      }
    }
  }
  else{ // sourceDict[make_pair(x, y)]
    if(edge_counter == 0){
      if(fill_counter == 4){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 1)){
          return 0;
        }
      }
    }
    else if(edge_counter == 1){
      if(fill_counter == 3){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 1)){
          return 0;
        }
      }
    }
    else{ // edge_counter == 2
      if(fill_counter == 2){
        if(!(fill_dict.find(assignment[x][y]) != fill_dict.end() && fill_dict[assignment[x][y]] == 1)){
          return 0;
        }
      }
    }
  }
  return 1;
}

vector<vector<int>> Recursive_backtracking(vector<vector<int>> assignment, vector<var> var_list){
  //#1 Check if the assignment is completed => no zero in the 2d array
  // if completed, return
  // cout<<"hh" << endl;
  if(var_list.empty()) return assignment;
  // while(!var_list.empty()){//if the var_list is not empty, the maze is not looped throughly
    countAssignment ++;
    var var_cur = var_list.back();//value
    var_list.pop_back();
    //if vlaue is consistent with assignment given constraints
    //check if it's a source
    int x = var_cur.i;
    int y = var_cur.j;
    // cout << "x: " << x << "  " << "y: " << y << " " <<endl;
    // For each value in color_count
    for (int i = 1; i <= color_count; i++){
      assignment[x][y] = i;//assign current color to assignment
      //Check Constraints
      if (!checkConstraints(assignment, x, y, sourceDict)){
        // cout << "1" << endl;
        assignment[x][y] = 0;
        continue;
      }
      if(x+1 < assignment.size() && assignment[x+1][y]){
        if(!checkConstraints(assignment, x+1, y, sourceDict)){
          // cout << "2" << endl;
          assignment[x][y] = 0;
          continue;
        }
      }
      if(y+1 < assignment.size() && assignment[x][y+1]){
        if(!checkConstraints(assignment, x, y+1, sourceDict)){
          // cout << "3" << endl;
          assignment[x][y] = 0;
          continue;
        }
      }
      if(x-1 >= 0 && assignment[x-1][y]){
        if(!checkConstraints(assignment, x-1, y, sourceDict)){
          // cout << "4" << endl;
          assignment[x][y] = 0;
          continue;
        }
      }
      if(y-1 >= 0 && assignment[x][y-1]){
        if(!checkConstraints(assignment, x, y-1, sourceDict)){
          // cout << "5" << endl;
          assignment[x][y] = 0;
          continue;
        }
      }
      //else pass constraints test
      vector<vector<int>> ret = Recursive_backtracking(assignment, var_list);
      if (ret != vector<vector<int>>()) return ret;
    }
  // }
  return vector<vector<int>>();
}

int main()
{
  clock_t t1,t2;
  t1=clock();

  color_count = 0;//count the number of color
  ifstream input("input77.txt");
  string line;
  vector<vector<int> > assignment;
  vector<var> var_list;
  int line_count = 0;

  //Read the graph and preprocess
  while(getline(input, line)){
    vector<int> temp;
    for(int i = 0; i < line.size(); i++){
      if(isupper(line[i])){
        if(color2int.find(line[i]) == color2int.end()){//current color is not in the map yet
          color_count ++;//Counter ++
          color2int[line[i]] = color_count; //add current color to map
          int2color[color_count] = line[i];
          temp.push_back(color_count); //push the int representation of current color into temp
        }
        else{
          temp.push_back(color2int[line[i]]);
        }
        sourceDict[make_pair(line_count, i)] = 1;
      }
      else{
        temp.push_back(0);
        sourceDict[make_pair(line_count, i)] = 0;
        var x(line_count, i);
        var_list.push_back(x);
      }

    }
    line_count ++;
    assignment.push_back(temp);
  }

  convert(assignment);

  cout << endl;
  //Process the "assignment" first
  vector<vector<int>> result = Recursive_backtracking(assignment,var_list);
  if(result == vector<vector<int>>()){
    cout <<"yyyyyy"<<endl;
  }
  convert(result);
  cout<<countAssignment<<endl;
  t2=clock();
  float diff ((float)t2-(float)t1);
  cout<<diff<<endl;
  return 0;
}
