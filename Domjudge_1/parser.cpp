#include<iostream>
#include<vector>
#include<string>
#include<regex>
using namespace std;

struct record{
    int index;
    int trans;
    string tmpres;
};

string stmts(vector<string> &sin);
struct record stmt(int index,vector<string> &sin,string restmp,int trans);
struct record primary(int index,vector<string> &sin,string restmp,int trans);
struct record primary_tail(int index,vector<string> &sin,string restmp,int trans);

struct record ID(int index,vector<string> &sin,string restmp,int trans);
struct record DOT(int index,vector<string> &sin,string restmp,int trans);
struct record LBR(int index,vector<string> &sin,string restmp,int trans);
struct record RBR(int index,vector<string> &sin,string restmp,int trans);
struct record STRLIT(int index,vector<string> &sin,string restmp,int trans);
vector<string> splitinput(string &input);

int main(){
    string input = "";
    string in;
    string res = "";
    string ress = "";
    while(getline(cin,in)){
        vector<string> splitin;
        splitin = splitinput(in);
        ress = stmts(splitin);
        if(ress.size() == 0){
            res = "";
            break;
        }
        res += ress;
        ress = "";
    }
    if(res.size() == 0){
        cout<<"invalid input";
    }
    else{
        cout<<res;
    }
    return 0;
}

struct record STRLIT(int index,vector<string> &sin,string restmp,int trans){
    if(sin[index][0] == '"' && sin[index][sin[index].size()-1] == '"'){
        if(count(sin[index].begin(),sin[index].end(),'"') == 2){
            restmp += "STRLIT ";
            restmp += sin[index];
            restmp += "\n";
            trans = 1;
            index++;
        }
    }
    else{
        trans = 0;
    }
    return {index,trans,restmp};
}

struct record ID(int index,vector<string> &sin,string restmp,int trans){
    regex idd("[A-Za-z_][A-Za-z0-9_]*");
    if(regex_match(sin[index],idd)){
        restmp += "ID ";
        restmp += sin[index];
        restmp += "\n";
        index++;
        trans = 1;
    }
    else{
        trans = 0;
    }
    return {index,trans,restmp};
}

struct record DOT(int index,vector<string> &sin,string restmp,int trans){
    if(sin[index] == "."){
        restmp += "DOT .\n";
        index++;
        trans = 1;
    }
    else{
        trans = 0;
    }
    return {index,trans,restmp};
}

struct record LBR(int index,vector<string> &sin,string restmp,int trans){
    if(sin[index] == "("){
        restmp += "LBR (\n";
        index++;
        trans = 1;
    }
    else{
        trans = 0;
    }
    return {index,trans,restmp};
}

struct record RBR(int index,vector<string> &sin,string restmp,int trans){
    if(sin[index] == ")"){
        restmp += "RBR )\n";
        index++;
        trans = 1;
    }
    else{
        trans = 0;
    }
    return {index,trans,restmp};
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
struct record stmt(int index,vector<string> &sin,string restmp,int trans){
    struct record s;
    s = primary(index,sin,restmp,trans);
    if(s.trans){
        return {s.index,s.trans,s.tmpres};
    }
    else{
        struct record ss = STRLIT(s.index,sin,s.tmpres,s.trans);
        if(ss.trans || sin[ss.index] == ")"){
            return {ss.index,1,ss.tmpres};
        }
        return {ss.index,0,ss.tmpres};
    }
}

struct record primary(int index,vector<string> &sin,string restmp,int trans){
    struct record p;
    p = ID(index,sin,restmp,trans);
    if(p.trans){
        if(p.index == sin.size()){
            return {p.index,p.trans,p.tmpres};
        }
        return primary_tail(p.index,sin,p.tmpres,p.trans);
    }
    else{
        return {p.index,p.trans,p.tmpres};
    }
}

struct record primary_tail(int index,vector<string> &sin,string restmp,int trans){
    struct record ptd;
    ptd = DOT(index,sin,restmp,trans);
    if(ptd.trans){
        if(ptd.index == sin.size()){
            return {ptd.index,ptd.trans,ptd.tmpres};
        }
        struct record ptid;
        ptid = ID(ptd.index,sin,ptd.tmpres,ptd.trans);
        if(ptid.trans){
            return primary_tail(ptid.index,sin,ptid.tmpres,ptid.trans);
        }
        else{
            struct record ptl;
            ptl = LBR(ptid.index,sin,ptid.tmpres,ptid.trans);
            if(ptl.trans){
                if(ptl.index == sin.size()){
                    return {ptl.index,ptl.trans,ptl.tmpres};
                }
                struct record ptls;
                ptls = stmt(ptl.index,sin,ptl.tmpres,ptl.trans);
                if(ptls.trans){
                    if(ptls.index == sin.size()){
                        return {ptls.index,ptls.trans,ptls.tmpres};
                    }
                    struct record ptlsr;
                    ptlsr = RBR(ptls.index,sin,ptls.tmpres,ptls.trans);
                    if(ptlsr.trans){
                        if(ptlsr.index == sin.size()){
                            return {ptlsr.index,ptlsr.trans,ptlsr.tmpres};
                        }
                        return primary_tail(ptlsr.index,sin,ptlsr.tmpres,ptlsr.trans);
                    }
                    else{
                        return {ptlsr.index,ptlsr.trans,ptlsr.tmpres};
                    }
                }
                else{
                    return {ptls.index,ptls.trans,ptls.tmpres};
                }
            }
            else{
                return {ptl.index,ptl.trans,ptl.tmpres};
            }
        }
    }
    else{
        struct record ptnl;
        ptnl = LBR(ptd.index,sin,ptd.tmpres,ptd.trans);
        if(ptnl.trans){
            if(ptnl.index == sin.size()){
                return {ptnl.index,ptnl.trans,ptnl.tmpres};
            }
            struct record ptnls;
            ptnls = stmt(ptnl.index,sin,ptnl.tmpres,ptnl.trans);
            if(ptnls.trans){
                if(ptnls.index == sin.size()){
                    return {ptnls.index,ptnls.trans,ptnls.tmpres};
                }
                struct record ptnlsr;
                ptnlsr = RBR(ptnls.index,sin,ptnls.tmpres,ptnls.trans);
                if(ptnlsr.trans){
                    if(ptnlsr.index == sin.size()){
                        return {ptnlsr.index,ptnlsr.trans,ptnlsr.tmpres};
                    }
                    return primary_tail(ptnlsr.index,sin,ptnlsr.tmpres,ptnlsr.trans);
                }
                else{
                    return {ptnlsr.index,ptnlsr.trans,ptnlsr.tmpres};
                }
            }
            else{
                return {ptnls.index,ptnls.trans,ptnls.tmpres};
            }
        }
        else{
            return {ptnl.index,ptnl.trans,ptnl.tmpres};
        }
    }
}

string stmts(vector<string> &sin){
    int index = 0;
    string ress = "";
    struct record sstmts = {0,0,ress};
    while(sstmts.index < sin.size()){
        sstmts = stmt(sstmts.index,sin,sstmts.tmpres,0);
        if(sstmts.trans == 0){
            sstmts.tmpres = "";
            break;
        }
    }
    if(sstmts.tmpres.size() != 0){
        return sstmts.tmpres;
    }
    return "";
}

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
        if(input[i] == '.'){
            if(tmp.size() != 0){
                split.push_back(tmp);
            }
            split.push_back(".");
            tmp = "";
        }
        else if(input[i] == '('){
            if(tmp.size() != 0){
                split.push_back(tmp);
            }
            split.push_back("(");
            tmp = "";
        }
        else if(input[i] == ')'){
            if(tmp.size() != 0){
                split.push_back(tmp);
            }
            split.push_back(")");
            tmp = "";
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
