#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include "header.h"

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t totalBytes = size * nmemb;
    std::cout << "!!Write callback triggered with : " << totalBytes << " bytes" << std::endl;

    std::ofstream* stream = static_cast<std::ofstream*>(userdata);
    stream->write(ptr, totalBytes);

    if (!stream->good()) {
        std::cerr << "!! Stream write failed!\n";
    }

    return totalBytes;
}

bool downloadFile(const std::string& url, const std::string& outfilename) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::ofstream outfile(outfilename, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file " << outfilename << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Use a lambda as callback to write directly to the file stream
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outfile);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << "\n";
        return false;
    }
    else {
        std::cout << "Successfully downloaded: " << url << " -> " << outfilename << "\n";
    }

    curl_easy_cleanup(curl);
    outfile.close();

    std::ifstream in(outfilename, std::ios::binary | std::ios::ate);
    if (in.is_open()) {
        std::streamsize size = in.tellg();
        std::cout << "! Saved file size: " << size << " bytes\n";
    }
    else {
        std::cerr << "! Failed to reopen file to check size\n";
    }

    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << "\n";
        return false;
    }
    return true;
}

std::string stripquotes(const std::string& input) {
    size_t start = input.find_first_not_of(" \t\r\n\"");
    size_t end = input.find_last_not_of(" \t\r\n\"");

    if (start == std::string::npos || end == std::string::npos) {
        return ""; // Empty or all whitespace/quotes
    }
    return input.substr(start, end - start + 1);
}


int gettexts(std::string filepath) {
    curl_global_init(CURL_GLOBAL_DEFAULT);



    std::cout << "Got path: [" << filepath << "]\n";

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: [" << filepath << "]\n";
        std::cin.get();
        return 1;
    }

    if (file.is_open()) {

        std::string line;
        bool isHeader = true;
        static int rowcount = 0;

        while (std::getline(file, line)) {

            rowcount += 1;
            std::cout <<"Rowcount : " << rowcount;
            if (isHeader) {
                isHeader = false;
                continue;
            }

            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> columns;
            std::string outfilename;
            std::string fileurl;
            std::string outfilenametrail;

            while (std::getline(ss, item, ',')) {
                columns.push_back(item);
            }

            if (columns.size() >= 5) {
                fileurl = stripquotes(columns[4]);
                std::cout << "raw link: [ " + fileurl + " ] \n";
                std::cout << "raw firstcolumn: [ " << columns[0]<<" ] \n";
                outfilenametrail = stripquotes(columns[0]);
                std::cout << "stripped outfilename: [ " << outfilenametrail << " ] \n";
                outfilename = "E:\\xml\\XMLFiles\\" + outfilenametrail+".xml";
                std::cout << outfilename;
                std::cout << "downloading from" + fileurl;
                downloadFile(fileurl, outfilename);

            }


            

        }
    }
    curl_global_cleanup();

    return 0;
}