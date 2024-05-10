//include <bits/stdc++.h>
#include <tuple>
#include <array>
#include <vector>
using namespace std;

typedef tuple <double, double>puntosbd;
//typedef tuple <puntosbd, double, Node *> entry;

struct Entry;

typedef struct node{
  vector<Entry> keys;
  bool is_leaf = false;
  int height = 0;
}Node;

struct Entry{
  puntosbd point;
  double cr;
  Node* child;
};

//struct Node{
  //vector<Entry> keys;
  //bool is_leaf = false;
  //int height = 0;
//};





