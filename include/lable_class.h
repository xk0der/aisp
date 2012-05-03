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
 


/** Label Class Header **/

#if !defined(_LABLE_CLASS_H)
#define _LABLE_CLASS_H

class Label
{
	struct linklist
	{
		long 	Line;
		char 	*label;
		char  	*data; //For terms
		linklist *next;
		linklist *prev;
	};
	linklist *root;
	linklist *curr;
	linklist *head;
	public:
		Label(void);
		void add(char *s,long l,char *d=NULL);
		int islabel(char *n);
		int isterm(char *n);
		long line(char *n);
		char *data(char *n,char *d);
		void deletelabel(char *n);
		void deleteterm(char *n);	
		~Label();
};

#endif
