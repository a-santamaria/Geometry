#ifndef RED_BLACK_BST_H
#define RED_BLACK_BST_H

#include "point.h"
#include <list>

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
    Segment key;          // key and data
    Node *left, *right;   // links to left and right subtrees
    bool color;           // color of parent link
    int N;                // subtree count

    Node(Segment _key, bool _color, int _N);
};

class RedBlackBST {
private:

    static const bool RED = true;
    static const bool BLACK = false;

    Node* root;

    bool isRed(Node* x);
    int size(Node* x);
    Node* get(Node* x, Segment key);
    Node* put(Node* h, Segment key);

    //Red-black tree deletion.
    Node* deleteMin(Node* h);
    Node* deleteMax(Node* h);
    Node* del(Node* h, Segment key);

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
    Node* floor(Node* x, Segment key);
    Node* ceiling(Node* x, Segment key);
    int rank(Segment key, Node* x);


public:
    RedBlackBST();
    int size();
    bool isEmpty();
    Node* get(Segment key);
    bool contains(Segment key);
    void put(Segment key);

    //Red-black tree deletion.
    void deleteMin();
    void deleteMax();
    void del(Segment key);

    //Ordered symbol table methods.
    Segment min();
    Segment max();
    bool floor(Segment key, Segment& res);
    bool ceiling(Segment key, Segment& res);
    int rank(Segment key);

    void printTree();
};


#endif
