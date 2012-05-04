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

/** Variable Class Header **/

#if !defined(_VARIABLE_CLASS_H)
#define _VARIABLE_CLASS_H

enum vtype {_num,_string,_bool,_refrence,
            _integer,_float,_double,_long,
            _a_string,
            _a_integer,
            _a_float,
            _a_double,
            _a_bool,
            _a_long,
            ____,// 4 _
           };		// Variable types


class Variable
{
    struct var
    {
        char *name;			// Name of Varibale
        double datanum;		// Number Data	-- Stores string length in case of a string variable
        char *datastr;			// String Data
        char databool;			// Boolean Data

        // Array storage area
        int *ip;					// Array of .... integers
        char **cp;				//					String, Also used to store Boolean Values
        float *fp;				//					float
        double *dp;			//					double
        long int *lp;			//					long int
        //

        vtype Type;				// Type of Data stored
        char Scope[64];			// Variable Scope -- DEFAULT is global
        var *next;
        var *prev;
    };
    var *root;
    var *curr;
    var *head;
public:
    Variable();
    int creat_new(char *n,vtype t, int size, char *sp);
    int set_new(char *n,long index,char *d);
    int value_new(char *n,int index, char *sp,char *format);
    int creat(char *n,vtype t,char *d,char *sp);
    int set(char *n,char *d);
    int isvar(char *n);
    vtype type(char *n);
    void deletevar(void);
    void deletename(char *n);
    void deletenamescope(char *n,char *sp);
    int deletescope(char *sp);
    void valuescope(char *n,char *s,char format[]="%.0lf");
    void value(char *n,char format[]="%.0lf");
    char *scope(char *n);
    int varinscope(char *n,char *s);
    int isarray(char *n,char *sp);
    int bound(char *n,char *sp);
    ~Variable();
};

#endif
