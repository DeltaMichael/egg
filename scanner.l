%{
#include "parser.tab.h"
%}

%%
"|"         { return PIPE; }
\n          { return EOL; }
[ \t]       { /* ignore whitespace*/ }
[^ \t\n|]  { yylval.str = strdup(yytext); return WORD; }

%%
