#pragma once
#ifndef SDTER_INC_STACK_ELEMENT_H_
#define SDTER_INC_STACK_ELEMENT_H_

#include <string>

class StackElement {
public:
    StackElement();
    
    StackElement(int _t, std::string v);
    
    virtual ~StackElement();

    int type;
    std::string value;
    
private:
};

#endif // SDTER_INC_STACK_ELEMENT_H_