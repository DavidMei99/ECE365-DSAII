//
// ECE-365 Fall 2019 Assignment #4
// Di Mei
// merge_check.cpp
//

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
// global matrix used for DP
int dp[1001][1001];
// check merging string
bool merge(string str_a, string str_b, string str_c);

int main(){
    int numline = 1;
    string infile, outfile;
    ifstream in_file;
    ofstream out_file;
    // open files to read/write
    cout << "Enter name of input file: ";
    cin >> infile;
    cout << "Enter name of output file: ";
    cin >> outfile;
    in_file.open(infile, ios::in);
    out_file.open(outfile, ios::out);
    // read input file line by line and check merging
    if (in_file.is_open()){
        string line, str_a, str_b;
        while (getline(in_file, line)){
            if (numline % 3 == 1){
                str_a = line;
            }
            else if (numline % 3 == 2){
                str_b = line;
            }
            else{
                if (merge(str_a, str_b, line)){
                    int i = str_a.length();
				    int j = str_b.length();
                    // track the path of merging and make str_a characters uppercase
				    while (i > 0 && j >= 0){
                        if (dp[i][j] == true && (j == 0 || dp[i][j-1] == false)){
                            line[i+j-1] = toupper(line[i+j-1]);
                            i--;
                        }
                        else{
                            j--;
                        }
                    }
				    out_file << line << endl;
                }
                else{
                    out_file << "*** NOT A MERGE ***" << endl;
                }
            }
            numline++;
        }
    }
    // close files
    in_file.close();
    out_file.close();
    return 0;
}
// check merging string by using bottom-up DP
bool merge(string str_a, string str_b, string str_c){
    // if string lengths not matched
    if (str_a.length() + str_b.length() != str_c.length()){
        return false;
    }
    // initialize all entries to 0
    memset(dp, 0, sizeof(dp));
    // bottom-up DP
    for(int i = 0; i <= str_a.length(); i++){
        for(int j = 0; j <= str_b.length(); j++){
            if (i == 0 && j == 0){
                dp[i][j] = true;
            }
            else if (j == 0){
                if (str_a[i-1] == str_c[i-1]) dp[i][j] = dp[i-1][j];
            }
            else if (i == 0){
                if (str_b[j-1] == str_c[j-1]) dp[i][j] = dp[i][j-1];
            }
            else if (str_a[i-1] == str_c[i+j-1] && str_b[j-1] != str_c[i+j-1]){
                dp[i][j] = dp[i-1][j];
            }
            else if (str_b[j-1] == str_c[i+j-1] && str_a[i-1] != str_c[i+j-1]){
                dp[i][j] = dp[i][j-1];
            }
            else if (str_a[i-1] == str_c[i+j-1] && str_b[j-1] == str_c[i+j-1]){
                dp[i][j] = dp[i-1][j] || dp[i][j-1];
            }
        }
    }
    return dp[str_a.length()][str_b.length()];
}
