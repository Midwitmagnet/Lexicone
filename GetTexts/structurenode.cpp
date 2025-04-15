#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include "header.h"
#include <pugixml.hpp>
#include <unordered_set>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <algorithm> // For std::find_if
#include <utility>  // For std::pair
#include "header.h"
#include <iterator>

extern std::string sr_container_url;
extern std::ofstream& file;
extern std::string sr_container_node;

std::vector<std::pair<std::string, std::string>> indexing_vector;
int find_level(std::string eid) {
    int count = 0;
    int pos = 0;
    std::string lvl = "lvl";
    int len = lvl.length();
    while ((pos = eid.find(lvl, pos)) != std::string::npos) {
        ++count;
        pos += len;
    }
    return count;

    // when writing a level, set all levels below to empty
    // first one: container node url
}


void set_structure(pugi::xml_node& node) {
	std::string nodetype = node.name();
    std::string eId = node.attribute("eId").value();
    if (nodetype == "level") {
        nodetype = "level" + find_level(eId);
    }

    auto it = std::find_if(indexing_vector.begin(), indexing_vector.end(), [&nodetype](const auto& pair) {
        return pair.first == nodetype;
        });
    if (it != indexing_vector.end()) {
        int index = std::distance(indexing_vector.begin(), it);
        indexing_vector.erase(indexing_vector.begin()+index, indexing_vector.end());
    }
    std::pair<std::string, std::string> structurepair(nodetype, eId);

    pugi::xml_node nat_lang_num_node = node.child("num");
    std::string nat_lang_num;
    pugi::xml_node nat_lang_desc_node = node.child("heading");
    std::string nat_lang_desc;
    if (nat_lang_num_node) {
        nat_lang_num = nat_lang_num_node.text().as_string();
    }
    if (nat_lang_desc_node) {
        nat_lang_desc = nat_lang_desc_node.text().as_string();
    }
    file << "<http://lexicone.com/structurenode/"<<eId<<"> a ex:structurenode ;\n";
    file << "   ex:eId \"" << eId << "\" ;\n"; 
    file << "   ex:header \"" << nat_lang_num << "\" ; \n";
    file << "   ex:header_title \"" << nat_lang_desc << "\" ;\n";
    file << "   ex:ispartof " << sr_container_node << ">";
    if (indexing_vector.size() == 0) {
        file << " .\n";
    }
    else {
        file << " ;\n";
        for (size_t i = 0; i < indexing_vector.size(); ++i) {
            if (i == indexing_vector.size() - 1) {
                file << "   ex:ispartof <http://lexicone.com/structurenode/" << indexing_vector.at(i).second << "> .\n";
            }
            else {
                file << "   ex:ispartof <http://lexicone.com/structurenode/" << indexing_vector.at(i).second << "> ;\n";
            }
            

        }
    }
    indexing_vector.push_back(structurepair);
    //test
}