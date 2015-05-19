" Vim syntax file
" Language:       AIS - AISP
" Maintainer:     amit@xkoder.com
" URL:            http://github.com/xk0der/aisp
" Version:        0.0.1
" Last Change:    19/May/2015
"
" Optimized for -lang ais

" Quit when a syntax file was already loaded
if exists("b:current_syntax")
finish
endif

syn keyword aispKeywords contained fun ret if usr cls dis ift iff if endif while loop whl inp switch usr set get nln var fun equ jmp inc dec add sub mul div jmr ret end rev isg isl def else not and or xor trim ltrim rtrim wnd endswitch elseif break rnd rand seed timer for next do till until term endfor ucase toupper lcase tolower left right mid cut len dim push pop redim stackokay chr asc instr rinstr 
syn keyword aispPreProcKeywords contained ifdef define defined endif ifndef
syn match aispSep       /,/ contained
syn match aispEndBracket /)/ contained
syn match aispStartBracket /(/ contained
syn match aispFunStart "@" contained
syn match aispPreProcStart "!" contained
syn keyword aispBool contained TRUE FALSE
syn match anything /\S\+/ contains=aispSep,aispStartBracket,aispEndBracket,aispFunStart,aispPreProcStart,aispKeywords,aispBool contained
syn match anythingPreProc /\S\+/ contains=aispSep,aispStartBracket,aispEndBracket,aispFunStart,aispPreProcStart,aispPreProcKeywords,aispBool contained
syn match aispFun /@\S\+(.*)/ contains=anything contained
syn match aispPreProc /!\S\+(.*)/ contains=anythingPreProc contained
syn region aispComment start=/^/ end=/$/  contains=aispFun,aispPreProc

hi def link anything            Special
hi def link aispStartBracket    Type
hi def link aispEndBracket      Type
hi def link aispSep             Type
hi def link aispFunStart        Type
hi def link aispPreProcStart    Number
hi def link aispBool            String
hi def link aispKeywords        Function 
hi def link aispPreProcKeywords Define
hi def link containedAnything   String
hi def link aispComment         Comment

let b:current_syntax = "ais"

" vim: ts=4
