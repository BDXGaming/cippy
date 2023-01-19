#include <iostream>
#include <string>
#include <string.h>
#include "utils.h"

using namespace std;

//string[1] commands = {"sys.std.write"};

class Output{
    public:
        void write(string content){
            std::cout << content;
        }
};

class sys{
    public:
        Output standard_output = Output();

};

vector<string> get_module_cmds(){
    vector<string> cmds;
    cmds.push_back("sys.std.write");
    return cmds;
}


void handle_command(string cmd, string input){
    if(contains(get_module_cmds(), cmd)){
        cout << "YES";
    }else{
        cout << "NO";
    }
}