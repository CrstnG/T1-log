//include <bits/stdc++.h>
#include <tuple>
#include <array>
#include <vector>
using namespace std;
typedef tuple < double, double> puntosbd;
typedef tuple <puntosbd, double, Node*> entry;


class Node{
    public:
      vector <entry> v;
      bool is_leaf = false;
      int accesor = 0;
};