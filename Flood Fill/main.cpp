#include "cs221util/catch.hpp"
#include <limits.h>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "filler.h"
#include "solidColorPicker.h"
#include "gridColorPicker.h"
#include "gradientColorPicker.h"
#include "rainbowColorPicker.h"

using namespace std;
using namespace cs221util;

#define GRADIENTTESTIMAGE "originals/cloud.png"
#define GRADIENTX 50
#define GRADIENTY 50
#define GRADIENTTOLERANCE 0.02
#define GRADIENTFRAMEFREQ 100
#define GRADIENTRADIUS 70

int main() {

    PNG img;
    img.readFromFile(GRADIENTTESTIMAGE);
    string name = "test";

    HSLAPixel px1;
    px1.h = 0; px1.s = 1.0; px1.l = 0.5;
    HSLAPixel px2;
    px2.h = 300; px2.s = 1.0; px2.l = 0.5;

    animation anim;
    anim = filler::fillGradientDFS(img, GRADIENTX, GRADIENTY, px1, px2,
                                   GRADIENTRADIUS, GRADIENTTOLERANCE,
                                   GRADIENTFRAMEFREQ);

    anim.write("images/" + name + ".gif");

    cout << "------------------------------------------" << endl;
    cout << "Done!" << endl;
    cout << "------------------------------------------" << endl;

//    PNG img;
//    cout << "------------------------------------------" << endl;
//    cout << "Image Name (eg. name) : ";
//    string name;
//    cin >> name;
//
//    img.readFromFile("originals/" + name + ".png");

//    cout << "------------------------------------------" << endl;
//    cout << "Tolerance Amount (eg. 0.25) : ";
//    double tol;
//    cin >> tol;
    return 0;
}

