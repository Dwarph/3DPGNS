#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include "./DiamondSquare.h"
#include <vector>

using namespace std;



int main() {
    DiamondSquare *diamondSquare = new DiamondSquare(7, 5);

    vector<vector<float> > ds = diamondSquare->getHeightMap();

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            cout << " " << ds.at(i).at(j);
        }
        cout << endl;
    }

    return 0;
}
