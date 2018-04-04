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

int xx,yy; // size of the maze
int food_count = 0;
int total = 0;
int expanded_node = 0;

class Node{
public:
  // i,j means the coordinate of the food
  int i; 
  int j;
  int num; // from 1 to # of food
  vector<int> dist; // distance from this node to every node in the maze
                    // from 1 to # of food, 0 for current position
  // constructor
  Node(int i, int j, int num){
    this->i = i;
    this->j = j;
    this->num = num;
  }

  bool operator < (const Node& node) const{
        return (num < node.num);
  }
};


// sorting for vector of tuple
template<int M, template<typename> class F = std::less>
struct TupleCompare
{
    template<typename T>
    bool operator()(T const &t1, T const &t2)
    {
        return F<typename tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
    }
};

// print vector
void printVector(vector<vector<int> > v){
  for(int i = 0; i < v.size(); i++){
    for(int j = 0; j < v[0].size(); j++){
      cout << v[i][j];
    }
    cout << endl;
  }
}

// print maze
void print_graph(vector<vector<int> > maze_array){
  for(int i = 0; i < maze_array.size(); i++){
    for(int j = 0; j < maze_array[0].size(); j++){
      if(maze_array[i][j] == -1){
        cout<<"%";
      }
      else if(maze_array[i][j] == 0){
        cout<<" ";
      }
      else if(maze_array[i][j] == -6){
        cout<<".";
      }
      else if(maze_array[i][j] == -2){
        cout<<"P";
      }
      else{
        cout << hex << maze_array[i][j];
      }
    }
    cout<<endl;
  }
}

// BFS for finding distance
int findPathBFS(vector<vector<int> >& maze_array, vector<vector<int> >& visited, queue<pair<int,int> >& bfs_queue, vector<vector<vector<pair<int,int> > > >& path, int end_i, int end_j)
{
  while(!bfs_queue.empty())
  {
    pair<int,int> p = bfs_queue.front();
    bfs_queue.pop();
    int curr_x = p.first;
    int curr_y = p.second;
    if(curr_x == end_i && curr_y == end_j)
    {
      break;
    }
    else{
      vector<pair<int,int> > last_path = path[curr_x][curr_y];
      visited[curr_x][curr_y] = 1;
      if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){
        pair<int,int> pp = make_pair(curr_x - 1, curr_y);
        bfs_queue.push(pp);
        path[curr_x - 1][curr_y] = last_path;
        path[curr_x - 1][curr_y].push_back(pp);
      }
      if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){
        pair<int,int> pp = make_pair(curr_x, curr_y - 1);
        bfs_queue.push(pp);
        path[curr_x][curr_y - 1] = last_path;
        path[curr_x][curr_y - 1].push_back(pp);
      }
      if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){
        pair<int,int> pp = make_pair(curr_x + 1, curr_y);
        bfs_queue.push(pp);
        path[curr_x + 1][curr_y] = last_path;
        path[curr_x + 1][curr_y].push_back(pp);
      }
      if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){
        pair<int,int> pp = make_pair(curr_x, curr_y + 1);
        bfs_queue.push(pp);
        path[curr_x][curr_y + 1] = last_path;
        path[curr_x][curr_y + 1].push_back(pp);
      }
    }

  }
  vector<pair<int,int> > target = path[end_i][end_j];
  return target.size() - 1;
}

void calculate_distance(vector<vector<int> >& maze_array, vector<Node>& food_available){
  // set -1
  for(int m = 0; m < food_available.size(); m++){
    for(int n = 0; n < food_available[m].dist.size(); n++){
      food_available[m].dist[n] = -1;
    }
  }
  // calculate distance between two vertices
  for(int m = 0; m < food_available.size(); m++){
    for(int n = m+1; n < food_available.size(); n++){
      int start_i = food_available[m].i;
      int start_j = food_available[m].j;
      int end_i = food_available[n].i;
      int end_j = food_available[n].j;
      vector<vector<vector<pair<int,int> > > > path;
      vector<vector<int> > visited;
      for(int i = 0; i < xx; i++){
          vector<vector<pair<int,int> > > first;
          vector<int> visited_temp;
          for(int j = 0; j < yy; j++){
              visited_temp.push_back(0);
              vector<pair<int,int> > second;
              first.push_back(second);
          }
          visited.push_back(visited_temp);
          path.push_back(first);
      }
      visited[start_i][start_j] = 1;
      pair<int,int> p = make_pair(start_i, start_j);
      path[start_i][start_j].push_back(p);
      queue <pair<int,int> > bfs_queue;
      bfs_queue.push(p);//initial point
      int distance = findPathBFS(maze_array, visited, bfs_queue, path, end_i, end_j);
      int num1 = food_available[m].num;
      int num2 = food_available[n].num;
      food_available[m].dist[num2] = distance;
      food_available[n].dist[num1] = distance;
    }
  }
  // set the distance between the vertex and itself to 0
  for(int m = 0; m < food_available.size(); m++){
      int num = food_available[m].num;
      food_available[m].dist[num] = 0;
  }
}

// calcualte distance between current position and all foods
int calculate_distance_single(vector<vector<int> >& maze_array, vector<Node>& food_available, Node& start){
    start.dist[0] = 0;
    int min_dist = INT_MAX;
    for(int m = 0; m < food_available.size(); m++){
        int start_i = start.i;
        int start_j = start.j;
        int end_i = food_available[m].i;
        int end_j = food_available[m].j;
        vector<vector<vector<pair<int,int> > > > path;
        vector<vector<int> > visited;
        for(int i = 0; i < xx; i++){
            vector<vector<pair<int,int> > > first;
            vector<int> visited_temp;
            for(int j = 0; j < yy; j++){
                visited_temp.push_back(0);
                vector<pair<int,int> > second;
                first.push_back(second);
            }
            visited.push_back(visited_temp);
            path.push_back(first);
        }
        visited[start_i][start_j] = 1;
        pair<int,int> p = make_pair(start_i, start_j);
        path[start_i][start_j].push_back(p);
        queue <pair<int,int> > bfs_queue;
        bfs_queue.push(p);//initial point
        int distance = findPathBFS(maze_array, visited, bfs_queue, path, end_i, end_j);
        food_available[m].dist[0] = distance;
        int num = food_available[m].num;
        start.dist[num] = distance;
        if(distance < min_dist){
            min_dist = distance;
        }
    }
    return min_dist; // return minimum distance between current position and one food
}

// calculate MST
int calculate_heuristic(vector<Node>& node_array){
  double h = 0;
  vector<pair<int,int>> distance;
  map<int,int> visited;
  for(int i = 0; i < node_array.size(); i++){
    Node temp = node_array[i];
    pair<int,int> p;
    if(temp.num != 0)
      p = make_pair(INT_MAX, temp.num);
    else
      p = make_pair(0, temp.num);
    // p.first is distance
    // p.second is the num of node
    distance.push_back(p);
  }
  for(int i = 0; i < node_array.size(); i++){
    // find and pop out the min distance node
    sort(distance.begin(), distance.end());
    int node_num = distance[0].second;
    int hh = distance[0].first;
    h += hh;
    distance[0] = distance[distance.size()-1];
    distance.pop_back();
    // label as visited using map
    visited[node_num] = 1;
    // find the node's dist vector
    vector<int> curr_dist;
    for(int j = 0; j < node_array.size(); j++){
      if(node_array[j].num == node_num){
        curr_dist = node_array[j].dist;
        break;
      }
    }
    // for all unvisited neighbor of this node
    for(int j = 0; j < curr_dist.size(); j++){
      // if cost is less than distance, update distance
      if(j != node_num && curr_dist[j] != -1 && visited.find(j) == visited.end()){
        int idx = -1;
        for(int m = 0; m < distance.size(); m++){
          if(distance[m].second == j){
            idx = m;
            break;
          }
        }
        if(curr_dist[j] < distance[idx].first){
          distance[idx].first = curr_dist[j];
        }
      }
    }
  }
  // cout << "heuristic: " << h << endl;
  return h;
}



// A star search, return the destination
pair<int,int> findPathAStar(vector<vector<int> >& maze_array, vector<vector<int> >& visited, vector<tuple<double,int,int>> ll, vector<Node>& food_available, vector<vector<vector<pair<int,int> > > >& path){
  while(!ll.empty()){
    sort(ll.begin(), ll.end(), TupleCompare<0>());
    tuple<double,int,int> tp = ll[0];
    ll[0] = ll[ll.size()-1];
    ll.pop_back();
    int curr_x = get<1>(tp);
    int curr_y = get<2>(tp);
    visited[curr_x][curr_y] = 1;

    // check if current position is an end point
    for(int i = 0; i < food_available.size(); i++){
      Node temp = food_available[i];
      if(curr_x == temp.i && curr_y == temp.j){
        total += path[curr_x][curr_y].size() - 1;
        return make_pair(curr_x, curr_y);
      }
    }
    vector<pair<int,int> > last_path = path[curr_x][curr_y];
    int path_len = last_path.size();
    if(curr_y + 1 < yy && !visited[curr_x][curr_y + 1] && maze_array[curr_x][curr_y + 1] == 0){

      // add to frontier
      Node curr_point(curr_x, curr_y + 1, 0);
      for(int i = 0; i <= food_count; i++){
        curr_point.dist.push_back(-1);
      }
      int min_dist = calculate_distance_single(maze_array, food_available, curr_point);
      vector<Node> input = food_available;
      input.push_back(curr_point);

      double h = calculate_heuristic(input) + min_dist;
      tuple<double,int,int> tp = make_tuple(h, curr_x, curr_y + 1);
      ll.push_back(tp);
      expanded_node ++;

      // add to path
      pair<int,int> pp = make_pair(curr_x, curr_y + 1);
      path[curr_x][curr_y + 1] = last_path;
      path[curr_x][curr_y + 1].push_back(pp);
    }
    if(curr_x - 1 >= 0 && !visited[curr_x - 1][curr_y] && maze_array[curr_x - 1][curr_y] == 0){

      // add to frontier
      Node curr_point(curr_x - 1, curr_y, 0);
      for(int i = 0; i <= food_count; i++){
        curr_point.dist.push_back(-1);
      }
      int min_dist = calculate_distance_single(maze_array, food_available, curr_point);
      vector<Node> input = food_available;
      input.push_back(curr_point);

      double h = calculate_heuristic(input) + min_dist;
      tuple<double,int,int> tp = make_tuple(h, curr_x - 1, curr_y);
      ll.push_back(tp);
      expanded_node ++;

      // add to path
      pair<int,int> pp = make_pair(curr_x - 1, curr_y);
      path[curr_x - 1][curr_y] = last_path;
      path[curr_x - 1][curr_y].push_back(pp);
    }
    if(curr_x + 1 < xx && !visited[curr_x + 1][curr_y] && maze_array[curr_x + 1][curr_y] == 0){

      // add to frontier
      Node curr_point(curr_x + 1, curr_y, 0);
      for(int i = 0; i <= food_count; i++){
        curr_point.dist.push_back(-1);
      }
      int min_dist = calculate_distance_single(maze_array, food_available, curr_point);
      vector<Node> input = food_available;
      input.push_back(curr_point);

      double h = calculate_heuristic(input) + min_dist;
      tuple<double,int,int> tp = make_tuple(h, curr_x + 1, curr_y);
      ll.push_back(tp);
      expanded_node ++;

      // add to path
      pair<int,int> pp = make_pair(curr_x + 1, curr_y);
      path[curr_x + 1][curr_y] = last_path;
      path[curr_x + 1][curr_y].push_back(pp);
    }
    if(curr_y - 1 >= 0 && !visited[curr_x][curr_y - 1] && maze_array[curr_x][curr_y - 1] == 0){

      // add to frontier
      Node curr_point(curr_x, curr_y - 1, 0);
      for(int i = 0; i <= food_count; i++){
        curr_point.dist.push_back(-1);
      }
      int min_dist = calculate_distance_single(maze_array, food_available, curr_point);
      vector<Node> input = food_available;
      input.push_back(curr_point);

      double h = calculate_heuristic(input) + min_dist;
      tuple<double,int,int> tp = make_tuple(h, curr_x, curr_y - 1);
      ll.push_back(tp);
      expanded_node ++;

      pair<int,int> pp = make_pair(curr_x, curr_y - 1);
      path[curr_x][curr_y - 1] = last_path;
      path[curr_x][curr_y - 1].push_back(pp);
    }

  }
}

int main(){
  // read maze from txt and initialize
  vector<Node> food_available;
  ifstream maze("abc.txt");
  string line;
  vector<vector<int> > maze_array;
  int start_i,start_j = -1;
  int end_i, end_j = -1;
  int line_count = 0;

  while(getline(maze,line)){
    vector<int> temp;
    for(int i = 0; i < line.size(); i++){
      if(line[i] == '%'){
        temp.push_back(-1);
      }
      else{
        if(line[i] == 'P'){
          start_i = line_count;
          start_j = i;
          temp.push_back(0);
        }
        else if(line[i] == '.'){
          food_count ++;
          end_i = line_count;
          end_j = i;
          temp.push_back(0);
          Node n(end_i, end_j, food_count);
          food_available.push_back(n);
        }
        else {
          temp.push_back(0);
        }
      }
    }
    maze_array.push_back(temp);
    line_count ++;
  }

  xx = maze_array.size();
  yy = maze_array[0].size();

  // push 1+count position in node's dist
  for(int i = 0; i < food_available.size(); i++){
    for(int j = 0; j <= food_count; j++){
      food_available[i].dist.push_back(-1);
    }
  }
  // store total path from start to every food
  vector<vector<pair<int,int> > > total_path;
  vector<vector<int>> maze_array_ = maze_array;
  int num_eaten = 1;
  // prepare to call A*
  while(!food_available.empty()){
    calculate_distance(maze_array, food_available);
    // preparation for A*. Initialize data structures
    vector<vector<vector<pair<int,int> > > > path;
    vector<vector<int> > visited;
    for(int i = 0; i < xx; i++){
      vector<vector<pair<int,int> > > first;
      vector<int> visited_temp;
      for(int j = 0; j < yy; j++){
        visited_temp.push_back(0);
        vector<pair<int,int> > second;
        first.push_back(second);
      }
      path.push_back(first);
      visited.push_back(visited_temp);
    }
    pair<int,int> p = make_pair(start_i, start_j);
    path[start_i][start_j].push_back(p);
    visited[start_i][start_j] = 1;
    vector<tuple<double,int,int>> frontier;
    Node curr_point(start_i, start_j, 0);
    for(int i = 0; i <= food_count; i++){
      curr_point.dist.push_back(-1);
    }
    int min_dist = calculate_distance_single(maze_array, food_available, curr_point);
    vector<Node> input = food_available;
    input.push_back(curr_point);

    /* debug purporse*/
    // for(int i = 0; i < input.size(); i++){
    //   for(int j = 0; j < input[i].dist.size(); j++){
    //     cout << input[i].dist[j] << " ";
    //   }
    //   cout << endl;
    // }
    // cout << endl;
    // cout << endl;

    double heuristic = calculate_heuristic(input) + min_dist;
    tuple<double,int,int> t = make_tuple(heuristic, start_i, start_j);
    frontier.push_back(t);
    expanded_node ++;
    pair<int,int> endpoint = findPathAStar(maze_array, visited, frontier, food_available, path);

    // update info for next food
    start_i = endpoint.first;
    start_j = endpoint.second;
    total_path.push_back(path[endpoint.first][endpoint.second]);

    /* debug purpose */
    // vector<vector<int>> maze_array_ = maze_array;
    // for(int k = 0; k < path[endpoint.first][endpoint.second].size(); k++){
    //   maze_array_[path[endpoint.first][endpoint.second][k].first][path[endpoint.first][endpoint.second][k].second] = -6;
    // }
    // print_graph(maze_array_);

    maze_array_[endpoint.first][endpoint.second] = num_eaten;
    num_eaten ++ ;

    // remove found food
    for(int k = 0; k < food_available.size(); k++){
      if(food_available[k].i == endpoint.first && food_available[k].j == endpoint.second){
        food_available[k] = food_available[food_available.size()-1];
        food_available.pop_back();
        break;
      }
    }
  }
  print_graph(maze_array_);
  cout << dec << "solution cost: " << total << endl;
  cout << "expanded node: "<< expanded_node << endl;
  return 0;
}
