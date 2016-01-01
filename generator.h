/*
 * Project: VYPe 2015
 *
 * Authors: 
 *
 *
 * 
 * File: generator.h
 * Generate assembly.
 */

#ifndef GENERATOR_H
#define	GENERATOR_H

#include <map>
#include <string>
#include <typeinfo>
#include <sstream>
#include "table_symbols.h"

using namespace std;

class Stack
{
public:
    int sp;
    int fp;
    unsigned size;

    Stack(unsigned size);
    string push(unsigned i);
    string pop(unsigned i);  
};

class Generator
{
public:
    Generator();
    string run(deque<symbol*> *table);
    Stack stack;
    map<symbol*, string> address_table;
    string data;
    unsigned data_counter;
    string allocateVariables(deque<symbol*> *table);

    //	isAddressable(Instruction i);

};

#endif	/* GENERATOR_H */

