#include <string>
#include <string.h>

using namespace std;

bool contains(vector<string> iter, string target){

    for(string it: iter){
        if(target == it){
            return true;
        }
    }
    return false;
}