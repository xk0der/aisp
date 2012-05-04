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
 
/** Version & Help **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

// Prints Version info
void version(void)
{
	std::cout << APP << " " << VER <<std::endl;
	std::cout << INFO << std::endl;
	std::cout << "Version : " << VERSION << " " << RELEASE << std::endl;
	std::cout << "Build   : " << BUILD <<std::endl;
	std::cout << "(C) 2001 Amit Singh" << std::endl;
	std::cout << "This program is copyright protected under the GNU GPL license" <<std::endl;
	std::cout  << "For more information visit www.gnu.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Contact me at amit@remail.com" << std::endl;
	std::cout << std::endl;
}

// Prints a short help message
void help(void)
{
	std::cout << APP << " " << VER << " " << INFO << std::endl;
	std::cout << "Build : " << BUILD << std::endl;
	std::cout << std::endl;
	std::cout << "Usage..."<<std::endl;
	std::cout << "script [-filename] [option(s)]"<<std::endl;
	std::cout << "'filename' is the name of the script file to interpret" <<std::endl;
	std::cout << "if no extension is supplied '.ais' is appended to the file name!"<<std::endl;
	std::cout << std::endl;
	std::cout << "Option List.."<<std::endl;
	std::cout << "--o		Generate's a temporary script file after doing" << std::endl;
	std::cout << "		the pre-interpretation processing on it!" <<std::endl;
	std::cout << "		Usefull for Debugging"  << std::endl;
	std::cout << "--case [on|off]    on,'1' & true will turn on case sensitivity" << std::endl;
	std::cout << "       [1|0]       off,'0' & false will turn it off (Default)" <<std::endl;
	std::cout << "       [true,false]"<<std::endl;
	std::cout << "--e		emit internals! produces a text file in the current dirctory"<<std::endl;
	std::cout << "		Which contains the script core as it is being processed!"<<std::endl;
	std::cout << "		Usefull for debugging!"<<std::endl;
	std::cout << "--ver		Displays the version number of AISP"<<std::endl;
	std::cout << "--help		Displays this help message"<<std::endl;
	std::cout << std::endl;
	std::cout << "(Under development)"<<std::endl;
	std::cout << "Please report bugs & crashes to amit@remail.com" << std::endl;
	std::cout << std::endl;
}
