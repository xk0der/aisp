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
 
/** Parse @DIS() **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// Argument    : A string containing escape sequences eg: '\n'
// Description : Converts the escape sequences in the string to their
//				 Respective ASCII characters
// Returns     : Converted String
char *parsedis(char *str)
{
	int i=0,j=0;
	char ch;
	char *tmp;
	
	int l= strlen(str);
	
	tmp = new char[l+2];
	if(!tmp)
		error(ERR_MEMALLOC,"parsedis()");
	
	
	for(i=0;i<l;i++)
	{
		if(str[i]=='\\' && i+1<l)
		{			switch(str[i+1])
			{	
				case 'n':					// Newline
					ch = '\n';
					break;
				case 't':					// tab
					ch = '\t';
					break;
				case '9':					// (
					ch = '(';
					break;
				case '0':					// )
					ch = ')';
					break;
				case '2':					// @
					ch = '@';
					break;
				case '{':					// [
					ch = '[';
					break;
				case '}':					// ]
					ch = ']';
					break;
				case '\\':					// '\'	
					ch = '\\';
					break;
				case 'c':
				case '<':
					ch = ',';				// ,
					break;
				case 'a':					// BELL
					ch ='\a';
					break;
				case 'v':
					ch = '\v';				// Vertical TAB
					break;
				case 'b':
					ch = '\b';				// BACKSPACE
					break;
				default:
					ch = str[i+1];
					break;
			}
			i++;
			tmp[j] = ch;
			j++;
		}
		else
		{
			tmp[j] = str[i];
			j++;
		}
				}
	tmp[j] = 0x00;
	strcpy(str,tmp);
	
	delete[] tmp;
	
	return str;
}

// Argument    : A string containing characters which need to be escaped
// Description : Converts the characters required to be escaped to their
//				 Respective escape sequences
// Returns     : Converted String
// *IMPORTANT  : size of the string 'str' should be big enough to accomodate
//               the new expanded string!
char *anti_parsedis(char *str)
{
	unsigned int i,j;
	char *tmp;
	tmp = new char[strlen(str)*2+10];
	
	if(!tmp) error(ERR_MEMALLOC,"anti_parsedis();");
		
	j = 0;
	for(i=0;i<strlen(str);i++)
	{
		switch(str[i])
		{
			case '@': tmp[j++] = '\\';  tmp[j++] = '2';  
					 break;
			case '(': tmp[j++] = '\\';  tmp[j++] = '9';  
					 break;
			case ')': tmp[j++] = '\\';  tmp[j++] = '0';  
					 break;
			case '[': tmp[j++] = '\\';  tmp[j++] = '{';  
					 break;
			case ']': tmp[j++] = '\\';  tmp[j++] = '}';  
					 break;
			case '\n': tmp[j++] = '\\';  tmp[j++] = 'n';  
					 break;
			case '\t': tmp[j++] = '\\';  tmp[j++] = 't';
					 break;
			case ',': tmp[j++] = '\\';  tmp[j++] = '<';  
					 break;
			case '\\': tmp[j++] = '\\';  tmp[j++] = '\\';  
					 break;
			case '\b': tmp[j++] = '\\';  tmp[j++] = 'b';  
					 break;	
			case '\v': tmp[j++] = '\\';  tmp[j++] = 'v';  
					 break;
			default:
					tmp[j++] = str[i];		}	}
	tmp[j] = 0x00; 
	strcpy(str,tmp);
	delete[] tmp;
	return str;
}

