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
 
// Make default configuration file
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

void makeconfig(void)
{
	int makeconf=FALSE;
	char ch;
		
	// OS Specific commands
	stemp3(255);
	strcpy(temp3,"mkdir ");	// Will RUN ONLY on DOS(WinXX) & Linux
	
	if(!getenv("HOME"))		//error(ERR_UNKNOWN,"env HOME not def");
	{
			std::cout << "HOME environment variable not present" << std::endl;
			std::cout << "Shall I use the current directory?" << std::endl;
			std::cout << "If you answer yes a directory named '" << INIT_DIR <<"' will be created in the" <<std::endl;
			std::cout << "Present directory and the configuration file will be saved there!"<<std::endl;
			std::cout << "Are you sure you want to continue ? (y/N) :";
			ch = getchar();	}
	
	strcat(temp3,getenv("HOME"));
	strcat(temp3,INIT_DIR);
	system(temp3);
	stemp3(2);
	// ..ends!
	
	std::fstream conf(conf_file,std::ios::in);
	if(conf)
	{		std::cout << std::endl << "\a" << conf_file <<" file already present at";
		std::cout <<" " << INIT_DIR << std::endl;
		std::cout << "Do you want to overwrite it? (y/N) : ";
		std::cin >> ch;
		if(ch=='Y' || ch=='y')
			makeconf = TRUE;	}
	else
		makeconf = TRUE;
	
	conf.close();
	if(makeconf)
	{
		conf.open(conf_file,std::ios::out);
		if(!conf)
			error(ERR_OPEN,conf_file);
			
				conf << "; " << APP << " " <<INFO <<std::endl;
		conf << "; Version : " << VERSION << "\tRelease:" << RELEASE <<std::endl;
		conf << "; Build   : " << BUILD <<std::endl;
		conf << ";" <<std::endl;
		conf << "; This file was automatically generated using the --makeconf comand line switch" <<std::endl;
		conf << "; " << std::endl;
		conf << "; This file contains configuration information for AISP"<<std::endl;
		conf << "; general format is exp1=exp2"<<std::endl;
		conf << "; This will set exp1 to the value exp2"<<std::endl;
		conf << "; Line beginning with semicolon are ingnored and can be use for"<<std::endl;
		conf << "; inserting comments." <<std::endl;
		conf << std::endl;
		conf << "[DIRECTORIES]"<<std::endl;
		conf << "include="<<getenv("HOME")<<INIT_DIR<<"/include"<<std::endl;
		conf << "source=."<<std::endl;
		conf << std::endl;
		conf << "[DEFAULT]" <<std::endl;
		conf << "CASE=OFF"<<std::endl;
		conf << std::endl;
		conf << "; Configuration Script ends"<<std::endl;
		conf.close();
		
		std::cout << "Configuration file " << CONFIG_FILE << " generated at " << getenv("HOME") <<INIT_DIR << std::endl;
	}
	else
		std::cout << "Unable to generate " << conf_file <<std::endl;
	conf.close();
	
	
}
