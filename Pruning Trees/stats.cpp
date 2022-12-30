#include "stats.h"

stats::stats(PNG &im) {

    sumHueX.resize(im.width(), vector<double>(im.height()));
    sumHueY.resize(im.width(), vector<double>(im.height()));
    sumSat.resize(im.width(), vector<double>(im.height()));
    sumLum.resize(im.width(), vector<double>(im.height()));
    hist.resize((im.width()), vector<vector<int>>(im.height(), vector<int>(36)));

    for (long j = 0; j < im.height(); j++) {
        for (long i = 0; i < im.width(); i++) {

            HSLAPixel *curr = im.getPixel(i, j);
            double currHueX = cos(curr->h * PI / 180);
            double currHueY = sin(curr->h * PI / 180);
            int k = curr->h / 10;

            //base case
            if (i == 0 and j == 0) {
                sumHueX[0][0] = currHueX;
                sumHueY[0][0] = currHueY;
                sumSat[0][0]  = im.getPixel(0, 0)->s;
                sumLum[0][0]  = im.getPixel(0, 0)->l;
            }

            //case 1: first row
            else if (j == 0) {
                sumHueX[i][0] = currHueX + sumHueX[i - 1][0];
                sumHueY[i][0] = currHueY + sumHueY[i - 1][0];
                sumSat[i][0]  = curr->s  + sumSat[i - 1][0];
                sumLum[i][0]  = curr->l  + sumLum[i - 1][0];
                hist[i][0]    = hist[i - 1][0];
            }

            //case 2: first column
            else if (i == 0) {
                sumHueX[0][j] = currHueX + sumHueX[0][j - 1];
                sumHueY[0][j] = currHueY + sumHueY[0][j - 1];
                sumSat[0][j]  = curr->s  + sumSat[0][j - 1];
                sumLum[0][j]  = curr->l  + sumLum[0][j - 1];
                hist[0][j]    = hist[0][j - 1];
            }

            //case 3: everything else
            else {
                sumHueX[i][j] = currHueX + sumHueX[i - 1][j] + sumHueX[i][j - 1] - sumHueX[i - 1][j - 1];
                sumHueY[i][j] = currHueY + sumHueY[i - 1][j] + sumHueY[i][j - 1] - sumHueY[i - 1][j - 1];
                sumSat[i][j]  = curr->s  + sumSat[i - 1][j]  + sumSat[i][j - 1]  - sumSat[i - 1][j - 1];
                sumLum[i][j]  = curr->l  + sumLum[i - 1][j]  + sumLum[i][j - 1]  - sumLum[i - 1][j - 1];

                for (int k_ = 0; k_ < 36; k_++) {
                    hist[i][j][k_] = hist[i - 1][j][k_] + hist[i][j - 1][k_] - hist[i - 1][j - 1][k_];
                }
            }

            hist[i][j][k]++;
        }
    }
}

long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
    return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

HSLAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {

    HSLAPixel avg;
    long area = rectArea(ul, lr);

    int a = ul.first - 1;
    int d = ul.second - 1;
    int c = lr.first;
    int b = lr.second;

    double sat = sumSat[c][b];
    double lum = sumLum[c][b];
    double hx  = sumHueX[c][b];
    double hy  = sumHueY[c][b];

    //case 1: first row 
    if (ul.first != 0 and ul.second == 0) {
        sat -= sumSat[a][b];
        lum -= sumLum[a][b];
        hx  -= sumHueX[a][b];
        hy  -= sumHueY[a][b];
    }

    //case 2: first column
    else if (ul.first == 0 and ul.second != 0) {
        sat -= sumSat[c][d];
        lum -= sumLum[c][d];
        hx  -= sumHueX[c][d];
        hy  -= sumHueY[c][d];
    }

    //case 3: everything else
    else if (ul.first != 0 and ul.second != 0) {
        sat = sat - sumSat[a][b]  - sumSat[c][d]  + sumSat[a][d];
        lum = lum - sumLum[a][b]  - sumLum[c][d]  + sumLum[a][d];
        hx  = hx  - sumHueX[a][b] - sumHueX[c][d] + sumHueX[a][d];
        hy  = hy  - sumHueY[a][b] - sumHueY[c][d] + sumHueY[a][d];
    }

    double avg_h = atan2(hy, hx) * (180 / PI);
    if (avg_h > 360) avg_h -= 360;
    if (avg_h < 0)   avg_h += 360;

    avg.h = avg_h;
    avg.s = sat / area;
    avg.l = lum / area;
    avg.a = 1.0;

    return avg;
}

double stats::entropy(pair<int, int> ul, pair<int, int> lr) {

    vector<int> distn;
    long area = rectArea(ul, lr);

    int a = ul.first - 1;
    int b = lr.second;
    int c = lr.first;
    int d = ul.second - 1;

    distn.resize(36);
    distn = hist[c][b];
    double entropy = 0.0;

    for (int k = 0; k < 36; k++) {

        if (ul.first != 0 and ul.second == 0) {
            distn[k] -= hist[a][b][k];
        } else if (ul.first == 0 and ul.second != 0) {
            distn[k] -= hist[c][d][k];
        } else if (ul.first != 0 and ul.second != 0) {
            distn[k] -= (hist[a][b][k] + hist[c][d][k] - hist[a][d][k]);
        }

        if (distn[k] > 0) entropy += ((double) distn[k] / (double) area) * log2((double) distn[k] / (double) area);

    }

    return -1 * entropy;
}
