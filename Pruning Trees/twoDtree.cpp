#include "twoDtree.h"

/* node constructor */
twoDtree::Node::Node(pair<int, int> ul, pair<int, int> lr, HSLAPixel a)
        : upLeft(ul), lowRight(lr), avg(a), LT(NULL), RB(NULL) {}

/* destructor */
twoDtree::~twoDtree() {
    clear();
}

/* copy constructor */
twoDtree::twoDtree(const twoDtree &other) {
    copy(other);
}

/* operator= */
twoDtree &twoDtree::operator=(const twoDtree &rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/* twoDtree constructor */
twoDtree::twoDtree(PNG &imIn) {

    stats s(imIn);

    width = imIn.width();
    height = imIn.height();

    pair<int, int> ul(0, 0);
    pair<int, int> lr(width - 1, height - 1);

    root = buildTree(s, ul, lr, true);
}

/* buildTree helper for twoDtree constructor */
twoDtree::Node *twoDtree::buildTree(stats &s, pair<int, int> ul, pair<int, int> lr, bool vert) {

    Node *currNode = new Node(ul, lr, s.getAvg(ul, lr));
    bool lt_dir = !vert;
    bool rb_dir = !vert;
    double min_entropy = LARGE;
    double total_area = s.rectArea(ul, lr);

    pair<int, int> best_lt_ul;
    pair<int, int> best_lt_lr;
    pair<int, int> best_rb_ul;
    pair<int, int> best_rb_lr;

    if (ul != lr) {

        pair<int, int> lt_ul = ul;
        pair<int, int> lt_lr = lr;
        pair<int, int> rb_ul = ul;
        pair<int, int> rb_lr = lr;

        if (vert) { // vertical split

            for (int x = ul.first; x < lr.first; x++) {

                lt_lr.first = x;
                rb_ul.first = x + 1;

                double lt_area = s.rectArea(lt_ul, lt_lr);
                double lt_entropy = s.entropy(lt_ul, lt_lr);
                double lt_weighted = lt_entropy * lt_area / total_area;

                double rb_area = s.rectArea(rb_ul, rb_lr);
                double rb_entropy = s.entropy(rb_ul, rb_lr);
                double rb_weighted = rb_entropy * rb_area / total_area;

                double weighted_entropy = lt_weighted + rb_weighted;

                if (weighted_entropy <= min_entropy) {
                    min_entropy = weighted_entropy;

                    best_lt_ul = lt_ul;
                    best_lt_lr = lt_lr;
                    best_rb_ul = rb_ul;
                    best_rb_lr = rb_lr;
                }
            }

        } else { // horizontal split

            for (int y = ul.second; y < lr.second; y++) {

                lt_lr.second = y;
                rb_ul.second = y + 1;

                double lt_area = s.rectArea(lt_ul, lt_lr);
                double lt_entropy = s.entropy(lt_ul, lt_lr);
                double lt_weighted = lt_entropy * (lt_area / total_area);

                double rb_area = s.rectArea(rb_ul, rb_lr);
                double rb_entropy = s.entropy(rb_ul, rb_lr);
                double rb_weighted = rb_entropy * (rb_area / total_area);

                double weighted_entropy= lt_weighted + rb_weighted;

                if (weighted_entropy <= min_entropy) {
                    min_entropy = weighted_entropy;

                    best_lt_ul = lt_ul;
                    best_lt_lr = lt_lr;
                    best_rb_ul = rb_ul;
                    best_rb_lr = rb_lr;
                }
            }
        }

        if (best_lt_ul.first == best_lt_lr.first)        lt_dir = false;   // LT is vert  / horiz only
        else if (best_lt_ul.second == best_lt_lr.second) lt_dir = true;    // LT is horiz / vert  only

        currNode->LT = buildTree(s, best_lt_ul, best_lt_lr, lt_dir);

        if (best_rb_ul.first == best_rb_lr.first)        rb_dir = false;   // RB is vert  / horiz only
        else if (best_rb_ul.second == best_rb_lr.second) rb_dir = true;    // RB is horiz / vert  only

        currNode->RB = buildTree(s, best_rb_ul, best_rb_lr, rb_dir);
    }

    return currNode;
}

/* render twoDtree into a png */
PNG twoDtree::render() {

    PNG result(width, height);
    renderRec(result, root);
    return result;

}

/* recursive helper function for render */
void twoDtree::renderRec(PNG &img, Node *node) {

    if (node->LT != NULL and node->RB != NULL) {
        renderRec(img, node->LT);
        renderRec(img, node->RB);
    } else {
        pair<int, int> ul = node->upLeft;
        pair<int, int> lr = node->lowRight;

        for (int j = ul.second; j <= lr.second; j++) {
            for (int i = ul.first; i <= lr.first; i++) {
                HSLAPixel *pixelToChange = img.getPixel(i, j);
                *pixelToChange = node->avg;
            }
        }
    }
}


/* prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void twoDtree::prune(double tol) {

    checkChildren(root, tol);

}

/* recursive helper function for prune */
void twoDtree::checkChildren(Node *curr, double tol) {

    if (curr == NULL) return;

    if (checkTol(curr, tol, curr->avg)) {

        checkChildren(curr->LT, tol);
        checkChildren(curr->RB, tol);

    } else {

        clearRec(curr->LT);
        curr->LT = NULL;
        clearRec(curr->RB);
        curr->RB = NULL;
    }
}

/* recursive helper function for checkChildren */
bool twoDtree::checkTol(Node *curr, double tol, HSLAPixel avg) {

    if (curr == NULL) return false;

    if (curr->avg.dist(avg) < tol) {

        if (checkTol(curr->LT, tol, avg)) return true;
        if (checkTol(curr->RB, tol, avg)) return true;

        return false;

    } else {

        return true;

    }
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the twoDtree */
void twoDtree::clear() {
    clearRec(root);
}

/* recursive helper function for clear */
void twoDtree::clearRec(Node *curr) {
    if (curr) {
        clearRec(curr->LT);
        clearRec(curr->RB);
        delete curr;
    }
}

/* helper function for copy constructor and op= */
void twoDtree::copy(const twoDtree &orig) {
    root = copyRec(orig.root);
    height = orig.height;
    width = orig.width;
}

/* recursive helper function for copy */
twoDtree::Node *twoDtree::copyRec(Node *copy) {
    if (copy != NULL) {
        Node *curr = new Node(copy->upLeft, copy->lowRight, copy->avg);

        curr->LT = copyRec(copy->LT);
        curr->RB = copyRec(copy->RB);

        return curr;
    }

    return NULL;
}


