/*
 * Project: VYPe 2015
 *
 * Authors: 
 * 
 * 
 * 
 * File: generator.cpp
 * Generate assembly.
 */

#include "generator.h"
#include <iostream>

Stack::Stack(unsigned size) {
    this->size = size;
    sp = size;
    fp = size;
}

string Stack::push(unsigned i) {
    sp -= i;

    stringstream mips;
    mips << "addi $sp,$sp,-" << i << endl;
    return mips.str();
}

string Stack::pop(unsigned i) {
    sp += i;

    stringstream mips;
    mips << "addi $sp,$sp," << i << endl;
    return mips.str();
}

Generator::Generator() : stack(8192) {
    stringstream ds;
    ds << endl << ".data" << endl;
    data = ds.str();
    data_counter = 0;
}

string Generator::run(deque<symbol*> *table) {
    stringstream mips, func, header;
    header << ".text" << endl <<
	    ".org 0" << endl <<
	    "li $sp," << stack.size << endl <<
	    "li $fp," << stack.size << endl <<
	    "jal main" << endl <<
	    "BREAK" << endl;

    for (deque<symbol*>::iterator i = table->begin(); i != table->end(); ++i) {
	if ((*i)->type == SFUNC) {
	    stack.fp = stack.size;
	    stack.sp = stack.size;

	    func_info &f = (*i)->func;
	    allocateVariables(&(f.symbols));

//	    	    for (list<Instruction*>::iterator j = f.instructions.begin(); j != f.instructions.end(); ++j) {
//	    		func << string((*j)->generate(this));
//	    
//	    		if (i->name == "main") {
//	    		    func << "jr $ra // Return" << endl;
//	    		}
//	    	    }
	}
    }

    mips << header.str() << func.str() << data << endl;
    return mips.str();
}

//Generator::isAddressable(Instruction i) {
//	if (address_table.find(i) == address_table.end()) {
//		return "";
//	}
//}

string Generator::allocateVariables(deque<symbol*> *table) {
    std::stringstream ss;

    for (deque<symbol*>::iterator v = table->begin(); v != table->end(); ++v) {
	if ((*v)->type == SID) {
	    int offset = stack.sp - stack.fp;
	    ss.str("");
	    ss << offset << "($fp)";
//	    address_table.insert(make_pair(&variable, ss.str()));
            
	    if ((*v)->dataType == DINT || (*v)->dataType == DSTRING) {
		stack.push(4); // 4B
	    } else if ((*v)->dataType == DCHAR) {
		stack.push(1); // 1B
	    }
        }
    }

    return "a";
}
