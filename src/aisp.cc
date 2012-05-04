/**************************************************************************************************\
  
 						       |""""""" |"""""""   |"""""""  |"""""""
  						       |     |     |       |     |   |     |   
  						       |  ___|     |       |  ___    |  ___|
  						       |           |             |   |    
  						       |     |     |       |"    |   |  
  						       |     |  |""   ""   | """""   |
  
  								 Amitz Interpreted Script Processor  
  
\***************************************************************************************************/
/*''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``*\
**                             AISP Amitz Interpreted Script Processor							  **
**                              Linux/C++ port of the QBasic version.							  **
**                                  Dated        : 05/August/2001								  **
**                                  Last Updated : 17/December/2001								  **
**                                          Amit Singh											  **
**                                   (c) 2001-2012, Amit Singh (xk0der) 						  **
**                   Please Read the GNU General Public License (GPLv3) at http://www.gnu.org     **
**                               for the licensing terms of this software!						  **
**																								  **
**											Contact me at										  **
**									       amit@xkoder.com										  **
\*''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``''\__/``*/

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
 


#include <iostream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cctype>
#include <ctime>
#include "aisp.h"

	
// Main
int main (int argc, char *argv[])
{
	initialize();				// Initialize the processor
	
	handle_args(argc,argv);		// Handle any command line arguments
	loadais (filename);			// Load the AISP Script file into memory
	preinterpret();				// Perform some preinterpretation processing
	interpret();				// Interpret the Script
	unloadais();				// Unload the script
	
	cleanup();					// Clean up the mess!! ;)
	
	return 0;					// if we've reached here everything has gone fine
}
