/*
 * Project: VYPe 2015
 *
 * Authors: 
 *
 *
 * 
 * File: instruction.cpp
 * Classes for instructions of three address code.
 */

#include "instruction.h"

//////////////////////////////////// Label

int Label::maxid = 0;

Label::Label(const string &prefix)
{
    std::stringstream stream;
    stream << prefix << Label::maxid++;
    this->id = stream.str();
    stream.str("");
}

Label::Label(symbol *f)
{
    std::stringstream stream;

    if (f->name != "main")
    {
        stream << "FUNC_";
    }

    stream << f->name;

    this->id = stream.str();
    stream.str("");
}

Label::Label(const Label &label)
{
    this->id = label.id;
}

string Label::str()
{
    std::stringstream stream;
    stream << "label " << id;
    return stream.str();
}

string Label::generate(Generator* g)
{
    stringstream ss;
    ss << endl << id << ":" << endl;
    return ss.str();
}

//////////////////////////////////// ExpressionInst

string ExpressionInst::str()
{
    std::stringstream stream;

    if (op == ONEG)
    {
        stream << result->name << " = " << OSTR[op] << " " << var1->name;
    }
    else
    {
        stream << result->name << " = " << var1->name << " " << OSTR[op] << " " << var2->name;
    }

    return stream.str();
}

string ExpressionInst::generate(Generator* g)
{ // result = a op b
    string rd, rs, rt;
    stringstream mips;

    rd = (g->address_table.find(result))->second;
    rs = (g->address_table.find(var1))->second;
    rt = (g->address_table.find(var2))->second;

    if (op == OPLUS)
    { // rd ← rs + rt ~ ADD rd, rs, rt
        mips << "lw $4, " << rs << " // ADD" << endl <<
                "lw $5, " << rt << endl <<
                "add $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == OMINUS)
    { // rd ← rs - rt ~ SUB rd, rs, rt
        mips << "lw $4, " << rs << " // SUB" << endl <<
                "lw $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == OMUL)
    { // rd ← rs × rt ~ MUL rd, rs, rt
        mips << "lw $4, " << rs << " // MUL" << endl <<
                "lw $5, " << rt << endl <<
                "mul $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == ODIV || op == OMOD)
    { // (HI, LO) ← rs / rt ~ DIV rs, rt
        mips << "lw $4, " << rs << " // DIV" << endl <<
                "lw $5, " << rt << endl <<
                "div $4, $5" << endl;

        if (op == ODIV)
        {
            mips << "mflo $3" << endl <<
                    "sw $3, " << rd << endl;
        }
        else if (op == OMOD)
        {
            mips << "mfhi $3" << endl <<
                    "sw $3, " << rd << endl;
        }

    }
    else if (op == OLT)
    {
        Label ltrue("LT_TRUE_");
        Label lend("LT_END_");

        mips << "lw  $4, " << rs << " // LT" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "bltz $3, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;

    }
    else if (op == OLTE)
    {
        Label ltrue("LTE_TRUE_");
        Label lend("LTE_END_");

        mips << "lw  $4, " << rs << " // LTE" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "blez $3, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;

    }
    else if (op == OGT)
    {
        Label ltrue("GT_TRUE_");
        Label lend("GT_END_");

        mips << "lw  $4, " << rs << " // GT" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "bgtz $3, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;

    }
    else if (op == OGTE)
    {
        Label ltrue("GTE_TRUE_");
        Label lend("GTE_END_");

        mips << "lw  $4, " << rs << " // EQ" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "bgez $3, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;

    }
    else if (op == OEQ)
    {

        Label ltrue("EQ_TRUE_");
        Label lend("EQ_END_");

        mips << "lw  $4, " << rs << " // EQ" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "beq $3, $10, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;
    }
    else if (op == ONEQ)
    {
        Label ltrue("NEQ_TRUE_");
        Label lend("NEQ_END_");

        mips << "lw  $4, " << rs << " // NEQ" << endl <<
                "lw  $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "bne $3, $10, " << ltrue.id << endl <<                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;

    }
    else if (op == OAND)
    {
        Label lfalse("AND_FALSE_");
        Label lend("AND_END_");

        mips << "lw  $4, " << rs << " // AND" << endl <<
                "lw  $5, " << rt << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "beq $4, $10, " << lfalse.id << endl <<
                "beq $5, $10, " << lfalse.id << endl <<                                                                
                "sw  $11, " << rd << endl <<
                "b " << lend.id << endl <<
                lfalse.id << ":" << endl <<
                "sw  $10, " << rd << endl <<
                lend.id << ":" << endl;
    }
    else if (op == OOR)
    {
        Label ltrue("OR_TRUE_");
        Label lend("OR_END_");

        mips << "lw  $4, " << rs << " // OR" << endl <<
                "lw  $5, " << rt << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "bne $4, $10, " << ltrue.id << endl <<
                "bne $5, $10, " << ltrue.id << endl <<                                                                
                "sw  $10, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $11, " << rd << endl <<
                lend.id << ":" << endl;
    }
    else if (op == ONEG)
    {
        Label ltrue("NEG_TRUE_");
        Label lend("NEG_END_");

        mips << "lw  $4, " << rs << " // OR" << endl <<
                "li  $10, 0" << endl <<
                "li  $11, 1" << endl <<
                "beq $4, $10, " << ltrue.id << endl <<
                "sw  $11, " << rd << endl <<
                "b " << lend.id << endl <<
                ltrue.id << ":" << endl <<
                "sw  $10, " << rd << endl <<
                lend.id << ":" << endl;
    }

    return mips.str();
}
//////////////////////////////////// CastInst

string CastInst::str()
{
    std::stringstream stream;
    stream << result->name << " = (" << DSTR[type] << ") " << var->name;
    return stream.str();
}

string CastInst::generate(Generator * g)
{ // result = (type) a
    stringstream mips;
    mips << "CAST" << endl;
    return mips.str();
}

//////////////////////////////////// LoadInst

string LoadInst::str()
{
    stringstream stream;
    stream << result->name << " = " << DSTR[result->dataType];
    return stream.str();
}

string LoadInst::generate(Generator * g)
{ // a = const
    stringstream data, mips;
    string address = (g->address_table.find(result))->second;

    if (result->dataType == DINT)
    {
        data << "const_" << g->data_counter << ":" <<
                "\t" << ".word " << result->var.ival << endl;

        mips << "la $7, const_" << g->data_counter << " // Load int" << endl <<
                "lw $3, ($7)" << endl <<
                "sw $3, " << address << endl;
    }
    else if (result->dataType == DCHAR)
    {
        data << "const_" << g->data_counter << ":\t" <<
                ".byte " << result->var.sval << endl;

        mips << "la $3, const_" << g->data_counter << " // Load char" << endl <<
                "lb $3, ($7)" << endl <<
                "sb $3, " << address << endl;
    }
    else if (result->dataType == DSTRING)
    {
        string sval = result->var.sval;
        sval.erase(0, 1);
        sval.erase(sval.size() - 1);

        data << "const_" << g->data_counter << ":\t" <<
                ".asciz " << "\"" << sval << "\"" << endl;

        mips << "la $3, const_" << g->data_counter << " // Load string" << endl << // Get string address
                "sw $3, " << address << endl;
    }

    g->data_counter++;

    g->data += data.str();
    return mips.str();
}

//////////////////////////////////// AssignmentInst

string AssignmentInst::str()
{
    std::stringstream stream;
    stream << result->name << " = " << var->name;
    return stream.str();
}

string AssignmentInst::generate(Generator * g)
{ // a = var
    stringstream mips;
    string res, var1;
    var1 = (g->address_table.find(var))->second;
    res = (g->address_table.find(var))->second;

    if (result->dataType == DINT || result->dataType == DSTRING)
    {
        mips << "lw $3, " << var1 << " // Assign " << DSTR[g->address_table.find(var)->first->dataType] << endl <<
                "sw $3, " << res << endl;
    }
    else if (result->dataType == DCHAR)
    {
        mips << "lb $3, " << var1 << " // Assign char" << endl <<
                "sb $3, " << res << endl;
    }

    return mips.str();
}

//////////////////////////////////// JumpInst

string JumpInst::str()
{
    std::stringstream stream;
    stream << "jump to " << label->id;
    return stream.str();
}

string JumpInst::generate(Generator * generator)
{ // goto a
    stringstream mips;
    mips << "j " << label->id << " // Jump" << endl;
    return mips.str();
}

//////////////////////////////////// JumpFalseInst

string JumpFalseInst::str()
{
    std::stringstream stream;
    stream << "if not " << cond->name << " jump to " << label->id;
    return stream.str();
}

string JumpFalseInst::generate(Generator * g)
{ // if not a then goto b
    stringstream mips;
    mips << "JFALSE " << label->id << endl;
    return mips.str();
}

//////////////////////////////////// CallInst

string CallInst::str()
{
    std::stringstream stream;

    if (result)
    {
        stream << result->name << " = ";
    }

    stream << fce->name << "(";

    bool first = true;
    for (deque<symbol*>::iterator i = args.begin(); i != args.end(); ++i)
    {

        if (!first)
        {
            stream << ", ";
        }

        stream << (*i)->name;
        first = false;
    }

    stream << ")";
    return stream.str();
}

string CallInst::generate(Generator * g)
{ // a = func(b,c)
    stringstream mips;

    if (fce->name == "print")
    {
        for (deque<symbol*>::iterator i = args.begin(); i != args.end(); ++i)
        {
            symbol* var = *i;
            string address = g->address_table.find(var)->second;

            if (var->dataType == DINT)
            {
                mips << "lw $9, " << address << "" << endl <<
                        "print_int $9" << endl;
            }
            else if (var->dataType == DCHAR)
            {
                mips << "lb $9, " << address << "" << endl <<
                        "print_char $9" << endl;
            }
            else if (var->dataType == DSTRING)
            {
                mips << "lw $9, " << address << "" << endl <<
                        "print_string $9" << endl;
            }
        }
    }
    else if (fce->name == "main")
    {
        mips << g->stack.push(4); // Allocate 4B for $ra
        mips << "sw $ra, ($fp)" << endl; // Store $ra to stack
    }
    else
    {
        // Jump s tím, že musím uložit na zásobník SP a FP a takové ty srandy.
        mips << "jal " << fce->name << " // Call" << endl;
        mips << g->stack.push(4); // Allocate 4B for $ra
        mips << "sw $ra, ($sp)" << endl; // Store $ra to stack
    }

    return mips.str();
}

CallInst::~CallInst()
{
    args.clear();
}

//////////////////////////////////// ReturnInst

string ReturnInst::str()
{
    std::stringstream stream;
    stream << "return";

    if (result != NULL)
    {
        stream << " " << result->name;
    }

    return stream.str();
}

string ReturnInst::generate(Generator * g)
{ // return a
    stringstream mips;
    mips << "jr $ra // Return" << endl;
    return mips.str();
}

/* end of file */
