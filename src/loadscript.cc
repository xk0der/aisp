/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
/* Load scripts */
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"

char	**script;		// Points to the script loaded in the memory
long	total_lines;	// Total number of lines in the script file

// Used by maketemp(); and makedebugtemp(); functions
char xstrx[255];
char xtempx[255];

void loadais(char *filename)
{
	char str[255];
	total_lines = 0;
	script = NULL;

	int i=0;
	
	maketemp(filename);

	std::fstream sf(TEMPFILE,std::ios::in);
	if(!sf)
	{
		error(ERR_OPEN,"loadfile();");
		exit(0);	
	}
	
	while(sf.getline(str,254))
	{
		trim(str);
		if(instr(str,"@")!=-1  || str[0]==':' || str[0]=='!')
		{	
				total_lines++;
		}	
	}
		
	sf.close();
		
	sf.open(TEMPFILE,std::ios::in);
	if(!sf)
	{
		error(ERR_OPEN,"loadais(); 2");
		exit(0);
	}
	
	
	script = new char*[total_lines];
	if(!script)
	{
		error(ERR_MEMALLOC,"loadfile()");
		exit(1);
	}	
	
	while(sf.getline(str,254))
	{
		trim(str);
		if(instr(str,"@")!=-1 || str[0]==':' || str[0]=='!')
		{
			*(script+i) = new char[strlen(str)+1];
			if(!(*(script+i)))
			{
				error(ERR_MEMALLOC,"loadfile() 2");
				exit(ERR_MEMALLOC);
			}
			strcpy(script[i],str);
			i++;
		}	
	}
	
	//int i;
	//std::cout << "Complete script ... " << std::endl;
	//for(i=0;i<total_lines;i++)
	//		std::cout << script[i] << std::endl;
	//tmpi = getchar();
	
	
	sf.close();
	remove(TEMPFILE);
}

void unloadais(void)
{
	int i;
	for(i=total_lines-1;i>=0;i--)
		delete[] script[i];
	delete[] script;
	script = NULL;
}

// Makes a temporary file
void maketemp(char const *fn)
{
	int flag = NOTSET;
	int waselse = NOTSET;
	int tempi;
	char _xstrx[255];
	
	int towrite=1;
	int count = 0;
	
	// Last File Name, Line where jumped into include file
	char *file;
	char fname[MAXINCLEVEL][255]; int lineno[MAXINCLEVEL];
	int rec=0;
	
	int inif=0;
	
	int done=FALSE; // we are not done yet!!
	int linec=0;
	
	file = new char[strlen(fn)+1];
	if(!file) error(ERR_MEMALLOC,"maketemp();");
	strcpy(file,fn);	
	
	std::fstream sf(file,std::ios::in);
	if(!sf)
	{
		std::cout << "In maketemp:Error opening script file " << file << std::endl;
		exit(1);	}
	
	std::fstream tf(TEMPFILE,std::ios::out);	
	if(!tf)
	{
		std::cout << "In maketemp:Error opening temporary file " << TEMPFILE << std::endl;
		exit(1);	}
	
	while(!done)
	{
		flag = NOTSET;
		waselse = NOTSET;
		if(sf.getline(xstrx,254))
		{			linec++;
			trim(xstrx);
			
			//std::cout << "Read >" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
			
			if(xstrx[0]=='!' && instr(xstrx,"!define")!=-1)
			{				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(instr(xtempx,",")!=-1)
				{
					scopy(xstrx,xtempx,instr(xstrx,"(")+1,instr(xstrx,",")-1);
					trim(xtempx);
					scopy(xstrx,_xstrx,instr(xstrx,",")+1,rinstr(xstrx,")")-1);				}
					
				if(term.isterm(xtempx))
				{	
					std::cout << "While processing :"<< xstrx << std::endl;
					error(ERR_TERMPRESENT,"maketemp();",linec);
				}	
				
				if(instr(xstrx,",")!=-1)
					term.add(xtempx,0,_xstrx);
				else
					term.add(xtempx,0);
				
				continue;
			}
			
			if(xstrx[0]=='!' && instr(xstrx,"!undef")!=-1)
			{				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(!term.isterm(xtempx))
				{
					std::cout << "While processing :"<< xstrx << std::endl;
					error(ERR_TERMDEC,"maketemp();",linec);
				}	
				
				term.deleteterm(xtempx);
				continue;
			}

			if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1){ 	inif--; continue; }
			
			if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1)
			{
				inif--;
				tempi = 1;
				while(sf.getline(xstrx,254))
				{
					trim(xstrx);
					//std::cout << "!else()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
					if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) tempi++;
					if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1) tempi--;
					if(tempi == 0){ flag = SET; break;}
					if(inif<0) break;					}
				
				if(flag) continue;
							}
			
			if(xstrx[0]=='!' && instr(xstrx,"!ifdef")!=-1)
			{				tempi = inif;
				inif++;
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(!term.isterm(xtempx))
				{
					while(sf.getline(xstrx,254))
					{
						trim(xstrx);
						//std::cout << "!ifdef()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
						if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) inif++;
						if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1){ waselse = SET; inif--; }
						if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1) 
						{
							if(!waselse) inif--;
							waselse = NOTSET;
						}
						if(inif == tempi)
						{ 
							if(waselse) inif++;
							flag = SET; break; 
						}
						if(inif<0) break;						}
					if(flag) continue;
				}
			}

			if(xstrx[0]=='!' && instr(xstrx,"!ifndef")!=-1)
			{				tempi = inif;
				inif++;
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(term.isterm(xtempx))
				{
					while(sf.getline(xstrx,254))
					{
						trim(xstrx);
						//std::cout << "!ifndef()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
						if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) inif++;
						if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1){waselse = SET; inif--;}
						if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1)
						{ 
							if(!waselse) inif--;
							waselse = NOTSET;	
						}	
						
						if(inif == tempi)
						{ 
							if(waselse) inif++;
							flag = SET; break;
						}
						if(inif<0) break;						}
					
					if(flag) continue;				}
			}
		
			
			if(inif<0)
			{	
				std::cout<< "While processing :" << xstrx << std::endl;
				error(ERR_P_MISSINGIF,"maketemp();");
			}	
			
			/** Handle Switch Case **/			
			if(instr(xstrx,"@switch")!=-1)	
			{
				stack.push((long)towrite);
				count++;
				towrite = count;
								
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,strlen(xstrx));
				scopy(xstrx,_xstrx,0,instr(xstrx,"@")-1);
				sprintf(xstrx,"%s@switch(%i %s",_xstrx,towrite,xtempx);
			}	
			if(instr(xstrx,"@endswitch")!=-1)	towrite = stack.pop();
			
			if(instr(xstrx,":@case")!=-1)
			{
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,strlen(xstrx));
				scopy(xstrx,_xstrx,0,instr(xstrx,":@case")-1);
				sprintf(xstrx,"%s:@case(%i %s",_xstrx,towrite,xtempx);
				
			}
			if(instr(xstrx,":@default")!=-1)
			{
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,strlen(xstrx));
				scopy(xstrx,_xstrx,0,instr(xstrx,":@default")-1);
				sprintf(xstrx,"%s:@default(%i%s",_xstrx,towrite,xtempx);
			}
			/** Switch Case Done */
			
			if(xstrx[0]=='!' && instr(xstrx,"!include")!=-1)
			{
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				
				trim(xtempx);
				
				// Push
				strcpy(fname[rec],file);
				lineno[rec]=linec;
				rec++;
				//
				
				if(rec>=MAXINCLEVEL)
				{	
					sf.close();
					tf.close();
					delete[] file;
					remove(TEMPFILE);
					error(ERR_INCLUDEMAX,"maketemp();");
				}	
				
				sf.close();
				delete[] file;
				
				sf.open(xtempx,std::ios::in);
				if(!sf)
				{
					sf.close();
						if(include_dir)
							make_include_file(xtempx);				
				}					
				sf.close();
				
				file = new char[strlen(xtempx)+1];
				if(!file) error(ERR_MEMALLOC,"maketemp();");
				strcpy(file,xtempx);
				
				sf.open(file,std::ios::in);
				if(!sf)
				{
					std::cout << "In maketemp:Error opening script file " << file << std::endl;
					exit(1);				
				}
				linec = 0;
			}
			else //Something other than '!include'
			{
				if(instr(xstrx,"@")!=-1 || xstrx[0]==':')
					//|| xstrx[0]=='!')
				{		
					//if(xstrx[0]!=':') tf<<"@MAD(";
					tf << xstrx << std::endl;									// Write to file if script code!
					//if(xstrx[0]!=':') tf << ')';
					//tf << std::endl;					
				}
			}
		}
		else // Reading from file failed!
		{
			sf.close();
			rec--;
			if(rec<0){ done = TRUE; break; } // We are done now!!
			
			delete[] file;
			file = new char[strlen(fname[rec])+1];
			if(!file) error(ERR_MEMALLOC,"maketemp();");
			
			// pop
			strcpy(file,fname[rec]);
			//
			sf.open(file,std::ios::in);
			if(!sf)
			{
				std::cout << "In maketemp:Error opening script file " << file << std::endl;
				exit(1);			}
			
			linec = 0;
			while(sf.getline(xstrx,254))
			{
				linec++;
				if(linec>=lineno[rec]) break;
			}
					}
	}
	
	if(inif>0)
	{
		std::cout<< "While processing :" << xstrx << std::endl;
		error(ERR_P_MISSINGENDIF,"maketemp();");
	}	
	
	sf.close();
	tf.close();
	delete[] file;}

// Outputs a temporary file for debugging
void makedebugtemp(char const *fn)
{
	int flag = NOTSET;
	int waselse = NOTSET;
	int tempi;
	
	// @switch
	int  towrite = 1;
	int count = 0;
	//
	
	char _xstrx[255];
	// Last File Name,Line where jumped into include file
	char *file;
	char fname[MAXINCLEVEL][255]; int lineno[MAXINCLEVEL];
	int rec=0;
	
	int inif = 0;
	
	int done=FALSE; // we are not done yet!!
	int linec=0;
	int cnt = 0;
	
	file = new char[strlen(fn)+1];
	if(!file) error(ERR_MEMALLOC,"makedebugtemp();");
	strcpy(file,fn);	
	
	std::fstream sf(file,std::ios::in);
	if(!sf)
	{
		std::cout << "In makedebugtemp:Error opening script file " << file << std::endl;
		exit(1);	}
	
	std::fstream tf(TEMPDEBUG,std::ios::out);	
	if(!tf)
	{
		std::cout << "In makedebugtemp:Error opening temporary file " << TEMPFILE << std::endl;
		exit(1);	}
	
	tf << "This file " << TEMPDEBUG << " is automaticaly generated using the switch --o"<<std::endl;
	tf << "You can use it to debug your scripts!" << std::endl;
	tf << "The line number outputted when an error occurs is printed on the first column"<<std::endl;
	tf << APP << " " << VER  << std::endl;
	tf << "Build : " << BUILD << std::endl;
	tf << std::endl;	
	
	tf << "------- contents of file : " << file << " -----------" << std::endl;
		
	while(!done)
	{
		flag = NOTSET;
		waselse = NOTSET;
		if(sf.getline(xstrx,254))
		{			linec++;
			strcpy(_xstrx,xstrx);
			trim(xstrx);
			
			//std::cout <<"["<< _xstrx <<"]"<< std::endl ;
			
			/** Pre-Interpret Start **/
			
			if(xstrx[0]=='!' && instr(xstrx,"!define")!=-1)
			{				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				
				if(instr(xtempx,",")!=-1)
				{
					scopy(xstrx,xtempx,instr(xstrx,"(")+1,instr(xstrx,",")-1);
					trim(xtempx);
				}
				
				if(term.isterm(xtempx))
				{	
					std::cout << "While processing :"<< xstrx << std::endl;
					error(ERR_TERMPRESENT,"makedebugtemp();",linec);
				}	
				
				term.add(xtempx,0);
				continue;
			}
			
			if(xstrx[0]=='!' && instr(xstrx,"!undef")!=-1)
			{				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(!term.isterm(xtempx))
				{
					std::cout << "While processing :"<< xstrx << std::endl;
					error(ERR_TERMDEC,"makedebugtemp();",linec);
				}	
				
				term.deleteterm(xtempx);
				continue;
			}

			if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1){ 	inif--; continue; }
			
			if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1)
			{
				inif--;
				tempi = 1;
				while(sf.getline(xstrx,254))
				{
					trim(xstrx);
					//std::cout << "!else()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
					if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) tempi++;
					if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1) tempi--;
					if(tempi == 0){ flag = SET; break;}
					if(inif<0) break;					}
				
				if(flag) continue;
							}
			
			if(xstrx[0]=='!' && instr(xstrx,"!ifdef")!=-1)
			{				tempi = inif;
				inif++;
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(!term.isterm(xtempx))
				{
					while(sf.getline(xstrx,254))
					{
						trim(xstrx);
						//std::cout << "!ifdef()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
						if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) inif++;
						if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1){ waselse = SET; inif--; }
						if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1) 
						{
							if(!waselse) inif--;
							waselse = NOTSET;
						}
						if(inif == tempi)
						{ 
							if(waselse) inif++;
							flag = SET; break; 
						}
						if(inif<0) break;						}
					if(flag) continue;
				}
			}

			if(xstrx[0]=='!' && instr(xstrx,"!ifndef")!=-1)
			{				tempi = inif;
				inif++;
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				trim(xtempx);
				if(term.isterm(xtempx))
				{
					while(sf.getline(xstrx,254))
					{
						trim(xstrx);
						//std::cout << "!ifndef()>" <<xstrx << " > " <<inif <<","<<tempi << std::endl;
						if(xstrx[0]=='!' && (instr(xstrx,"!ifdef")!=-1 || instr(xstrx,"!ifndef")!=-1)) inif++;
						if(xstrx[0]=='!' && instr(xstrx,"!else")!=-1){waselse = SET; inif--;}
						if(xstrx[0]=='!' && instr(xstrx,"!endif")!=-1)
						{ 
							if(!waselse) inif--;
							waselse = NOTSET;	
						}	
						
						if(inif == tempi)
						{ 
							if(waselse) inif++;
							flag = SET; break;
						}
						if(inif<0) break;						}
					
					if(flag) continue;				}
			}
		
			
			if(inif<0)
			{	
				std::cout<< "While processing :" << xstrx << std::endl;
				error(ERR_P_MISSINGIF,"makedebugtemp();");
			}	
			
			/** Handle Switch Case **/			
			if(instr(xstrx,"@switch")!=-1)	
			{
				stack.push((long)towrite);
				count++;
				towrite = count;
								
				scopy(_xstrx,xtempx,instr(_xstrx,"(")+1,strlen(_xstrx));
				scopy(_xstrx,xstrx,0,instr(_xstrx,"@")-1);
				sprintf(_xstrx,"%s@switch(%i %s",xstrx,towrite,xtempx);
				
				strcpy(xstrx,_xstrx);
				trim(xstrx);
			}	
			if(instr(xstrx,"@endswitch")!=-1)	towrite = stack.pop();
			
			if(instr(xstrx,":@case")!=-1)
			{
				scopy(_xstrx,xtempx,instr(_xstrx,"(")+1,strlen(_xstrx));
				scopy(_xstrx,xstrx,0,instr(_xstrx,":@case")-1);
				sprintf(_xstrx,"%s:@case(%i %s",xstrx,towrite,xtempx);
				
				strcpy(xstrx,_xstrx);
				trim(xstrx);
			}
			if(instr(xstrx,":@default")!=-1)
			{
				scopy(_xstrx,xtempx,instr(_xstrx,"(")+1,strlen(_xstrx));
				scopy(_xstrx,xstrx,0,instr(_xstrx,":@default")-1);
				sprintf(_xstrx,"%s:@default(%i%s",xstrx,towrite,xtempx);
				
				strcpy(xstrx,_xstrx);
				trim(xstrx);
			}
			/** Switch Case Done */
			
			
			/** Pre-Interpret Done **/
			
			if(xstrx[0]=='!' && instr(xstrx,"!include")!=-1)
			{
				scopy(xstrx,xtempx,instr(xstrx,"(")+1,rinstr(xstrx,")")-1);
				
				trim(xtempx);
				// Push
				strcpy(fname[rec],file);
				lineno[rec]=linec;
				rec++;
				//
				
				if(rec>=MAXINCLEVEL)
				{	
					sf.close();
					tf.close();
					delete[] file;
					remove(TEMPFILE);
					error(ERR_INCLUDEMAX,"makedebugtemp();");
				}	
				
				sf.close();
				delete[] file;
				
				sf.open(xtempx,std::ios::in);
				if(!sf)
				{
						if(include_dir)
							make_include_file(xtempx);				}
				sf.close();
					
				file = new char[strlen(xtempx)+1];
				if(!file) error(ERR_MEMALLOC,"makedebugtemp();");
				strcpy(file,xtempx);
				
				sf.open(file,std::ios::in);
				if(!sf)
				{
					std::cout << "In makedebugtemp:Error opening script file " << file << std::endl;
					exit(1);				}
				tf << "------- Including file : " << file << " -----------" << std::endl;
				linec = 0;
			}
			else //Something other than '!include'
			{
				if(instr(xstrx,"@")!=-1 || xstrx[0]==':' )
					//|| xstrx[0]=='!')
				{
					tf << cnt <<" " <<_xstrx << std::endl;									
					cnt++;				}
				else
					if(xstrx[0]!='!') tf << " " <<_xstrx << std::endl;									
			}
		}
		else // Reading from file failed!
		{
			tf << "------- file : " << file << " -- Ends -----------" << std::endl <<std::endl;
			sf.close();
			rec--;
			if(rec<0){ done = TRUE; break; } // We are done now!!
			
			delete[] file;
			file = new char[strlen(fname[rec])+1];
			if(!file) error(ERR_MEMALLOC,"makedebugtemp();");
			
			// pop
			strcpy(file,fname[rec]);
			//
			sf.open(file,std::ios::in);
			if(!sf)
			{
				std::cout << "In makedebugtemp:Error opening script file " << file << std::endl;
				exit(1);			}
			
			linec = 0;
			while(sf.getline(xstrx,254))
			{
				linec++;
				if(linec>=lineno[rec]) break;
			}
					}
	}
		
	if(inif>0) error(ERR_P_MISSINGENDIF,"makedebugtemp();");
	
	sf.close();
	tf.close();
	delete[] file;
}

void preinterpret(void)
{
	char *tmp;
	int i;
		
	for(i=0;i<total_lines;i++)
	{
		if(script[i]==NULL) // Handle Error conditions!!
		{
			script[i] = new char[2];
			if(!script[i]) error(ERR_MEMALLOC,"preinterpret();");
				
			script[i][0]=' ';
			script[i][1]=0x00;
			continue;		}
		if(script[i][0]==':') // We found a label
		{
			tmp = new char[strlen(script[i])+2];
			//stemp3(strlen(script[i])+1);
			scopy(script[i],tmp,1,strlen(script[i]));
			label.add(tmp,i);
			//std::cout << temp3 << std::endl;
			delete[] tmp;
		}	
	}
}

char *make_include_file(char *filename)
{
	char temp[255];
	strcpy(temp,include_dir);
	strcat(temp,"/");
	strcat(temp,filename);
	strcpy(filename,temp);
	return filename;}
