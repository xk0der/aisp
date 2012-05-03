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
 
/** Stack Class Header **/

#if !defined(_STACK_CLASS_H)
#define _STACK_CLASS_H

class Stack
{
	struct linklist
	{
		long int data;
		linklist *next;
		linklist *prev;
	};
	int err;				// Error Flag
	linklist *root;
	linklist *curr;
	public:
		Stack(void);
		void push(long int  d);
		long int  pop();
		int okay(void);
		~Stack();
};
#endif



