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

// Processing configuration scripts
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// These are the Pre-defined TERMS
char *predefined_terms[]=
{
    "__VER__",VER,					// AISP version number (Shortform)
    "__VERSION__",VERSION,			// AISP version number (Full form)
    "__APP__",APP,					// Application Name
    "__RELEASE__",RELEASE,			// Release type (Alpha, Beta etc..)
    "__INFO__",INFO,				// Information about the APPLICATION
    "__BUILD__",BUILD, 				// Date & Time stamp, when the code was compiled
    "@@","NULL"						// LIST Terminating character
};


char *bleft(char *,char *);
char *bright(char *,char *);

int config_present;				// TRUE if configuration file present
char *include_dir;
char *source_dir;

char conf_file[255];

// Initialize AISP before loading and interpretting a script
void initialize(void)
{
    char temp[255];
    char left[255],right[255];
    config_present = FALSE;

    if(!getenv("HOME")) error(ERR_UNKNOWN,"env HOME not defind!");

    strcpy(conf_file,getenv("HOME"));
    strcat(conf_file,INIT_DIR);
    strcat(conf_file,"/");
    strcat(conf_file,CONFIG_FILE);

    include_dir=NULL;
    source_dir=NULL;

    // Add predefined TERMS
    int i=0;
    while(!cmp(predefined_terms[i],"@@"))
    {
        term.add(predefined_terms[i],0,predefined_terms[i+1]);
        i+=2;
    }

    // Try to open the Configuration file for reading!
    std::ifstream conf(conf_file);

    if(conf)	// if this is successfull that means there exists a configuration file
    {
        config_present = TRUE;

        // Read the file and set apropriate Global vars
        while(conf.getline(temp,254))
        {
            trim(temp);
            if(temp[0]==';') continue;		// line beginning with ';' are ignored
            // Can be used for comments within the config file
            // Get the left and right portion of the expression exp1=exp2
            bleft(temp,left);
            bright(temp,right);

            if(cmp(left,"include"))			// specifies the !include() directory
            {
                include_dir = new char[strlen(right)+1];
                strcpy(include_dir,right);
            }
            else if(cmp(left,"source"))		// specifies the directory containg script files
            {
                source_dir = new char[strlen(right)+1];
                strcpy(source_dir,right);
            }
            else if(cmp(left,"define"))		// !define()'s a TERM
            {
                bleft(right,left);
                stemp3(100);
                bright(right,temp3);
                term.add(left,0,temp3);
                stemp3(1);
            }
            else if(cmp(left,"undef"))		// !undef()ines's a TERM
            {
                term.deleteterm(right);
            }
        }
    }
}

// from a expression of type
// exp1=exp2
// Breaks it and returns the left one i.e. 'exp1'
char *bleft(char *str1,char *str2)
{
    int i=instr(str1,"=");
    scopy(str1,str2,0,i-1);
    return str2;
}

// from a expression of type
// exp1=exp2
// Breaks it and returns the left one i.e. 'exp2'
char *bright(char *str1,char *str2)
{
    int i=instr(str1,"=");
    scopy(str1,str2,i+1,strlen(str1)-1);
    return str2;
}
