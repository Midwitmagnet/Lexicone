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

extern std::string sr_container_url;
extern std::vector<std::pair<std::string, std::string>> indexing_vector;
extern std::ofstream& file;
extern std::string sr_container_node;


std::string handle_inline(pugi::xml_node& p_node) {
    std::string inlined_text;
    for (pugi::xml_node inline_node : p_node.children("inline")) {
        if (inline_node.text().empty()) {
            continue;
        }
        inlined_text += inline_node.text().as_string();
    }
    return inlined_text;
}


void write_structure_for_nodes(std::string& eId) {
    file << "   ex:ispartof " << sr_container_node << "> ;\n";
    for (size_t i = 0; i < indexing_vector.size(); ++i) {
        if (i == indexing_vector.size() - 1) {
            file << "   ex:ispartof <http://lexicone.com/structurenode/" << indexing_vector.at(i).second << "> .\n";
        }
        else {
            file << "   ex:ispartof <http://lexicone.com/structurenode/" << indexing_vector.at(i).second << "> ;\n";

        }

    }
}

void write_article(std::string& eId, std::string& article_text) {
    file << "<http://lexicone.com/articlenode/" << eId << "> a ex:articlenode ;\n";
    file << "   ex:hastext \"" << article_text << "\" ;\n";
    write_structure_for_nodes(eId);

}