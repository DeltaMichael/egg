%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
%}

%union {
    char* str;
    int d;
}

%token <str> WORD
%token PIPE
%token EOL

%type <str> command sequence

%destructor { free($$); } WORD

%%

seqlist:
       | seqlist sequence EOL { printf("%s\n", $2); free($2); }
       ;

sequence: sequence PIPE command { strcat($1, " PIPE "); strcat($1, $3); $$ = $1; free($3); }
        | command { $$ = $1; }
        ;

command: command WORD { $$ = strcat($1, $2); free($2); }
       | WORD { char* out = calloc(256, sizeof(char)); strcat(out, $1); $$ = out; free($1); }
       ;

%%

int main(int argc, char** argv) {
    yyparse();
    yylex_destroy();  // Clean up lexer resources
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
