#include <iostream>
#include <cstdlib>

char* g_filepath = "/tmp/";

char * getFilePath(void) { 	return g_filepath; }



int main()
{
    if(const char* env_p = std::getenv("PATH"))
        std::cout << "Your PATH is: " << env_p << '\n';
}
