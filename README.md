# Compiler Final-project
## Make file
```
$ make
```
## Main idea
- flow chart
![](https://imgur.com/QouClUd.png)
- consteruct AST tree
```
// tree node
struct node{
        int value;
        char type;                     // grammar type
        char* idd;                     // store id name
        struct node* leftchild;
        struct node* midchild;
        struct node* rightchild;
};
```

- Grammar type (Please use another way to record or you will get lots of troubles)

```
print-num                            ----->    'O'
print-bool                           ----->    'o'
variable                             ----->    'V'
number                               ----->    'N'
bool                                 ----->    'B'
params                               ----->    'P'
no params (no ids)                   ----->    'x'
exps                                 ----->    'E'
fun-call '('FUNNAME PARAMS')'        ----->    'f'
fun-call '('FUNEXP PARAMS')'         ----->    'F'
funexp    (lambda)                   ----->    'L'
ids                                  ----->    'v'
ifexp                                ----->    'I'
greatop   (>)                        ----->    'g'
smallop   (<)                        ----->    'l'
equalop   (=)                        ----->    'e'
modop     (%)                        ----->    'r'
divop     (/)                        ----->    'd'
mulop     (*)                        ----->    'm'
minusop   (-)                        ----->    's'
plusop    (+)                        ----->    'p'
andop                                ----->    'a'
orop                                 ----->    'z'
notop                                ----->    'n'
```

- check tree when construct the tree
```
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
```

## Some Examples
```
(print-num (+ 1 (+ 2 3 4) ( 4 5 6) (/ 8 3) (mod 10 3)))
```
![](https://imgur.com/YEP5fbp.png)
```
(print-num (if (< 1 2) (+ 1 2 3) (* 1 2 3 4 5)))
```
![](https://imgur.com/KZlsJoO.png)
