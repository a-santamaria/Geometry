#ifndef RED_BLACK_BST_H
#define RED_BLACK_BST_H

#include "point.h"

/******************************************************************************
 *  Left Leaniing Red-Black Tree implementation
 *  Based in the implementation propoused by Robert Sedgewick
 *  paper Left-leaning Red-Black Trees:
 *   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.139.282
 *
 *  code: http://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
 *
 *  A symbol table implemented using a left-leaning red-black BST.
 *  This is the 2-3 version.
 *
 *
 ******************************************************************************/


class Node {
public:
    double key;         // key
    Segment* val;       // associated data
    Node *left, *right; // links to left and right subtrees
    bool color;         // color of parent link
    int N;              // subtree count

    Node(double _key, Segment* _val, bool _color, int _N);
};

class RedBlackBST {
private:

    static const bool RED = true;
    static const bool BLACK = false;

    Node* root;

    bool isRed(Node* x);
    int size(Node* x);
    Segment* get(Node* x, double key);
    Node* put(Node* h, double key, Segment* val);

    //Red-black tree deletion.
    Node* deleteMin(Node* h);
    Node* deleteMax(Node* h);
    Node* del(Node* h, double key);

    //Red-black tree helper functions.
    Node* rotateRight(Node* h);
    Node* rotateLeft(Node* h);
    void flipColors(Node* h);
    Node* moveRedLeft(Node* h);
    Node* moveRedRight(Node* h);
    Node* balance(Node* h);

    //Ordered symbol table methods.
    Node* min(Node* x);
    Node* max(Node* x);
    Node* floor(Node* x, double key);
    Node* ceiling(Node* x, double key);
    int rank(double key, Node* x);


public:
    RedBlackBST();
    int size();
    bool isEmpty();
    Segment* get(double key);
    bool contains(double key);
    void put(double key, Segment* val);

    //Red-black tree deletion.
    void deleteMin();
    void deleteMax();
    void del(double key);

    //Ordered symbol table methods.
    double min();
    double max();
    double floor(double key);
    double ceiling(double key);
    int rank(double key);
};


#endif
