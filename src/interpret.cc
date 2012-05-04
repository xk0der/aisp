/*
 *  AISP - Amitz Interpreted Script Processor
 *  (c) 2001-2012, Amit Singh
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, visit: http://www.gnu.org/licenses/
 */
 
/** Interpret **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

int case_in_use = 0;  	// 0 = Not Case sensitive , 1 = Case Sensitive 	
int emit = FALSE;
char	*exp;			// Current expression

void interpret()
{
	cline = begin; 		// Initialize cline
	
	int i, done=0, l;		
	int a,b;
	char t[255];		// Token
	char *arg;
	int atrflag = NOTSET;
	
	int noatr = 0;
		
	Stack atr,lb,lsb;			// Stack for '@' and '(' symbols
							// lsb = LEFT SQUARE Bracket '['
	while(!done)
	{
		if(!returned)	// Have we returned from a function or a sub routine?
		{	
			l = strlen(script[cline]);
			exp = new char[l+1];
			if(!exp)
			error(ERR_MEMALLOC,"interpret(); 1");
		
			strcpy(exp,script[cline]);
		}
		else
		{
			l = strlen(expr);
			exp = new char[l+1];
			if(!exp)
				error(ERR_MEMALLOC,"interpre(); 0");
			strcpy(exp,expr);
			i = ir;
			a = ar;
			i = rexp(rev,i,a);
			l = strlen(exp);
		}
		returned = FALSE;
		
		for(i=0;i<l;i++)
		{
			if(exp[i]=='@'){ atr.push(i); atrflag = SET;} // Save position of '@' if found
			if(exp[i]=='(')	// Save position of '(' if found
			{ 
				lb.push(i);
				if(!atrflag)	// If '(' was found without a matching '@'
				{
					atr.push(noatr);	// Store a dummy postion for '@'
					noatr = i+1;		// Calculate new dummy position
				}
				atrflag = NOTSET;
			}
			
			if(exp[i]=='[') lsb.push(i);
			if(exp[i]==']') 
			{
				
				a = b = lsb.pop();
				if(!lsb.okay())error(ERR_BCOUNT,"[]");
					
				arg =  new char[(i- (b+1))+2 ];
				if(!arg)		error(ERR_MEMALLOC,"interpret([])");
					
				if(b+1==i) strcpy(arg," "); 	// If there isn't any space between '(' & ')' copy a SPACE
				else scopy(exp,arg,b+1,i-1);	// else the arguments
				
				stemp3(MAXSIZE);
				strcpy(temp3,arg);
				evaluate(temp3);
				i = rexp(temp3,i,a);
				
				l = strlen(exp);					// Reset the lenght of the 'exp'	
				
				if(i>=l) break; // Break out of for
				if(instr(exp,"@")==-1 && instr(exp,"(")==-1 && instr(exp,")")==-1){ i=l; break;	}
				
				delete[] arg;
				continue;			}
			
			if(exp[i]==')')	// If ')' encountered
			{				
				a = atr.pop();	// Retreive position of last '@'
				b = lb.pop();		// and '('
						
				if( !atr.okay() || !lb.okay())	// Check if the pop operations were successfull
				{
					if(!atr.okay() && lb.okay()) continue;		// Handle error condition and hope that it rectifies the problem
							
					error(ERR_BCOUNT,"interpret()");
					exit(ERR_BCOUNT);
				}
				scopy(exp,t,a+1,b-1);			// Extract the token
				arg = new char[(i- (b+1))+2 ]; 	// +2 to account for function having no arguments
				if(!arg)		error(ERR_MEMALLOC,"interpret()");

				// Get arguments
				if(b+1==i) strcpy(arg," "); 	// If there isn't any space between '(' & ')' copy a SPACE
				else scopy(exp,arg,b+1,i-1);	// else the arguments
				
				setcase(t);					// Set proper case of the token
				
				i = process(t, arg,i,a);			// Process the token
				l = strlen(exp);					// Reset the lenght of the 'exp'	
				
				delete[] arg;
				
				if(i>=l) break; // Break out of for
				if(instr(exp,"@")==-1 && instr(exp,"(")==-1 && instr(exp,")")==-1){ i=l; break;	}
			}
		}
				
		do
			lb.pop();	// Empty left bracket  '(' stack
		while(lb.okay());
		
		do
			atr.pop();	// Empty '@' symbol stack
		while(atr.okay());
		
		do
			lsb.pop();	// Empty '[' symbol stack
		while(lsb.okay());
		
		delete[] exp;
		cline++;		// Proceed to next line
		if(cline>=total_lines) done = 1;		// End script if we've reached its end
	}
	
	// Report any missing @endif()'s in the script
	if(script_inif>0) error(ERR_MISSINGENDIF,"interpret();");
}
