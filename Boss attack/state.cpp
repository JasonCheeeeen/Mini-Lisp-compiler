#include<iostream>
#include<string>
#include<vector>
#include<regex>
using namespace std;
struct record{
    int index;
    int trans;
    string state;
};
struct record s1(int index,int trans,string state,string);
struct record s2(int index,int trans,string state,string);
struct record s3(int index,int trans,string state,string);
struct record s4(int index,int trans,string state,string);

int main(){
    string in;
    getline(cin,in);
    int index = 0;
    int trans = 0;
    string state;
    struct record first = {index,trans,state};
    if(in.size() == 1){
        cout<<"NO";
        return 0;
    }
    while(1){
        /*if(first.index == 0){
            first = s1(first.index,first.trans,first.state,in);
            if(first.trans == 0){
                cout<<"NO";
                return 0;
            }
        }*/
        switch(first.trans){
            case 0:
                first = s1(first.index,first.trans,first.state,in);
                break;
            case -100:
                cout<<"NO";
                return 0;
                break;
            case 2:
                first = s2(first.index,first.trans,first.state,in);
                break;
            case 3:
                first = s3(first.index,first.trans,first.state,in);
                break;
            case 4:
                first = s4(first.index,first.trans,first.state,in);
                break;
            case 100:
                cout<<"YES "<<first.state;
                return 0;
                break;
            default:
                break;
        }

    }
}

struct record s4(int index,int trans,string state,string sin){
    state = "s4";
    if(sin[index] == '$'){
        trans = 100;
        return {index,trans,state};
    }
    else if(sin[index] == 'c'){
        trans = -100;
        return {index,trans,state};
    }
    else if(sin[index] == 'a'){
        trans = -100;
        return {index,trans,state};
    }
    else if(sin[index] == 'b'){
        trans = -100;
        return {index,trans,state};
    }
}

struct record s3(int index,int trans,string state,string sin){
    state = "s3";
    if(sin[index] == '$'){
        trans = 100;
        return {index,trans,state};
    }
    if(sin[index] == 'c'){
        index++;
        trans = 4;
        return {index,trans,state};
    }
    else if(sin[index] == 'a'){
        index++;
        trans = 3;
        return {index,trans,state};
    }
    else if(sin[index] == 'b'){
        trans = -100;
        return {index,trans,state};
    }
}

struct record s2(int index,int trans,string state,string sin){
    state = "s2";
    if(sin[index] == '$'){
        trans = -100;
        return {index,trans,state};
    }
    else if(sin[index] == 'c'){
        trans = -100;
        return {index,trans,state};
    }
    else if(sin[index] == 'a'){
        index++;
        trans = 2;
        return {index,trans,state};
    }
    else if(sin[index] == 'b'){
        index++;
        trans = 4;
        return {index,trans,state};
    }
}

struct record s1(int index,int trans,string state,string sin){
    state = "s1";
    if(index == 0 && sin[index] == 'c'){
        trans = -100;
        return {index,trans,state};
    }
    if(sin[index] == 'a'){
        index++;
        trans = 2;
        return {index,trans,state};
    }
    else if(sin[index] == 'b'){
        index++;
        trans = 3;
        return {index,trans,state};
    }
}
