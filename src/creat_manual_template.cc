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

/** Creat Manual Template **/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

#define MAX_ADP	100

void creat_manual_template(void)
{
	int cnt,l;
	char tadp[255];
	char tmp[255];
	std::fstream adp;
	std::fstream file("template.txt", std::ios::out);

	if(!file) 
	{
		std::cout << "Error creating file template.txt"<<std::endl;
		exit(2);
	}

	char tokens[___][64];
	int i,j;

	// copy
	for(i=0;i<___;i++)
		strcpy(tokens[i],tstr[i]);


	// Sort	
	for(i=___-2;i>0;i--)
	{
		for(j=0;j<=i;j++)
		{
			//std::cout << ">> " << tokens[j] << " , " << tokens[j+1];
			if(cmpx(tokens[j],tokens[j+1])>0)
			{
				//std::cout << "  cmp = " <<cmpx(tokens[j],tokens[j+1]);
				strcpy(xtempx,tokens[j]);
				strcpy(tokens[j],tokens[j+1]);
				strcpy(tokens[j+1],xtempx);			
			}
			//std::cout << std::endl;		
		}	
	}

	// Create Template
	file <<"		       |\"\"\"\"\"\"\" |\"\"\"\"\"\"\"   |\"\"\"\"\"\"\"  |\"\"\"\"\"\"\""<<std::endl;
	file <<"		       |     |     |       |     |   |     |   "<<std::endl;
	file <<"	 	       |  ___|     |       |  ___    |  ___|"<<std::endl;
	file <<"	  	       |           |             |   |    "<<std::endl;
	file << " 	 	       |     |     |       |\"    |   |  "<<std::endl;
	file <<"  		       |     |  |\"\"   \"\"   | \"\"\"\"\"   |"<<std::endl;
	file<<std::endl;
	file<<  "			Amitz Interpreted Script Processor  "<<std::endl;
	file << "			=================================="<<std::endl;

	file << APP <<  " " << VER<<std::endl;
	file << INFO << std::endl;
	file << "Version "<< VERSION << ", "<<  RELEASE << " Release" << std::endl;
	file << "Build   "<< BUILD <<std::endl;
	file << "(C) Amit Singh under GNU General Public License"<<std::endl;
	file << "Please visit www.gnu.org for more information"<<std::endl;
	file << std::endl;
	file << "Manual Version 0.0.1" << std::endl;
	file <<"--------------------------------------------------------------------------------"<<std::endl;
	file << std::endl;

	// ** add AISP Document Pieces files (.adp) 1-MAX_ADP (0 has been reserved)
	std::cout << "Adding beginning sections .adp files. . ." <<std::endl;
	for(cnt=1;cnt<=MAX_ADP;cnt++)
	{
		sprintf(tadp,"%ia.adp",cnt);
		adp.open(tadp,std::ios::in);
		if(adp)
		{
			std::cout << tadp;
			l=0;
			while(adp.getline(tmp,254))
			{		
				l++;//std::cout << ".";
				file << tmp << std::endl;
			}			std::cout << " " << l <<" line(s) Added"<<std::endl;
			file <<std::endl;		}
		else
			break;

		adp.close();
	}	// ** end nn.adp

	file << "Token Listing with Description"<<std::endl;
	file << "'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`"<<std::endl;

	std::cout << std::endl << "Adding Token .adp files"<<std::endl;
	for(i=0;i<___;i++)
	{
		sprintf(tadp,"%s.adp",tokens[i]);
		adp.close();
		adp.open(tadp,std::ios::in);
		if(adp) std::cout << tokens[i] <<".adp ";

		file << "["<< i+1 << "] @" << tokens[i] << "()" << std::endl;
		file <<"\t- Syntax      : ";
		if(adp)
		{
			cnt=0;
			adp.getline(tmp,254);
			while(instr(tmp,"<end>")==-1)
			{
				if(cnt>0)file <<"\t                ";
				file << tmp <<std::endl;
				cnt++;
				adp.getline(tmp,254);			
			}		
		}
		else
			file <<std::endl;

		file <<"\t- Discription : ";
		if(adp)
		{
			cnt=0;
			adp.getline(tmp,254);
			while(instr(tmp,"<end>")==-1)
			{
				if(cnt>0)file <<"\t                ";
				file << tmp <<std::endl;
				cnt++;
				adp.getline(tmp,254);			
			}		
		}
		else
			file <<std::endl;

		file <<"\t- Returns     : ";
		if(adp)
		{
			cnt=0;
			adp.getline(tmp,254);
			while(instr(tmp,"<eof>")==-1)
			{
				if(instr(tmp,"<end>")!=-1)
				{
					file <<"\t* Note        : ";
					cnt = 0;
				}				
				if(cnt>0)file <<"\t                ";
				file << tmp <<std::endl;
				cnt++;
				adp.getline(tmp,254);			
			}		
		}
		else
			file <<std::endl;

		file << std::endl;
		file << std::endl;

		adp.close();	
	}

	// ** add AISP Document Pieces files (.adp) 1-MAX_ADP (0 has been reserved)
	std::cout << std::endl << std::endl << "Adding ending sections .adp files. . ." <<std::endl;
	for(cnt=1;cnt<=MAX_ADP;cnt++)
	{
		sprintf(tadp,"%ie.adp",cnt);
		adp.open(tadp,std::ios::in);
		if(adp)
		{
			std::cout <<tadp;
			l= 0;
			while(adp.getline(tmp,254))
			{		
				l++; //std::cout << ".";
				file << tmp << std::endl;
			}			std::cout << " " <<l<<" line(s) Added"<<std::endl;
			//std::cout << "Added"<<std::endl;
			file <<std::endl;		
		}
		else
			break;

		adp.close();
	}


	file << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-="<<std::endl;
	file << "File ends \t"<< APP <<  " " << VER << std::endl;
	file << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-="<<std::endl;

	file.close();
	std::cout <<std::endl;
}

