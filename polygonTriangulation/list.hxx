#include "list.h"
#include <cstddef>
#include <iostream>



template <typename T>
Node<T>::Node() {
    prev = NULL;
    next = NULL;
}

template <typename T>
Node<T>::Node( T _data ){
    prev = NULL;
    next = NULL;
    data = _data;
}



template <typename T>
List<T>::List() {
    head = NULL;
    tail = NULL;
}

template <typename T>
void List<T>::push_back(T _data){
    if( head == NULL ){
        head = new Node<T>( _data );
        tail = head;
    } else {
        Node<T>* aux = new Node<T>( _data );
        aux->prev = tail;
        tail->next = aux;
        tail = tail->next;
    }
}

template <typename T>
void List<T>::push_front(T _data){
    if( head == NULL ){
        head = new Node<T>( _data );
        tail = head;
    } else {
        Node<T>* aux = new Node<T>( _data );
        aux->next = head;
        head->prev = aux;
        head = head->prev;
    }
}


template <typename T>
void List<T>::print() {
    Node<T>* curr = head;
    std::cout << "-----------list----------" << std::endl;
    while( curr != NULL ){
        std::cout << curr->data << " ";
        curr = curr->next;
    }
    std::cout << std::endl << "----------------------" << std::endl;
}
