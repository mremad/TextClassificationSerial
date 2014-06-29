//
//  LinkedList.cpp
//  text2
//
//  Created by Moumen Mohamed on 6/16/14.
//  Copyright (c) 2014 Moumen Mohamed. All rights reserved.
//

#include "LinkedList.h"



void LinkedList::Destroy()
{
	while(head)
	{
		Node* old = head;
		head = head->Next();
		delete old;
	}
}

/**
 * Print the contents of the list
 */
void LinkedList::Print() {
    
    // Temp pointer
    Node *tmp = head;
    
    // No nodes
    if ( tmp == NULL ) {
        cout << "EMPTY" << endl;
        return;
    }
    
    // One node in the list
    if ( tmp->Next() == NULL ) {
        
        cout << "( " +tmp->Data()+ ", ";
        cout << tmp->Position();
        cout << ")";
        cout << " --> ";
        cout << "NULL" << endl;
    }
    else {
        // Parse and print the list
        do {
            cout << "( " +tmp->Data()+ ", ";
            cout << tmp->Position();
            cout << ")";
            cout << " --> ";
            tmp = tmp->Next();
        }
        while ( tmp != NULL );
        
        cout << "NULL" << endl;
    }
}

/**
 * Append a node to the linked list
 */
int LinkedList::Append(string data, int position) {
    
    int pos=0;
    // Create a new node
    Node* newNode = new Node();
    newNode->setNode(data,position);
    newNode->SetNext(NULL);
    
    // Create a temp pointer
    Node *tmp = head;
    
    if ( tmp != NULL ) {
        // Nodes already present in the list
        // Parse to end of list
        while ( tmp->Next() != NULL ) {
            tmp = tmp->Next();
            pos++;
        }
        
        // Point the last node to the new node
        tmp->SetNext(newNode);
        pos++;
    }
    else {
        // First node in the list
        head = newNode;
    }
    
    return pos;
}

/**
 * Delete a node from the list
 */
void LinkedList::Delete(string data) {
    
    // Create a temp pointer
    Node *tmp = head;
    
    if ( tmp == NULL )
        return;
    
    // Last node of the list
    if ( tmp->Next() == NULL ) {
        delete tmp;
        head = NULL;
    }
    else {
        // Parse thru the nodes
        Node *prev;
        do {
            if ( tmp->Data() == data ) break;
            prev = tmp;
            tmp = tmp->Next();
        } while ( tmp != NULL );
        
        // Adjust the pointers
        prev->SetNext(tmp->Next());
        
        // Delete the current node
        delete tmp;
    }
}


bool LinkedList::Exists(string data) {
    
    // Create a temp pointer
    Node *tmp = head;
    
    if ( tmp == NULL )
        return false;
    else {
        // Parse thru the nodes
        Node *prev;
        do {
            if (  tmp->Data() == data )
                return true;
            prev = tmp;
            tmp = tmp->Next();
        } while ( tmp != NULL );
       
    }
    return false;
}

int LinkedList::getPositionValue(string data) {
    
    // Create a temp pointer
    int pos=-1;
    Node *tmp = head;
    
    if ( tmp == NULL )
        return -1;
    else {
        // Parse thru the nodes
        Node *prev;
        do {
            
            if (  tmp->Data() == data )
                return tmp->Position();
            prev = tmp;
            tmp = tmp->Next();
        } while ( tmp != NULL );
        
    }
    return -1;
}

