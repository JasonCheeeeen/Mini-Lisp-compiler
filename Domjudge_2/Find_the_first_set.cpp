#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<map>
using namespace std;

map<char,vector<char>> res_input;
map<char,vector<string>> getstate(vector<string>);
void getfirst(char, map<char,vector<string>>);
void getoutput(map<char,vector<char>>);

int main(){
    vector<string> input = {};
    map<char,vector<string>> ori_input;
    string in;
    while(getline(cin,in)){
        input.push_back(in);
    }
    ori_input = getstate(input);
    for(auto &it:ori_input){
        getfirst(it.first,ori_input);
    }
    getoutput(res_input);
}

void getoutput(map<char,vector<char>> res){
    for(auto &it:res){
        cout<<it.first<<" ";
        for(int i=0;i<it.second.size();i++){
            cout<<it.second[i];
        }
        cout<<endl;
    }
    cout<<"END_OF_FIRST\n";
}

void getfirst(char key, map<char,vector<string>> input){
    vector<string> state = input[key];
    for(int i=0;i<state.size();i++){
        int next = 1;
        for(int j=0;j<state[i].size() && next;j++){
            next = 0;
            if('a' <= state[i][j] && state[i][j] <= 'z'){
                res_input[key].push_back(state[i][j]);
            }
            else if('A' <= state[i][j] && state[i][j] <= 'Z'){
                if(res_input[state[i][j]].empty()){
                    getfirst(state[i][j], input);
                }
                for(int r=0;r<res_input[state[i][j]].size();r++){
                    if(res_input[state[i][j]][r] == ';'){
                        next = 1;
                        if(j == state[i].size()-1){
                            res_input[key].push_back(res_input[state[i][j]][r]);
                        }
                    }
                    else{
                        res_input[key].push_back(res_input[state[i][j]][r]);
                    }
                }
            }
            else if(state[i][j] == ';'){
                next = 1;
                res_input[key].push_back(state[i][j]);
            }
            else if(state[i][j] == '$'){
                next = 0;
                res_input[key].push_back(state[i][j]);
            }
        }
    }
    sort(res_input[key].begin(),res_input[key].end());
    vector<char> tmp = {};
    for(int h=0;h<res_input[key].size();h++){
        if(find(tmp.begin(),tmp.end(),res_input[key][h]) == tmp.end()){
            tmp.push_back(res_input[key][h]);
        }
    }
    res_input[key] = tmp;
}

map<char,vector<string>> getstate(vector<string> input){
    map<char,vector<string>> state;
    for(int i=0;i<input.size();i++){
        if(input[i] == "END_OF_GRAMMAR"){
            continue;
        }
        state[input[i][0]] = {};
        string tmp = "";
        for(int j=2;j<input[i].size();j++){
            if(input[i][j] == '|'){
                state[input[i][0]].push_back(tmp);
                tmp = "";
                continue;
            }
            tmp += input[i][j];
            if(j == input[i].size()-1 && tmp.size() != 0){
                state[input[i][0]].push_back(tmp);
            }
        }
    }
    return state;
}
