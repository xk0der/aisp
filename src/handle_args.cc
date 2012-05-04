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
 
/** Handle Command Line Arguments **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

void handle_args(int argc,char *argv[])
{
		
	int i;
	filename = NULL;
	char *tmp;
	int MakeDebugFlag = NOTSET;
	
	if(argc>=2)
	{
		for(i=1;i<argc;i++)
		{
			if(instr(argv[i],"-")==-1 && instr(argv[i],"--")==-1 && !filename)
			{
				filename = new char[strlen(argv[i])+1+4]; // +4 for .AIS
				if(!filename) {
					error(ERR_MEMALLOC,"main();");
					exit(ERR_MEMALLOC);
				}	
				scopy(argv[i],filename,0,strlen(argv[i])-1);
			}
			if(instr(argv[i],"--ver")!=-1)
			{
				version();
				exit(0);
			}
			if(instr(argv[i],"--help")!=-1)
			{
				help();
				exit(0);			
			}
			if(instr(argv[i],"--o")!=-1)
			{
				MakeDebugFlag = SET;			}
			
			if(instr(argv[i],"--e")!=-1)
			{
				std::fstream efp("~aisp_emit.txt",std::ios::out);
				if(!efp) error(ERR_OPEN,"~aisp_emit.txt, handle_args.cc(--e)");
				
				efp << "This file '~aisp_emit.txt' was automatically generated using the --e switch" <<std::endl;
				efp << "This file contains the script core as it executes\nIt may be helfull for debugging"<<std::endl;
				efp << APP << " " << INFO << " " << VER <<std::endl;
				efp << "Build : "<< BUILD <<std::endl;
				
				
				std::cout << "Emmiting ON, emmited data in file ~aisp_emit.txt" <<std::endl;
				efp.close();
				emit = TRUE;			}
			
			if(instr(argv[i],"--makeconf")!=-1)
			{	
				makeconfig();
				exit(0);
			}	
			
			if(instr(argv[i],"--t")!=-1)
			{
				version();
				std::cout << std::endl;
				creat_manual_template();
				std::cout << "Template created 'template.txt'" << std::endl;
				exit(0);			}
			
            if(instr(argv[i],"--include") != -1 || instr(argv[i], "-I") != -1)
			{
				if(i+1 >= argc)
				{
					error(ERR_ARGCNT, "--include");
					exit(1);				
                }
				trim(argv[i + 1]);
				include_dir = new char[strlen(argv[i + 1] + 1)];
                strcpy(include_dir, argv[i + 1]);
                i++;
            }
			
			if(instr(argv[i],"--case")!=-1)
			{
				if(i+1>=argc)
				{
					error(ERR_ARGCNT,"--case");
					exit(1);				
                }
				trim(argv[i+1]);
				
				if(cmp(argv[i+1],"1") || cmp(argv[i+1],"on") || cmp(argv[i+1],"true"))
					case_in_use = 1;
				else if(cmp(argv[i+1],"0") || cmp(argv[i+1],"off") || cmp(argv[i+1],"false"))
					case_in_use = 0;
				else
				{
					error(ERR_INVALIDARG,"--case");
					exit(ERR_INVALIDARG);				
                }

                i++;
             }
		}
	}
	
	if (!filename)
	{
		//stemp3(255);
		tmp = new char[255];
		
		version();
		
		std::cout << "Enter Filename :";
		std::cin.getline(tmp,254);
		filename = new char[strlen(tmp)+1+6];
	
		if(!filename) 
			error(ERR_MEMALLOC,"main();2");
	
		scopy(tmp,filename,0,strlen(tmp)-1);
	}

	if(instr(filename,".")==-1)
		sprintf(filename,"%s.ais",filename);
	
	//std::cout << "File name string length = " << strlen(filename) <<  "("<<  filename<< ")" << std::endl;
	//tmpi = getchar();
	
	if(MakeDebugFlag)
	{
		std::cout << "Generating Debug file.." <<std::endl;
		makedebugtemp(filename);
		std::cout << "Debugging file generated :" << TEMPDEBUG << std::endl;
		exit(0);	}
	
}
