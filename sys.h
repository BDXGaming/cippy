#include <iostream>
#include <string>
#include <string.h>
#include "utils.h"
#include <map>

using namespace std;

class Output{
    public:
        void write(string content){
            std::cout << content;
        }
};

class sys{
    public:
        Output standard_output = Output();
        // map<string, class> commands; 
        // commands = {{"sys.stdout.write", standard_output}};

        static vector<string> get_module_cmds(){
            vector<string> cmds;
            cmds.push_back("sys.stdout.write");
            return cmds;
        }

};


void handle_command(string cmd, string input){
    if(contains(sys::get_module_cmds(), cmd)){
        cout << "YES";
    }else{
        cout << "NO";
    }
}