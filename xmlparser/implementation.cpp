#include <iostream>
#include <pugixml.hpp>
#include <cstring>
#include <pqxx/pqxx>
#include <cstdlib>
#include <string>
#include "xmlprsrheader.h"

using std::cout;
using std::cin;
using std::endl;

extern std::string statutename;

extern std::unique_ptr<pqxx::connection> conn;


//fix: inlinename

//fix: disp

//fix: blocklist


std::string getdbdata() {

    //std::string pghost_input, pgdatabase_input, pguser_input, pgpassword_input;
    //cout << "pghost, pgdatabase, pguser, pgpassword: ";
    //cin >> pghost_input >> pgdatabase_input >> pguser_input >> pgpassword_input;

    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //std::string getdbdata = "dbname = " + pgdatabase_input + " user = " + pguser_input + " password = " + pgpassword_input;

    std::string getdbdata = "host=localhost dbname=lexicone user=postgres password=1234";

    return getdbdata;
}


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

void processe_blockList_nodes(pugi::xml_node& blocklist_node, const std::string& intro_text, pqxx::stream_to& stream, int& processed_count, std::string& art_name) {
    for (pugi::xml_node item = blocklist_node.child("item"); item; item = item.next_sibling("item")) {
        pugi::xml_node nested_item = item.child("blockList");
        if (nested_item) {
            std::string nested_intro_text = intro_text + nested_item.child("listIntroduction").text().as_string();
            processe_blockList_nodes(nested_item, nested_intro_text, stream, processed_count, art_name);
        }
        else {
            pugi::xml_node p_node = item.child("p");
            if (!p_node) {
                std::cerr << "Warning: " + art_name + " is item without p node, skipping" << std::endl;
                continue;
            }
            else {
                pugi::xml_node inlined_node = p_node.child("inline");
                std::string suffix_text;
                if (inlined_node) {
                    suffix_text = handle_inline(p_node);

                }
                else {
                    suffix_text = p_node.text().as_string();
                }
                std::string combined_text = intro_text + " " + suffix_text;
                std::string name = item.attribute("eId").as_string();
                stream << std::make_tuple(name, combined_text, "");
                processed_count += 1;
                }
            }
        }
    }



void process_paragraphs(pugi::xml_node node, pqxx::stream_to& stream, int& processed_count) {


        try {
            if (!strcmp(node.name(), "paragraph")) {
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

                    processe_blockList_nodes(blocklist_node, intro_text, stream, processed_count, art_name);
                }

                else {
                    pugi::xml_node p_node = content_node.child("p");
                    if (!p_node) {
                        std::cerr << "Warning: "+art_name+" has content without p node, skipping" << std::endl;
                        return;
                    }
                    if (p_node.first_child() && p_node.first_child().type() == pugi::node_pcdata && !p_node.first_child().next_sibling()) {
                        std::string article_text;
                        pugi::xml_node inlined_node = p_node.child("inline");
                        if (inlined_node) {
                            std::string article_text = handle_inline(p_node);

                        }
                        else {
                            article_text = p_node.text().as_string();
                        }

                        stream << std::make_tuple(art_name, article_text, " ");
                        processed_count += 1;
                    }
                    else {

                        std::string para_text;
                        for (pugi::xml_node child = p_node.first_child(); child; child = child.next_sibling()) {
                            if (child.type() == pugi::node_pcdata) {
                                para_text += child.text().as_string();
                            }
                        }

                        stream << std::make_tuple(art_name, para_text, " ");
                        processed_count += 1;
                    }
                }
            }

        }
        catch (const std::exception& e) {
            // Proper exception handling
            std::cerr << "Error in get_articles: " << e.what() << std::endl;
            throw; // Re-throw to let caller handle or log the exception
        }
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            process_paragraphs(child, stream, processed_count);
    }
}


void get_articles(pugi::xml_node& root, pqxx::connection& conn, std::string table_name) {
    try {
        pqxx::work txn(conn);

        std::cout << "work begins" << endl;

        pqxx::table_path tablename = { table_name };
        pqxx::stream_to stream = pqxx::stream_to::table(txn, tablename, { "eid", "text", "futureproof" });

        int processed_count = 0;
        try {
            process_paragraphs(root, stream, processed_count);

            stream.complete();
            txn.commit();
            std::cout << "statute successfully loaded into DB, processed " << processed_count << " paragraphs" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error during processing: " << e.what() << std::endl;
            // Don't commit if there was an error
            return;
        }


        try {
            pqxx::work verify_txn(conn);
            pqxx::result r = verify_txn.exec("SELECT COUNT(*) FROM " + table_name);
            std::cout << "Verification query shows " << r[0][0].as<int>() << " rows in the table." << std::endl;
            verify_txn.commit();
        }

        catch (const std::exception& e) {
            std::cerr << "Error verifying data: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Database error in get_articles: " << e.what() << std::endl;
    }
}

