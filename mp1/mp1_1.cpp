#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <stack>
#include <queue>
using namespace std;

int xx,yy;
int end_i,end_j;
vector<vector<vector<pair<int,int> > > > path; // structure to store path when reaching every point in maze

/* print out 2-d vector */
void printVector(vector<vector<int> > v){
  for(int i = 0; i < v.size(); i++){
    for(int j = 0; j < v[0].size(); j++){
      cout << v[i][j];
    }
    cout << endl;
  }
}

/* for sorting tuples */
template<int M, template<typename> class F = std::less>
struct TupleCompare
{
    template<typename T>
    bool operator()(T const &t1, T const &t2)
    {
        return F<typename tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
    }
};

void findPathBFS(vector<vector<int> >& maze_array, vector<vector<int> >& visited, queue<pair<int,int> >& bfs_queue)
{
  int step = 0;
  while(!bfs_queue.empty())
  {
    step ++;
    pair<int,int> p = bfs_queue.front();
    bfs_queue.pop();
    int curr_x = p.first;
    int curr_y = p.second;
    if(curr_x == end_i && curr_y == end_j) // found
    {
      break;
    }
    else{
      vector<pair<int,int> > last_path = path[curr_x][curr_y];
      // explore up
      if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){
        visited[curr_x - 1][curr_y] = 1; // mark visited
        pair<int,int> pp = make_pair(curr_x - 1, curr_y);
        bfs_queue.push(pp); 
        // add to path
        path[curr_x - 1][curr_y] = last_path;
        path[curr_x - 1][curr_y].push_back(pp);
      }
      // explore left
      if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){
        visited[curr_x][curr_y - 1] = 1;
        pair<int,int> pp = make_pair(curr_x, curr_y - 1);
        bfs_queue.push(pp);
        path[curr_x][curr_y - 1] = last_path;
        path[curr_x][curr_y - 1].push_back(pp);
      }
      // explore down
      if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){
        visited[curr_x + 1][curr_y] = 1; 
        pair<int,int> pp = make_pair(curr_x + 1, curr_y);
        bfs_queue.push(pp);
        path[curr_x + 1][curr_y] = last_path;
        path[curr_x + 1][curr_y].push_back(pp);
      }
      // explore right
      if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){
        visited[curr_x][curr_y + 1] = 1;
        pair<int,int> pp = make_pair(curr_x, curr_y + 1);
        bfs_queue.push(pp);
        path[curr_x][curr_y + 1] = last_path;
        path[curr_x][curr_y + 1].push_back(pp);
      }
    }

  }
  vector<pair<int,int> > target = path[end_i][end_j];
  for(int i = 0; i < target.size(); i++){
    int x = target[i].first;
    int y = target[i].second;
    maze_array[x][y] = 6;
  }
  cout << "Step cost of BFS is " << step << endl;
  cout << "Path length of BFS is " << target.size() << endl;
}

void findPathDFS(vector<vector<int> >& maze_array, vector<vector<int> >& visited, stack<pair<int,int> >& dfs_stack){
  int step = 0;
  while(!dfs_stack.empty()){
    step ++;
    pair<int,int> p = dfs_stack.top();
    dfs_stack.pop();
    int curr_x = p.first;
    int curr_y = p.second;
    if(curr_x == end_i && curr_y == end_j){ // found
      break;
    }
    else{
      vector<pair<int,int> > last_path = path[curr_x][curr_y];
      // explore up
      if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){
        visited[curr_x - 1][curr_y] = 1;
        pair<int,int> pp = make_pair(curr_x - 1, curr_y);
        dfs_stack.push(pp);
        path[curr_x - 1][curr_y] = last_path;
        path[curr_x - 1][curr_y].push_back(pp);
      }
      // explore left
      if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){
        visited[curr_x][curr_y - 1] = 1;
        pair<int,int> pp = make_pair(curr_x, curr_y - 1);
        dfs_stack.push(pp);
        path[curr_x][curr_y - 1] = last_path;
        path[curr_x][curr_y - 1].push_back(pp);
      }
      // explore right
      if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){
        visited[curr_x + 1][curr_y] = 1;
        pair<int,int> pp = make_pair(curr_x + 1, curr_y);
        dfs_stack.push(pp);
        path[curr_x + 1][curr_y] = last_path;
        path[curr_x + 1][curr_y].push_back(pp);
      }
      // explore down
      if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){
        visited[curr_x][curr_y + 1] = 1;
        pair<int,int> pp = make_pair(curr_x, curr_y + 1);
        dfs_stack.push(pp);
        path[curr_x][curr_y + 1] = last_path;
        path[curr_x][curr_y + 1].push_back(pp);
      }
    }
  }
  vector<pair<int,int> > target = path[end_i][end_j];
  for(int i = 0; i < target.size(); i++){
    int x = target[i].first;
    int y = target[i].second;
    maze_array[x][y] = 6;
  }
  
  cout << "Step cost of DFS is " << step << endl;
  cout << "Path length of DFS is " << target.size() << endl;
}

void findPathGreedy(vector<vector<int> >& maze_array, vector<vector<int> >& visited, vector<tuple<int,int,int>> ll){
  int step = 0;
  while(!ll.empty()){
    sort(ll.begin(), ll.end(), TupleCompare<0>());
    tuple<int,int,int> tp = ll[0];
    ll[0] = ll[ll.size()-1];
    ll.pop_back();
    int curr_x = get<1>(tp);
    int curr_y = get<2>(tp);
    step ++;

    if(curr_x == end_i && curr_y == end_j){
      break;
    }
    else{
      vector<pair<int,int> > last_path = path[curr_x][curr_y];
      if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){
        visited[curr_x - 1][curr_y] = 1; // make visited

        // add to list
        int h = + abs(end_i - (curr_x - 1)) + abs(end_j - curr_y);
        tuple<int,int,int> tp = make_tuple(h, curr_x - 1, curr_y);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x - 1, curr_y);
        path[curr_x - 1][curr_y] = last_path;
        path[curr_x - 1][curr_y].push_back(pp);
      }
      if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){
        visited[curr_x][curr_y - 1] = 1;

        // add to list
        int h = + abs(end_i - curr_x) + abs(end_j - (curr_y - 1));
        tuple<int,int,int> tp = make_tuple(h, curr_x, curr_y - 1);
        ll.push_back(tp);

        pair<int,int> pp = make_pair(curr_x, curr_y - 1);
        path[curr_x][curr_y - 1] = last_path;
        path[curr_x][curr_y - 1].push_back(pp);
      }
      if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){
        visited[curr_x + 1][curr_y] = 1;

        // add to list
        int h = + abs(end_i - (curr_x + 1)) + abs(end_j - curr_y);
        tuple<int,int,int> tp = make_tuple(h, curr_x + 1, curr_y);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x + 1, curr_y);
        path[curr_x + 1][curr_y] = last_path;
        path[curr_x + 1][curr_y].push_back(pp);
      }
      if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){
        visited[curr_x][curr_y + 1] = 1;

        // add to list
        int h = abs(end_i - curr_x) + abs(end_j - (curr_y + 1));
        tuple<int,int,int> tp = make_tuple(h, curr_x, curr_y + 1);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x, curr_y + 1);
        path[curr_x][curr_y + 1] = last_path;
        path[curr_x][curr_y + 1].push_back(pp);
      }
    }
  }
  vector<pair<int,int> > target = path[end_i][end_j];
  for(int i = 0; i < target.size(); i++){
    int x = target[i].first;
    int y = target[i].second;
    maze_array[x][y] = 6;
  }
  cout << "Step cost of Greedy best-first is " << step << endl;
  cout << "Path length of Greedy best-first is " << target.size() << endl;
}

void findPathAStar(vector<vector<int> >& maze_array, vector<vector<int> >& visited, vector<tuple<int,int,int>> ll){
  int step = 0;
  while(!ll.empty()){
    sort(ll.begin(), ll.end(), TupleCompare<0>());
    tuple<int,int,int> tp = ll[0];
    ll[0] = ll[ll.size()-1];
    ll.pop_back();
    int curr_x = get<1>(tp);
    int curr_y = get<2>(tp);
    step ++;

    if(curr_x == end_i && curr_y == end_j){
      break;
    }
    else{
      vector<pair<int,int> > last_path = path[curr_x][curr_y];
      int path_len = last_path.size();
      if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){
        visited[curr_x - 1][curr_y] = 1; // make visited

        // add to frontier
        int h = path_len + abs(end_i - (curr_x - 1)) + abs(end_j - curr_y);
        tuple<int,int,int> tp = make_tuple(h, curr_x - 1, curr_y);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x - 1, curr_y);
        path[curr_x - 1][curr_y] = last_path;
        path[curr_x - 1][curr_y].push_back(pp);
      }
      if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){
        visited[curr_x][curr_y - 1] = 1;

        // add to frontier
        int h = path_len + abs(end_i - curr_x) + abs(end_j - (curr_y - 1));
        tuple<int,int,int> tp = make_tuple(h, curr_x, curr_y - 1);
        ll.push_back(tp);

        pair<int,int> pp = make_pair(curr_x, curr_y - 1);
        path[curr_x][curr_y - 1] = last_path;
        path[curr_x][curr_y - 1].push_back(pp);
      }
      if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){
        visited[curr_x + 1][curr_y] = 1;

        // add to frontier
        int h = path_len + abs(end_i - (curr_x + 1)) + abs(end_j - curr_y);
        tuple<int,int,int> tp = make_tuple(h, curr_x + 1, curr_y);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x + 1, curr_y);
        path[curr_x + 1][curr_y] = last_path;
        path[curr_x + 1][curr_y].push_back(pp);
      }
      if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){
        visited[curr_x][curr_y + 1] = 1;

        // add to frontier
        int h = path_len + abs(end_i - curr_x) + abs(end_j - (curr_y + 1));
        tuple<int,int,int> tp = make_tuple(h, curr_x, curr_y + 1);
        ll.push_back(tp);

        // add to path
        pair<int,int> pp = make_pair(curr_x, curr_y + 1);
        path[curr_x][curr_y + 1] = last_path;
        path[curr_x][curr_y + 1].push_back(pp);
      }
    }
  }
  vector<pair<int,int> > target = path[end_i][end_j];
  for(int i = 0; i < target.size(); i++){
    int x = target[i].first;
    int y = target[i].second;
    maze_array[x][y] = 6;
  }
  cout << "Step cost of A* is " << step << endl;
  cout << "Path length of A* is " << target.size() << endl;
}

/* print out maze */
void print_graph(vector<vector<int> > maze_array){
  for(int i = 0; i < maze_array.size(); i++){
    for(int j = 0; j < maze_array[0].size(); j++){
      if(maze_array[i][j] == 1){
        cout<<"%";
      }
      else if(maze_array[i][j] == 0){
        cout<<" ";
      }
      else if(maze_array[i][j] == 6 || maze_array[i][j] == 3){
        cout<<".";
      }
      else if(maze_array[i][j] == 2){
        cout<<"P";
      }
    }
    cout<<endl;
  }
}

int main(){
  ifstream maze("abc.txt");
  string line;
  vector<vector<int> > maze_array;
  int start_i,start_j = -1;
  int line_count = 0;
  vector<vector<int> > visited;

  // read maze
  while(getline(maze,line)){
    vector<int> temp;
    vector<int> visited_temp;
    for(int i = 0; i < line.size(); i++){
      visited_temp.push_back(0);
      if(line[i] == '%'){
        temp.push_back(1);
      }
      else{
        if(line[i] == 'P'){
          start_i = line_count;
          start_j = i;
          temp.push_back(2);
        }
        else if(line[i] == '.'){
          end_i = line_count;
          end_j = i;
          temp.push_back(0);
        }
        else{
          temp.push_back(0);
        }
      }
    }
    maze_array.push_back(temp);
    visited.push_back(visited_temp);
    line_count ++;
  }
  xx = maze_array.size();
  yy = maze_array[0].size();

  // initialize data structure in searching method
  stack <pair<int,int> > dfs_stack;
  queue <pair<int,int> > bfs_queue;
  pair<int,int> p = make_pair(start_i, start_j);
  dfs_stack.push(p);
  bfs_queue.push(p);
  visited[start_i][start_j] = 1;

  for(int i = 0; i < xx; i++){
    vector<vector<pair<int,int> > > first;
    for(int j = 0; j < yy; j++){
      vector<pair<int,int> > second;
      first.push_back(second);
    }
    path.push_back(first);
  }
  path[start_i][start_j].push_back(p);


  vector<tuple<int,int,int>> ll; // tuple content: heuristic, i, j
  int h = abs(start_i - start_j) + abs(end_i - end_j);
  tuple<int,int,int> tp = make_tuple(h, start_i, start_j);
  ll.push_back(tp);

  vector<vector<int> > maze_array2 = maze_array;
  vector<vector<int> > maze_array3 = maze_array;
  vector<vector<int> > maze_array4 = maze_array;
  vector<vector<int> > visited2 = visited;
  vector<vector<int> > visited3 = visited;
  vector<vector<int> > visited4 = visited;
  vector<tuple<int,int,int>> ll2 = ll;

  // print result
  findPathBFS(maze_array3, visited3, bfs_queue);
  print_graph(maze_array3);
  cout<<endl;
  findPathDFS(maze_array2, visited2, dfs_stack);
  print_graph(maze_array2);
  cout<<endl;
  findPathGreedy(maze_array, visited, ll2);
  print_graph(maze_array);
  cout<<endl;
  findPathAStar(maze_array4, visited4, ll);
  print_graph(maze_array4);

  return 0;
}
