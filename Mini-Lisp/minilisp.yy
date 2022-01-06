%{
    #include<iostream>
    #include<cstdlib>
    #include<cstring>
    #include<unordered_map>
    #include<vector>
    using namespace std;
    int yylex(void);
    void yyerror(const char*);

    struct node{
        int value;
        char type;                  // grammar type
        char* idd;                  // store id name
        struct node* leftchild;
        struct node* midchild;
        struct node* rightchild;
    };

    struct defnode{
        char* idd;
        int value;
        vector<char*> para;
        struct node* eexp;
    };
    int def_count = 0;
    vector<struct defnode*> recdefnode = {};

    struct node* root;
    int res_p = 0;                                  // original value of plus operation
    int res_m = 1;                                  // original value of multiply operation
    int res_a = 1;                                  // original value of and operation
    int res_z = 0;                                  // original value of or operation
    int isdef = 0;                                  // know it is in def or not
    unordered_map<char*,int> def_number;            // record define number
    vector<char*> notinfunpara = {};                // store not in define lambda
    vector<char*> infunpara = {};                   // store in define lambda
    vector<int> notinfunparanum = {};
    vector<int> infunparanum = {};
    vector<int> inputparanum = {};
    void add(struct node*);
    void mul(struct node*);
    void aand(struct node*);
    void oor(struct node*);
    void getparaid(struct node*);                   // get not in define parameter
    void getparanum(struct node*);                  // get not in define input number
    void idnumcombine(struct node*);                // combine not in define id & number
    void getdefpara(struct node*);                  // get in define parameter
    void getinputnum(struct node*);                 // get in define input number
    void idnumcombine2(struct node*,vector<char*>); // combine in define id & number
    void postorder(struct node*);
    void checknode(struct node*);
    struct node* getnode(int,char,struct node*,struct node*,struct node*);
%}

%union{
    int iv;           // int value
    int bv;           // bool value
    char* sv;         // string
    struct node* nv;  // node
}

/* 
    this has a problem, initially, i use "typedef struct node Node" to simplify,
    but in .l file, it has include the .hh file, but not include %{ %} section in 
    .yy file, then "Node" cannot be the type, it so confused!
*/  

// %token <> specify the type
%token <iv> NUMBER
%token <bv> BOOL
%token <sv> ID
%token PLUS MINUS MUL DIV MOD GREAT SMALL EQUAL AND OR NOT
%token DEF LAMBDA IF PRINTNUM PRINTBOOL
%type <nv> PROG STMTS STMT PRINTSTMT DEFSTMT
%type <nv> EXP EXPS VARIABLE NUMOP LOGICALOP FUNEXP FUNCALL IFEXP
%type <nv> PLUSOP MINUSOP MULOP DIVOP MODOP GREATOP SMALLOP EQUALOP
%type <nv> ANDOP OROP NOTOP 
%type <nv> FUNIDS IDS FUNBODY PARAMS FUNNAME TESTEXP THANEXP ELSEEXP

%%

PROG            :    STMTS                              {root = $1;}
                ;
STMTS           :    STMT STMTS                         {$$ = getnode(0,'S',$1,NULL,$2);}
                |    STMT                               {$$ = $1;}
                ;
STMT            :    EXP                                {$$ = $1;}
                |    PRINTSTMT                          {$$ = $1;}
                |    DEFSTMT                            {$$ = $1;}
                ;
DEFSTMT         :    '('DEF VARIABLE EXP')'             {$$ = getnode(0,'D',$3,NULL,$4);}
                ;
VARIABLE        :    ID                                 {$$ = getnode(0,'V',NULL,NULL,NULL); $$->idd = $1; /*cout<<$$->idd;*/}
                ;
PRINTSTMT       :    '('PRINTNUM EXP')'                 {$$ = getnode(0,'O',$3,NULL,NULL);}
                |    '('PRINTBOOL EXP')'                {$$ = getnode(0,'o',$3,NULL,NULL);}
                ;
EXP             :    NUMBER                             {$$ = getnode($1,'N',NULL,NULL,NULL);}
                |    BOOL                               {$$ = getnode($1,'B',NULL,NULL,NULL);}
                |    NUMOP                              {$$ = $1;}
                |    LOGICALOP                          {$$ = $1;}
                |    IFEXP                              {$$ = $1;}
                |    VARIABLE                           {$$ = $1;}
                |    FUNEXP                             {$$ = $1;}
                |    FUNCALL                            {$$ = $1;}
                ;
FUNCALL         :    '('FUNEXP PARAMS')'                {$$ = getnode(0,'F',$2,NULL,$3);}
                |    '('FUNNAME PARAMS')'               {$$ = getnode(0,'f',$2,NULL,$3);}
                ;
FUNNAME         :    VARIABLE                           {$$ = $1;}
                ;
PARAMS          :    EXP PARAMS                         {$$ = getnode(0,'P',$1,NULL,$2);}
                |                                       {$$ = getnode(0,'x',NULL,NULL,NULL);} // n -> null
                ;
FUNEXP          :    '('LAMBDA FUNIDS FUNBODY')'        {$$ = getnode(0,'L',$3,NULL,$4);}
                ;
FUNIDS          :    '(' IDS ')'                        {$$ = $2;}
                ;
IDS             :    VARIABLE IDS                       {$$ = getnode(0,'v',$1,NULL,$2);}
                |                                       {$$ = getnode(0,'x',NULL,NULL,NULL);} // n -> null
                ;
FUNBODY         :    EXP                                {$$ = $1;}
                ;
IFEXP           :    '('IF TESTEXP THANEXP ELSEEXP')'   {$$ = getnode(0,'I',$4,$3,$5);}
                ;
TESTEXP         :    EXP                                {$$ = $1;}
                ;
THANEXP         :    EXP                                {$$ = $1;}
                ;
ELSEEXP         :    EXP                                {$$ = $1;}
                ;
NUMOP           :    PLUSOP                             {$$ = $1;}
                |    MINUSOP                            {$$ = $1;}
                |    MULOP                              {$$ = $1;}
                |    DIVOP                              {$$ = $1;}
                |    MODOP                              {$$ = $1;}
                |    GREATOP                            {$$ = $1;}
                |    SMALLOP                            {$$ = $1;}
                |    EQUALOP                            {$$ = $1;}
                ;
GREATOP         :    '('GREAT EXP EXP')'                {$$ = getnode(0,'g',$3,NULL,$4);}
                ;
SMALLOP         :    '('SMALL EXP EXP')'                {$$ = getnode(0,'l',$3,NULL,$4);}
                ;
EQUALOP         :    '('EQUAL EXP EXPS')'               {$$ = getnode(0,'e',$3,NULL,$4);}
                ;
MODOP           :    '('MOD EXP EXP')'                  {$$ = getnode(0,'r',$3,NULL,$4);}
                ;
DIVOP           :    '('DIV EXP EXP')'                  {$$ = getnode(0,'d',$3,NULL,$4);}
                ;
MULOP           :    '('MUL EXP EXPS')'                 {$$ = getnode(0,'m',$3,NULL,$4);}
                ;
MINUSOP         :    '('MINUS EXP EXP')'                {$$ = getnode(0,'s',$3,NULL,$4);}
                ;
PLUSOP          :    '('PLUS EXP EXPS')'                {$$ = getnode(0,'p',$3,NULL,$4);}
                ;
LOGICALOP       :    ANDOP                              {$$ = $1;}
                |    OROP                               {$$ = $1;}
                |    NOTOP                              {$$ = $1;}
                ;
ANDOP           :    '('AND EXP EXPS')'                 {$$ = getnode(0,'a',$3,NULL,$4);}
                ;
OROP            :    '('OR EXP EXPS')'                  {$$ = getnode(0,'z',$3,NULL,$4);}
                ;
NOTOP           :    '('NOT EXP')'                      {$$ = getnode(0,'n',$3,NULL,NULL);}
                ;
EXPS            :    EXP EXPS                           {$$ = getnode(0,'E',$1,NULL,$2);}
                |    EXP                                {$$ = $1;}
                ;
%%

void postorder(struct node* current){
    if(current == NULL){
        return;
    }
    switch(current->type){
        case 'O': // output number
            postorder(current->leftchild); // right is NULL
            cout<<current->leftchild->value<<"\n";
            break;
        case 'o': // output bool value (bv)
            postorder(current->leftchild);
            if(current->leftchild->value == 1){
                cout<<"#t\n";
            }
            else{
                cout<<"#f\n";
            }
            break;
        case 'f': // fun-call-name
            postorder(current->rightchild);
            if(current->rightchild->type == 'x'){
                for(int i=0;i<recdefnode.size();i++){
                    if(!strcmp(recdefnode[i]->idd,current->leftchild->idd)){
                        current->value = recdefnode[i]->value;
                        break;
                    }
                }
            }
            else{
                getinputnum(current->rightchild);
                for(int i=0;i<recdefnode.size();i++){
                    if(!strcmp(recdefnode[i]->idd,current->leftchild->idd)){
                        idnumcombine2(recdefnode[i]->eexp,recdefnode[i]->para);
                        postorder(recdefnode[i]->eexp);
                    	 current->value = recdefnode[i]->eexp->value;
                         inputparanum.clear();
                    	 break;
                    }
                }
            }
            break;
        case 'F': // fun-call-exp
            if(isdef == 0){
                getparaid(current->leftchild->leftchild);
                getparanum(current->rightchild);
                idnumcombine(current->leftchild->rightchild);
                postorder(current->leftchild->rightchild);
                current->value = current->leftchild->rightchild->value;
                notinfunpara.clear();
                notinfunparanum.clear();
            }
            break;
        case 'V': // variable
            if(current->value != 0){
                break;
            }
            for(auto &it:def_number){
                if(!strcmp(it.first,current->idd)){
                    //cout<<current->idd<<endl;
                    current->value = it.second;
                    break;
                }
            }
            /* 
                In this case, it has a large trap, since in define case,
                i store the variable and its value into map, and then i 
                use "map[variable] == current->idd" to find its value,
                but in fact, the address of map[variable] is different
                of current->idd, so it cannot use this way, if used, it will
                happen a accident like: "abc" != "abc" the fucking phenomenon,
                so i use "strcmp" to solve this trouble.
            */
            break;
        case 'D': // def
            isdef = 1;
            if(current->rightchild->type != 'L'){
                postorder(current->rightchild);
                def_number.insert({current->leftchild->idd,current->rightchild->value});
            }
            else{
                struct defnode* tmp = (struct defnode*)malloc(sizeof(struct defnode));
                if(current->rightchild->leftchild->type == 'x'){
                    tmp->value = current->rightchild->rightchild->value;
                    tmp->idd = current->leftchild->idd;
                    recdefnode.push_back(tmp);
                }
                else{
                    tmp->value = 0;
                    tmp->idd = current->leftchild->idd;
                    getdefpara(current->rightchild->leftchild);
                    tmp->para = infunpara;
                    infunpara.clear();
                    tmp->eexp = current->rightchild->rightchild;
                    recdefnode.push_back(tmp);
                }
            }
            isdef = 0;
            break;
        case 'I': // if
            postorder(current->leftchild);
            postorder(current->midchild);
            postorder(current->rightchild);
            if(current->midchild->value == 1){
                current->value = current->leftchild->value;
            }
            else{
                current->value = current->rightchild->value;
            }
            break;
        case 'a': // and
            postorder(current->leftchild);
            postorder(current->rightchild);
            aand(current);
            current->value = res_a;
            res_a = 1;
            break;
        case 'z': // or(z)
            postorder(current->leftchild);
            postorder(current->rightchild);
            oor(current);
            current->value = res_z;
            res_z = 0;
            break;
        case 'n': // not
            postorder(current->leftchild);
            current->value = 1 - current->leftchild->value;
            break;
        case 'p': // plus
            postorder(current->leftchild);
            postorder(current->rightchild);
            add(current);
            current->value = res_p;
            res_p = 0; // initial
            break; 
        case 's': // minus
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = current->leftchild->value - current->rightchild->value;
            break;
        case 'm': // multiply
            postorder(current->leftchild);
            postorder(current->rightchild);
            mul(current);
            current->value = res_m;
            res_m = 1; // initial
            break;
        case 'd': // divide
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = current->leftchild->value / current->rightchild->value;
            break;
        case 'r': // remainder
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = current->leftchild->value % current->rightchild->value;
            break;
        case 'g': // greater
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = (current->leftchild->value > current->rightchild->value) ?  1 : 0;
            // cout<<"test "<<current->value<<"\n";
            break;
        case 'l': // lower
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = (current->leftchild->value < current->rightchild->value) ?  1 : 0;
            // cout<<"test "<<current->value<<"\n";
            break;
        case 'e': // equal
            postorder(current->leftchild);
            postorder(current->rightchild);
            current->value = (current->leftchild->value == current->rightchild->value) ?  1 : 0;
            // cout<<"test "<<current->value<<"\n";
            break;
        default:
            postorder(current->leftchild);
            postorder(current->rightchild);
            break;
    }
}

void idnumcombine2(struct node* current,vector<char*> ppara){
    if(current == NULL){
        return;
    }
    idnumcombine2(current->leftchild,ppara);
    idnumcombine2(current->rightchild,ppara);
    if(current->type == 'V'){
        for(int i=0;i<ppara.size();i++){
            if(!strcmp(current->idd,ppara[i])){
                current->value = inputparanum[i];
                break;
            }
        }
    }
}

void getinputnum(struct node* current){
    if(current == NULL){
        return;
    }
    if(current->type == 'f'){
        inputparanum.push_back(current->value);
        return;
    }
    getinputnum(current->leftchild);
    getinputnum(current->rightchild);
    if(current->type == 'N'){
        inputparanum.push_back(current->value);
    }
}

void getdefpara(struct node* current){
    if(current == NULL){
        return;
    }
    getdefpara(current->leftchild);
    getdefpara(current->rightchild);
    if(current->type == 'V'){
        infunpara.push_back(current->idd);
    }
}

void idnumcombine(struct node* current){
    if(current == NULL){
        return;
    }
    idnumcombine(current->leftchild);
    idnumcombine(current->rightchild);
    if(current->type == 'V'){
        for(int i=0;i<notinfunpara.size();i++){
            if(!strcmp(notinfunpara[i],current->idd)){
                current->value = notinfunparanum[i];
                break;
            }
        }
    }
}

void getparanum(struct node* current){
    if(current == NULL){
        return;
    }
    getparanum(current->leftchild);
    getparanum(current->rightchild);
    if(current->type == 'N'){
        notinfunparanum.push_back(current->value);
    }
}

void getparaid(struct node* current){
    if(current == NULL){
        return;
    }
    getparaid(current->leftchild);
    getparaid(current->rightchild);
    if(current->type == 'V'){
        notinfunpara.push_back(current->idd);
    }
}

void oor(struct node* current){
    if(current->leftchild != NULL){
        if(current->leftchild->value == 1){
            res_z = 1;
            return;
        }
        if(current->leftchild->type == 'E'){
            oor(current->leftchild);
        }
    }
    if(current->rightchild != NULL){
        if(current->rightchild->value == 1){
            res_z = 1;
            return;
        }
        if(current->rightchild->type == 'E'){
            oor(current->rightchild);
        }
    }
}

void aand(struct node* current){
    if(current->leftchild != NULL){
        if(current->leftchild->value == 0){
            res_a = 0;
            return;
        }
        if(current->leftchild->type == 'E'){
            aand(current->leftchild);
        }
    }
    if(current->rightchild != NULL){
        if(current->rightchild->value == 0){
            res_a = 0;
            return;
        }
        if(current->rightchild->type == 'E'){
            aand(current->leftchild);
        }
    }
}

void add(struct node* current){
    if(current->leftchild != NULL){
        res_p += current->leftchild->value;
        if(current->leftchild->type == 'E'){
            add(current->leftchild);
        }
    }
    if(current->rightchild != NULL){
        res_p += current->rightchild->value;
        if(current->rightchild->type == 'E'){
            add(current->rightchild);
        }
    }
}

void mul(struct node* current){
    if(current->leftchild != NULL){
        if(current->leftchild->type != 'E'){
            res_m = res_m * current->leftchild->value;
        }
        if(current->leftchild->type == 'E'){
            mul(current->leftchild);
        }
    }
    if(current->rightchild != NULL){
        if(current->rightchild->type != 'E'){
            res_m = res_m * current->rightchild->value;
        }
        if(current->rightchild->type == 'E'){
            mul(current->rightchild);
        }
    }
}

struct node* getnode(int v, char t, struct node* left, struct node* mid, struct node* right){
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    tmp->value = v;
    tmp->type = t;
    tmp->leftchild = left;
    tmp->midchild = mid;
    tmp->rightchild = right;
    return tmp;
}

// use postorder to check value and type in each node
void checknode(struct node* current){
    if(current == NULL){
        return;
    }
    checknode(current->leftchild);
    checknode(current->midchild);
    checknode(current->rightchild);
    cout<<current->type<<" "<<current->value<<" "<<endl;
}

void yyerror(const char* msg){
    cout<<"syntax error";
};

int main(){
    yyparse();
    postorder(root);
    //checknode(root->rightchild->leftchild->rightchild);
    /*for(auto &it:def_number){
        cout<<it.first<<" "<<it.second<<"\n";
    }*/
    /*for(int i=0;i<recdefnode.size();i++){
        cout<<recdefnode[i]->idd<<" "<<recdefnode[i]->value<<" ";
        for(int j=0;j<recdefnode[i]->para.size();j++){
            cout<<recdefnode[i]->para[j]<<" ";
        }
        cout<<"\n";
    }*/
    return 0;
}