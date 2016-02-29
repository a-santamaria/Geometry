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
    Point key;            // key
    std::list<Segment*> val; // associated data
    Node *left, *right;   // links to left and right subtrees
    bool color;           // color of parent link
    int N;                // subtree count

    Node(Point _key, Segment* seg, bool _color, int _N);
    Node(Point _key, std::list<Segment*> _val, bool _color, int _N);
};

class RedBlackBST {
private:

    static const bool RED = true;
    static const bool BLACK = false;

    Node* root;

    bool isRed(Node* x);
    int size(Node* x);
    std::list<Segment*>* get(Node* x, Point key);
    Node* put(Node* h, Point key, Segment* val);

    //Red-black tree deletion.
    Node* deleteMin(Node* h);
    Node* deleteMax(Node* h);
    Node* del(Node* h, Point key);

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
    Node* floor(Node* x, Point key);
    Node* ceiling(Node* x, Point key);
    int rank(Point key, Node* x);


public:
    RedBlackBST();
    int size();
    bool isEmpty();
    std::list<Segment*>* get(Point key);
    bool contains(Point key);
    void put(Point key, Segment* val);
    void putNeighbors(Point key, Segment* f, Segment* s);

    //Red-black tree deletion.
    void deleteMin();
    void deleteMax();
    void del(Point key);
    void del(Point key, Segment* prev);

    //Ordered symbol table methods.
    Point min();
    Point max();
    Node* floor(Point key);
    Node* ceiling(Point key);
    int rank(Point key);
};


#endif
