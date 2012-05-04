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



/** Function class Header**/

#if !defined(_FUNCTION_CLASS_H)
#define _FUNCTION_CLASS_H

class Function
{
    struct linklist
    {
        char *fname;		// Function name
        long start;			// Starting Line
        long end;			// End Line
        int argc;			// Number of arguments
        char *Argt;		// 'Type' of arguments
        linklist *next;
        linklist *prev;
    };
    linklist *root;
    linklist *head;
    linklist *curr;
public:
    Function(void);
    void add(char *n,long s,long e,int na,char *at);
    int isfun(char *n);
    long line(char *n);
    long eline(char *n);
    int narg(char *n);
    char argt(char *n,int i);
    ~Function();
};
#endif
