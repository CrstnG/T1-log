#include <iostream>
#include <tuple>
using namespace std;

int main() {
    tuple<int, string, float> t1 = make_tuple(10, "Test", 3.14);

    cout << "El primer valor en la tupla es: " << get<0>(t1) << endl;
    cout << "El segundo valor en la tupla es: " << get<1>(t1) << endl;
    cout << "El tercer valor en la tupla es: " << get<2>(t1) <<endl;

    return 0;
}
