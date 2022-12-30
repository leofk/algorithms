#include <iostream>
#include <vector>
using namespace std;

int LARGE = 100000;

/*
** this fills a vector with random numbers
 * except v[0] is always -1 for 1 based indexing
*/
std::vector<int> generateVector(int size)
{
    std::vector<int> v(size);
    v.at(0) = -1;

    for(int i = 1; i < size; i++) {
        v.at(i) = rand() % size;
    }
    return v;
}

/*
 * helper simlar to soln' in the handout
 */
int helper(vector<int> &soln, int i) {
    if (i < 1) return LARGE;
    else return soln[i];
}

/*
 * uses precomputed array soln to find the vertices in the minimum dominating set
 */
vector<int> findMinDomSet(vector<int> &path, vector<int> &soln, int result, int i) {
    std::vector<int> minDomSet;

    if (helper(soln, i) < helper(soln, i - 1)) {
        minDomSet.push_back(path[i]);
        result -= path[i];
    } else {
        minDomSet.push_back(path[i-1]);
        result -= path[i-1];
        i--;
    }

    while (result > 0) {
        int h1 = helper(soln, i - 1);
        int h2 = helper(soln, i - 2);
        int h3 = helper(soln, i - 3);

        if (h3 <= h1 and h3 <= h2) {
            minDomSet.push_back(path[i - 3]);
            result -= path[i - 3];
            i-=3;
        }

        else if (h2 <= h1){
            minDomSet.push_back(path[i - 2]);
            result -= path[i - 2];
            i-=2;
        }

        else {
            minDomSet.push_back(path[i - 1]);
            result -= path[i - 1];
            i-=1;
        }
    }

    return minDomSet;
}

/*
 * Updates each element in soln to its corresponding total min
 * returns the total min given an starting index i
 */
int totalMin(vector<int> &path, vector<int> &soln, int i) {
    if (i < 1) return LARGE;

    if (i == 1) {
        soln[i] = path[i];
        return soln[i];
    }

    else {
        for (int j = 1; j<=i; j++) {
            int h1 = helper(soln, j-1) + path[j];
            int h2 = helper(soln, j-2) + path[j];
            int h3 = helper(soln, j-3) + path[j];

            int minPath = min(min(h1, h2), h3);
            if (j<=2) soln[j] = min(path[j], minPath);
            else soln[j] = minPath;
        }
    }

    return min(soln[i], soln[i-1]);
}

int main()
{
    // Represent a path in a graph using an array of ints
    // path[i] is the weight of vertex i
    vector<int> path;
    int size = 7; // arbitrary
    path = generateVector(size+1); // size+1 for 1 based indexing. i[0] will be -1.

    cout << "Input Array: [ ";
    for (int j: path)
        cout << j << ' ';
    cout << "]" << endl;

    // array the same size as input path
    // used to keep track of total min at each index
    vector<int> soln(size+1);
    for(int i = 0; i < size+1; i++) {
        soln.at(i) = -1; // init all elements to -1
    }

    // compute the sum of the min weights from vertex n
    int i = path.size()-1;
    int result = totalMin(path, soln, i);
    cout << "Total Min Weight: " << result << endl;

    // print the vertices that belong to the minimum dominating set
    vector<int> minDomSet;
    minDomSet = findMinDomSet(path, soln, result, i);

    cout << "Min Weight Dominating Set: [ ";
    for (int j: minDomSet)
        cout << j << ' ';
    cout << "]" << endl;

    return 0;
}
