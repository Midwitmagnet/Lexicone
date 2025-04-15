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

//when making the structure nodes: in the map, only the n_count is saved, as the node will contain the rest and it is easier to find
extern std::string statute;
extern pugi::xml_node root;
using namespace std;
extern std::string sr_container_node;
extern std::ofstream& file;
extern std::string sr_container_no;


std::unordered_set<std::string> structure_tags = { "book", "chapter", "title", "section", "level", "part" };





void iterator_names(pugi::xml_node& child) {
	std::string textname;
	std::string textabbr;
	if ((child.name() == "FRBRname") && child.attribute("xml:lang").as_string() == "de") {
        textname = child.attribute("value").as_string();
        textabbr = child.attribute("shortForm").as_string();
        std::cout << "has name" << textname << "\n";

        file << "<" << sr_container_node << "> ;";
		file	<< "    ex:hasName \"" << textname << "\"";

		if (!textabbr.empty()) {
			file << " ;\n ex:hasAbbreviation \"" << textabbr << "\"";
		}

		// End the triple block with a dot
		file << " .\n";
	}
	for (pugi::xml_node nextchild = child.first_child(); nextchild; nextchild = child.next_sibling()) {
        iterator_names(nextchild);
	}
}






void process_blockList_nodes(pugi::xml_node& blocklist_node, const std::string& intro_text, std::string& art_name) {

    for (pugi::xml_node item = blocklist_node.child("item"); item; item = item.next_sibling("item")) {
        pugi::xml_node nested_item = item.child("blockList");
        if (nested_item) {
            std::string nested_intro_text = intro_text + nested_item.child("listIntroduction").text().as_string();
            process_blockList_nodes(nested_item, nested_intro_text, art_name);
        }
        else {
            pugi::xml_node p_node = item.child("p");
            if (!p_node) {
                std::cerr << "Warning: " + art_name + " is item without p node, skipping" << std::endl;
                continue;
            }
            else {
                pugi::xml_node inlined_node = p_node.child("inline");
                std::string name = item.attribute("eId").as_string();
                std::string suffix_text;
                if (inlined_node) {
                    suffix_text = handle_inline(p_node);
                    write_article(name, suffix_text);

                }
                else {
                    suffix_text = p_node.text().as_string();
                }
                std::string combined_text = intro_text + " " + suffix_text;
                write_article(name, combined_text);
            }
        }
    }
}



void iterate_over_file(pugi::xml_node& node) {
    try {
        if (structure_tags.contains(std::string(node.name()))) {
            set_structure(node);
           


        }
        if (!strcmp("paragraph", node.name())) {
            if (!node.attribute("eId")) {
                std::cerr << "Warning: paragraph without eId attribute, skipping" << std::endl;
                return;
            }
            std::string art_name = node.attribute("eId").as_string();
            pugi::xml_node content_node = node.child("content");
            if (!content_node) {
                std::cerr << "Warning: paragraph without content node, skipping" << std::endl;
                return;
            }
            pugi::xml_node blocklist_node = content_node.child("blockList");
            if (blocklist_node) {
                std::cerr << "Debug: Found blockList in " << art_name
                    << ", contains items: " << (blocklist_node.child("item") ? "YES" : "NO") << std::endl;
                std::string intro_text = blocklist_node.child("listIntroduction").text().as_string();
                std::cout << "found blocklist_node \n";

                process_blockList_nodes(blocklist_node, intro_text, art_name);
            }

            else {
                pugi::xml_node p_node = content_node.child("p");
                if (!p_node) {
                    std::cerr << "Warning: " + art_name + " has content without p node, skipping" << std::endl;
                    return;
                }
                if (p_node.first_child() && p_node.first_child().type() == pugi::node_pcdata && !p_node.first_child().next_sibling()) {
                    std::string article_text;
                    pugi::xml_node inlined_node = p_node.child("inline");
                    if (inlined_node) {
                        std::string article_text = handle_inline(p_node);
                        write_article(art_name, article_text);

                    }
                    else {
                        article_text = p_node.text().as_string();
                        write_article(art_name, article_text);
                    }

                }

            }



        }
    }catch (const std::exception& e) {
        std::cerr << "Database error in get_articles: " << e.what() << std::endl;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        iterate_over_file(child);
    }
}