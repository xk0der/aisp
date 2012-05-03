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
 
/** Function Handling **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"


void buildpara(char *t,int n)
{
	vtype vt=____; // 4 _
	
	char *tmp = new char[strlen(t)+6];
	if(!tmp) error(ERR_MEMALLOC,"buildpara();");
	sprintf(tmp,"@%s()",t);
	
	unsigned int i=0,a;
	unsigned int c=0;
	char  **v;
	int l = flist.line(t);
	
	v = new char*[n];
	if(!v) error(ERR_MEMALLOC,"buildpara();");
	
	while(script[l][i]!='(' && i<strlen(script[l])) i++;
	i++;
	while(script[l][i]!='(' && i<strlen(script[l])) i++;	i++;
	
	a=i-1;
	while(i<strlen(script[l]))
	{
		if(script[l][i]==',' || script[l][i]==')')
		{
			*(v+c) = new char[i-a+2];
			if(!v[c]) error(ERR_MEMALLOC,tmp);
			
			scopy(script[l],v[c],a+1,i-1);
		
			c++;
			a=i;		}
		
		if(script[l][i]==')') break;
			
		i++;	}
	//c--;
	
	//std::cout << "Value of c = " << c << std::endl;
	//std::cout << "Value of n = " << n << std::endl;
	//std::cout << "script line = " << script[l] << std::endl;
	//std::cout << " t = " << t << std::endl;
	//std::cout << " narg = "<< narg <<std::endl;
	
	if(c<(unsigned int)n) error(ERR_ARGCNT,tmp);
		
	for(i=0;i<(unsigned int)n;i++)
	{
			switch(flist.argt(t,i))
			{
				case 'N': vt = _num;
					break;
				case 'S': vt = _string;
					break;
				case 'B': vt = _bool;
					break;
				case 'R': 
				case 'V': vt =_refrence;
					break;
				default:
					error(ERR_VARTYPE,tmp);
			}
				//std::cout << "Creating Variable No -  " <<  i <<" Named :" << v[i] <<std::endl;
				if(varg[i]==NULL)
					error(ERR_ARGCNT,tmp);
				var.creat(v[i],vt,varg[i],scope);
	}	
	//std::cout << "Yehan aaya" <<std::endl;
	delete[] tmp;}

char *getfunname(char *str)
{
	unsigned int i=0,j=0;
	
	while(exp[i]!='(' && i<strlen(exp)) i++;
	i++;
	while(exp[i]!='(' && i<strlen(exp)){str[j]=exp[i]; i++; j++;}
	str[j] = 0x00;
	return str;}

// Gets argument types in @fun() token!
char *getfunarg(char *str)
{
	unsigned int i=0,j=0;
	while(exp[i]!='(' && i<strlen(exp)) i++;
	i++;
	while(exp[i]!='(' && i<strlen(exp)) i++;
	i++;
	
	j=0;
	while(exp[i]!=')' && i<strlen(exp))
	{
		if(exp[i]!=',')
		{
			str[j] = exp[i];
			j++;
			if(j>MAXSIZE-2)
				error(ERR_MEMVIOLATION,"getfunarg();");		}
		i++;	}
	str[j] = 0x00;
	return str;}

int findfun(char *name, int *s, int *e)
{
	int i=0;	
	unsigned int j;
	int flag=NOTSET;
	
	setcase(name);
	
	while(i<total_lines)
	{
		if(instr(script[i],"@def")!=-1 && instr(script[i],name)!=-1)
		{ 
			flag = SET;
			j = instr(script[i],name)+strlen(name);
			while(script[i][j]!='(' && j<strlen(script[i]))
			{
				if(script[i][j]!=' ') flag = NOTSET;
				j++;
			}
			j = instr(script[i],name)-1;
			while(script[i][j]!='(' && j>=0)
			{
				if(script[i][j]!=' ') flag = NOTSET;
				j--;
			}
			 if(flag) break;
		}
		i++;	}
	
	if(flag)
	{
		*s = i;
		i++;
		flag = NOTSET;
		while(i<total_lines)
		{
			if(instr(script[i],"@def")!=-1) break;
			if(instr(script[i],"@ret")!=-1){ flag = SET; break; }
			i++;		}
		
		if(flag)
		{
			*e = i;
			return 1;		}
		else
			error(ERR_MISSINGRET,"findfun();");
		
		return 0;
	}
	else
		return 0;	
}

// Set @rev() to value returned from @ret() token
int setreturned(char *str)
{
	char *tmp;
	returned = TRUE;
	int ta,ti,tl;
	ta = stack.pop(); // a
	ti = stack.pop(); // i
	tl = stack.pop(); // cline
	if(!stack.okay()) error(ERR_STACKPOP,"@ret()");
	
	tmp = new char[strlen(EXP_VAR)+1+40];
	if(!tmp) error(ERR_MEMALLOC,"@ret()");
	sprintf(tmp,"%s%.0f",EXP_VAR,recurse-1);
	var.valuescope(tmp,INTERNAL); // exp
	var.deletenamescope(tmp,INTERNAL);
	
	if(expr) delete[] expr;
	if(rev) delete[] rev;
	expr = NULL;
	rev = NULL;
	expr = new char[strlen(val)+2];
	if(!expr) error(ERR_MEMALLOC,"setreturned();");
	strcpy(expr,val);
	
	rev = new char[strlen(str)+1];
	if(!rev) error(ERR_MEMALLOC,"setreturned();2");
	
	strcpy(rev,str);
			
	ir = ti;
	ar = ta;
	return tl;
}
