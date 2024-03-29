#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <array>
#include <map>
#include "sys.h"
#include "calculator.h"
#include "functions.h"

#define py_true "True"
#define py_false "False"
#define True true
#define False false

using namespace std;

string keywords[9] = {"print", "if", "elif", "else", "def", "(", ")", "    ", "sys.stdout.write"};
string conditionals[3] = {"if", "elif", "else"};
char nums[10] = {'0','1','2', '3', '4','5','6','7','8','9'};
char alpha[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char alphaUpper[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

string strip(string input)
{
    string res;
    for (char c : input)
    {
        if (c != ' ')
        {
            res += c;
        }
    }
    return res;
}

string drop_whitespace(string input)
{
    string res;
    bool reached_content = false;
    for (char c : input)
    {
        if (c != ' ' || reached_content)
        {
            reached_content = true;
            res += c;
        }
    }
    return res;
}

int getEnd(string line)
{
    int end;

    end = line.find("\")");
    if (end != 0)
    {
        return end;
    }
    return 0;
}
void showList(vector<string> ls)
{
    /*
     * This method is used to display a list<string> to the console.
     */
    std::cout << "[";
    for (string ele : ls)
    {
        std::cout << ele << ", ";
    }
    std::cout << "]" << endl;
}

vector<string> lex(string line)
{
    string token = "";
    vector<string> lexed_line;
    for (char c : line)
    {
        if (c != ' ')
        {
            token += c;
        }
        else
        {
            lexed_line.push_back(token);
            token = "";
        }
    }
    lexed_line.push_back(token);
    return lexed_line;
}

vector<string> parseLine(string line)
{
    vector<string> parsed_line;
    string holder;
    bool exempt;
    for (int i = 0; i < line.length(); i++)
    {

        exempt = false;

        if (line[i] == ')')
        {
            parsed_line.push_back(holder);
            holder = "";
        }

        if (line[i] == ':')
        {
            parsed_line.push_back(holder);
            holder = "";
        }
        if (line[i] == '=' && (line[i + 1] != '='))
        {
            parsed_line.push_back(holder);
            parsed_line.push_back("=");
            holder = "";
            exempt = true;
        }
        if (!(exempt))
        {
            holder += line[i];
        }

        for (string kwd : keywords)
        {
            if (kwd == holder)
            {
                parsed_line.push_back(holder);
                holder = "";
            }
        }
    }

    if (holder != "")
    {
        parsed_line.push_back(holder);
    }
    // showList(parsed_line);
    return parsed_line;
}

map<string, string> get_variables(vector<vector<string>> parsed_lines)
{
    map<string, string> varibles;
    string last, key;
    bool is_var = false;
    for (vector<string> line : parsed_lines)
    {
        for (string s : line)
        {
            if (s == "=")
            {
                is_var = true;
                key = last;
            }
            else if (is_var)
            {
                varibles.insert({strip(key), s});
                is_var = false;
            }
            last = s;
        }
    }
    return varibles;
}

bool contains(string obj, string check[], int size)
{
    for (int i = 0; i < size; i++)
    {
        string ls = check[i];
        if (obj == ls)
        {
            return true;
        }
    }
    return false;
}

bool contains(char obj, char check[], int size)
{
    for (int i = 0; i < size; i++)
    {
        char ls = check[i];
        if (obj == ls)
        {
            return true;
        }
    }
    return false;
}

string get_content(string inital_content, map<string, string> vars)
{
    string content = inital_content;
    if (inital_content[0] == '"')
    {
        content = inital_content.substr(1, inital_content.length() - 2);
    }
    else if (vars.count(inital_content) == 1)
    {
        content = get_content(drop_whitespace(vars.at(inital_content)), vars);
    }
    return content;
}

void eval(vector<vector<string>> code, map<string, string> vars)
{

    bool in_conditional = false;
    string condition = "";
    for (vector<string> line : code)
    {
        string content = "";
        bool has_func = false;
        for (int i = 0; i < line.size(); i++)
        {

            // Handles conditional code
            if (contains(line[i], conditionals, conditionals->size()))
            {
                in_conditional = true;
            }
            else if (in_conditional)
            {

                if (line[i] == ":")
                {
                    if (condition == py_true)
                    {
                        in_conditional = false;
                        condition = "";
                    }
                }
                else
                {
                    if (line[i] != " ")
                    {
                        condition += strip(line[i]);
                    }
                }

                continue;
            }
            // Handles the print statement
            else if (line[i] == "print")
            {
                has_func = true;
            }
            else if (line[i] != "(" && line[i] != ")")
            {
                if (has_func)
                {
                    content += line[i];
                }
            }
        }
        if (has_func)
        {
            print(get_content(content, vars));
        }
    }
}

/**
 * Gets all the imports from the lines in the files
 */
vector<string> get_imports(vector<vector<string>> lines)
{
    vector<string> imports;
    for (vector<string> line : lines)
    {
        bool next = false;
        for (string word : line)
        {
            vector<string> words = lex(word);
            // showList(words);

            if (words.size() == 1)
            {
                word = words[0];
                if (word == "import")
                {
                    next = true;
                }
                else if (next)
                {
                    imports.push_back(word);

                    next = false;
                }
            }
            else
            {
                for (string word : words)
                {

                    if (word == "import")
                    {
                        next = true;
                    }
                    else if (next)
                    {
                        imports.push_back(word);
                        next = false;
                    }
                }
            }
        }
    }
    return imports;
}

bool isMath(string input)
{
    char allowed_chars[7] = {'+', '-', '*', '/', '^', ')','('};
    for (char c : input)
    {
        if(!(ArrayUtils::contains(allowed_chars, c, 7)) && !(ArrayUtils::contains(nums, c, 10))){
            return false;
        }
    }

    return true;
}

vector<string> load_default_functions(){
    vector<string> functions;
    functions.push_back("print");
    functions.push_back("exit");

    return functions;
}

map<string, string> load_command_map(vector<string> funcs){
    map<string, string> cm;
    for(string cmd: funcs){
        cm.insert({cmd, "local"});
    }
    return cm;
}

int main(int argc, char **argv)
{
    typedef map<string, map<string, list<string>>> dict;

    vector<string> functions = load_default_functions();
    vector<vector<string>> lines;
    map<string, string> vars;
    vector<string> imports;
    string myline;
    vector<string> loaded_cmds;
    map<string, string> command_map = load_command_map(functions);

    if (!(argv[1]))
    {
        string input = "";
        cout << "Loading CIPPY 0.0.1 Early Release Beta" << endl;
        while (true)
        {
            cout << "\n>>>";
            getline(cin, input);

            if (input == "exit()")
            {
                return 0;
            }
            if(isMath(input)){
                cout << math::calc(input);
            }else{
                lines.clear();
                // vector<string> line = lex(input);
                vector<string> pline = parseLine(input);
                lines.push_back(pline);
                eval(lines,vars);
            }
        }
    }

    ifstream myfile;
    myfile.open(argv[1]);

    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, myline);
            int t = myfile.tellg();
            lines.push_back(parseLine(myline));
        }
    }
    vars = get_variables(lines);
    imports = get_imports(lines);
    eval(lines, vars);
    sys s = sys();
    sys::handle_command("sys.stdout.write", "Test from active command handler");

    return 0;
}