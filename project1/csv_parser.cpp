#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <unordered_map>

#include "attribute_vector.cpp"

class CSVParser {
public:
    std::unordered_map<std::string, int> classifications;
    std::vector<AttributeVector*> parsed_data;

    CSVParser (std::string file_name, int attribs) {
        csv_data.open(file_name);
        num_attr = attribs;
        has_parsed = false;
        line_delimiter = ',';
    }

    CSVParser (std::string fn, int a, char delim): CSVParser(fn, a) {
        line_delimiter = delim;
    }

    ~CSVParser () {
        for (int i = 0; i < parsed_data.size(); i++)
            delete parsed_data[i];
    }

    void parse () {
        // Only parse once.
        if (has_parsed) return;

        has_parsed = true;
        std::string line;

        // While the file has lines, read a line into `line`
        while (std::getline(csv_data, line)) {
            std::vector<std::string> line_data;
            // std::vector<double> line_data;
            std::stringstream line_stream(line);
            std::string cell;

            // Delimit the line by commas and read the attributes.
            for (int i = 0; i < num_attr; i++) {
                std::getline(line_stream, cell, line_delimiter);
                line_data.push_back(cell);
                // line_data.push_back(atof(cell.c_str()));
            }

            // Read in the class name.
            std::getline(line_stream, cell, ',');

            int class_name_value;
            auto search = classifications.find(cell);
            // If the class name is not in our map, pair it with the current
            // size of the map. This has the effect of assigning unique,
            // numeric class identifiers to each class name.
            if (search == classifications.end()) {
                class_name_value = classifications.size();
                classifications.insert(std::make_pair(cell, class_name_value));
            // Otherwise, record the class name's identifier.
            } else {
                class_name_value = search->second;
            }

            parsed_data.push_back(
                new AttributeVector(line_data, class_name_value)
            );
        }
    }

private:
    std::ifstream csv_data;
    char line_delimiter;
    bool has_parsed;
    int num_attr;
};
