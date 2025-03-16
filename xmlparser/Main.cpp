

#include <iostream>
#include <pugixml.hpp>
#include <cstring>

int main() {
    std::string input;
    std::cout << "Type the abbreviation of the norm as it is typed in your file (fe zpo): ";
    std::cin >> input;
    std::string fileget;

    fileget = "C:\\lexicode\\" + input + ".xml";


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileget.c_str());



    if (!result) {
        std::cerr << "Failed to load file: " << result.description() << "\n";
        return -1;
    }

    pugi::xml_node root = doc.document_element();
    if (root) {
        std::cout << "Root element name: " << root.name() << "\n";

    }
    else {
        std::cerr << "No root element found.\n";
    }

    std::string a;
    std::string p;

    std::cout << "Enter article number (if its w. literal, enter articlenumber_lit: ";
    std::cin >> a;

    std::cout << "Enter paragraph number with _in before: ";
    std::cin >> p;

    std::string attrstrg = "art_" + a + "_e /para" + p;



    for (pugi::xml_node child = root.first_child(); child != nullptr; child = child.next_sibling()) {

        if (child.attribute("eId").value() == attrstrg) {
            std::cout << child.child("content").child("p").child_value();
        }


    }
}





  
