/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

/* Main program */

int main()
{
   EvalState state;
   Program program;

   while (true)
   {
      try
      {
         processLine(getLine(), program, state);
      }
      catch (ErrorException &ex)
      {
         cout << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state)
{
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);

   string firstToken = scanner.nextToken();

   if (firstToken[0] >= '0' && firstToken[0] <= '9')
   {
      string secondToken = scanner.nextToken();
      int lineNumber = stringToInteger(firstToken);
      if (secondToken == "")
         program.removeSourceLine(lineNumber);

      else if (secondToken == "PRINT")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new PrintStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "LET")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new LetStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "REM")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new RemStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "INPUT")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new InputStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "END")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new EndStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "GOTO")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new GotoStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (secondToken == "IF")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new IfStmt(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else
         error("SYNTAX ERROR");
   }
   else
   {
      if (firstToken == "LIST")
      {
         int linenumber = program.getFirstLineNumber();
         while (linenumber != -1)
         {
            cout << program.getSourceLine(linenumber) << endl;
            linenumber = program.getNextLineNumber(linenumber);
         }
      }
      else if (firstToken == "RUN")
      {
         int linenumber = program.getFirstLineNumber();
         while (linenumber != -1)
         {
            if (program.getSourceLine(linenumber) == "")
               error("LINE NUMBER ERROR");
            state.setCurrentLine(program.getNextLineNumber(linenumber));
            program.getParsedStatement(linenumber)->execute(state);
            linenumber = state.getCurrentLine();
         }
      }
      else if (firstToken == "QUIT")
         exit(0);
      else if (firstToken == "CLEAR")
      {
         program.clear();
         state.clear();
      }
      else if (firstToken == "PRINT")
      {
         Statement *stmt = new PrintStmt(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (firstToken == "LET")
      {
         Statement *stmt = new LetStmt(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (firstToken == "INPUT")
      {
         Statement *stmt = new InputStmt(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (firstToken == "HELP")
         cout << "Stub implementation of BASIC" << endl;
      else
         error("SYNTAX ERROR");
   }

   //Expression *exp = parseExp(scanner);
   //int value = exp->eval(state);
   //cout << value << endl;
   //delete exp;
}
