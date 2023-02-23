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

class ArrayUtils{
    public:
        static bool contains(char ls[], char c, int len){
            /**
             * This function checks if the given char array contains the char provided.
             */
            for(int i =0; i<len; i++){
                char ele = ls[i];
                if(ele == c){
                    return true;
                }
            }
            return false;
        }
};