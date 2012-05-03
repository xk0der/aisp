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
 
/** String Handling Functions **/

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// Trims the leading whitespaces from a string
// Arguments : str = String to to be LTRIMmed
// Returns pointer to the trimmed string
char *ltrim(char *str)
{
     int i,l = strlen(str);

     while(str[0]==' ' || str[0]=='\t')
     {
         for(i=1;i<l;i++)
               str[i-1]=str[i];
         l--; str[l] = 0x00;
     }
     return str;
}

// Trims the trailing whitespaces from a string
// Arguments : str = String to to be RTRIMmed
// Returns pointer to the trimmed string
char *rtrim(char *str)
{
     int i,l=strlen(str);
     i = l-1;
     while(str[i]==' ' ||str[i]=='\t') i--;
     str[i+1]=0x00;
     return str;
}

// Trim the leading and trailing whitespaces from a string
// Arguments : str = String to to be TRIMmed
// Returns pointer to the trimmed string
char *trim(char *str)
{
     	str = rtrim(ltrim(str));
     	return str;
}

// Finds s2 in s1
// Arguments :
// s1 = String in which the searched will be performed
// s2 = String to be searched
// startfrom = Starting postion of the search, DEFAULT = 0
// Returns : 
//	STARTING POSITION where the string was found
// -1 if Unsuccesfull		
int instr(char *s1,char *s2,int startfrom)
{
    int j,i=0,l1 = strlen(s1), l2 = strlen(s2),flag;
    if(l2>l1) return -1;

    for(i=startfrom;i<=(l1-l2);i++)
    {
          flag = SET;
          for(j=0;j<l2;j++)
          {
              if(s1[i+j]!=s2[j]) flag = NOTSET;
          }
          if(flag) return i;
    }
    return -1;
}

// Finds s2 in s1 BUT starts searching from the RIGHT
// Arguments :
// s1 = String in which the searched will be performed
// s2 = String to be searched
// Returns : 
//	STARTING POSITION where the string was found
// -1 if Unsuccesfull		
int rinstr(char *s1,char *s2)
{
    int j,i=0,l1 = strlen(s1), l2 = strlen(s2),flag;
    if(l2>l1) return -1;

    for(i=(l1-l2);i>=0;i--)
    {
          flag = SET;
          for(j=0;j<l2;j++)
          {
              if(s1[i+j]!=s2[j]) flag = NOTSET;
          }
          if(flag) return i;
    }
    return -1;
}

// Copy from s1 to s2, start pos l to end pos r
// Arguments :
// s1 = String to copy form
// s2 = String to copy to
// l = Starting position in 's1' (LEFT)
// r = End position in 's1' (RIGHT)
void scopy(char *s1,char *s2, int l, int r)
{
	int i,j;
	for(i=l,j=0;i<=r;i++,j++)
		s2[j]=s1[i];
	s2[j]=0x00;
}

// Converts string to upper case
// Arguments : str = String whose case will be changed
// Returns pointer to the UPPER CASE String
char *ucase(char *str)
{
	int l = strlen(str),i;
	for(i=0;i<l;i++) str[i] = toupper(str[i]);
	return str;
}

// Converst string to upper case
// Arguments : str = String whose case will be changed
// Returns pointer to the Lower Case String
char *lcase(char *str)
{
	int l = strlen(str), i;
	for(i=0;i<l;i++) str[i] = tolower(str[i]);
	return str;	
}

// Compares two strings and return TRUE if they are identical
int cmp(char *str1,char *str2)
{
	if(strcmp(str1,str2)==0) return TRUE;
	return FALSE;	
}

// Set the case of the string passed on the basis of the value of
// the global varibale 'case_in_use'
// Used to set the case of TOKENS in the script file loaded
char *setcase(char *t)
{
	 if(case_in_use==0)
		t = lcase(t);
	 return t;
}

// Compares a & b
// Handles numeric values correctly
// Returns -1 if a<b
//			  0 if a==b
//			 +1 if a>b
int cmpx(char *a,char *b)
{
	unsigned int i;
	double ai,bi;
	int flaga = NOTSET;
	int flagb = NOTSET;
	// check if both the strings are numeric values!
	for(i=0;i<strlen(a);i++)
		if((a[i]<'0' || a[i]>'9') && a[i]!='.'&& a[i]!=' ' && a[i]!='\t' && a[i]!='-' && a[i]!='+') flaga = SET; // Non-numeric 

	for(i=0;i<strlen(b);i++)
		if((b[i]<'0' || b[i]>'9' ) && b[i]!='.' && b[i]!=' ' && b[i]!='\t' && a[i]!='-' && a[i]!='+') flagb = SET;

	if(!flaga && !flagb) // They are numeric values
	{
		ai = strtod(a,NULL);
		bi = strtod(b,NULL);
	
		if(ai<bi) return -1;
		else if(ai>bi) return 1;
		else return 0;		}
	// They are string values
	
	ai = strcmp(a,b);
	if(ai<0) return -1;
	else if(ai>0) return 1;
	else return 0;	
}
