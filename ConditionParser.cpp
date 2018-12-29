#include <iostream>
#include "ConditionParser.h"
using namespace std;

ConditionParser::ConditionParser(Parser* newParser, Interpreter* newInter)
{
    parser = newParser;
    inter = newInter;
}


/**
 * Method extracts the condition and brackets content from a condition block and runs them
 * @param it
 */
void ConditionParser::execute(vector<string>::iterator &it)
{
    vector<string> condition;
    vector<string> bracketsContent;
    vector<string> newVec;
    string newLine;
    int bracketsCounter = 0; // This variable is used for the function to know when it reached the end of the condition block


    it++;
    // We get the condition from the code
    while (*it != "{")
    {
        condition.push_back(*it);
        it++;
    }
    bracketsCounter++;
    condition.push_back("\n"); // We add \n in the end of the condition to mark it's ending

    it++;
    if(*it != "\n") {
        throw RunTimeException("Error: statement after opening bracket");
    }

    it++;
    // We get the brackets content from the code block until we reach it's specifc colsing braket
    while (!((*it == "}") &&  (bracketsCounter == 1)))
    {
        // If we see an opening bracket we increase the counter by one
        if (*it == "{")
        {
            bracketsCounter++;
        }

        // If we see an opening bracket we decrease the counter by one
        if (*it == "}")
        {
            bracketsCounter--;
        }

        bracketsContent.push_back(*it);
        it++;
    }

    this->condition = condition;
    this->backupCondition = condition;
    this->bracketsContent = bracketsContent;
}


/**
 * Method checks if the condition of the code block is satisfied
 * @param it
 */
bool ConditionParser::conditionSatisfied()
{
    condition = backupCondition;
    vector<string>::iterator it = condition.begin();

    // We get a boolean expression which represent the if the condition is true or false
    Expression *boolValue =  this->parser->getNumericExpression(it);
    if (boolValue != NULL)
    {
        double conditionVal = boolValue->calculate();
        delete boolValue;

        if (conditionVal != 0) {
            return true;

        } else {
            return false;
        }
    }

    else
    {
        throw RunTimeException("Error: invalid conditional expression");
    }
}


/**
 * Method runs the code block inside the brackets
 * @param it
 */
void ConditionParser::runContent()
{
    inter->interperate(bracketsContent);
}


