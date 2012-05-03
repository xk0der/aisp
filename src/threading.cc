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
 
// Threading
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "aisp.h"
/*
	* Thread invoked 
			- Save scope & line#
			- get a new scope
			- execute the code for some time
	* Thread put to sleep (transfer control to other thread!)
			- Save old threads scope & line#
			- Don't delete the old thread scope's vars
			- set scope to new one
			- execute the script code for some time
	* Use the above two steps to execute threads and main code
	* A Thread gets over
			- delete it's scope var
			- Go to any other waiting thread
						OR
			   Return to main code			

	# Working of the thread class
		- Creat thread	: Public
			o Here we can specifiy the priority
			o gotta specify which FUNCTION is to be run as a thread
				
		- DoThread		: Public
			o No argument
			  Called in every cycle of the processor
			  Will Manage the thread
			o Swapping execution context of threads
			o Pointing Instruction Pointer to currect line
			o Restoring the scope for "current" context
			
		- Delete thread	: Private
			o Destroy the thread and it's associated data
			o Notify all the "listening" parts of code that
			  a thread has ended
			
		- Allot Time slice : Private
			o Depeding on the priority will handle which 
			  thread allot how much time!
			 
		- put to sleep	: Public
			o Temporarly put a thread to sleep
		- Wake thread	: Public
			o Wake up a sleeping thred
		- Thread Info	: Public
			o Get thread info
				Like: Has it ended, sleeping or awake
					  Other relevant info

		
		= If some threads have been put to sleep, and the main code is about
		to end, notify that some threads are still alive.
		= If the main code ends and threads are still alive destroy them
		  makeing sure they terminate correctly i.e. gracefully !!

*/
