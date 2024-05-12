//include <bits/stdc++.h>
#include <tuple>
#include <array>
#include <vector>
using namespace std;

typedef tuple <double, double>puntosbd;
//typedef tuple <puntosbd, double, Node *> entry;

struct Entry;
struct Node;

struct Entry{
  puntosbd point;
  double cr;
  Node* child;
};

struct Node{
  vector<Entry> keys;
  //bool is_leaf = false;
  int height (){return search_height(keys);}
  
  int search_height (vector<Entry> keys){
     int actual_height = 0;
     for (auto &entrada: keys){
          if(entrada.child == nullptr){
            actual_height = 0;
          }
          else{
            actual_height = max(actual_height,1 + search_height(entrada.child->keys));
          }  
     }
     return actual_height;
  }
};






