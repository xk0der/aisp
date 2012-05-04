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
 
/** Function class **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

Function::Function(void)
		{
			root = new linklist;
			if(!root)
			{
				error(ERR_MEMALLOC,"function::function();");
				exit(ERR_MEMALLOC);
			}
			root->prev = NULL;
			root->next = NULL;
			head = root;
			curr = root;
		}
		// name, start line, end line, Number of arguments, argument types string
void Function::add(char *n,long s,long e,int na,char *at)
		{
			head->start = s;
			head->end = e;
			head->argc = na;
			head->fname = new char[strlen(n)+1];
			if(!head->fname){
				error(ERR_MEMALLOC,"function::add();");
				exit(ERR_MEMALLOC);
			}	
			strcpy(head->fname,n);
			head->fname = setcase(head->fname);
			
			head->Argt = new char[head->argc+1];
			if(!head->Argt){
				error(ERR_MEMALLOC,"function::add();");
				exit(ERR_MEMALLOC);
			}	
			
			strcpy(head->Argt,at);
			head->Argt = ucase(head->Argt);
			
			head->next = new linklist;
			if(!head->next)
			{
				error(ERR_MEMALLOC,"function::function();");
				exit(ERR_MEMALLOC);
			}
			head->next->prev = head;
			head = head->next;
			head->next = NULL;
		}	
				
int Function::isfun(char *n)
		{
			curr = root;
			setcase(n);
			while(curr->next)
			{
				if(cmp(curr->fname,n))
					return	1;
				curr = curr->next;
			}
			return 0;
		}
		
long Function::line(char *n)
		{	
			if(!isfun(n))
			{	
				error(ERR_FUNDEC,"function::line();");
				exit(ERR_FUNDEC);
			}
			return curr->start;
		}
		
long Function::eline(char *n)
		{
			if(!isfun(n))
			{	
				error(ERR_FUNDEC,"function::line();");
				exit(ERR_FUNDEC);
			}
			return curr->end;
		}
		
int Function::narg(char *n)
		{
			if(!isfun(n))
			{	
				error(ERR_FUNDEC,"function::line();");
				exit(ERR_FUNDEC);
			}
			return curr->argc;
		}
		
char Function::argt(char *n,int i)
		{
			if(!isfun(n))
			{	
				error(ERR_FUNDEC,"function::argt();");
				exit(ERR_FUNDEC);
			}
			
			if(i<0 || i>curr->argc-1) return 0;
			return curr->Argt[i];			
		}
		
Function::~Function()
		{
			curr = NULL;
			root = NULL;
			while(head->prev)
			{
				head = head->prev;
				//if(head->next->fname) delete[] head->next->fname;
				//if(head->next->Argt) delete[] head->next->Argt;	
				delete head->next;
			}
			//if(head->fname) delete[] head->fname;
			//if(head->Argt) delete[] head->Argt;	
			delete head;
		}

