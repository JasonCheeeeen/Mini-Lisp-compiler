#include<iostream>
#include<string>
#include<vector>
#include<regex>
using namespace std;

struct record{
    string state;
    string text;
};

struct record2{
    int index;
    int trans;
    string res;
};

vector<string> splitinput(string);
vector<struct record> getstate(vector<string> &);
string htmlDocument(vector<struct record>);
struct record2 htmlElement(int,int,string,vector<struct record>);
struct record2 htmlContent(int,int,string,vector<struct record>);
struct record2 htmlAttributeList(int,int,string,vector<struct record>);
struct record2 TAG_OPEN(int,int,string,vector<struct record>);
struct record2 htmlAttribute(int,int,string,vector<struct record>);
struct record2 htmlChardata(int,int,string,vector<struct record>);
struct record2 attribute(int,int,string,vector<struct record>);
struct record2 TAG_OPEN(int,int,string,vector<struct record>);
struct record2 TAG_NAME(int,int,string,vector<struct record>);
struct record2 TAG_CLOSE(int,int,string,vector<struct record>);
struct record2 TAG_OPEN_SLASH(int,int,string,vector<struct record>);
struct record2 TAG_EQUALS(int,int,string,vector<struct record>);
struct record2 HTML_TEXT(int,int,string,vector<struct record>);
struct record2 DOUBLE_QUOTE_STRING(int,int,string,vector<struct record>);
struct record2 SINGLE_QUOTE_STRING(int,int,string,vector<struct record>);

int main(){
    string in;
    string op;
    vector<string> splinput;
    vector<struct record> instate;
    getline(cin,in);
    splinput = splitinput(in);
    instate = getstate(splinput);
    op = htmlDocument(instate);
    if(op.size() == 0){
        cout<<"invalid";
    }
    else{
        cout<<op;
    }
    return 0;
}

struct record2 attribute(int index,int trans,string res,vector<struct record> in){
    struct record2 hd = DOUBLE_QUOTE_STRING(index,trans,res,in);
    if(hd.trans){
        hd.res += "attribute\n";
        return {hd.index,hd.trans,hd.res};
    }
    struct record2 hds = SINGLE_QUOTE_STRING(hd.index,hd.trans,hd.res,in);
    if(hds.trans){
        hds.res += "attribute\n";
        return {hds.index,hds.trans,hds.res};
    }
    return {hds.index,hds.trans,hds.res};
};

struct record2 htmlChardata(int index,int trans,string res,vector<struct record> in){
    struct record2 ht = HTML_TEXT(index,trans,res,in);
    if(ht.trans){
        ht.res += "htmlCharData\n";
        trans = 1;
        return {ht.index,ht.trans,ht.res};
    }
    return {ht.index,ht.trans,ht.res};
};

struct record2 htmlAttribute(int index,int trans,string res,vector<struct record> in){
    int check = 0;
    struct record2 hn = TAG_NAME(index,trans,res,in);
    if(hn.trans){
        check = 1;
        if(hn.index == in.size()){
            hn.res += "htmlAttribute\n";
            return {hn.index,hn.trans,hn.res};
        }
        struct record2 hne = TAG_EQUALS(hn.index,hn.trans,hn.res,in);
        if(hne.trans){
            check = 1;
            if(hne.index == in.size()){
                hne.res += "htmlAttribute\n";
                return {hne.index,hne.trans,hne.res};
            }
            hne = attribute(hne.index,hne.trans,hne.res,in);
            if(check == 1){
                hne.res += "htmlAttribute\n";
            }
            return {hne.index,hne.trans,hne.res};
        }
        if(check == 1){
            hne.res += "htmlAttribute\n";
        }
        return {hne.index,hne.trans,hne.res};
    }
    return {hn.index,hn.trans,hn.res};
}

struct record2 htmlAttributeList(int index,int trans,string res,vector<struct record> in){
    int check = 0;
    struct record2 hah = htmlAttribute(index,trans,res,in);
    if(hah.trans){
        check = 1;
        if(hah.index == in.size()){
            hah.res += "htmlAttributeList\n";
            return {hah.index,hah.trans,hah.res};
        }
        hah = htmlAttributeList(hah.index,hah.trans,hah.res,in);
        if(check == 1){
            hah.res += "htmlAttributeList\n";
        }
        return {hah.index,hah.trans,hah.res};
    }
    return {hah.index,1,hah.res};
}

struct record2 htmlContent(int index,int trans,string res,vector<struct record> in){
    int check = 0;
    struct record2 hh = htmlChardata(index,trans,res,in);
    if(hh.trans){
        check = 1;
        if(hh.index == in.size()){
            hh.res += "htmlContent\n";
            return {hh.index,hh.trans,hh.res};
        }
        hh = htmlContent(hh.index,hh.trans,hh.res,in);
        if(check == 1){
            hh.res += "htmlContent\n";
        }
        return {hh.index,hh.trans,hh.res};
    }
    struct record2 hhe = htmlElement(hh.index,hh.trans,hh.res,in);
    if(hhe.trans){
        check = 1;
        if(hhe.index == in.size()){
            hhe.res += "htmlContent\n";
            return {hhe.index,hhe.trans,hhe.res};
        }
        hhe = htmlContent(hhe.index,hhe.trans,hhe.res,in);
        if(check == 1){
            hhe.res += "htmlContent\n";
        }
        return {hhe.index,hhe.trans,hhe.res};
    }
    return {hhe.index,1,hhe.res};
}

struct record2 htmlElement(int index,int trans,string res,vector<struct record> in){
    int check = 0;
    struct record2 ho = TAG_OPEN(index,trans,res,in);
    if(ho.trans){
        check = 1;
        if(ho.index == in.size()){
            ho.res += "htmlElement\n";
            return {ho.index,ho.trans,ho.res};
        }
        struct record2 hon = TAG_NAME(ho.index,ho.trans,ho.res,in);
        if(hon.trans){
            check = 1;
            if(hon.index == in.size()){
                hon.res += "htmlElement\n";
                return {hon.index,hon.trans,hon.res};
            }
            struct record2 hona = htmlAttributeList(hon.index,hon.trans,hon.res,in);
            if(hona.trans){
                check = 1;
                if(hona.index == in.size()){
                    hona.res += "htmlElement\n";
                    return {hona.index,hona.trans,hona.res};
                }
                struct record2 honac = TAG_CLOSE(hona.index,hona.trans,hona.res,in);
                if(honac.trans){
                    check = 1;
                    if(honac.index == in.size()){
                        //honac.res += "htmlElement\n";
                        return {honac.index,honac.trans,""};
                    }
                    struct record2 honachc = htmlContent(honac.index,honac.trans,honac.res,in);
                    if(honachc.trans){
                        if(honachc.index == in.size()){
                            //honachc.res += "htmlElement\n";
                            cout<<honachc.res;
                            return {honachc.index,0,honachc.res};
                        }
                        struct record2 honachcs = TAG_OPEN_SLASH(honachc.index,honachc.trans,honachc.res,in);
                        if(honachcs.trans){
                            if(honachcs.index == in.size()){
                                honachcs.res += "htmlElement\n";
                                return {honachcs.index,honachcs.trans,honachcs.res};
                            }
                            struct record2 honachcsn = TAG_NAME(honachcs.index,honachcs.trans,honachcs.res,in);
                            if(honachcsn.trans){
                                if(honachcsn.index == in.size()){
                                    honachcsn.res += "htmlElement\n";
                                    return {honachcsn.index,honachcsn.trans,honachcsn.res};
                                }
                                struct record2 honachcsnc = TAG_CLOSE(honachcsn.index,honachcsn.trans,honachcsn.res,in);
                                if(honachcsnc.trans){
                                    if(honachcsnc.index == in.size()){
                                        honachcsnc.res += "htmlElement\n";
                                        return {honachcsnc.index,honachcsnc.trans,honachcsnc.res};
                                    }
                                }
                                honachcsnc.res += "htmlElement\n";
                                return {honachcsnc.index,honachcsnc.trans,honachcsnc.res};
                            }
                            honachcsn.res += "htmlElement\n";
                            return {honachcsn.index,honachcsn.trans,honachcsn.res};
                        }
                        honachcs.res += "htmlElement\n";
                        return {honachcs.index,honachcs.trans,honachcs.res};
                    }
                    honachc.res += "htmlElement\n";
                    return {honachc.index,honachc.trans,honachc.res};
                }
                honac.res += "htmlElement\n";
                return {honac.index,honac.trans,honac.res};
            }
            hona.res += "htmlElement\n";
            return {hona.index,hona.trans,hona.res};
        }
        hon.res += "htmlElement\n";
        return {hon.index,hon.trans,hon.res};
    }
    return {ho.index,ho.trans,ho.res};
}

struct record2 TAG_OPEN_SLASH(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_OPEN_SLASH"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

struct record2 TAG_EQUALS(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_EQUALS"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}
struct record2 TAG_TEXT(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_TEXT"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}
struct record2 DOUBLE_QUOTE_STRING(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "DOUBLE_QUOTE_STRING"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}
struct record2 SINGLE_QUOTE_STRING(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "SINGLE_QUOTE_STRING"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

struct record2 TAG_NAME(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_NAME"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

struct record2 TAG_CLOSE(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_CLOSE"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

struct record2 TAG_OPEN(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "TAG_OPEN"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

struct record2 HTML_TEXT(int index,int trans,string res,vector<struct record> in){
    if(in[index].state == "HTML_TEXT"){
        trans = 1;
        index ++;
    }
    else{
        trans = 0;
    }
    return {index,trans,res};
}

string htmlDocument(vector<struct record> in){
    int index = 0;
    int trans = 0;
    string res = "";
    struct record2 hd = {index,trans,res};
    while(hd.index < in.size()){
        hd = htmlElement(hd.index,hd.trans,hd.res,in);
        if(hd.trans == 0){
            hd.res = "";
            return "";
        }
    }
    cout<<temperature;
    if(hd.res.size() != 0){
        hd.res += "htmlDocument\n";
        hd.res += "valid";
    }
    return hd.res;
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
