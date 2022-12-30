#ifndef _TWODTREE_H_
#define _TWODTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"

#define LARGE 10000000000

using namespace std;
using namespace cs221util;

/**
 * twoDtree: This is a structure used in decomposing an image
 * into rectangles of similarly colored pixels.
 */

class twoDtree {
private:

    /**
     * The Node class is private to the tree class via the principle of
     * encapsulation---the end user does not need to know our node-based
     * implementation details.
     */
    class Node {
    public:
        Node(pair<int, int> ul, pair<int, int> lr, HSLAPixel a); // Node constructor

        pair<int, int> upLeft;
        pair<int, int> lowRight;
        HSLAPixel avg;
        Node *LT; // left or top child rectangle
        Node *RB; // right or bottom child rectangle

    };


public:

    /**
     * twoDtree destructor.
     * Destroys all of the memory associated with the
     * current twoDtree. This function should ensure that
     * memory does not leak on destruction of a twoDtree.
     */
    ~twoDtree();

    /**
     * Copy constructor for a twoDtree. GIVEN
     * Since twoDtrees allocate dynamic memory (i.e., they use "new", we
     * must define the Big Three). This depends on your implementation
     * of the copy funtion.
     *
     * @param other The twoDtree  we are copying.
     */
    twoDtree(const twoDtree &other);

    /**
     * Constructor that builds a twoDtree out of the given PNG.
     * Every leaf in the tree corresponds to a pixel in the PNG.
     * Every non-leaf node corresponds to a rectangle of pixels
     * in the original PNG, represented by an (x,y) pair for the
     * upper left corner of the rectangle and an (x,y) pair for
     * lower right corner of the rectangle. In addition, the Node
     * stores a pixel representing the average color over the
     * rectangle.
     *
     * Every node's left and right children correspond to a partition
     * of the node's rectangle into two smaller rectangles. The node's
     * rectangle is split by the horizontal or vertical line that
     * results in the two smaller rectangles whose Information Gain
     * is as large as possible. (or correspondingly, whose weighted
     * sum of entropies is as small as possible. see online spec.)
     *
     * Note that splits will alternate between vertical and
     * horizontal, at every level of the tree, beginning with a
     * vertical split. A 1xw rectangle will only have vertical
     * splits remaining, and a hx1 rectangle will only have
     * horizontal splits remaining.
     *
     * The LT child of the node will contain the upper left corner
     * of the node's rectangle, and the RB child will contain the
     * lower right corner. (see illustrations within the spec.)
     *
    * This function will build the stats object used to score the
    * splitting lines. It will also call helper function buildTree.
     */
    twoDtree(PNG &imIn);

    /**
     * Overloaded assignment operator for twoDtrees.
     * Part of the Big Three that we must define because the class
     * allocates dynamic memory. This depends on your implementation
     * of the copy and clear funtions.
     *
     * @param rhs The right hand side of the assignment statement.
     */
    twoDtree &operator=(const twoDtree &rhs);

    /**
     * Render returns a PNG image consisting of the pixels
     * stored in the tree. may be used on pruned trees. Draws
     * every leaf node's rectangle onto a PNG canvas using the
     * average color stored in the node.
     */
    PNG render();

    /**
     *  Prune function trims subtrees as high as possible in the tree.
     *  A subtree is pruned (cleared) if all of the subtree's leaves are within
     *  tol of the average color stored in the root of the subtree.
     *  Pruning criteria should be evaluated on the original tree, not
     *  on any pruned subtree. (we only expect that trees would be pruned once.)
     *
    * You may want a recursive helper function for this one.
     */
    void prune(double tol);


private:

    /**
      * Private member variables
    */
    Node *root; // ptr to the root of the twoDtree
    int height; // height of PNG represented by the tree
    int width; // width of PNG represented by the tree

    /**
      * Private helper function for the constructor. Recursively builds
      * the tree according to the specification of the constructor.
      * @param s Contains the data used to split the rectangles
      * @param ul upper left point of current node's rectangle.
      * @param lr lower right point of current node's rectangle.
      * @param vert indicates if the split should be vertical or not.
    */
    Node *buildTree(stats &s, pair<int, int> ul, pair<int, int> lr, bool vert);

    /**
      * Private Recursive helpful function for render
      * @param img Output image that is rendered
      * @param node The root of the tree
    */
    void renderRec(PNG &img, Node *node);

    /**
      * Private Recursive helpful function for prune
      * Destroys all children if they are within the tolerance
      * @param curr Node to check
      * @param tol Desired tolerance
    */
    void checkChildren(Node *curr, double tol);

    /**
      * Private Recursive helpful function for checkChildren
      * Returns false if all children are within tolerance.
      * Returns true if any children are outside of tolerance.
      * @param curr Node to check
      * @param tol Desired tolerance
      * @param avg The root of the subtree's HSLAPixel value
    */
    bool checkTol(Node *node, double tol, HSLAPixel avg);

    /**
     * Private helpful function for destructor.
     * Destroys all dynamically allocated memory associated with the
     * current twoDtree class.
    */
    void clear();

    /**
      * Private Recursive helpful function for clear
      * @param node The root of the tree
    */
    void clearRec(Node *node);

    /**
     * Private helpful function for copy constructor.
     * Copies the parameter other twoDtree into the current twoDtree.
     * Does not free any memory. Called by copy constructor and op=.
     * @param other The twoDtree to be copied.
    */
    void copy(const twoDtree &other);

    /**
      * Private Recursive helpful function for copy
      * @param node The root of the tree
    */
    Node *copyRec(Node *node);

};

#endif
