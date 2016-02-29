#include "redBlackBST.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

Node::Node(double _key, Segment* _val, bool _color, int _N) :
    key(_key), val(_val), color(_color), N(_N) {
    this->left = this->right = NULL;
}

RedBlackBST::RedBlackBST(){
    root = NULL;
}

bool RedBlackBST::isRed(Node* x) {
    if(x == NULL) return false;
    return x->color == RED;
}

int RedBlackBST::size(Node* x) {
    if (x == NULL) return 0;
    return x->N;
}

int RedBlackBST::size() {
    return size(root);
}

bool RedBlackBST::isEmpty() {
    return root == NULL;
}

/***************************************************************************
*  Standard BST search.
***************************************************************************/

Segment* RedBlackBST::get(double key) {
    return get(root, key);
}

Segment* RedBlackBST::get(Node* x, double key) {
    while (x != NULL) {
        if  (key == x->key) return x->val;
        if  (key < x->key)  x = x->left;
        else                x = x->right;
    }
    return NULL;
}

bool RedBlackBST::contains(double key) {
    return get(key) != NULL;
}

/***************************************************************************
*  Red-black tree insertion.
***************************************************************************/
void RedBlackBST::put(double key, Segment* val) {
    if (val == NULL) {
        del(key);
        return;
    }

    root = put(root, key, val);
    root->color = BLACK;
}

Node* RedBlackBST::put(Node* h, double key, Segment* val) {
    if (h == NULL) return new Node(key, val, RED, 1);

    //TODO agregar a la lista (posiblemente)
    if       (key == h->key) h->val = val;
    else if  (key < h->key)  h->left  = put(h->left,  key, val);
    else                     h->right = put(h->right, key, val);

    // fix-up any right-leaning links
    if (isRed(h->right) && !isRed(h->left))       h = rotateLeft(h);
    if (isRed(h->left)  &&  isRed(h->left->left)) h = rotateRight(h);
    if (isRed(h->left)  &&  isRed(h->right))      flipColors(h);
    h->N = size(h->left) + size(h->right) + 1;

    return h;
}

/***************************************************************************
*  Red-black tree deletion.
***************************************************************************/

// Removes the smallest key and associated value from the symbol table.
void RedBlackBST::deleteMin() {
    assert (!isEmpty());

    // if both children of root are black, set root to red
    if (!isRed(root->left) && !isRed(root->right))
        root->color = RED;

    root = deleteMin(root);
    if (!isEmpty()) root->color = BLACK;
}

// delete the key-value pair with the minimum key rooted at h
Node* RedBlackBST::deleteMin(Node* h) {
    if (h->left == NULL)
        return NULL;

    if (!isRed(h->left) && !isRed(h->left->left))
        h = moveRedLeft(h);

    h->left = deleteMin(h->left);
    return balance(h);
}


// Removes the largest key and associated value from the symbol table.
void RedBlackBST::deleteMax() {
    if (!isEmpty());

    // if both children of root are black, set root to red
    if (!isRed(root->left) && !isRed(root->right))
        root->color = RED;

    root = deleteMax(root);
    if (!isEmpty()) root->color = BLACK;
}

// delete the key-value pair with the maximum key rooted at h
Node* RedBlackBST::deleteMax(Node* h) {
    if (isRed(h->left))
        h = rotateRight(h);

    if (h->right == NULL)
        return NULL;

    if (!isRed(h->right) && !isRed(h->right->left))
        h = moveRedRight(h);

    h->right = deleteMax(h->right);

    return balance(h);
}

void RedBlackBST::del(double key) {
    if (!contains(key)) return;

    // if both children of root are black, set root to red
    if (!isRed(root->left) && !isRed(root->right))
        root->color = RED;

    root = del(root, key);
    if (!isEmpty()) root->color = BLACK;
}

Node* RedBlackBST::del(Node* h, double key) {
    // assert get(h, key) != NULL;

    if (key < h->key)  {
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = del(h->left, key);
    }
    else {
        if (isRed(h->left))
            h = rotateRight(h);
        if (key == h->key && h->right == NULL)
            return NULL;
        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);
        if (key == h->key) {
            Node* x = min(h->right);
            h->key = x->key;
            h->val = x->val;
            // h.val = get(h.right, min(h.right).key);
            // h.key = min(h.right).key;
            h->right = deleteMin(h->right);
        }
        else h->right = del(h->right, key);
    }
    return balance(h);
}



/***************************************************************************
*  Red-black tree helper functions.
***************************************************************************/

// make a left-leaning link lean to the right
Node* RedBlackBST::rotateRight(Node* h) {
   // assert (h != NULL) && isRed(h.left);
   Node* x = h->left;
   h->left = x->right;
   x->right = h;
   x->color = x->right->color;
   x->right->color = RED;
   x->N = h->N;
   h->N = size(h->left) + size(h->right) + 1;
   return x;
}

// make a right-leaning link lean to the left
Node* RedBlackBST::rotateLeft(Node* h) {
   Node* x = h->right;
   h->right = x->left;
   x->left = h;
   x->color = x->left->color;
   x->left->color = RED;
   x->N = h->N;
   h->N = size(h->left) + size(h->right) + 1;
   return x;
}

// flip the colors of a node and its two children
void RedBlackBST::flipColors(Node* h) {
   h->color = !h->color;
   h->left->color = !h->left->color;
   h->right->color = !h->right->color;
}

// Assuming that h is red and both h.left and h.left.left
// are black, make h.left or one of its children red.
Node* RedBlackBST::moveRedLeft(Node* h) {
   flipColors(h);
   if (isRed(h->right->left)) {
       h->right = rotateRight(h->right);
       h = rotateLeft(h);
       flipColors(h);
   }
   return h;
}

// Assuming that h is red and both h.right and h.right.left
// are black, make h.right or one of its children red.
Node* RedBlackBST::moveRedRight(Node* h) {
   flipColors(h);
   if (isRed(h->left->left)) {
       h = rotateRight(h);
       flipColors(h);
   }
   return h;
}

// restore red-black tree invariant
Node* RedBlackBST::balance(Node* h) {
   if (isRed(h->right))                        h = rotateLeft(h);
   if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
   if (isRed(h->left) && isRed(h->right))      flipColors(h);

   h->N = size(h->left) + size(h->right) + 1;
   return h;
}


/***************************************************************************
*  Ordered symbol table methods.
***************************************************************************/

// Returns the smallest key in the symbol table.
double RedBlackBST::min() {
    assert (!isEmpty());
    return min(root)->key;
}

// the smallest key in subtree rooted at x; NULL if no such key
Node* RedBlackBST::min(Node* x) {
    if (x->left == NULL) return x;
    else                 return min(x->left);
}

// Returns the largest key in the symbol table.
double RedBlackBST::max() {
    assert (!isEmpty());
    return max(root)->key;
}

// the largest key in the subtree rooted at x; NULL if no such key
Node* RedBlackBST::max(Node* x) {
    if (x->right == NULL) return x;
    else                 return max(x->right);
}

// Returns the largest key in the symbol table less than or equal to <tt>key</tt>.
double RedBlackBST::floor(double key) {
    assert (!isEmpty());
    Node* x = floor(root, key);
    assert (x != NULL);
    return x->key;
}

// the largest key in the subtree rooted at x less than or equal to the given key
Node* RedBlackBST::floor(Node* x, double key) {
    if (x == NULL) return NULL;

    if (key == x->key) return x;
    if (key < x->key)  return floor(x->left, key);
    Node* t = floor(x->right, key);
    if (t != NULL) return t;
    else           return x;
}

// Returns the smallest key in the symbol table greater than or equal to <tt>key</tt>.
double RedBlackBST::ceiling(double key) {
    assert (!isEmpty());

    Node* x = ceiling(root, key);
    assert (x != NULL);
    return x->key;
}

// the smallest key in the subtree rooted at x greater than or equal to the given key
Node* RedBlackBST::ceiling(Node* x, double key) {
    if (x == NULL) return NULL;
    if (key == x->key) return x;
    if (key > x->key)  return ceiling(x->right, key);
    Node* t = ceiling(x->left, key);
    if (t != NULL) return t;
    else           return x;
}

// Return the number of keys in the symbol table strictly less than <tt>key</tt>.
int RedBlackBST::rank(double key) {
    return rank(key, root);
}

// number of keys less than key in the subtree rooted at x
int RedBlackBST::rank(double key, Node* x) {
    if (x == NULL) return 0;
    if      (key < x->key) return rank(key, x->left);
    else if (key > x->key) return 1 + size(x->left) + rank(key, x->right);
    else              return size(x->left);
}
