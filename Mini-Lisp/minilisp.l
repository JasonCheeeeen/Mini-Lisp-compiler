%option noyywrap
%{
    #include<iostream>
    #include<cstdlib>
    #include"minilisp.tab.hh"
    void yyerror(char*);
%}

letter   [a-z]
digit    [0-9]
number   0|[1-9]{digit}*|-[1-9]{digit}*
id       {letter}({letter}|{digit}|"-")*

/* operation */

Plus     [+]
Minus    [-]
Mul      [*]
Div      [/]
Mod      "mod"
Greater  [>]
Smaller  [<]
Equal    [=]
And      "and"
Or       "or"
Not      "not"
Bool     #[t|f]

Def      "define"
Lam      "lambda"
If       "if"
%%

{Plus}          {return PLUS;}
{Minus}         {return MINUS;}
{Mul}           {return MUL;}
{Div}           {return DIV;}
{Mod}           {return MOD;}
{Greater}       {return GREAT;}
{Smaller}       {return SMALL;}
{Equal}         {return EQUAL;}
{And}           {return AND;}
{Or}            {return OR;}
{Not}           {return NOT;}
{Def}           {return DEF;}
{Lam}           {return LAMBDA;}
{If}            {return IF;}
"("             {return yytext[0];}
")"             {return yytext[0];}
"print-num"     {return PRINTNUM;}
"print-bool"    {return PRINTBOOL;}
{number}        {yylval.iv = atoi(yytext); return NUMBER;}
{id}            {yylval.sv = strdup(yytext); return ID;}
\n
.
{Bool}          {if(yytext[1] == 't'){
                    yylval.bv = 1;
                }
                else{
                    yylval.bv = 0;
                }
                return BOOL;}
%%