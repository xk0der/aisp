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
 
/** Stack Class **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"


// Constructor
Stack::Stack(void)
{
	root = new linklist;
	if(!root)
		error(ERR_MEMALLOC,"Stack::Stack();");

	root->prev = NULL;
	root->next = NULL;
	curr = root;
	err = FALSE;
}

// Pushes a 'long int' value on the stack
void Stack::push(long int d)
{
	err= FALSE;
	curr->data = d;
	
	curr->next = new linklist;
				
	if(!curr->next)
		error(ERR_MEMALLOC,"Stack::push();");
	
				
	curr->next->prev = curr;
	curr = curr->next;
	curr->next = NULL;
}	

// Pops the value from the stack
long int Stack::pop()
{	
	err = FALSE;
	if(curr->prev)
	{
		curr = curr->prev;
		//if(curr->next->data) delete[] curr->next->data;
		delete curr->next;
		curr->next = NULL;
	}
	else
	{
		err=TRUE;
		return -255;
	}
	return curr->data;
}
		
// Returns TRUE if the last POP operation was succesfull
int Stack::okay(void)
{	return !err;
}

// Destructor
Stack::~Stack()
{
	do
		pop();		// Empty Stack
	while(okay()) ;
	delete root;
}

