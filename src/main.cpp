#include <iostream>

using namespace std;

/**
 * @brief Main function
 * 
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int 
 */
int main(int argc, char *argv[])
{
    try
    {
        cout << "Hello, World!" << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Unknown exception" << endl;
        return 1;
    }
    
    return 0;
}