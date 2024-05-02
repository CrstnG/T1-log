//include <bits/stdc++.h>
#include <tuple>
#include <array>
#include <vector>
using namespace std;

typedef tuple <double, double>puntosbd;
//typedef tuple <puntosbd, double, Node *> entry;

double euclideanDistance(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<0>(p2);
    return sqrt(dx * dx + dy * dy);
}

//class Node{
  //  public:
    //  vector <entry> v;
      //bool is_leaf = false;
      //int accesor = 0;
//};

