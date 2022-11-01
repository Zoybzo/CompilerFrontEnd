#include "inc/StackElement.h"

#include <cstring>

StackElement::StackElement() {}

StackElement::StackElement(int t, std::string v) {
    type = t;
    value = v;
}

StackElement::~StackElement() {}
