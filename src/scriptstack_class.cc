/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
/** Script Stack Class **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// Constructor
scriptStack::scriptStack(void)
{
	root = new linklist;
	if(!root)
		error(ERR_MEMALLOC,"Stack::Stack();");

	root->prev = NULL;
	root->next = NULL;
	curr = root;
	err = FALSE;
}
			
// Pushes a String on the stack
void scriptStack::push(char *d)
{
	err= FALSE;
	curr->data = new char[strlen(d)+1];
	if(!curr->data) error(ERR_MEMALLOC,"scriptStack::push();");
	strcpy(curr->data,d);
	
	curr->next = new linklist;
				
	if(!curr->next)
		error(ERR_MEMALLOC,"Stack::push();");
	
				
	curr->next->prev = curr;
	curr = curr->next;
	curr->next = NULL;
	curr->data = NULL;
}	
		
// Pops the value from the stack
char *scriptStack::pop(char *d)
{	
	err = FALSE;
	if(curr->prev)
	{
		curr = curr->prev;
		if(curr->next->data) delete[] curr->next->data;
		delete curr->next;
		curr->next = NULL;
	}
	else
	{
		err=TRUE;
		return NULL;
	}
	strcpy(d,curr->data);
	return d;
}

// Returns TRUE if the last POP operation was succesfull
int scriptStack::okay(void)
{	return !err;
}
		
// Destructor
scriptStack::~scriptStack()
{
	char *tmp=new char[255];
	if(!tmp) error(ERR_MEMALLOC,"@scriptSatck();");
		
	do
		pop(tmp);
	while(okay()) ;
	
	if(root->data) delete[] root->data;
	
	delete root;
}

