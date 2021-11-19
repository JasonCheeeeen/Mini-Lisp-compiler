#include<iostream>
#include<vector>
using namespace std;
vector<string> splitinput(string &input){
    string tmp;
    vector<string> split;
    for(int i=0;i<input.size();i++){
        if(input[i] == ' '){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            continue;
        }
        else if(input[i] == '+'){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back("+");
        }
        else if(input[i] == '-'){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back("-");
        }
        else if(input[i] == '*'){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back("*");
        }
        else if(input[i] == '/'){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back("/");
        }
        else if(input[i] == '('){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back("(");
        }
        else if(input[i] == ')'){
            if(tmp.size() != 0){
                split.push_back(tmp);
                tmp = "";
            }
            split.push_back(")");
        }
        else{
            tmp += input[i];
        }
    }
    if(tmp.size() != 0){
        split.push_back(tmp);
    }
    return split;
}
string getoutput(vector<string> &sin){
    string res = "";
    for(int i=0;i<sin.size();i++){
        if(sin[i] == "+"){
            res += "PLUS\n";
        }
        else if(sin[i] == "-"){
            res += "MINUS\n";
        }
        else if(sin[i] == "*"){
            res += "MUL\n";
        }
        else if(sin[i] == "/"){
            res += "DIV\n";
        }
        else if(sin[i] == "("){
            res += "LPR\n";
        }
        else if(sin[i] == ")"){
            res += "RPR\n";
        }
        else{
            res += "NUM ";
            res += sin[i];
            res += "\n";
        }
    }
    return res;
}
int main(){
    string input = "";
    string in;
    while(getline(cin,in)){
        input += in;
        input += ' ';
    }
    vector<string> splitin;
    splitin = splitinput(input);
    string res;
    res = getoutput(splitin);
    cout<<res;
    return 0;
}
