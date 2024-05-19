// Header for printing the game processes
#pragma once

// STL
#include <fstream>
#include <map>
#include <sstream>
#include <string>

#include<iostream>

class writer_class{
    public:
        void write_csv(std::vector <std::vector <std::string>> data, std::string filename){
            unsigned int num_col = data.size();
            unsigned int num_row = data[0].size();

            std::fstream fout;

            // opening an existing csv or creating a new csv file
            fout.open(filename, std::ios::out);

            for(unsigned int row_iter = 0; row_iter < num_row; ++ row_iter){
                for(unsigned int col_iter = 0; col_iter < num_col; ++ col_iter){
                    fout << data[col_iter][row_iter];

                    if(col_iter < num_col - 1){
                        fout << ",";
                    }
                }

                fout << "\n";
            }

            // close the csv file
            fout.close();
        };

        // write one line file
        void write_one_line(std::string text, std::string filename){
            std::fstream fout;
            fout.open(filename, std::ios::out);
            fout << text;
            fout.close();
        }

        // write blank file
        void write_blank(std::string filename){
            std::fstream fout;
            fout.open(filename, std::ios::out);
            fout.close();
        }
};

class reader_class{
    public:
        std::map <std::string, std::vector <std::string>> read_csv(std::string filename){
            // find number of cols and rows
            std::ifstream in(filename);
            int num_col = 0;
            int num_row = 0;
            if(in){
                std::string line;

                // the first line
                // not counted in num_row
                std::getline(in, line);
                {
                    std::stringstream sep(line);
                    std::string field;

                    while(std::getline(sep, field, ',')){
                        num_col += 1;
                    }
                }

                while(std::getline(in, line)){
                    num_row += 1;
                }
            }

            // close file
            in.close();

            // record col names
            in.open(filename);
            std::vector <std::string> key;
            std::vector <std::vector <std::string>> value;
            if(in){
                // the first line
                std::string line;
                int row_iter = 0;
                key.reserve(num_col);
                value = std::vector <std::vector <std::string>> (num_col);

                std::getline(in, line);
                {
                    std::stringstream sep(line);
                    std::string field;

                    while(std::getline(sep, field, ',')){
                        key.push_back(field);
                        value[key.size() - 1] = std::vector <std::string> (num_row);
                    }
                }

                while(std::getline(in, line)){
                    int col_iter = 0;

                    std::stringstream sep(line);
                    std::string field;

                    while(std::getline(sep, field, ',')){
                        value[col_iter][row_iter] = field;
                        col_iter += 1;
                    }

                    row_iter += 1;
                }
            }

            // close file
            in.close();

            // map key-values
            std::map <std::string, std::vector <std::string>> key_value;
            for(unsigned int col_iter = 0; col_iter < num_col; ++ col_iter){
                key_value.insert(std::pair <std::string, std::vector <std::string>> (key[col_iter], value[col_iter]));
            }

            return key_value;
        };

        std::string read_one_line(std::string filename){
            std::ifstream in(filename);
            std::string line;

            if(in){
                std::getline(in, line);
            }

            in.close();

            return line;
        }
};
