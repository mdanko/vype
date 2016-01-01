/*
 * Project: VYPe 2015
 *
 * Authors: 
 *
 *
 * 
 * File: instruction.h
 * Classes for instructions of three address code.
 */

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>
#include "generator.h"
#include "error.h"
#include "table_symbols.h"

using namespace std;

//////////////////////////////////// label

class Label : public Instruction {
private:
	static int maxid;

public:
	string id;
	string str();
	string generate(Generator* g);

	Label(Function *f);
  Label(const string &prefix);
	Label(const Label &label);

	virtual Instruction* ptr() {
		return this;
	}
};

//////////////////////////////////// result = a op b

class ExpressionInst : public Instruction {
public:
	symbol *var1;
	symbol *var2;
	symbol *result;
	OpType op;
	string str();
	string generate(Generator* g);

	ExpressionInst* ptr() {
		return this;
	}
};

//////////////////////////////////// result = (type) a

class CastInst : public Instruction {
public:
	symbol *var;
	symbol *result;
	DataType type;
	string str();
	string generate(Generator* g);

	CastInst* ptr() {
		return this;
	}
};

//////////////////////////////////// a = value

class LoadInst : public Instruction {
public:
	symbol *result;
	string str();
	string generate(Generator* g);

	LoadInst* ptr() {
		return this;
	}
};

//////////////////////////////////// a = b;

class AssignmentInst : public Instruction {
public:
	symbol *var;
	symbol *result;
	string str();
	string generate(Generator* g);
};

//////////////////////////////////// goto a

class JumpInst : public Instruction {
public:
	Label *label;
	string str();
	string generate(Generator* g);
};

//////////////////////////////////// if not a then goto b

class JumpFalseInst : public Instruction {
public:
	symbol *cond;
	Label *label;
	string str();
	string generate(Generator* g);
};

//////////////////////////////////// a = func(b,c)

class CallInst : public Instruction {
public:
	Function *fce;
	list<symbol*> args;
	symbol *result;
	string str();
	string generate(Generator* g);

	~CallInst();
};

//////////////////////////////////// return a

class ReturnInst : public Instruction {
public:
	symbol *result;
	string str();
	string generate(Generator* g);
};

#endif
