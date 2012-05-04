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
 

/** For Class Header **/

#if !defined(_FOR_CLASS_H)
#define _FOR_CLASS_H

class For
{
	struct linklist
	{
		double init;
		double final;
		double step;
		char *name;
		linklist *next;
		linklist *prev;
	};
	int err;				// Error Flag
	linklist *root;
	linklist *curr;
	public:
		For(void);
		void push(char *n,double i,double f,double s);
		double pop(char *n,double &f, double &s);
		int okay(void);
		~For();
};

#endif
