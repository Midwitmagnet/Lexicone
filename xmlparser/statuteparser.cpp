// xmlparser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//in: xml files of legal text: verdicts, correspondance, statutes
//out: custom containers with text, tags and pointers

#include <iostream>
#include <pugixml.hpp>
#include <cstring>

//int main()
//{
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

    //vector that indexes the article name to the path within the tree
    std::vector<std::pair<std::string, std::string>> inhaltsverzeichnis;

    createindex(doc);
    //iterate thruogh all w stops for articles, paragraphs etc. and build index w/location in tree and name

       //st

   void treeaccountant(pugi::xml_node a){
       //if (levelname==article) {
       // createarticlecontainer(a)
       // goto next nextchild of parent
       //if (children)
       // 
       // 
       //elif (!nextchild)
       //      (if(!gotoparent+1)
       //         (goto
       //else goto treeccountant(nextchild)

    typedef void xml_node_iterator;      

    for (pugi::xml_node_iterator it = root.begin(), it != root.end(), ++it) {
        //if (levelname == article)
        //  make class (norm)
        //  make path
        std::string path;
        std::string node_name= node.name();
        path = "/" + node_name + path;

        node = node.parent();

        //after going back up from a node, this checks if we are at the root and, if so, adds the path to the index
        if (!node) {
            std::pair < string, string> indexentry;
            indexentry.first = path;
            indexentry.second = artname;
            inhaltsverzeichnis.push_back(indexentry);
        }


        //start at first level:

        //goto child
        //if child == x: goto child
        //if child == y: do z //append vector
            //if child == 
        //while (nextchild)

       }
    }
    
    std::cin.get();
    return 0;
}

// build own pointer w additional info and registry index. delete memory after execution, but leave registers

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
