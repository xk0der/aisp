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


/*
** aisp.h
** Contains declaration of functions used by all modules
** and 'extern' definitions of almost all the global variables
** also includes the other modules header files so need to include them
** separtely.
** Also contains all the #define's, which contain : error codes,min max
** values for some data .. some string constants for default values etc.
**
** (c) Amit Singh under the GNU's General Public License
** Please visit www.gnu.org for more informatoin
** Contact me at : amit@remail.com
*/

#if !defined(_AISP_H)
#define _AISP_H

// Include all modules header files
#include "function_class.h"
#include "lable_class.h"
#include "scriptstack_class.h"
#include "stack_class.h"
#include "variable_class.h"
#include "for_class.h"

/** defines **/
// Application Info
#define	VERSION			"0.4.5 (debugging)"
#define VER				"V0.4.5(d)"
#define RELEASE			"Alpha"
#define	APP				"AISP"
#define INFO			"Amitz Interpreted Script Processor"
#define BUILD			__DATE__ " " __TIME__	//  Time stamp of compilation
// Include Levels
#define MAXINCLEVEL	100		// I guess that much should be enough (may be!! ;)

//Flag Values
#define SET				1
#define NOTSET			0

// Boolean values
#define TRUE				1
#define FALSE				0

// Internal Variable info
#define	EXP_VAR			"#_AISP_EXP_#"		// stores "exp" between function calls and subroutine branching
#define INTERNAL		"#_internal_var_#"	// Scope of internal variables

// Initializtion scripts & configuration files!
#define INIT_DIR		"/.aisp"
#define	CONFIG_FILE		"aisp.conf"

// NUM STRING SIZE
#define MAXSIZE	500

// Error types
#define ERR_NONE			0		//Clean compile
// File Errors
#define ERR_OPEN			1		//Unable to open file
#define ERR_WRITE			2		//Unable to write to disk
#define ERR_READ			3		//Cant read
// Memory Error
#define ERR_MEMALLOC		4		//Memory alloc err
// Command Line
#define ERR_INVALIDARG 		5		//Invalid arguement (parameter) on cmd line
// Stack error
#define ERR_STACKPOP		6		//Stack underflow
// Preinterpreter
#define ERR_INCLUDEMAX 		7
#define ERR_P_MISSINGIF 	8		// Missing !ifdef or !ifndef
#define ERR_P_MISSINGENDIF 	9		// missing !endif
#define ERR_TERMDEC			10		// Term not declared
#define ERR_TERMPRESENT		11		// Term already declared

//Script Errors
#define ERR_ARGCNT			101		//Argument count mismatch
#define ERR_ARGTYPE			102		//Arg type mismatch
#define ERR_VARDEC			103		//Variable not declared
#define ERR_VARTYPE			104		//Var type mismatch
#define ERR_FUNDEC			105		//Function not declared
#define ERR_BCOUNT			106		//Bracket Count '(' & ')'
#define ERR_LABELDEF		107		//Label not defined
#define ERR_DIVBYZERO		108		//Div by zero
#define ERR_RET				109		//Unexpected @ret() token encountered
#define ERR_VARPRESENT 		110		//Variable already present!!
#define ERR_TOKENDEF		111		//User defined function not defined
#define ERR_MISSINGRET		112		//No return statement
#define ERR_FUNPRESENT 		113		//Func already there
#define ERR_MEMVIOLATION 	114 	// Memory violation
#define ERR_MISSINGIF		115		// Missing @if()
#define ERR_MISSINGENDIF 	116 	// Missing @endif()
#define ERR_MISSINGLOOP 	117 	// missing @loop() or @wnd() for a @while() or @whl()
#define ERR_MISSINGWHILE 	118		// Missing @while() for  @loop() (@whl() for a @wnd())
#define ERR_MISSINGSWITCH 	119		// Missing @switch()
#define ERR_MISSINGCASE 	120		// Missing @case()
#define ERR_MISSINGENDSWITCH 121	// Missing @endswitch()
#define ERR_MISSINGFOR		122		// No Matching @for() for @next()
#define ERR_MISSINGDO		123		// No Matching @do() for @till() or @until()
#define ERR_INDEX_OUTOFBOUND 124	// Array Index out of bound
#define ERR_LVALUE			125		// Something other than a variable to the left of assignment operator '='			

// Misc
#define ERR_UNKNOWN	255		//This one's unknown at the moment!

// Temorary files
#define TEMPFILE	".temporary_script_file.tmp.can_be_deleted_safely.ais" // Deleted automatically (except for some 'unknown' cases ;) 
#define TEMPDEBUG 	"~temporary_debug.ais"	// Generated using a command line switch

//extern enum tcode;
enum tcode {
    _cls,
    _dis,
    _ift,
    _iff,
    _if,
    _endif,
    _while,
    _loop,
    _whl,
    _inp,
    _switch,
    _usr,
    _set,
    _get,
    _nln,
    _var,
    _fun,
    _equ,
    _jmp,
    _inc,
    _dec,
    _add,
    _sub,
    _mul,
    _div,
    _jmr,
    _ret,
    _end,
    _rev,
    _isg,
    _isl,
    _def,
    _else,
    _not,
    _and,
    _or,
    _xor,
    _trim,
    _ltrim,
    _rtrim,
    _wnd,
    _endswitch,
    _elseif,
    _break,
    _rnd,
    _rand,
    _seed,
    _timer,
    _for,
    _next,
    _do,
    _till,
    _until,
    _term,
    _endfor,
    _ucase,
    _toupper_,		// _toupper conflicts with declaration in ctype.h
    _lcase,
    _tolower_,		// _tolower conflicts with declaration in ctype.h
    _left,
    _right,
    _mid,
    _cut,
    _len,
    _dim,
    _push,
    _pop,
    _redim,
    _stackokay,
    _chr,
    _asc,
    _instr,
    _rinstr,
    ___
}; // 3 _


extern char *tstr[];

extern char *predefined_terms[];

extern int case_in_use;  // 0 = Not Case sensitive , 1 = Case Sensitive

extern char	**script;
extern long	total_lines;		// Total number of lines in the script file
extern long 	cline;			// current line number being executed!
extern char	*exp;			// Current expression
extern int		narg;			// argc
extern char 	**varg;			// argv
//char	*temp;
extern char	*temp1;
extern char	*temp2;
extern char	*temp3;	// *** use function stemp3(); to allocate memory for this var
extern long	 tmpl;
extern int	 tmpi;
extern int 	 tmpi1;
extern double tmpd;
extern float  tmpf;
extern char	*val;
extern char *filename;

extern double recurse;

extern int returned;		// @ret() executed??
extern char *expr;			// previous expression stored here
extern int	   ir;			// 'i'
extern int     ar;			// 'a'
extern char *rev;			// Return Value by @ret() token

extern char scope[64];		// 0(ZERO) Signifies Global Scope

extern char xstrx[255];
extern char xtempx[255];

extern int script_inif;
extern int script_inloop;
extern int script_inswitch;
extern int script_infor;

extern long	begin; 				// @begin() from here!

extern int scriptStackOkay;

extern int emit;	// TRUE or FLASE, emit memory for debugging YES or NO!

extern int config_present;
extern char *include_dir;
extern char *source_dir;

extern char conf_file[255];

// Function declarations
void loadais(char *);
void unloadais();
void error(int, char *,int=-1);
void interpret();
void scopy(char *,char *, int, int);
char *rtrim(char *);
char *ltrim(char *);
char *trim(char *);
int instr(char *,char *,int=0);
int rinstr(char *,char *);
int process(char *, char *, int &,int &);
int getarg(char *);
char *ucase(char *);
char *lcase(char *);
int cmp(char *, char *);
int rexp(char *,int &,int &);
void stemp3(int);
void version(void);
void help(void);
void handle_args(int,char **);
void preinterpret(void);
char *setcase(char *);
char *parsedis(char *);
int inc_expvar(void);
int dec_expvar(void);
int setreturned(char *);
char *getfunname(char *);
char *getfunarg(char *);
int findfun(char *,int *, int *);
void setscope(char *);
void buildpara(char *,int);
int cmpx(char *,char *);
int countline(char *);
void loadfile(char *);
void maketemp(char const*);
void makedebugtemp(char const*);
void creat_manual_template(void);
char *anti_parsedis(char *);
char *evaluate(char *);
void initialize(void);
char *make_include_file(char *);
void cleanup(void);
void makeconfig(void);


extern Variable var;			// Variable List
extern Function flist;			// User defined token List
extern Label label;				// label list
extern Label term;				// Terms defined using !define()
extern Stack  stack;			// Script stack used while invoking subroutines and functions
extern Stack  loopstack;		// stack for @while..@loop(),@do()..@till() and @do()..@until()
extern Stack  forstack;			// Stack for @for()..@next()
extern scriptStack ScriptStack;	// Stack accessible from script
extern For fordata;				// Stores data for @for()..@next() loop

#endif
