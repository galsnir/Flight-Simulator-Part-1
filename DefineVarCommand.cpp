#include <iostream>
#include "DefineVarCommand.h"

DefineVarCommand::DefineVarCommand(Parser* parser)
{
    this->parser = parser;
}

/**
 * Method add a vriable to the symbol table and assigns a value to him
 * @param it
 */
void DefineVarCommand::execute(vector<string>::iterator &it) {
    Expression* varExp;

    it++;
    string varName = *it; // we get the var name from the code
    it++;

    // if the variable has not value to assign in the code we will assign a defultive 0
    if (*it == "\n")
    {
        this->parser->addVar(varName,0);
    }

    else if (*it == "=")
    {
        it++;

        if(*it == "bind") // We check if it is a bind assigning or regular assigning
        {
            it++;
            takeOffQuotes(it);
            if (this->parser->variableHavePath(*it))
            {
                string path = parser->getPathByVariable(*it);
                this->parser->assignPath(varName, path);
            }

            else if(parser->checkValidityOfPath(*it))
            {
                this->parser->addVar(varName, 0);
                this->parser->addPath(varName,*it);
            }

            else
            {
                throw RunTimeException("Error defining new value: path is invalid");
            }

        }

        else
        {
            varExp = parser->getNumericExpression(it);
            if(varExp != NULL)
            {
                // We update the symbol table with the new var value
                parser->addVar(varName, varExp->calculate());
                delete varExp;
            }
            else
            {
                throw RunTimeException("Trying to assign not numeric value");
            }
        }
    }
    else
    {
        throw RunTimeException("Error: invalid syntax of defining new value");
    }

    it++;
}

void DefineVarCommand :: takeOffQuotes(vector<string>::iterator &it) {
    unsigned long length = it.operator->()->length();

    if(length >= 2 && it.operator->()->at(0) == '\"' && it.operator->()->at(length - 1) == '\"') {
        it.operator->()->erase(0,1);
        it.operator->()->erase(it.operator->()->length() - 1, 1);
    }
}