/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/** Error Handling **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// All error messages are displayed by this routine
// This routine has NO RETURN i.e. if this function is called the
// Programm terminates with an ERROR Message
// Arguments:
//	code = Error Code (One of the ERR_ #defineS)
//  msg = a supplementry message to be displayed along with the error message
//  i = Character postion where the interpret(); function is currently
//		  processing, Set only in the process(); function
void error(int code,char *msg,int i)
{
    std::cout << "Error("<<code<<"): ";
    switch(code)
    {
    case ERR_MEMALLOC:
        std::cout << "Memory allocation error!";
        break;
    case ERR_OPEN:
        std::cout << "Unable to open file!";
        break;
    case ERR_WRITE:
        std::cout << "Unable to write to file!";
        break;
    case ERR_READ:
        std::cout << "Unable to read from file!";
        break;
    case ERR_ARGCNT:
        std::cout << "Argument count mismatch.";
        break;
    case ERR_ARGTYPE:
        std::cout << "Argument type mismatch.";
        break;
    case ERR_VARDEC:
        std::cout << "Variable not decalred.";
        break;
    case ERR_VARTYPE:
        std::cout << "Invalid 'type' for variable.";
        break;
    case ERR_FUNDEC:
        std::cout << "Function not declared.";
        break;
    case ERR_BCOUNT:
        std::cout << "Mismatched parentheses.";
        break;
    case ERR_LABELDEF:
        std::cout << "Label not defined";
        break;
    case ERR_INVALIDARG:
        std::cout << "Inavlid arguments for command line option(s).";
        break;
    case ERR_DIVBYZERO:
        std::cout << "Division by zero.";
        break;
    case ERR_STACKPOP:
        std::cout << "Stack underflow.";
        break;
    case ERR_RET:
        std::cout << "Unexpected @ret() token encountred.";
        break;
    case ERR_VARPRESENT:
        std::cout << "Variable already declared.";
        break;
    case ERR_TOKENDEF:
        std::cout << "Function not defined.";
        break;
    case ERR_MISSINGRET:
        std::cout << "Missing @ret() token in function definition.";
        break;
    case ERR_FUNPRESENT:
        std::cout << "Function already defined.";
        break;
    case ERR_INCLUDEMAX:
        std::cout << "Too many files to include";
        break;
    case ERR_P_MISSINGIF:
        std::cout << "Missing !ifdef() or !ifndef() pre-interpretation directive.";
        break;
    case ERR_P_MISSINGENDIF:
        std::cout << "!endif() without !ifdef() or !ifndef().";
        break;
    case ERR_TERMDEC:
        std::cout << "TERM not declared.";
        break;
    case ERR_TERMPRESENT:
        std::cout << "TERM already declared.";
        break;
    case ERR_MISSINGIF:
        std::cout << "encountered @endif() without a matching @if().";
        break;
    case ERR_MISSINGENDIF:
        std::cout << "encountered @if() without a matching @endif() token.";
        break;
    case ERR_MISSINGWHILE:
        std::cout << "encountered @loop() without a matching @while() token.";
        break;
    case ERR_MISSINGLOOP:
        std::cout << "encountered @while() without a matching @loop() token.";
        break;
    case ERR_MISSINGSWITCH:
        std::cout << "@endswitch() token encountered without a matching @switch().";
        break;
    case ERR_MISSINGENDSWITCH:
        std::cout << "No matching @endswitch() found for @switch() token.";
        break;
    case ERR_MISSINGCASE:
        std::cout << "No matching @case().";
        break;
    case ERR_MISSINGFOR:
        std::cout << "@next() without a matching @for().";
        break;
    case ERR_MISSINGDO:
        std::cout << "@until() or @till() token encountered without a matching @do().";
        break;
    case ERR_UNKNOWN:
        std::cout << "Unknown error encountered!!";
        break;
    default:
        std::cout << "Fatal Error!!!";
    }
    std::cout <<" - " << ((msg)?(msg):(NULL)) << std::endl;
    std::cout << "Line No. :" << cline << std::endl << ((_expr)?(_expr):(NULL)) << std::endl;
    std::cout << ((script[cline])?(script[cline]):(NULL)) << std::endl;
    std::cout << "Position :" << i << std::endl;

    // CLeanup
    unloadais();
    cleanup();

    exit(code);
}

