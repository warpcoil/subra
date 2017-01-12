#ifndef VECTOR_H
#define VECTOR_H

#include "stddef.h"
#include "oslib/types.h"
#include "stdint.h"
#include "oslib/error.h"
#include "oslib/hw-func.h"

/* This version of Vector differs immensely from the std::vector implementation */

/* Firstly [] and at() perform the same */
/* Secondly [] is a read operation and not assignment */
/* Vector only stores pointers/references of T! and not objects */
/* Stack like behaviour, unified */

namespace warp {

template <class T> class Vector {
private:

    typedef struct Element {
        T * el;
        Element * next;
    } Element; //Linked list

    Element * elements;
    size_t nElements;

public:
    //Warning no constructor that takes nElements to reserve as argument, it if fully dynamic
    Vector() { //Default constructor

        elements = 0;
        nElements = 0;

    }

    ~Vector() { //Default destructor

        while (elements->next != NULL) {
            Element * el = elements;
            elements = elements->next;
            delete el;
        }

    }

    size_t size() { //Return size (nElements)

        return nElements;

    }

    bool empty() { //Returns whether empty

        if (nElements > 0)
            return false; //Not Empty

        return true; //Is Empty

    }

    T * operator [] (size_t elementId) const { //Read, identical to at() -- non conformant

        return at(elementId);

    }

    T * at (size_t elementId) { //alias for [], both behave identically or NULL if not found

        if (elementId > (size() - 1))
        {
            message("Vector only has %i elements: Overflow", (int)size());
            halt();
        }

        size_t e = 0;

        while (elements->next != NULL) {
            if (e == elementId) {
                return elements->el;
            }
            e++;
        }

        message("Overflow has occurred in a place where it cannot occur");
        halt();

        return NULL;

    }

    void push(T * element) { //Push onto stack like vector [add reference]

        Element * newElement = new Element();

        newElement->el = element;
        newElement->next = elements;
        nElements++;

        //Push everything down
        elements = newElement;

    }

    T * pop() { //Pop off stack like vector [and delete reference]

        if (size() == 0)
            return NULL;

        Element * existingElement = elements;
        T * el = existingElement->el;
        elements = existingElement->next;

        delete existingElement;
        nElements--;

        return el;

    }

    T * front() { //Return beginning of stack like vector [no delete]

        return elements->el;

    }

    T * back() { //Return end of stack like vector [no end]

        if (size() == 0)
            return NULL;

        T * el = elements;

        while (el->next != NULL)
            el = el->next;

        return el->el;

    }

    bool Test() {
        return true;
    }
};

}


#endif
