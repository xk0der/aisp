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

/** Process **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// Temporary variables
char	*temp1;
char	*temp2;
char 	*temp3;	// ***
long	 tmpl;
int		 tmpi;
int 	 tmpi1;
double	 tmpd;
float 	 tmpf;
char	*val;
char 	*filename;

double recurse = 0;			// reflects how deep we've gone recursing
// ZERO means root level

int returned = FALSE;		// @ret() executed??
char *expr;					// previous expression stored here
int	   ir;					// 'i'
int     ar;					// 'a'
char *rev;

char scope[64]="0";		// 0(ZERO) Signifies Global Scope



int script_inif = 0;
int script_inloop = 0;
int script_inswitch = 0;
int script_infor = 0;

long	begin = 0; 				// @begin() from here!

int scriptStackOkay = TRUE;

Variable var;			// Variable List
Function flist;			// User defined token List
Label label;				// label list
Label term;			// Terms defined using !define()
Stack  stack;		// Script stack used while invoking subroutines and functions
Stack  loopstack;	// while() etc. loop stacks
Stack  forstack;		// For loop stack
scriptStack ScriptStack;	// Stack accessible from the script
For fordata;			// Data of a for loop

// Token strings
char *tstr[]= {"cls","dis","ift","iff","if","endif","while","loop","whl","inp","switch","usr","set","get",
               "nln","var","fun","equ","jmp","inc","dec","add","sub","mul","div","jmr","ret","end","rev","isg","isl",
               "def","else","not","and","or","xor","trim","ltrim","rtrim","wnd","endswitch","elseif","break",
               "rnd","rand","seed","timer","for","next","do","till","until","term","endfor","ucase","toupper","lcase",
               "tolower","left","right","mid","cut","len","dim","push","pop","redim","stackokay","chr","asc",
               "instr","rinstr",
               "@@"
              };

long 	cline;			// current line number being executed!
int		narg;			// argc (for user defined tokens)
char 	**varg;			// argv (for user defined tokens)


// Process the tokens passed by interpret();
// stemp3(); function and char *tempX; can be used ONLY in this function! (X = Nothing,1,2,3)
// tmpi,tmpl,tmpd and tmpf ALSO to be used by this function ONLY
int process(char *t, char *arg, int &i,int &a)
{
    int flag = NOTSET;
    int token = -1;
    int j=0;
    t = trim(t);
    while(strcmp(tstr[j],"@@")!=0)	{
        if(strcmp(tstr[j],t)==0) {
            token = j;
            break;
        }
        j++;
    }

    if(token==-1)
    {
        if(flist.isfun(t))	// Is it a user defined token??
        {
            stack.push(cline);
            stack.push(i);
            stack.push(a);

            stemp3(strlen(EXP_VAR)+1+40);
            sprintf(temp3,"%s%.0f",EXP_VAR,recurse);
            var.creat(temp3,_string,_expr,INTERNAL);	// store _expr!

            getarg(arg);

            cline = flist.line(t) - 1;
            i = strlen(_expr) + 2;

            recurse++;
            sprintf(scope,"%i",(int)recurse);		// Change Scope!!!

            buildpara(t,flist.narg(t));	// Build parameters if any
        }
        return i;
    }
    getarg(arg);

    switch(token)
    {
    case _cls:
        system("clear");
        i=rexp("cls",i,a);
        break;
    case _dis:
        if(narg!=0)
        {
            stemp3(strlen(varg[0])+1);
            strcpy(temp3,varg[0]);
            temp3 = parsedis(temp3);
            std::cout << temp3;
            i = rexp(varg[0],i,a);
        }
        else
            i = rexp(" ",i,a);

        break;
    case _nln:
        i = rexp("\n",i,a);
        break;
    case _inp:
        if(narg<1)
            error(ERR_ARGCNT,"@inp()",i);
        stemp3(MAXSIZE);
        fflush(stdin);
        std::cin.getline(temp3,MAXSIZE-1);

        varg[0] = trim(varg[0]);
        if(!var.varinscope(varg[0],scope))
            error(ERR_VARDEC,"@inp()",i);

        anti_parsedis(temp3);

        if(var.isarray(varg[0],scope))
        {
            if(narg<2)	error(ERR_ARGCNT,"@inp()",i);
            tmpi = atoi(varg[1]);
            var.set_new(varg[0],tmpi,temp3);
        }
        else					var.set(varg[0],temp3);

        i = rexp(temp3,i,a)+1;
        break;
    case _ift:
        if(narg!=2)
        {
            error(ERR_ARGCNT,"@ift()",i);
            break;
        }
        varg[0] = ucase(varg[0]);
        if(cmp(varg[0],"TRUE"))
        {
            i = rexp(varg[1],i,a);
        }
        else if(cmp(varg[0],"FALSE"))
            i = rexp("",i,a);
        else
        {
            error(ERR_ARGTYPE,"@ift()",i);
        }

        break;
    case _iff:
        if(narg!=2)
        {
            error(ERR_ARGCNT,"@iff()",i);
            break;
        }
        varg[0] = ucase(varg[0]);
        if(cmp(varg[0],"FALSE"))
        {
            i = rexp(varg[1],i,a);
        }
        else if(cmp(varg[0],"TRUE"))
            i = rexp("",i,a);
        else
        {
            error(ERR_ARGTYPE,"@iff()",i);
        }

        break;
    case _dim:
        if(narg<2) error(ERR_ARGCNT,"@dim();");
        ucase(varg[1]);
        trim(varg[1]);
        trim(varg[2]);

        if(tmpi>2) tmpi = atoi(varg[2]); //size
        else	tmpi = 1;

        if(tmpi<=0) error(ERR_ARGTYPE,"@dim()");

        //i,f,d,l,s,b,n
        switch(varg[1][0])
        {
        case 'I':
            tmpl = (long int) _a_integer;
            break;
        case 'F':
            tmpl = (long int) _a_float;
            break;
        case 'D':
            tmpl = (long int) _a_double;
            break;
        case 'L':
            tmpl = (long int) _a_long;
            break;
        case 'S':
            tmpl = (long int) _a_string;
            break;
        case 'B':
            tmpl = (long int) _a_bool;
            break;
        case 'N':
            tmpl = (long int) _a_double;
            break;
        }

        var.creat_new(varg[0],(vtype)tmpl,tmpi,scope);
        i=rexp(varg[0],i,a);
        break;
    case _usr:
        if(narg<2)
            error(ERR_ARGCNT,"@usr()",i);
        varg[1] = ucase(trim(varg[1]));
        switch(varg[1][0])
        {
        case 'N':
            tmpi = _num;
            stemp3(2);
            strcpy(temp3,"0");
            break;
        case 'S':
            tmpi = _string;
            stemp3(4);
            strcpy(temp3,"  ");
            break;
        case 'B':
            tmpi = _bool;
            stemp3(6);
            strcpy(temp3,"FALSE");
            break;
        default:
            error(ERR_VARTYPE,"@usr()",i);
        }
        varg[0] = trim(varg[0]);

        if(var.varinscope(varg[0],scope))
            error(ERR_VARPRESENT,"@usr()",i);

        var.creat(varg[0],(vtype)tmpi,temp3,scope);
        i=rexp(varg[0],i,a);
        break;
    case _set:
        if(narg<2)
            error(ERR_ARGCNT,"@set()",i);
        trim(varg[0]);

        if(var.isarray(varg[0],scope))
        {
            if(narg<3) error(ERR_ARGCNT,"@set()",i);
            tmpi = atoi(varg[1]);
            if(tmpi<0 || tmpi>var.bound(varg[0],scope)) error(ERR_INDEX_OUTOFBOUND,"@set()");
            var.set_new(varg[0],tmpi,varg[2]);
            i = rexp(varg[2],i,a);
        }
        else
        {
            if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@set()",i);
            var.set(varg[0],varg[1]);
            i = rexp(varg[1],i,a);
        }
        break;
    case _get:
        if(narg<1) error(ERR_ARGCNT,"@get()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@get()");
        if(narg>=2) tmpi= atoi(varg[1]);
        else tmpi = 0;

        if(var.isarray(varg[0],scope))
        {
            if(narg<2) tmpi = 0;

            tmpi = atoi(varg[1]);
            if(tmpi<0 || tmpi>var.bound(varg[0],scope)) error(ERR_INDEX_OUTOFBOUND,"@get()");

            if(narg>=3) var.value_new(varg[0],tmpi,scope,varg[2]);
            else var.value_new(varg[0],tmpi,scope,NULL);
        }
        else
        {
            if(narg>=2)
            {
                stemp3(strlen(varg[1])+4);
                sprintf(temp3,"%slf",varg[1]);
                var.value(varg[0],temp3);
            }
            else var.value(varg[0]);
        }

        i = rexp(val,i,a);
        delete[] val;
        break;
    case _var:
        if(narg<1) error(ERR_ARGCNT,"@var()",i);
        stemp3(MAXSIZE);
        i = rexp(varg[0],i,a);
        break;
    case _fun:
        if(narg<1)	error(ERR_ARGCNT,"@fun()",i);
        stemp3(MAXSIZE);
        getfunname(temp3);

        if(flist.isfun(temp3))
            error(ERR_FUNPRESENT,"@fun",i);

        if(!findfun(temp3,&tmpi,&tmpi1)) // Get start & end line number of function
            error(ERR_TOKENDEF,"@fun()",i);

        temp1 = new char[MAXSIZE];
        if(!temp1) error(ERR_MEMALLOC,"@fun()",i);

        getfunarg(temp1);

        flist.add(temp3,tmpi,tmpi1,strlen(temp1),temp1);

        delete[] temp1;
        i = rexp(temp3,i,a);
        break;
    case _equ:
        if(narg<2) error(ERR_ARGCNT,"@equ()",i);
        if(cmp(varg[0],varg[1]))
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);

        break;
    case _isg:
        if(narg<2) error(ERR_ARGCNT,"@equ()",i);

        if(cmpx(varg[0],varg[1])>0)
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);

        break;
    case _isl:
        if(narg<2) error(ERR_ARGCNT,"@equ()",i);

        if(cmpx(varg[0],varg[1])<0)
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);

        break;
    case _inc:
        if(narg<1) error(ERR_ARGCNT,"@inc()",i);
        if(var.type(varg[0])!=_num) error(ERR_VARTYPE,"@inc()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@inc()",i);

        var.value(varg[0]);
        tmpd = strtod(val,NULL);
        tmpd++;
        stemp3(strlen(val)+20);
        sprintf(temp3,"%f",tmpd);
        i=rexp(temp3,i,a);
        var.set(varg[0],temp3);
        delete[] val;
        break;
    case _dec:
        if(narg<1) error(ERR_ARGCNT,"@dec()",i);
        if(var.type(varg[0])!=_num) error(ERR_VARTYPE,"@dec()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@dec()",i);

        var.value(varg[0]);
        tmpd = strtod(val,NULL);
        tmpd--;
        stemp3(strlen(val)+20);
        sprintf(temp3,"%f",tmpd);
        i=rexp(temp3,i,a);
        var.set(varg[0],temp3);
        delete[] val;
        break;
    case _jmp:
        if(narg<1) error(ERR_ARGCNT,"@jmp()",i);
        trim(varg[0]);
        if(strlen(varg[0])<1) break;
        //std::cout << "Jumping to : " << varg[0] <<"**" << label.islabel(varg[0]) << std::endl;
        tmpl = label.line(varg[0]);
        cline = tmpl-1;
        i = strlen(_expr)+2; // signal completion of execution
        script_inif = 0;
        break;
    case _add:
        if(narg<2) error(ERR_ARGCNT,"@add()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@add()",i);

        var.value(varg[0]);
        tmpd = strtod(val,NULL);
        tmpd = tmpd+strtod(varg[1],NULL);
        stemp3(strlen(varg[1])+30);
        sprintf(temp3,"%f",tmpd);
        i = rexp(temp3,i,a);
        var.set(varg[0],temp3);
        delete[] val;
        break;
    case _sub:
        if(narg<2) error(ERR_ARGCNT,"@sub()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@sub()",i);

        var.value(varg[0]);
        tmpd = strtod(val,NULL);
        tmpd = tmpd-strtod(varg[1],NULL);
        stemp3(strlen(varg[1])+30);
        sprintf(temp3,"%f",tmpd);
        i = rexp(temp3,i,a);
        var.set(varg[0],temp3);
        delete[] val;
        break;
    case _mul:
        if(narg<2) error(ERR_ARGCNT,"@mul()",i);

        if(!var.varinscope(varg[0],scope)) {
            error(ERR_VARDEC,"@mul()",i);
        }

        var.value(varg[0]);
        tmpd = strtod(val,NULL);
        tmpd = tmpd * strtod(varg[1],NULL);
        stemp3(strlen(varg[1])+30);
        sprintf(temp3,"%f",tmpd);
        i = rexp(temp3,i,a);
        var.set(varg[0],temp3);
        delete[] val;
        break;
    case _div:
        if(narg<2) error(ERR_ARGCNT,"@div()",i);

        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@div()",i);

        var.value(varg[0]);

        tmpd = strtod(varg[1],NULL);
        if(tmpd==0) error(ERR_DIVBYZERO,"@div()",i);

        tmpd = strtod(val,NULL) / tmpd;

        stemp3(strlen(varg[1])+30);
        sprintf(temp3,"%f",tmpd);

        i = rexp(temp3,i,a);
        var.set(varg[0],temp3);

        delete[] val;
        break;
    case _jmr:
        if(narg<1) error(ERR_ARGCNT,"@jmp()",i);
        trim(varg[0]);
        if(strlen(varg[0])<1) break;

        stack.push(cline);
        stack.push(i);
        stack.push(a);

        stemp3(strlen(EXP_VAR)+1+40);
        sprintf(temp3,"%s%.0f",EXP_VAR,recurse);
        var.creat(temp3,_string,_expr,INTERNAL);	// store _expr!

        tmpl = label.line(varg[0]);
        cline = tmpl-1;
        i = strlen(_expr)+2;	// signal completion of execution!
        recurse++;			// recurse to one more level!
        break;
    case _ret:
        if(narg<1)
        {
            stemp3(2);
            temp3[0]=0x00;
        }
        else
        {
            stemp3(strlen(varg[0])+1);
            strcpy(temp3,varg[0]);
        }
        //std::cout << "In ret temp3 = **" << temp3 <<"**" << std::endl;
        cline = setreturned(temp3) - 1;

        i=strlen(_expr)+2;


        recurse--;		// recurse back!
        if(recurse<0)
        {
            if(recurse<0) error(ERR_RET,"@ret()",i);	// How can we get back to where we never were!!
        }
        else
        {
            if(!cmp(scope,"0"))// If we are already Global no need to change scope!
            {
                var.deletescope(scope);				// otherwise Delete all the variables of the present scope
                sprintf(scope,"%i",(int)recurse);		// and Change Scope!!!
            }
        }
        break;
    case _end:
        cline = total_lines+1;
        i = strlen(_expr)+2;
        break;
    case _rev:
        if(strlen(rev)<1)
            i = rexp(" ",i,a);
        else
            i = rexp(rev,i,a);
        break;
    case _def:
        //tmpi = stack.pop(); // No. of arguments
        if(cmp(scope,"0"))
        {
            tmpi = cline;
            while(instr(script[tmpi],"@ret")==-1)
            {
                tmpi++;
                if(tmpi>=total_lines)
                    error(ERR_MISSINGRET,"@def",i);
            }
            cline = tmpi;
            i=strlen(_expr)+2;
        }
        i = rexp("def",i,a);
        break;
    case _if:
        if(narg<1) error(ERR_ARGCNT,"@if()",i);
        trim(varg[0]);
        ucase(varg[0]);
        tmpi = script_inif;
        tmpl = cline;
        script_inif++;
        if(cmp(varg[0],"FALSE"))
        {   flag = NOTSET;
            while(cline<total_lines)
            {
                //std::cout << "@if() >"<< script[cline] << " > " << script_inif << "," << tmpi << std::endl;
                cline++;
                if(instr(script[cline],"@if")!=-1) script_inif++;
                if(instr(script[cline],"@endif")!=-1) script_inif--;
                if(instr(script[cline],"@else")!=-1)
                {
                    flag = SET;
                    if(script_inif-1 == tmpi) break;
                }
                if(script_inif == tmpi) {
                    flag = SET;
                    break;
                }
            }
            if(!flag) error(ERR_MISSINGENDIF,"@if()",i);
        }
        else if(cmp(varg[0],"TRUE"))
        {
            ;
        }
        else
        {   error(ERR_ARGTYPE,"@if()",i);
        }
        i = rexp(varg[0],i,a);
        break;
    case _endif:
        script_inif--;
        if(script_inif<0) error(ERR_MISSINGIF,"@endif()",i);
        i = rexp("endif",i,a);
        break;
    case _else:
        tmpl = cline;
        tmpi = 1;
        flag = NOTSET;
        while(cline<total_lines)
        {
            cline++;
            if(instr(script[cline],"@if")!=-1) tmpi++;
            if(instr(script[cline],"@endif")!=-1) tmpi--;
            if(tmpi == 0) {
                flag = SET;
                break;
            }
        }
        if(!flag) error(ERR_MISSINGENDIF,"@else()",i);
        script_inif--;
        i = rexp("else",i,a);
        break;
    case _not:
        if(narg<1) error(ERR_ARGCNT,"@not()");
        trim(varg[0]);
        ucase(varg[0]);
        if(cmp(varg[0],"TRUE"))
            i = rexp("FALSE",i,a);
        else if(cmp(varg[0],"FALSE"))
            i = rexp("TRUE",i,a);
        else
            error(ERR_ARGTYPE,"@not",i);
        break;
    case _and:
        if(narg<2) error(ERR_ARGCNT,"@and()");
        trim(varg[0]);
        trim(varg[1]);
        ucase(varg[0]);
        ucase(varg[1]);

        if((!cmp(varg[0],"TRUE") && !cmp(varg[0],"FALSE")) || (!cmp(varg[1],"TRUE") && !cmp(varg[1],"FALSE")))
            error(ERR_ARGTYPE,"@and()",i);

        if(cmp(varg[0],varg[1]))
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);

        break;
    case _or:
        if(narg<2) error(ERR_ARGCNT,"@and()");
        trim(varg[0]);
        trim(varg[1]);
        ucase(varg[0]);
        ucase(varg[1]);

        if((!cmp(varg[0],"TRUE") && !cmp(varg[0],"FALSE")) || (!cmp(varg[1],"TRUE") && !cmp(varg[1],"FALSE")))
            error(ERR_ARGTYPE,"@or()",i);

        if(cmp(varg[0],"TRUE") || cmp(varg[1],"TRUE"))
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);
        break;
    case _xor:
        if(narg<2) error(ERR_ARGCNT,"@and()");
        trim(varg[0]);
        trim(varg[1]);
        ucase(varg[0]);
        ucase(varg[1]);

        if((!cmp(varg[0],"TRUE") && !cmp(varg[0],"FALSE")) || (!cmp(varg[1],"TRUE") && !cmp(varg[1],"FALSE")))
            error(ERR_ARGTYPE,"@xor()",i);

        if((cmp(varg[0],"TRUE") || cmp(varg[1],"TRUE")) && !cmp(varg[0],varg[1]))
            i = rexp("TRUE",i,a);
        else
            i = rexp("FALSE",i,a);
        break;
        break;
    case _trim:
        if(narg<1) error(ERR_ARGCNT,"@trim()",i);
        trim(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _ltrim:
        if(narg<1) error(ERR_ARGCNT,"@ltrim()",i);
        ltrim(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _rtrim:
        if(narg<1) error(ERR_ARGCNT,"@rtrim()",i);
        rtrim(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _whl:	// For backward compatibility
    case _while:
        if(narg<1) error(ERR_ARGCNT,"@while()",i);
        trim(varg[0]);
        ucase(varg[0]);
        //std::cout << "In while " << varg[0] << std::endl;
        if(cmp(varg[0],"FALSE"))
        {
            tmpi=script_inloop;
            tmpl=cline;
            script_inloop++;
            flag = NOTSET;
            while(cline<total_lines)
            {
                //std::cout << "Skipping "<<std::endl;
                cline++;
                if(instr(script[cline],"@while")!=-1) script_inloop++;
                if(instr(script[cline],"@loop")!=-1) script_inloop--;
                if(script_inloop==tmpi) {
                    flag = SET;
                    break;
                }
            }
            if(!flag) error(ERR_MISSINGLOOP,"@while",i);
        }
        else if(cmp(varg[0],"TRUE"))
        {
            script_inloop++;
            //std::cout << "Out here " << std::endl;
            loopstack.push(cline);
        }
        else
            error(ERR_ARGTYPE,"@while()",i);
        break;
    case _wnd:	// for backward compatibility
    case _loop:
        //std::cout << "Script loop="<<script_inloop << std::endl;
        script_inloop--;
        if(script_inloop<0)
            error(ERR_MISSINGWHILE,"@loop()",i);
        cline = loopstack.pop() - 1;
        break;
    case _switch:
        if(narg<1) error(ERR_ARGCNT,"@switch()",i);
        script_inswitch++;
        stemp3(strlen(varg[0])+1+strlen("@jmp(@default())")+10);
        sprintf(temp3,"@case(%s)",varg[0]);
        if(!label.islabel(temp3)) sprintf(temp3,"@default(%i)",(int)strtod(varg[0],NULL));
        if(!label.islabel(temp3))
        {
            tmpl = cline;
            tmpi = script_inswitch;
            flag = NOTSET;
            while(cline<total_lines)
            {
                cline++;
                if(instr(script[cline],"@switch")!=-1) script_inswitch++;
                if(instr(script[cline],"@endswitch")!=-1) script_inswitch--;
                if(script_inswitch==tmpi-1) {
                    flag=SET;
                    break;
                }
            }
            i = strlen(_expr)+2;
            if(!flag) error(ERR_MISSINGENDSWITCH,"@break()",i);
        }
        else
        {
            /*temp1 = new char[strlen(temp3)+strlen("@jmp(default())")+1];
            if(!temp1) error(ERR_MEMALLOC,"@switch()");
            sprintf(temp1,"@jmp(%s)",temp3);
            std::cout << "Temp1 = " << temp1 <<std::endl;
            std::cout << temp3 <<(label.islabel(temp3) ? " Is a label":" Is not a label") << std::endl;
            i=rexp("  @dis(hey!)",i,a);
            delete[] temp1;*/
            cline = label.line(temp3)-1;
            i = strlen(_expr)+2;
            //script_inswitch++;
        }
        //std::cout << " Exp = " << _expr << std::endl;
        break;
    case _break:
        tmpl = cline;
        tmpi = script_inswitch;
        flag = NOTSET;
        while(cline<total_lines)
        {
            cline++;
            if(instr(script[cline],"@switch")!=-1) script_inswitch++;
            if(instr(script[cline],"@endswitch")!=-1) script_inswitch--;
            if(script_inswitch==tmpi-1) {
                flag=SET;
                break;
            }
        }
        if(!flag) error(ERR_MISSINGENDSWITCH,"@break()",i);
        i = strlen(_expr)+2;
        break;
    case _endswitch:
        script_inswitch--;
        if(script_inswitch<0) error(ERR_MISSINGSWITCH,"@endswitch()",i);
        break;
    case _rnd:
    case _rand:
        if(narg<1) error(ERR_ARGCNT,"@rand()",i);
        tmpd = strtod(varg[0],NULL);
        if(tmpd==0) error(ERR_DIVBYZERO,"@rand()",i);
        tmpd = rand()%(long int)tmpd;
        stemp3(MAXSIZE);
        sprintf(temp3,"%f",(float)tmpd);
        i=rexp(temp3,i,a);
        break;
    case _seed:
        if(narg<1) error(ERR_ARGCNT,"@seed()",i);
        tmpd = strtod(varg[0],NULL);
        srand((unsigned int)tmpd);
        break;
    case _timer:
        stemp3(MAXSIZE);
        sprintf(temp3,"%f",(float)time(NULL));
        i = rexp(temp3,i,a);
        break;
    case _for:
        //   0   	  		1    2  	3
        //  Variable  Init  Final  Step
        if(narg<3) error(ERR_ARGCNT,"@for()",i);
        if(!var.varinscope(varg[0],scope)) error(ERR_VARDEC,"@for()",i);
        var.set(varg[0],varg[1]);
        script_infor++;
        forstack.push(cline);
        tmpf = 1; // Step
        if(narg>3) tmpf =(float)strtod(varg[3],NULL);
        fordata.push(varg[0],strtod(varg[1],NULL),strtod(varg[2],NULL),(double)tmpf);
        break;
    case _endfor:
    case _next:
    {
        temp1 = new char[MAXSIZE];
        if(!temp1) error(ERR_MEMALLOC,"@next()",i);

        stemp3(MAXSIZE);
        double data,final,step;
        fordata.pop(temp3,final,step);
        var.value(temp3);
        data = strtod(val,NULL);
        data+=step;
        sprintf(temp1,"%f",data);
        var.set(temp3,temp1);
        if(data<=final)
        {   i=strlen(_expr)+2;
            cline = forstack.pop();
            forstack.push(cline);
            fordata.push(temp3,data,final,step);
        }
        else
            forstack.pop();

        delete[] temp1;
    }
    if(script_infor<0) error(ERR_MISSINGFOR,"@next()",i);
    break;
    case _do:
        script_inloop++;
        loopstack.push(cline);
        break;
    case _until:
        if(narg<1) error(ERR_ARGCNT,"@until()",i);
        ucase(varg[0]);
        trim(varg[0]);
        script_inloop--;
        if(cmp(varg[0],"FALSE"))
        {
            cline = loopstack.pop() - 1;
            i = strlen(_expr)+2;
        }
        if(script_inloop<0) error(ERR_MISSINGDO,"@until()",i);
        break;
    case _till:
        if(narg<1) error(ERR_ARGCNT,"@till()",i);
        ucase(varg[0]);
        trim(varg[0]);
        script_inloop--;
        if(cmp(varg[0],"TRUE"))
        {
            cline = loopstack.pop() - 1;
            i = strlen(_expr)+2;
        }
        if(script_inloop<0) error(ERR_MISSINGDO,"@till()",i);
        break;
    case _term:
        if(narg<1) error(ERR_ARGCNT,"@term()");
        trim(varg[0]);
        if(!term.isterm(varg[0])) error(ERR_TERMDEC,"@term()");
        stemp3(MAXSIZE);
        term.data(varg[0],temp3);
        i = rexp(temp3,i,a);
        break;
    case _toupper_:
    case _ucase:
        if(narg<1) error(ERR_ARGCNT,"@ucase()");
        ucase(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _tolower_:
    case _lcase:
        if(narg<1) error(ERR_ARGCNT,"@lcase()");
        lcase(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _left:
        if(narg<2) error(ERR_ARGCNT,"@left()");
        tmpi = atoi(varg[1]);


        parsedis(varg[0]);
        tmpl = strlen(varg[0]);

        stemp3(tmpl+100);
        if(tmpi<=0) tmpi=1;
        if(tmpi>tmpl) tmpi = tmpl;
        scopy(varg[0],temp3,0,tmpi-1);

        anti_parsedis(temp3);
        i = rexp(temp3,i,a);
        break;
    case _right:
        if(narg<2) error(ERR_ARGCNT,"@right()");
        tmpi = atoi(varg[1]);

        parsedis(varg[0]);
        tmpl = strlen(varg[0]);

        stemp3(tmpl+100);
        if(tmpi<=0) tmpi=1;
        if(tmpi>tmpl) tmpi = tmpl;
        scopy(varg[0],temp3,(tmpl-tmpi),tmpl-1);

        anti_parsedis(temp3);
        i = rexp(temp3,i,a);
        break;
    case _cut:
    case _mid:
        if(narg<3) error(ERR_ARGCNT,"@mid()");
        tmpi = atoi(varg[1]);
        tmpi1 = atoi(varg[2]);


        parsedis(varg[0]);
        tmpl = strlen(varg[0]);

        stemp3(tmpl+100);
        if(tmpi<=0)  tmpi=1;
        if(tmpi1<=0) tmpi1=1;
        if(tmpi>tmpl) tmpi = tmpl;
        while((tmpi-1)+(tmpi1)>tmpl) tmpi1--;

        scopy(varg[0],temp3,tmpi-1,(tmpi-1)+(tmpi1-1));
        anti_parsedis(temp3);
        i = rexp(temp3,i,a);
        break;
    case _len:
        parsedis(varg[0]);
        tmpi = strlen(varg[0]);
        stemp3(MAXSIZE);
        sprintf(temp3,"%i",tmpi);
        i = rexp(temp3,i,a);
        break;
    case _push:
        if(narg<1) error(ERR_ARGCNT,"@push()",i);
        ScriptStack.push(varg[0]);
        i = rexp(varg[0],i,a);
        break;
    case _pop:
        stemp3(MAXSIZE);
        ScriptStack.pop(temp3);
        if(ScriptStack.okay()) {
            i = rexp(temp3,i,a);
            scriptStackOkay = TRUE;
        }
        else {
            i = rexp(" ",i,a);
            scriptStackOkay	= FALSE;
        }
        break;
    case _stackokay:
        if(ScriptStack.okay()) i = rexp("TRUE",i,a);
        else i = rexp("FALSE",i,a);
        break;
    case _chr:
        if(narg<1) error(ERR_ARGCNT,"@chr()",i);
        tmpi = atoi(varg[0]);
        stemp3(10);
        sprintf(temp3,"%c",tmpi);
        anti_parsedis(temp3);
        i = rexp(temp3,i,a);
        break;
    case _asc:
        if(narg<1) error(ERR_ARGCNT,"@asc()",i);
        parsedis(varg[0]);
        tmpi = varg[0][0];
        stemp3(10);
        sprintf(temp3,"%i",tmpi);
        i = rexp(temp3,i,a);
        break;
    case _instr:
        if(narg<2) error(ERR_ARGCNT,"@instr()",i);
        tmpi = instr(varg[0],varg[1]);
        stemp3(MAXSIZE);
        sprintf(temp3,"%i",tmpi);
        i = rexp(temp3,i,a);
        break;
    case _rinstr:
        if(narg<2) error(ERR_ARGCNT,"@rinstr()",i);
        tmpi = rinstr(varg[0],varg[1]);
        stemp3(MAXSIZE);
        sprintf(temp3,"%i",tmpi);
        i = rexp(temp3,i,a);
        break;
    default:
        std::cout << t << " <- This token not implemented yet!!" << std::endl;
        break;
    }

    for(j=narg-1; j>narg; j--)
    {
        delete[] varg[j];
        varg[j]=NULL;
    }
    delete[] varg;
    varg=NULL;
    narg = 0;

    //std::cout << "Press any key to continue..." << cline << std::endl;
    //std::cout << _expr << std::endl;
    //tmpi = getchar();

    if(emit)
    {
        std::fstream efp("~aisp_emit.txt",std::ios::app);
        if(!efp) error(ERR_OPEN,"~aisp_emit.txt in process();");
        efp <<"["<< cline<<"] _expr = "	<<_expr << std::endl;
        efp.close();
    }

    return i;
}

int getarg(char *arg)
{
    int l=strlen(arg),i,j,p;
    if(l<1) {
        narg=0;
        return narg;
    }
    narg = 0;
    for(i=0; i<l; i++)
        if(arg[i]==',') narg++;
    narg++;
    varg = new char*[narg];
    if(!varg)
    {
        error(ERR_MEMALLOC,"getarg()");
        exit(ERR_MEMALLOC);
    }

    j=0;
    p=0;
    for(i=0; i<l; i++)
    {   if(arg[i]==',')
        {
            varg[j] = new char[i-p + 1];
            if(!varg[j])
            {
                error(ERR_MEMALLOC,"getarg()2");
                exit(ERR_MEMALLOC);
            }
            scopy(arg,varg[j],p,i-1);
            j++;
            p = i+1;
        }

    }
    varg[j] = new char[i-p+1];
    if(!varg[j])
    {
        error(ERR_MEMALLOC,"getarg() 3 ");
        exit(ERR_MEMALLOC);
    }
    scopy(arg,varg[j],p,i-1);
    if(narg-1!=j)
    {
        error(ERR_UNKNOWN,"getarg()");
        exit(ERR_UNKNOWN);
    }
    return narg;
}

int rexp(char *rv,int &i,int &a)
{
    int l = strlen(_expr);
    int rvl = strlen(rv);
    int k,j;
    char *tmp;

    int newsize = (l - (i-a)) + rvl  + 1;

    //std::cout << "**Debug Info : ";
    //std::cout << "_expr = " << _expr << " | " << l << std::endl;
    //std::cout << "rv =" << rv << " | " << rvl << std::endl;
    //std::cout << " | newsize = " << newsize << std::endl;
    //std::cout << "**DInfo END" << std::endl;

    tmp = new char[newsize];

    if(!tmp)
    {
        error(ERR_MEMALLOC,"rexp()1");
        exit(ERR_MEMALLOC);
    }

    scopy(_expr,tmp,0,a-1);
    k=a;

    for(j=0; j<rvl; j++,k++)
        tmp[k] = rv[j];

    for(j=i+1; j<l; j++,k++)
        tmp[k] = _expr[j];
    tmp[k] = '\0';

    delete[] _expr;

    _expr = new char[newsize+1];
    if(!_expr)
    {
        error(ERR_MEMALLOC,"rexp()2");
        exit(ERR_MEMALLOC);
    }
    strcpy(_expr,tmp);
    _expr[newsize-1] = '\0';

    delete[] tmp;
    i = a-1;
    if(i<0) i=0;
    return i;
}


void  stemp3(int siz)
{
    if(temp3) delete[] temp3;
    temp3 = NULL;
    temp3 = new char[siz+1];
    if(!temp3) {
        error(ERR_MEMALLOC,"stemp3();");
        exit(ERR_MEMALLOC);
    }
}
