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

/* For Class */
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"


For::For(void)
{
    root = new linklist;
    if(!root)
        error(ERR_MEMALLOC,"For::For();");

    root->prev = NULL;
    root->next = NULL;
    root->name = NULL;
    curr = root;
    err = FALSE;
}
// Name of var,Initial,Final,Step
void For::push(char *n,double i,double f,double s)
{
    err= FALSE;
    curr->name = new char[strlen(n)+1];
    if(!curr->name) error(ERR_MEMALLOC,"For::push();");
    curr->init = i;
    curr->final = f;
    curr->step = s;
    strcpy(curr->name,n);

    curr->next = new linklist;

    if(!curr->next)
        error(ERR_MEMALLOC,"For::push();");


    curr->next->prev = curr;
    curr = curr->next;
    curr->next = NULL;
    curr->name = NULL;
}

double For::pop(char *n,double &f, double &s)
{
    err = FALSE;
    if(curr->prev)
    {
        curr = curr->prev;
        if(curr->next->name)
        {
            delete[] curr->next->name;
            curr->next->name = NULL;
        }
        delete curr->next;
        curr->next = NULL;
    }
    else
    {
        err=TRUE;
        return -255;
    }
    if(n) strcpy(n,curr->name);
    f = curr->final;
    s = curr->step;
    return curr->init;
}

int For::okay(void)
{   return !err;
}

For::~For()
{
    double tmp;
    do
        pop(NULL,tmp,tmp);
    while(okay()) ;

    if(root->name) delete[] root->name;

    delete root;
}

