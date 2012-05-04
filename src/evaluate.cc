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
 
/** Evaluate Expression **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

int isoper(char *);
int preceeds(char *,char *);
void postfix(char *str);

char *oper[]={"(",")","!","^","*","/","\\","%","+","-","~","<",">","<=",">=","==",
	"||","&&","++","--","=",
	"@@"};

char **A;
char **B;
int count;
	
scriptStack evalstack;

	
char *evaluate(char *str)
{
	int i;
	char tmp1[MAXSIZE];
	char tmp2[MAXSIZE];
	double a;
	double b;
	double res=0;
	int bol = FALSE;
	
	//std::cout << "Str1 = " << str << std::endl;
	
	postfix(str);
	
	for(i=0;i<count-1;i++)
	{
		bol = FALSE;
		if(isoper(B[i]))
		{
			if(cmp(B[i],"++") || cmp(B[i],"--") || cmp(B[i],"!")  || cmp(B[i],"-") )
			{
				evalstack.pop(tmp1);
				
				if(var.varinscope(tmp1,scope))
				{	
					var.value(tmp1);
					trim(val);
					if(cmp(val,"true")) b = 1;
					else if(cmp(val,"false")) b = 0;
					else b = strtod(val,NULL);
				}
				else
				{
					trim(tmp1);
					if(cmp(tmp1,"true")) b = 1;
					else if(cmp(tmp1,"false")) b = 0;
					else b = strtod(tmp1,NULL);
				}
				
				if(cmp(B[i],"++"))
				{
					res = (b+1);
					if(var.varinscope(tmp1,scope))
					{
						sprintf(tmp2,"%f",res);
						var.set(tmp1,tmp2);					}				}
				
				if(cmp(B[i],"--"))
				{
					res = (b-1);
					if(var.varinscope(tmp1,scope))
					{
						sprintf(tmp2,"%f",res);
						var.set(tmp1,tmp2);					}				}
				
				if(cmp(B[i],"!"))
				{
					bol = TRUE;
					res = (!b);
				}
				
				if(cmp(B[i],"-"))
				{
					res = (-b);
					evalstack.pop(tmp2);
					
					if(evalstack.okay() && !isoper(tmp2))
					{
						if(var.varinscope(tmp2,scope))
						{	
							var.value(tmp2);
							trim(val);
							if(cmp(val,"true")) a = 1;
							else if(cmp(val,"false")) a = 0;
							else a = strtod(val,NULL);
						}
						else
						{
							trim(tmp2);
							if(cmp(tmp2,"true")) a = 1;
							else if(cmp(tmp2,"false")) a = 0;
							else a = strtod(tmp2,NULL);
						}
						res = a - b;					}
					else
					{
						if(evalstack.okay())
							evalstack.push(tmp2);					}
					
				}
				
			}
			else
			{
				evalstack.pop(tmp1);
				evalstack.pop(tmp2);
				
				if(var.varinscope(tmp1,scope))
				{	
					var.value(tmp1);
					trim(val);
					if(cmp(val,"true")) b = 1;
					else if(cmp(val,"false")) b = 0;
					else b = strtod(val,NULL);
				}
				else
				{
					trim(tmp1);
					if(cmp(tmp1,"true")) b = 1;
					else if(cmp(tmp1,"false")) b = 0;
					else b = strtod(tmp1,NULL);
				}
				
				if(var.varinscope(tmp2,scope))
				{	
					var.value(tmp2);
					trim(val);
					if(cmp(val,"true")) a = 1;
					else if(cmp(val,"false")) a = 0;
					else a = strtod(val,NULL);
				}
				else
				{
					trim(tmp2);
					if(cmp(tmp2,"true")) a = 1;
					else if(cmp(tmp2,"false")) a = 0;
					else a = strtod(tmp2,NULL);
				}
					
				if(cmp(B[i],"+"))
				{
					res = a + b;				}
				
				if(cmp(B[i],"~"))
				{
					if(a<b)
					res = b - a; 	
					else
					res = a - b;				}
				
				if(cmp(B[i],"*"))
				{
					res =  a * b;				}
				
				if(cmp(B[i],"/"))
				{
					res = a /  b;				}
				
				if(cmp(B[i],"="))
				{
					if(var.varinscope(tmp2,scope) && var.type(tmp2)==_bool)
					{
						trim(tmp1);
						ucase(tmp1);
						a = strtod(tmp1,NULL);
						if(!cmp(tmp1,"TRUE") && !cmp(tmp1,"FALSE"))
						{
							if(a==0) strcpy(tmp1,"FALSE");
							else strcpy(tmp1,"TRUE");							}
						var.set(tmp2,tmp1);	
					}	
					else if(var.varinscope(tmp2,scope) && var.type(tmp2)==_string)
					{
						var.set(tmp2,tmp1);						}
					else if(var.varinscope(tmp2,scope) && var.type(tmp2)==_num)
					{	
						sprintf(tmp1,"%f",b);
						var.set(tmp2,tmp1);
						res = b;
					}	
					else
					{	
						error(ERR_LVALUE,"evaluate(=);");					}
				}
				
				if(cmp(B[i],"%"))
				{
					res = ((int)a % (int)b);				}
				
				if(cmp(B[i],"^"))
				{
					res = a;
					for(int t=1;t<b;t++)
						res*=a;
				}
				
				if(cmp(B[i],"\\"))
				{
					res = (int)(a / b);
				}
				
				if(cmp(B[i],"<"))
				{
					bol = TRUE;
					res = a <  b;				}
				
				if(cmp(B[i],">"))
				{
					bol = TRUE;
					res = a >  b;				}
				
				if(cmp(B[i],"<="))
				{
					bol = TRUE;
					res = a <=  b;				}
				
				if(cmp(B[i],">="))
				{
					bol = TRUE;
					res = a >=  b;				}
				
				if(cmp(B[i],"=="))
				{
					bol = TRUE;
					res = (a ==  b);				}
				
				if(cmp(B[i],"&&"))
				{
					bol = TRUE;
					res = (a &&  b);				}
				
				if(cmp(B[i],"||"))
				{
					bol = TRUE;
					res = (a ||  b);				}
								
			}
			
			if(bol)
			{
				if(res) strcpy(tmp1,"TRUE");
					else strcpy(tmp1,"FALSE");			}
			else
					sprintf(tmp1,"%f",res);
			
			evalstack.push(tmp1);		}
		else
		{
			evalstack.push(B[i]);		}	}
	
	strcpy(tmp2,tmp1);
	evalstack.pop(tmp1);
	if(cmp(tmp1,"@@"))
	{	
		//sprintf(tmp1,"%f",res);
		//std::cout << res << std::endl;
		strcpy(tmp1,tmp2);	}
	strcpy(str,tmp1);
	//std::cout << "str2 = "<<str<< ", res ="<<res << std::endl;
	
	return str;}

int isoper(char *op)
{
	int i=0,yes=FALSE;
	while(!cmp(oper[i],"@@")){if(cmp(oper[i],op)) {yes=TRUE; break;} i++;}
	return yes;	}

int preceeds(char *op1,char *op2)
{
	int i,j=0,yes=FALSE;
	if(isoper(op1) && isoper(op2))
	{
		i=0;
		while(!cmp(oper[i],"@@")){if(cmp(oper[i],op1) ) {j=i; break;} i++;}
		i=0;
		while(!cmp(oper[i],"@@")){if(cmp(oper[i],op2) ) { break;} i++;}	
		if(j<=i)	yes = TRUE;	}
	else
		yes = -1;
	
	return yes;}

void postfix(char *str)
{
	int i,l,last,j,t;
	char tmp[MAXSIZE];
	count =0;
	
	l = strlen(str);
	
	//std::cout << "str = " << str << std::endl << std::endl;
	
	for(i=0;i<l;i++)
	{
			 //std::cout << str[i];
			
			tmp[0]=str[i];
			if(i<l-1) tmp[1] = str[i+1];
			else tmp[1] = 'X';
			tmp[2] = 0x00;
			if(isoper(tmp))
			{
				//std::cout << std::endl << tmp << std::endl;
				count+=2;
				i+=strlen(tmp)-1;
				continue;			}
			
			tmp[0]=str[i];
			tmp[1]=0x00;
			if(isoper(tmp))
			{
				//std::cout << std::endl << tmp << std::endl;
				count+=2;
				continue;			}
					
	}
	count++;

	A = new char*[count+1+2];
	if(!A) error(ERR_MEMALLOC,"evaluate(A);");
	B = new char*[count+1+2];
	if(!B) error(ERR_MEMALLOC,"evaluate(B);");
	
	
	j = last = 0;
	A[j] = new char[2];
	if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
	strcpy(A[j],"(");
	j++;
	
	for(i=0;i<l;i++)
	{
			// std::cout << str[i];
			
			tmp[0]=str[i];
			if(i<l-1) tmp[1] = str[i+1];
			else tmp[1] = 'X';
			tmp[2] = 0x00;
			if(isoper(tmp))
			{
				A[j] = new char[i-last+1];
				if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
				j++;
				A[j] = new char[strlen(tmp)+1];
				if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
					
				
				scopy(str,A[j-1],last,i-1);
				strcpy(A[j],tmp);
				last = i + strlen(tmp);
				j++;
				
				//std::cout << std::endl << tmp << std::endl;
				i+=strlen(tmp)-1;
				continue;			}
			
			tmp[0]=str[i];
			tmp[1]=0x00;
			if(isoper(tmp))
			{
				A[j] = new char[i-last+1];
				if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
				j++;
				A[j] = new char[strlen(tmp)+1];
				if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
					
				
				scopy(str,A[j-1],last,i-1);
				strcpy(A[j],tmp);
				last = i + strlen(tmp);
				j++;

				//std::cout << std::endl << tmp << std::endl;
				continue;			}
					
	}
	A[j] = new char[i-last+2];
	if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
	scopy(str,A[j],last,strlen(str));
	j++;
	
	A[j] = new char[i-last+2];
	if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
	strcpy(A[j],")");
	j++;
	
	A[j] = new char[3];
	if(!A[j]) error(ERR_MEMALLOC,"evaluate(A[j]);");
	sprintf(A[j],"@@");

	//Remove Blanks
	count = j+1;
	t= 0;
	for(i=0;i<count;i++)
	{
		trim(A[i]);
		if(strlen(A[i])==0)
		{
			t++;
			for(j=i;j<count-1;j++)
			{
				strcpy(A[j],A[j+1]);			}
		}
	}
	
	
	//count = 0;
	/*for(i=0;!cmp(A[i],"@@");i++)
		std::cout << A[i] << ",";
	*/
	count = i;
	//std::cout <<std::endl;
	
	j=0;
	for(i=0;i<count;i++)
	{
	
		if(cmp(A[i],"(")){ evalstack.push(A[i]); continue;}
		
		if(cmp(A[i],")"))
		{
			evalstack.pop(tmp);
			while(!cmp(tmp,"(") && evalstack.okay())
			{
				B[j] = new char[strlen(tmp)+1];
				if(!B[j]) error(ERR_MEMALLOC,"evaluate(B[j]);");
				strcpy(B[j],tmp);	
				evalstack.pop(tmp);
				j++;			}
			continue;
		}
		
		if(isoper(A[i]))
		{
			evalstack.pop(tmp);
			while(preceeds(tmp,A[i]) && !cmp(tmp,"("))
			{
				B[j] = new char[strlen(tmp)+1];
				if(!B[j]) error(ERR_MEMALLOC,"evaluate(B[j]);");
				strcpy(B[j],tmp);	
				j++;
				
				evalstack.pop(tmp);
			}
			evalstack.push(tmp);			
			evalstack.push(A[i]);
		}
		else
		{
			B[j] = new char[strlen(A[i])+1];
			if(!B[j]) error(ERR_MEMALLOC,"evaluate(B[j]);");
			strcpy(B[j],A[i]);	
			j++;		}	}
	count = j;
		
	do 			
		evalstack.pop(str);		// Empty Stack;
	while(evalstack.okay());
}	
