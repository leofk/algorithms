#include "twoDtree.h"
#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"

using namespace cs221util;
using namespace std;

int main() {

    PNG img;
    cout << "------------------------------------------" << endl;
    cout << "Image Name (eg. name) : ";
    string name;
    cin >> name;

    img.readFromFile("images/original/" + name + ".png");

    cout << "------------------------------------------" << endl;
    cout << "Tolerance Amount (eg. 0.25) : ";
    double tol;
    cin >> tol;

    twoDtree t1(img);
    t1.prune(tol);
    PNG result = t1.render();
    result.writeToFile("images/edited/pruned_" + name + ".png");

    cout << "------------------------------------------" << endl;
    cout << "Done!" << endl;
    cout << "------------------------------------------" << endl;

    return 0;
}

