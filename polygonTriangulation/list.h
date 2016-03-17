#ifndef LIST_H
#define LIST_H

template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node();
    Node( T _data );
};

template <typename T>
class List {
public:
    Node<T>* head;
    Node<T>* tail;

    List();
    void push_back(T _data);
    void push_front(T _data);
    void print();

};


#endif
