//  ECE-365 Fall 2019 Assignment #1
//  spellcheck.cpp
//  spellcheck
//
//  Created by Di Mei on 9/21/19.
//

#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define INITIAL_SIZE 50000

using namespace std;

int main()
{
    string if_name, of_name, dict_name, line, word;
    char line_char;
    bool skip = false;
    int num_line = 0;
    clock_t start, end;
    ifstream dictionary, input;
    ofstream output;
    
    hashTable *dictHashTable = new hashTable(INITIAL_SIZE);
    cout << "Enter the name of dictionary: ";
    cin >> dict_name;
    
    start = clock(); // start to record time of loading dictionary
    dictionary.open(dict_name);
    if (!dictionary) {
        cerr << "Error: unable to open dictionary " << dict_name << " (" << strerror(errno) << ")" << endl;
        exit(0);
    }
    while (!dictionary.eof()) { // load dictionary to hashtable line by line
        getline(dictionary, line);
        for (int i = 0; i <= line.length(); i++) {
            line[i] = tolower(line[i]);
        }
        dictHashTable->insert(line);
    }
    dictionary.close();
    end = clock(); // stop time recording of loading dictionary
    cout << "Total time (in seconds) to load dictionary: " << double((end - start)) / CLOCKS_PER_SEC << endl;
    
    cout << "Enter the name of input file: ";
    cin >> if_name;
    cout << "Enter the name of output file: ";
    cin >> of_name;
    
    start = clock(); // start to record time of checking input document
    input.open(if_name);
    if (!input) {
        cerr << "Error: unable to open input document "<< if_name << " (" << strerror(errno) << ")" << endl;
        exit(0);
    }
    output.open(of_name);
    if (!output) {
        cerr << "Error: unable to open output file "<< of_name << " (" << strerror(errno) << ")" << endl;
        exit(0);
    }
    while (!input.eof()) { // spellcheck input document line by line
        getline(input, line);
        num_line++; // count number of lines
        for (int i = 0; i <= line.length(); i++) {
            line_char = tolower(line[i]);
            if (isalnum(line_char) || line_char == '\'' || line_char == '-') { // check valid characters
                word += line_char;
                if (isdigit(line_char)) skip = true; // ignore a word if it contains any digit
            }
            else if (word.length() > 0 && skip == false) {
                if (word.length() > 20) { // long word check
                    output << "Long word at line " << num_line << ", starts: " << word.substr(0,20) << endl;
                }
                else if (!dictHashTable->contains(word)) { // unknown word check
                    output << "Unknown word at line " << num_line << ": " << word << endl;
                }
                word = "";
            }
            else {
                word = "";
                skip = false;
            }
        }
    }
    input.close();
    output.close();
    end = clock(); // stop time recording of checking input document
    cout << "Total time (in seconds) to check document: " << double(end - start) / CLOCKS_PER_SEC << endl;
    
    return 0;
}
