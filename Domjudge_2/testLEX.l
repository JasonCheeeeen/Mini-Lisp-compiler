%{
	int count = 0;
	void print(char* tmp,int c){
		int ori_t = 0;
		int space = 0;
		while(tmp[ori_t] == '\t'){
			ori_t++;
			c--;
		}
		for(int i=0;i<c;i++){
			printf("    ");
		}
		printf("%s\n",tmp);
	}
%}
left	^.*\{.*$
right	^.*\}.*$
else	^\t*.*$
%%

{left}	{print(yytext,count); count++;}
{right}	{count--; print(yytext,count);}
\n	{}
{else}	{print(yytext,count);}

%%
int main(int argc, char argv[]){
	yylex();
	return 0;
}
