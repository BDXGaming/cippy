#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <map>

#define py_true "True"
#define py_false  "False"

using namespace std;

string keywords[8] = {"print", "if","elif","else", "def", "(", ")", "    "};
string conditionals[3] = {"if", "else if", "else"}; 
char alpha[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void print(string content){ 
    std::cout << content << endl;
}

string strip(string input){
    string res;
    for(char c: input){
        if(c != ' '){
            res += c;
        }
    }
    return res;
}

string drop_whitespace(string input){
    string res;
    bool reached_content= false;
    for(char c: input){
        if(c != ' ' || reached_content){
            reached_content = true;
            res += c;
        }
    }
    return res;
}

int getEnd(string line){
    int end;

    end = line.find("\")");
    if(end != 0){
        return end;
    }
    return 0;
}
void showList(vector<string> ls){
    /*
    * This method is used to display a list<string> to the console.
    */
    std::cout <<"[";
    for(string ele: ls){
        std::cout << ele << ", ";
    }
    std::cout << "]" << endl;
}

vector<string> parseLine(string line){
    vector<string> parsed_line;
    string holder;
    bool exempt;
    for(int i=0; i< line.length(); i++){

        exempt = false;

        if(line[i] == ')'){
            parsed_line.push_back(holder);
            holder = "";
        }

        if(line[i] == ':'){
            parsed_line.push_back(holder);
            holder = "";
        }
        if(line[i] == '=' && (line[i+1] != '=')){
            parsed_line.push_back(holder);
            parsed_line.push_back("=");
            holder = "";
            exempt = true;
        }
        if(!(exempt)){
            holder += line[i];
        }


        for(string kwd: keywords){
            if(kwd == holder){
                parsed_line.push_back(holder);
                holder = "";
            }
        }
    }

    if(holder != ""){
        parsed_line.push_back(holder);
    }
    

    return parsed_line;
}

map<string, string> get_variables(vector<vector<string>> parsed_lines){
    map<string, string> varibles;
    string last, key;
    bool is_var = false;
    for(vector<string> line: parsed_lines){
        for(string s: line){
            if(s == "="){
                is_var = true;
                key = last;
            }
            else if(is_var){
                varibles.insert({strip(key),s});
                is_var = false;
            }
            last = s;
        }
    }
    return varibles;
}

bool contains(string obj, string check[], int size){
    for(int i = 0; i < size; i++){
        string ls = check[i];
        if(obj == ls){
            return true;
        }
    }
    return false;
}

bool contains(char obj, char check[], int size){
    for(int i = 0; i < size; i++){
        char ls = check[i];
        if(obj == ls){
            return true;
        }
    }
    return false;
}

string get_content(string inital_content, map<string, string> vars){
    string content = inital_content;
    if(inital_content[0] == '"'){
        content = inital_content.substr(1, inital_content.length()-2);
    }else if(vars.count(inital_content) == 1){
        content = get_content(drop_whitespace(vars.at(inital_content)), vars);
    }
    return content;
}

void eval(vector<vector<string>> code, map<string,string> vars){

    bool in_conditional = false;    
    string condition = "";
    for(vector<string> line: code){
        string content = "";
        bool has_func = false;
        for(int i =0; i < line.size(); i++){

            // Handles conditional code
            if(contains(line[i], conditionals, conditionals->size())){
                in_conditional = true;
            }else if(in_conditional){

                if(line[i] == ":"){
                    if(condition == py_true){
                        in_conditional = false;
                        condition = "";
                    }
                }else{
                    if(line[i] != " "){
                        condition += strip(line[i]);
                    }
                }
                
                continue;
            }
            // Handles the print statement
            else if(line[i] == "print"){
                has_func = true;
            }
            else if(line[i] != "(" && line[i] != ")"){
                if(has_func){
                    content += line[i];
                }
            }
        }
        if(has_func){
            print(get_content(content, vars));
        }
    }
}


int main(){

    typedef map<string, map<string, list<string>>> dict;

    list<string> functions;
    vector<vector<string>> lines;
    map<string,string> vars;
    ifstream myfile; myfile.open("test.py");
    string myline;

    if ( myfile.is_open() ) {
        while ( myfile ) {
            getline (myfile, myline);
            int t = myfile.tellg();
            lines.push_back(parseLine(myline));
        }
    }
    vars = get_variables(lines);
    eval(lines, vars);
    return 0;
}

// Old Code
// if(myline.rfind("print") == 0){
//     print(myline.substr(7, getEnd(myline)-7));
// }