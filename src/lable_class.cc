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

/** Label Class */
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

Label::Label(void)
{
    root = new linklist;
    if(!root)
    {
        error(ERR_MEMALLOC,"Label::Label();");
        exit(ERR_MEMALLOC);
    }
    root->prev = NULL;
    root->next = NULL;
    curr = root;

    curr->label = NULL;
    curr->data = NULL;

    head = root;
}

void Label::add(char *s,long l,char *d)
{
    //trim(s);
    //std::cout << "Found label :" << s << "!!" << std::endl;
    //tmpi = getchar();
    head->label = new char[strlen(s)+1];
    if(!head->label)
    {   error(ERR_MEMALLOC,"Label::add();");
        exit(ERR_MEMALLOC);
    }
    strcpy(head->label,s);
    head->label = setcase(head->label);
    head->Line = l;
    if(d)
    {
        head->data = new char[strlen(d)+1];
        if(!head->data) error(ERR_MEMALLOC,"Label::add() 10");
        strcpy(head->data,d);
    }

    head->next = new linklist;
    if(!head->next)
    {
        error(ERR_MEMALLOC,"Label:add(); 2");
        exit(ERR_MEMALLOC);
    }
    head->next->prev = head;
    head = head->next;
    head->next = NULL;
    head->label = NULL;
    head->data = NULL;
}

int Label::islabel(char *n)
{
    curr = root;
    setcase(n);
    trim(n);
    while(curr->next)
    {
        //std::cout << curr->label << "," << n << std::endl;
        if(cmp(curr->label,n))
            return 1;
        curr = curr->next;
    }
    return 0;
}

int Label::isterm(char *n)
{
    return islabel(n);
}

long Label::line(char *n)
{
    if(!islabel(n))
    {
        error(ERR_LABELDEF,"Label::line();");
        exit(ERR_LABELDEF);
    }
    return curr->Line;
}

char *Label::data(char *n,char *d)
{
    if(!islabel(n)) error(ERR_LABELDEF,"Label::data()");
    strcpy(d,curr->data);
    return d;
}

void Label::deletelabel(char *n)
{
    if(!islabel(n))
        error(ERR_LABELDEF,"deletelabel();");

    linklist *temp;
    temp = curr;
    if(curr->prev)
    {
        curr = curr->prev;
        if(temp->next)  //   ...1->@->2->...    1->2->   free(@)
        {
            //std::cout << "1-@-2" << std::endl;
            curr->next = temp->next;
            temp->next->prev = curr;
            if(temp->label) delete[] temp->label;
            if(temp->data) delete[] temp->data;

            delete temp;
        }
        else			// ... 1->@->NULL		free(@)
        {
            //std::cout << "1-@-NULL" << std::endl;
            if(temp->label) delete[] curr->next->label;
            if(temp->data) delete[] curr->next->data;

            delete curr->next;
            curr->next = NULL;
            head = curr;
        }
    }
    else
    {
        if(curr->next) 	// NULL->@->2->...	NULL->2->...  free(@)
        {
            //std::cout << "NULL-@-2" << std::endl;
            curr = curr->next;
            root = curr;
            curr->prev = NULL;

            if(temp->label) delete[] temp->label;
            if(temp->data) delete[] temp->data;

            delete temp;
        }
        else			// NULL->@->NULL
        {
            //std::cout << "NULL-@-NULL" << std::endl;
            curr = root;
            // * Flag!^
            if(curr->label) delete[] curr->label;
            //if(temp->label) delete[] temp->label;
            if(curr->data) delete[] curr->data;
            curr->data = NULL;
            curr->label = NULL;
        }
    }
}

void Label::deleteterm(char *n)
{
    deletelabel(n);
}

Label::~Label()
{
    curr = NULL;
    root = NULL;
    while(head->prev)
    {
        head = head->prev;
        delete[] head->next->label;
        if(head->next->data) delete[] head->next->data;
        delete head->next;
    }
    if(head->label) delete[] head->label;
    if(head->data) delete[] head->data;
    delete head;
}
