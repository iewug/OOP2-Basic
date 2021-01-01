/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/error.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement()
{
   /* Empty */
}

Statement::~Statement()
{
   /* Empty */
}

PrintStmt::PrintStmt(TokenScanner &scanner)
{
   exp = readE(scanner, 0);
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

PrintStmt::~PrintStmt()
{
   delete exp;
}

void PrintStmt::execute(EvalState &state)
{
   cout << exp->eval(state) << endl;
}

LetStmt::LetStmt(TokenScanner &scanner)
{
   var = scanner.nextToken();
   if (var == "REM" || var == "LET" || var == "PRINT" || var == "INPUT" || var == "GOTO" || var == "IF" || var == "END" || var == "RUN" || var == "LIST" || var == "CLEAR" || var == "HELP" || var == "QUIT")
      error("SYNTAX ERROR");
   if (scanner.nextToken() != "=")
      error("SYNTAX ERROR");
   exp = readE(scanner, 0);
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

LetStmt::~LetStmt()
{
   delete exp;
}

void LetStmt::execute(EvalState &state)
{
   state.setValue(var, exp->eval(state));
}

RemStmt::RemStmt(TokenScanner &scanner)
{
   while (scanner.hasMoreTokens())
   {
      remark += scanner.nextToken();
      remark += " ";
   }
}

RemStmt::~RemStmt() {}

void RemStmt::execute(EvalState &state) {}

InputStmt::InputStmt(TokenScanner &scanner)
{
   var = scanner.nextToken();
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

InputStmt::~InputStmt()
{
   delete constantexp;
}

void InputStmt::execute(EvalState &state)
{
   string str;
   int value;
   while (true)
   {
      cout << " ? ";
      getline(cin, str);
      try
      {
         value = stringToInteger(str);
      }
      catch (ErrorException &ex)
      {
         cout << "INVALID NUMBER" << endl;
         continue;
      }
      constantexp = new ConstantExp(value);
      state.setValue(var, constantexp->eval(state));
      break;
   }
}

EndStmt::EndStmt(TokenScanner &scanner)
{
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

EndStmt::~EndStmt() {}

void EndStmt::execute(EvalState &state)
{
   state.setCurrentLine(-1);
}

GotoStmt::GotoStmt(TokenScanner &scanner)
{
   string str = scanner.nextToken();
   try
   {
      nextline = stringToInteger(str);
   }
   catch (ErrorException &ex)
   {
      cout << "SYNTAX ERROR" << endl;
   }
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

GotoStmt::~GotoStmt() {}

void GotoStmt::execute(EvalState &state)
{
   state.setCurrentLine(nextline);
}

IfStmt::IfStmt(TokenScanner &scanner)
{
   lexp = readE(scanner, 1);
   op = scanner.nextToken();
   rexp = readE(scanner, 1);
   string then = scanner.nextToken();
   if (then != "THEN")
      error("SYNTAX ERROR");
   nextline = stringToInteger(scanner.nextToken());
   if (scanner.hasMoreTokens())
      error("SYNTAX ERROR");
}

IfStmt::~IfStmt()
{
   delete lexp, rexp;
}

void IfStmt::execute(EvalState &state)
{
   int lval = lexp->eval(state);
   int rval = rexp->eval(state);
   if ((op == "<" && lval < rval) || (op == "=" && lval == rval) || (op == ">" && lval > rval))
      state.setCurrentLine(nextline);
}