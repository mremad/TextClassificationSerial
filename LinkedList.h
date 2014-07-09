//
//  LinkedList.h
//  text2
//
//  Created by Moumen Mohamed on 6/16/14.
//  Copyright (c) 2014 Moumen Mohamed. All rights reserved.
//

#ifndef __text2__LinkedList__
#define __text2__LinkedList__

#include <iostream>

#endif /* defined(__text2__LinkedList__) */

#include <string>
using namespace std;


// Node class
class Node {
    string data;
    int position;
    Node* next;
    
public:
    Node() {};
    void setNode(string aData, int aPos) { data = aData; position=aPos; };
    void SetNext(Node* aNext) { next = aNext; };
    string Data() { return data; };
    int Position(){return position;};
    Node* Next() { return next; };
};

// List class
class LinkedList {
    Node *head;
public:
    LinkedList() { head = NULL; };
	void Destroy();
    void Print();
    int Append(string data, int position);
    void Delete(string data);
    bool Exists(string data);
    int getPositionValue(string data);
};