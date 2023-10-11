#include <any>
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <functional>

#include "utils.h"

using namespace std;

class Output{
    public:
        void write(string content){
            std::cout << content;
        }
};

using OutputFunction = void (Output::*)(const std::string&);

class sys{
    public:
        Output standard_output;
        std::map<std::string, std::function<void(Output*, const std::string&)>> commands;

        sys() {
            commands["sys.stdout.write"] = [](Output* out, const std::string& s) {
                (out->*(&Output::write))(s);
            };
        }

        static vector<string> get_module_cmds(){
            vector<string> cmds;
            cmds.push_back("sys.stdout.write");
            return cmds;
        }

        static void handle_command(string cmd, string input){
            if(contains(sys::get_module_cmds(), cmd)){
                sys inst = sys();
                inst.commands[cmd](&inst.standard_output, input);
            }else{
                cout << "NO";
            }
        }

};