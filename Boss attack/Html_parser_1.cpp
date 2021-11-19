#include<iostream>
#include<string>
#include<vector>
#include<regex>
using namespace std;
struct record{
    string state;
    string text;
};
vector<string> splitinput(string);
vector<struct record> getstate(vector<string> &);

int main(){
    string in;
    vector<string> splinput;
    vector<struct record> instate;
    getline(cin,in);
    splinput = splitinput(in);
    instate = getstate(splinput);
    for(int i=0;i<instate.size();i++){
        if(i == instate.size()-1){
            cout<<instate[i].state<<" "<<instate[i].text;
        }
        else{
            cout<<instate[i].state<<" "<<instate[i].text<<endl;
        }
    }
    return 0;
}

vector<struct record> getstate(vector<string> &sin){
    int left = -1;
    regex single("'[^'<]*'");
    vector<struct record> tmpp = {};
    for(int i=0;i<sin.size();i++){
        if(sin[i] == "</"){
            tmpp.push_back({"TAG_OPEN_SLASH",sin[i]});
            left = 1;
            continue;
        }
        else if(sin[i] == "html" && left == 1){
            tmpp.push_back({"TAG_NAME",sin[i]});
            continue;
        }
        else if(sin[i] == "class" && left == 1){
            tmpp.push_back({"TAG_NAME",sin[i]});
            continue;
        }
        else if(sin[i] == "div" && left == 1){
            tmpp.push_back({"TAG_NAME",sin[i]});
            continue;
        }
        else if(sin[i] == "<"){
            tmpp.push_back({"TAG_OPEN",sin[i]});
            left = 1;
            continue;
        }
        else if(sin[i] == ">"){
            tmpp.push_back({"TAG_CLOSE",sin[i]});
            left = -1;
            continue;
        }
        else if(regex_match(sin[i],single)){
            string tmp = "";
            for(int j=1;j<sin[i].size()-1;j++){
                tmp += sin[i][j];
            }
            tmpp.push_back({"SINGLE_QUOTE_STRING",tmp});
            continue;
        }
        else if(sin[i] == "="){
            tmpp.push_back({"TAG_EQUALS",sin[i]});
            continue;
        }
        else{
            regex tagname("[a-z|A-Z|0-9]+");
            if(regex_match(sin[i],tagname) && left == 1){
                tmpp.push_back({"TAG_NAME",sin[i]});
                continue;
            }
            if(sin[i][0] == '"' && sin[i][sin[i].size()-1] == '"' && left == 1){
                string tmp = "";
                for(int j=1;j<sin[i].size()-1;j++){
                    tmp += sin[i][j];
                }
                tmpp.push_back({"DOUBLE_QUOTE_STRING",tmp});
            }
            else{
                tmpp.push_back({"HTML_TEXT",sin[i]});
            }
        }
    }
    vector<struct record> res = {};
    for(int i=0;i<tmpp.size();i++){
        if(tmpp[i].state == "HTML_TEXT"){
            string t = tmpp[i].text;
            int time = 0;
            for(int j=i+1;j<tmpp.size();j++){
                if(tmpp[j].state == "HTML_TEXT"){
                    t += " ";
                    t += tmpp[j].text;
                    time++;
                }
                else{
                    break;
                }
            }
            res.push_back({"HTML_TEXT",t});
            i += time;
        }
        else{
            res.push_back(tmpp[i]);
        }
    }
    return res;
}

vector<string> splitinput(string sin){
    string tmp = "";
    vector<string> tmpp = {};
    for(int i=0;i<sin.size();i++){
        if(sin[i] == ' '){
            if(tmp.size() != 0){
                tmpp.push_back(tmp);
                tmp = "";
            }
            continue;
        }
        if(sin[i] == '<'){
            if(sin[i+1] == '/'){
                if(tmp.size() != 0){
                    tmpp.push_back(tmp);
                    tmp = "";
                }
                tmpp.push_back("</");
                i++;
            }
            else{
                if(tmp.size() != 0){
                    tmpp.push_back(tmp);
                    tmp = "";
                }
                tmpp.push_back("<");
            }
        }
        else if(sin[i] == '>'){
            if(tmp.size() != 0){
                tmpp.push_back(tmp);
                tmp = "";
            }
            tmpp.push_back(">");
        }
        else if(sin[i] == '='){
            if(tmp.size() != 0){
                tmpp.push_back(tmp);
                tmp = "";
            }
            tmpp.push_back("=");
        }
        else{
            tmp += sin[i];
        }
    }
    if(tmp.size() != 0){
        tmpp.push_back(tmp);
    }
    return tmpp;
}
