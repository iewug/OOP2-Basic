/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() = default;

Program::~Program()
{
   map<int, Statement *>::iterator it;
   for (it = parsedStatement.begin(); it != parsedStatement.end(); it++)
      delete it->second;
}

void Program::clear()
{
   map<int, Statement *>::iterator it;
   for (it = parsedStatement.begin(); it != parsedStatement.end(); it++)
      delete it->second;
   sourseline.clear();
   parsedStatement.clear();
}

void Program::addSourceLine(int lineNumber, string line)
{
   if (sourseline.count(lineNumber) == 1)
      removeSourceLine(lineNumber);
   sourseline.insert(pair<int, string>(lineNumber, line));
}

void Program::removeSourceLine(int lineNumber)
{
   if (sourseline.count(lineNumber) == 0)
      return;
   delete parsedStatement[lineNumber];
   parsedStatement.erase(lineNumber);
   sourseline.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber)
{
   if (sourseline.count(lineNumber) == 1)
      return sourseline[lineNumber];
   return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt)
{
   if (sourseline.count(lineNumber) == 1)
   {
      if (parsedStatement.count(lineNumber) == 1)
      {
         delete parsedStatement[lineNumber];
         parsedStatement[lineNumber] = stmt;
      }
      else
         parsedStatement.insert(pair<int, Statement *>(lineNumber, stmt));
   }
   else
      error("ERROR");
}

Statement *Program::getParsedStatement(int lineNumber)
{
   if (parsedStatement.count(lineNumber) == 1)
      return parsedStatement[lineNumber];
   return NULL;
}

int Program::getFirstLineNumber()
{
   if (sourseline.size() == 0)
      return -1;
   return sourseline.begin()->first;
}

int Program::getNextLineNumber(int lineNumber)
{
   map<int, string>::iterator it = sourseline.find(lineNumber);
   it++;
   if (it == sourseline.end())
      return -1;
   return it->first;
}