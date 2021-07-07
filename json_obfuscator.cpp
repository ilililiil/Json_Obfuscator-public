// json_obfuscator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    //variable declaration
    char c;
    char buffer[7];
    string unicode_value = "";
    string mapper[100][4] = {};
    int i = 0;
    int x = 0;
    int count = 0;
    bool signal = false;
    string input_file_name = "";
    string replace_to = "\\u00";
    string replace_with = "@";

    if (argc == 2) {
        input_file_name = argv[1];
    }
    else
    {
        input_file_name = "input.txt";
    }
    
    ifstream input_file(input_file_name);
    ofstream output_file("output.txt");
    ofstream mapper_file("mapper.txt");

    //start of the program
    if (input_file.is_open())   
    {
        //read file untill the end of file
        while (input_file.peek() != EOF) {
            input_file.get(c);

            //checking the start of the "value" information
            if (c == ':') {
                signal = true;
                x = 2;
            }

            //checkinf if it is the end of the key value pair
            if (c == ',' || c == '\"' || c == '[')
            {
                if (signal == true) {
                    signal = false;
                }
            }

            //checking for json characters
            if (c != ' ' && c != ':' && c != '{' && c != '}' && c != '[' && c != ']' && c != ',' && c != '\n' && c != '\"') {

                if (signal == true) {
                    output_file << c;
                    mapper[i][x] = mapper[i][x] + c;
                    mapper[i][x+1] = mapper[i][x+1] + c;
                }
                else {
                //getting unicode value
                sprintf_s(buffer, "\\u%04x", (int)c);
                unicode_value = buffer;
                output_file << unicode_value;

                //padding the value
                mapper[i][x] = mapper[i][x] + c;
                mapper[i][x + 1] = mapper[i][x + 1] + unicode_value;
                
                }
            }
            else
            {
                output_file << c;
                if (c == ',') { i++; x = 0; }
            }
            
        }

        for (count = 0; count <= i; count++) {
            mapper_file << mapper[count][0] + " -> " + mapper[count][1] + "\n";
            
            if (mapper[count][2] != "" && mapper[count][3] != "") {
                mapper_file << mapper[count][2] + " -> " + mapper[count][3] + "\n";
            }
            
        }
    }
    else cout << "Unable to open file";

    // closing files
    input_file.close();
    output_file.close();
    mapper_file.close();

    // replacing pattern of output file
    string line;
    size_t len = replace_to.length();
    ifstream alter_output_file("output.txt");
    ifstream alter_mapper_file("mapper.txt");
    ofstream final_output_file("final_output.txt");
    ofstream final_mapper_file("final_mapper.txt");

    while (getline(alter_output_file, line))
    {
        while (true)
        {
            size_t pos = line.find(replace_to);
            if (pos != string::npos)
                line.replace(pos, len, replace_with);
            else
                break;
        }

        final_output_file << line << '\n';
    }
    
    // replacing pattern of mapper file

    while (getline(alter_mapper_file, line))
    {
        while (true)
        {
            size_t pos = line.find(replace_to);
            if (pos != string::npos)
                line.replace(pos, len, replace_with);
            else
                break;
        }

        final_mapper_file << line << '\n';
    }

    //closing filess
    alter_output_file.close();
    alter_mapper_file.close();
    final_output_file.close();
    final_mapper_file.close();

    return 0;
}