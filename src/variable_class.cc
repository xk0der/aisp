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

/** Variable Class **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

Variable::Variable()
{
    root = new var;
    if(!root)
    {
        error(ERR_MEMALLOC,"Variable::constuct()");
        exit(ERR_MEMALLOC);
    }
    root->prev = NULL;
    curr = root;
    curr->next = NULL;
    head = curr;
}

// Name, type, size, scope
int Variable::creat_new(char *n,vtype t, int size, char *sp)
{
    int l;
    char d[]="";

    if(varinscope(n,sp)) error(ERR_VARPRESENT,"Variable::creat_new();");
    if(size<=0) size = 1;

    head->name = new char[strlen(n)+1];
    if(!head->name)error(ERR_MEMALLOC,"Variable::creat_new()1");

    strcpy(head->name,n);
    head->name = setcase(head->name);
    head->Type = t;
    head->datanum = size;

    head->datastr = NULL;
    head->ip = NULL;
    head->fp = NULL;
    head->dp = NULL;
    head->lp = NULL;
    head->cp = NULL;

    switch(t)
    {
    case _a_integer:
        head->ip = new int[size];
        if(!head->ip) error(ERR_MEMALLOC,"Variable::creat_new(int);");
        break;
    case _a_float:
        head->fp = new float[size];
        if(!head->fp) error(ERR_MEMALLOC,"Variable::creat_new(float);");
        break;
    case _a_double:
        head->dp = new double[size];
        if(!head->dp) error(ERR_MEMALLOC,"Variable::creat_new(double);");
        break;
    case _a_long:
        head->lp = new long[size];
        if(!head->lp) error(ERR_MEMALLOC,"Variable::creat_new();");
        break;
    case _a_bool:
    case _a_string:
        head->cp = new char*[size];
        if(!head->cp) error(ERR_MEMALLOC,"Variable::creat_new();");
        size--;
        while(size>=0) head->cp[size--] = NULL;
        break;
    case _refrence:
        //std::cout << "Here script" << std::endl;
        char *tmp2,*tmp1;
        tmp2 = new char[64];
        if(!tmp2) error(ERR_MEMALLOC,"Vriable::creat(ref);");
        tmp1 = new char[64];
        if(!tmp1) error(ERR_MEMALLOC,"Vriable::creat(ref);");

        sprintf(tmp2,"%.0f",strtod(sp,NULL)-1);

        //std::cout << "head name = "<< head->name << std::endl;

        strcpy(tmp1,::scope);
        strcpy(::scope,tmp2);
        if(type(d) == _refrence)
        {
            varinscope(d,::scope);
            l = strlen(curr->datastr);
        }
        else
            l = strlen(d);


        head->datastr = new char[l+1];
        if(!head->datastr)
            error(ERR_MEMALLOC,"Variable::creat();");

        if(type(d) == _refrence)
        {
            sprintf(tmp2,"%.0f",strtod(sp,NULL)-1);
            varinscope(d,sp);
            //std::cout <<  " Copying r2r " << curr->datastr << std::endl;
            //std::cout << " ref var = " << d <<std::endl;
            strcpy(head->datastr,curr->datastr);
            head->datanum = curr->datanum;
        }
        else
        {
            strcpy(head->datastr,d);				  // Variable name (pointed to)
            head->datanum = strtod(sp,NULL); // Store Scope
        }
        strcpy(::scope,tmp1);
        delete[] tmp2;
        delete[] tmp1;
        break;
    default:
        return 1;
    }

    strcpy(head->Scope,sp);
    head->next = new var;
    if(!head->next)
    {
        error(ERR_MEMALLOC,"Variable::creat_new();");
        exit(ERR_MEMALLOC);
    }
    head->next->prev = head;
    head = head->next;
    head->next = NULL;
    head->name = NULL;


    head->datastr = NULL;
    head->ip = NULL;
    head->fp = NULL;
    head->dp = NULL;
    head->lp = NULL;
    head->cp = NULL;


    return 0;
}

// name, index, data
int Variable::set_new(char *n,long index,char *d)
{
    int l;

    if(!varinscope(n,::scope)) return 1;

    if(index<0 || index>=curr->datanum) error(ERR_INDEX_OUTOFBOUND,"Varibale::set_new();");

    switch(curr->Type)
    {
    case _a_integer:
        curr->ip[index] = atoi(d);
        break;
    case _a_long:
        curr->lp[index] = strtol(d,NULL,10);
        break;
    case _a_float:
        curr->fp[index] = strtod(d,NULL);
        break;
    case _a_double:
        curr->dp[index] = strtod(d,NULL);
        break;
    case _a_string:

        if(curr->cp[index]) delete curr->cp[index];

        curr->cp[index] = new char[strlen(d)+1];
        if(!curr->cp[index]) error(ERR_MEMALLOC,"Varibale::set_new(_a_string)");
        strcpy(curr->cp[index],d);
        break;
    case _a_bool:
        if(curr->cp[index]) delete curr->cp[index];

        ucase(d);
        trim(d);

        if(!(cmp(d,"TRUE") || cmp(d,"FALSE"))) error(ERR_ARGTYPE,"Variable::set_new(_a_bool)");

        curr->cp[index] = new char[6];
        if(!curr->cp[index]) error(ERR_MEMALLOC,"Varibale::set_new(_a_bool)");
        strcpy(curr->cp[index],d);
        break;
    case _refrence:
        // Store pointer (Variable name)
        char *tmp2,*tmp1;
        tmp2 = new char[64];
        if(!tmp2) error(ERR_MEMALLOC,"Vriable::set(ref);");
        tmp1 = new char[64];
        if(!tmp1) error(ERR_MEMALLOC,"Vriable::set(ref);");


        //std::cout << "head name = "<< head->name << std::endl;


        sprintf(tmp2,"%.0f",curr->datanum);

        strcpy(tmp1,::scope);
        strcpy(::scope,tmp2);

        l = strlen(d);

        /*varinscope(curr->datastr,::scope); // Get to variable's datastr

        curr->datastr = new char[l+1];
        if(!curr->datastr)
        	error(ERR_MEMALLOC,"Variable::set();");
        */
        set(curr->datastr,d);

        strcpy(::scope,tmp1);

        delete[] tmp2;
        delete[] tmp1;
        break;
    default:
        return 1;
    }

    return 0;
}

int Variable::value_new(char *n,int index, char *sp,char *format)
{
    if(!varinscope(n,sp)) error(ERR_VARDEC,"Variable::value_new();");
    if(index<0 || index>=curr->datanum) error(ERR_INDEX_OUTOFBOUND,"Varibale::set_new();");

    switch(curr->Type)
    {
    case _a_integer:
        val = new char[MAXSIZE];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_integer)");
        if(format)
            sprintf(val,format,curr->ip[index]);
        else
            sprintf(val,"%i",curr->ip[index]);
        break;
    case _a_float:
        val = new char[MAXSIZE];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_float)");
        if(format)
            sprintf(val,format,curr->fp[index]);
        else
            sprintf(val,"%.2f",curr->fp[index]);
        break;
    case _a_double:
        val = new char[MAXSIZE];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_double)");
        if(format)
            sprintf(val,format,curr->dp[index]);
        else
            sprintf(val,"%.4f",(float)curr->dp[index]);
        break;
    case _a_long:
        val = new char[MAXSIZE];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_long)");
        if(format)
            sprintf(val,format,curr->lp[index]);
        else
            sprintf(val,"%i",(int)curr->lp[index]);
        break;
    case _a_string:
        val = new char[strlen(curr->cp[index])+1];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_string)");
        sprintf(val,"%s",curr->cp[index]);
        break;
    case _a_bool:
        val = new char[6];
        if(!val) error(ERR_MEMALLOC,"Variable::value_new(_a_bool)");
        sprintf(val,"%s",curr->cp[index]);
        break;
    case _refrence:
        char *tmp;
        char *tmp2;

        tmp2 = new char[64];
        if(!tmp2) error(ERR_MEMALLOC,"Variable::value(ref); 5");

        tmp = new char[64];
        if(!tmp) error(ERR_MEMALLOC,"Variable::value(ref); 6");

        sprintf(tmp2,"%.0f",curr->datanum);

        strcpy(tmp,::scope);
        strcpy(::scope,tmp2);

        //std::cout << "Getting value of ->" << curr->datastr << std::endl;

        value_new(curr->datastr,index,::scope,NULL); // get value

        strcpy(::scope,tmp);

        delete[] tmp;
        delete[] tmp2;
        break;
    default:
        return 1;
    }
    return 0;
}



int Variable::creat(char *n,vtype t,char *d,char *sp)
{
    //std::cout << "Creating variable " << n << " ,"<< cline <<","<< exp <<std::endl;
    if(varinscope(n,sp))
    {
        error(ERR_VARPRESENT,"Variable::creat()");
        exit(1);
    }

    int  l = strlen(n);
    head->name = new char[l+1];
    if(!head->name)
    {
        error(ERR_MEMALLOC,"Variable::creat()1");
        exit(ERR_MEMALLOC);
    }
    strcpy(head->name,n);
    head->name = setcase(head->name);
    head->Type = t;

    head->datastr = NULL;
    head->ip = NULL;
    head->fp = NULL;
    head->dp = NULL;
    head->lp = NULL;
    head->cp = NULL;

    switch(t)
    {
    case _num:
        head->datanum = strtod(d,NULL);
        break;
    case _string:
        l = strlen(d);
        head->datastr = new char[l+1];
        if(!head->datastr)
        {
            error(ERR_MEMALLOC,"Variable::creat();");
            exit(ERR_MEMALLOC);
        }
        strcpy(head->datastr,d);
        head->datanum = strlen(head->datastr);
        break;
    case _bool:
        ucase(d);
        trim(d);
        if(cmp(d,"TRUE")) head->databool = 1;
        else if(cmp(d,"FALSE")) head->databool = 0;
        else error(ERR_ARGTYPE,"Variable::creat();");
        break;
    case _refrence:
        //std::cout << "Here script" << std::endl;
        char *tmp2,*tmp1;
        tmp2 = new char[64];
        if(!tmp2) error(ERR_MEMALLOC,"Vriable::creat(ref);");
        tmp1 = new char[64];
        if(!tmp1) error(ERR_MEMALLOC,"Vriable::creat(ref);");

        sprintf(tmp2,"%.0f",strtod(sp,NULL)-1);

        //std::cout << "head name = "<< head->name << std::endl;

        strcpy(tmp1,::scope);
        strcpy(::scope,tmp2);
        if(type(d) == _refrence)
        {
            varinscope(d,::scope);
            l = strlen(curr->datastr);
        }
        else
            l = strlen(d);


        head->datastr = new char[l+1];
        if(!head->datastr)
            error(ERR_MEMALLOC,"Variable::creat();");

        if(type(d) == _refrence)
        {
            sprintf(tmp2,"%.0f",strtod(sp,NULL)-1);
            varinscope(d,sp);
            //std::cout <<  " Copying r2r " << curr->datastr << std::endl;
            //std::cout << " ref var = " << d <<std::endl;
            strcpy(head->datastr,curr->datastr);
            head->datanum = curr->datanum;
        }
        else
        {
            strcpy(head->datastr,d);				  // Variable name (pointed to)
            head->datanum = strtod(sp,NULL); // Store Scope
        }
        strcpy(::scope,tmp1);
        delete[] tmp2;
        delete[] tmp1;
        break;
    default:
        return 1;
    }
    strcpy(head->Scope,sp);
    head->next = new var;
    if(!head->next)
    {
        error(ERR_MEMALLOC,"Variable::creat();");
        exit(ERR_MEMALLOC);
    }
    head->next->prev = head;
    head = head->next;
    head->next = NULL;
    head->name = NULL;

    head->datastr = NULL;
    head->ip = NULL;
    head->fp = NULL;
    head->dp = NULL;
    head->lp = NULL;
    head->cp = NULL;

    return 0;
}

int Variable::set(char *n,char *d)
{
    int l;

    if(varinscope(n,::scope))
    {
        switch(curr->Type)
        {
        case _num:
            curr->datanum = strtod(d,NULL);
            break;
        case _string:
            delete[] curr->datastr;
            l = strlen(d);
            curr->datastr = new char[l+1];
            if(!curr->datastr)
            {
                error(ERR_MEMALLOC,"Variable::creat();");
                exit(ERR_MEMALLOC);
            }
            strcpy(curr->datastr,d);
            curr->datanum = strlen(curr->datastr);
            break;
        case _bool:
            trim(d);
            ucase(d);
            if(cmp(d,"TRUE"))	curr->databool = TRUE;
            else if(cmp(d,"FALSE")) curr->databool = FALSE;
            else
                error(ERR_VARTYPE,"Variable::set()") ;
            break;
        case _refrence:
            // Store pointer (Variable name)
            char *tmp2,*tmp1;
            tmp2 = new char[64];
            if(!tmp2) error(ERR_MEMALLOC,"Vriable::set(ref);");
            tmp1 = new char[64];
            if(!tmp1) error(ERR_MEMALLOC,"Vriable::set(ref);");


            //std::cout << "head name = "<< head->name << std::endl;


            sprintf(tmp2,"%.0lf",curr->datanum);

            strcpy(tmp1,::scope);
            strcpy(::scope,tmp2);

            l = strlen(d);

            /*varinscope(curr->datastr,::scope); // Get to variable's datastr

            curr->datastr = new char[l+1];
            if(!curr->datastr)
            	error(ERR_MEMALLOC,"Variable::set();");
            */
            set(curr->datastr,d);

            strcpy(::scope,tmp1);

            delete[] tmp2;
            delete[] tmp1;
            break;
        default:
            return 0;
        }
        return 1;
    }
    return 0;	// Unsuccessful
}

int Variable::isvar(char *n)
{
    curr = root;
    setcase(n);
    while(curr->next)
    {
        if(cmp(curr->name,n))
            return 1;
        curr = curr->next;
    }
    return 0;
}

vtype Variable::type(char *n)
{
    if(!varinscope(n,::scope))
    {
        error(ERR_VARDEC,"Variable::type();");
        exit(ERR_VARDEC);
    }

    return curr->Type;
}

void Variable::deletevar(void)
{
    //std::cout << "The way to Hell"<<std::endl;
    var *temp;
    // We gotta delete '@' <):)
    temp = curr;
    if(curr->prev) // YES-@-??
    {
        curr = curr->prev;
        if(temp->next)  //   ...1->@->2->...    1->2->   free(@)
        {
            //std::cout << "1-@-2" << std::endl;
            curr->next = temp->next;
            temp->next->prev = curr;
            if(temp->name) delete[] temp->name;
            if(temp->datastr) delete[] temp->datastr;
            if(temp->ip) delete[] temp->ip;
            if(temp->fp) delete[] temp->fp;
            if(temp->dp) delete[] temp->dp;
            if(temp->lp) delete[] temp->lp;
            if(temp->cp)
                while(--temp->datanum) delete[] temp->cp[(long)temp->datanum];
            delete[] temp->cp;

            delete temp;
        }
        else			// ... 1->@->NULL		free(@)
        {
            // Remember we have already SHIFTED curr back!!
            // SO curr = '1'

            //std::cout << "1-@-NULL" << std::endl;
            if(curr->next->name) delete[] curr->next->name;
            if(curr->next->datastr) delete[] curr->next->datastr;
            if(curr->next->ip) delete[] curr->next->ip;
            if(curr->next->fp) delete[] curr->next->fp;
            if(curr->next->dp) delete[] curr->next->dp;
            if(curr->next->lp) delete[] curr->next->lp;
            if(curr->next->cp)
                while(--curr->next->datanum>=0) delete[] curr->next->cp[(long)curr->next->datanum];
            delete[] curr->next->cp;

            delete curr->next;
            curr->next = NULL;
            head = curr;
        }
    }
    else	// NO-@-?           curr             curr
    {   //       |                |
        if(curr->next) 	// NULL->@->2->...	NULL->2->...  free(@)
        {
            //std::cout << "NULL-@-2" << std::endl;
            curr = curr->next;
            root = curr;
            curr->prev = NULL;

            if(temp->name) delete[] temp->name;
            if(temp->datastr) delete[] temp->datastr;
            if(temp->ip) delete[] temp->ip;
            if(temp->fp) delete[] temp->fp;
            if(temp->dp) delete[] temp->dp;
            if(temp->lp) delete[] temp->lp;

            if(temp->cp)
                while(--temp->datanum>=0) delete[] temp->cp[(long)temp->datanum];
            delete[] temp->cp;

            delete temp;
        }
        else			// NULL->@->NULL
        {
            //std::cout << "NULL-@-NULL" << std::endl;
            curr = root;
            head = root;

            if(curr->name) delete[] curr->name;
            curr->name = NULL;
            temp = curr;
            if(temp->datastr) delete[] temp->datastr;
            if(temp->ip) delete[] temp->ip;
            if(temp->fp) delete[] temp->fp;
            if(temp->dp) delete[] temp->dp;
            if(temp->lp) delete[] temp->lp;

            if(temp->cp)
                while(--temp->datanum>=0) delete[] temp->cp[(long)temp->datanum];
            delete[] temp->cp;

            temp->datastr = NULL;
            temp->ip = NULL;
            temp->fp = NULL;
            temp->dp = NULL;
            temp->lp = NULL;
            temp->cp = NULL;
        }
    }
    //std::cout << "Done deletevar()"<< std::endl;
}

void Variable::deletename(char *n)
{
    curr = root;

    setcase(n);

    while(curr->next)
    {
        if(cmp(curr->name,n)) {
            deletevar();
            break;
        }
        curr = curr->next;
    }
}

void Variable::deletenamescope(char *n,char *sp)
{
    curr = root;

    setcase(n);

    while(curr->next)
    {
        if(cmp(n,curr->name) && cmp(sp,curr->Scope)) {
            deletevar();
            break;
        }
        curr = curr->next;
    }
}

int Variable::deletescope(char *sp)
{
    int cnt=0;
    curr = root;

    //std::cout << "Yehan pey hai 1"<<std::endl;

    while(curr->next)
    {
        //std::cout << "Going to delete " << curr->Scope << " " << sp << std::endl;
        if(cmp(curr->Scope,sp)) {
            deletevar();
            curr = root;
            cnt++;
        }
        if(curr->next)
            curr = curr->next;
    }

    //std::cout << "Kaam ho gaya Boss"<<std::endl;
    return cnt;

}

void Variable::valuescope(char *n,char *s,char format[])
{
    char *tmp;

    tmp = new char[strlen(s)+2];
    if(!tmp) error(ERR_MEMALLOC,"Variable::valuescope");
    //std::cout<<"Here\n";
    strcpy(tmp,::scope);
    strcpy(::scope,s);
    value(n,format);
    strcpy(::scope,tmp);
    //std::cout<<"here2\n";
    delete[] tmp;
    //std::cout<<"here3\n";
}

void Variable::value(char *n,char format[])
{
    if(!varinscope(n,::scope)) error(ERR_VARDEC,"Variable::value()");
    if(curr->Type == _num)
    {
        char *tmp;
        tmp = new char[MAXSIZE];
        if(!tmp) error(ERR_MEMALLOC,"Variable::value();");

        sprintf(tmp,format,curr->datanum);
        val = new char[strlen(tmp)+1];
        strcpy(val,tmp);
        //val[strlen(temp3)] = NULL;
    }
    else if(curr->Type == _refrence)
    {
        char *tmp;
        char *tmp2;

        tmp2 = new char[64];
        if(!tmp2) error(ERR_MEMALLOC,"Variable::value(ref); 5");

        tmp = new char[64];
        if(!tmp) error(ERR_MEMALLOC,"Variable::value(ref); 6");

        sprintf(tmp2,"%.0f",curr->datanum);

        strcpy(tmp,::scope);
        strcpy(::scope,tmp2);

        //std::cout << "Getting value of ->" << curr->datastr << std::endl;
        value(curr->datastr); // get value

        strcpy(::scope,tmp);

        delete[] tmp;
        delete[] tmp2;
    }
    else if(curr->Type == _bool)
    {
        val = new char[6];
        if(curr->databool==TRUE)
            strcpy(val,"TRUE");
        else
            strcpy(val,"FALSE");
    }
    else
    {
        val = new char[(int)(curr->datanum+1)];
        strcpy(val,curr->datastr);
        //val[(int)curr->datanum] = NULL;
    }

}

char *Variable::scope(char *n)
{
    if(isvar(n)) return curr->Scope;
    return NULL;
}

int Variable::varinscope(char *n,char *s)
{
    curr = root;

    setcase(n);

    while(curr->next)
    {
        //std::cout << "["<< curr->name << " ** "<< n << "," <<curr->Scope <<","<< s << "]" << std::endl;
        if(cmp(curr->name,n) && cmp(curr->Scope,s)) return TRUE;

        curr = curr->next;
    }
    return FALSE;
}

int Variable::isarray(char *n,char *sp)
{
    int ret = 0;
    if(varinscope(n,sp))
    {
        switch(curr->Type)
        {
        case _a_integer:
        case _a_float:
        case _a_double:
        case _a_long:
        case _a_string:
        case _a_bool:
            ret = 1;
            break;
        default:
            ret = 0;
        }
    }
    return ret;
}

int Variable::bound(char *n,char *sp)
{
    int ret = 0;
    if(varinscope(n,sp) && isarray(n,sp))
    {
        ret = (int)curr->datanum - 1;
    }
    return ret;
}
Variable::~Variable()
{
    curr = NULL;
    root = NULL;
    while(head->prev)
    {
        head = head->prev;
        if(head->next->name) delete[] head->next->name;
        if(head->next->datastr) delete[] head->next->datastr;
        if(head->next->ip) delete[] head->next->ip;
        if(head->next->fp) delete[] head->next->fp;
        if(head->next->dp) delete[] head->next->dp;
        if(head->next->lp) delete[] head->next->lp;

        delete head->next;
    }
    if(head->name) delete[] head->name;
    if(head->datastr) delete[] head->datastr;
    if(head->ip) delete[] head->ip;
    if(head->fp) delete[] head->fp;
    if(head->dp) delete[] head->dp;
    if(head->lp) delete[] head->lp;

    delete head;
    //delete[] val;
}


