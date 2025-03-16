#include <iostream>
#include <pugixml.hpp>




int main()
{
    std::string input;
    std::cout << "Type the abbreviation of the norm as it is typed in your file (fe zpo): ";
    std::cin >> input;
    std::string filegt;

    filegt = "C:\\lexicode\\" + input + ".xml";

    std::cout << fileget << std::end1;
    return 0;
}
