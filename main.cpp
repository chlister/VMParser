#include <iostream>
#include <stdio.h>

std::string splitInstruction( std::string);

using namespace std;
int main() {
    string instr[] = {"push constant 2", "push constant 10", "add"};
    for ( string line : instr){
        cout << splitInstruction(line) << endl;
    }

//    cout << "splitting string" << endl;
//    size_t found = linepush.find_last_not_of(' ');
//    cout << "last sentence: " << linepush.substr(found) << endl;
    // find command
    // find MemoryAccess
    // Find location
//    cout << splitInstruction("push local 4") << endl;

cout << "END PROGRAM" << endl;
    return 0;
}

string splitInstruction( string line) {
    string res;
    std::string lineBreaker = "\n";

    if (line.find("push") != string::npos) {
        // push method
        size_t found = line.find_last_of(' ');
        string mem = line.substr(found+1, size(line));
        if (line.find("constant") != string::npos) {
            res = "@" + mem + lineBreaker +
                   "D=A" + lineBreaker +
                   "@SP" + lineBreaker +
                   "A=M" + lineBreaker +
                   "M=D" + lineBreaker +
                   "@SP" + lineBreaker +
                   "M=M+1" + lineBreaker;
            return res;
        } else if (line.find("local") != string::npos) {
            res = "@" + mem + lineBreaker +
                   "D=A" + lineBreaker +
                   "@LCL" + lineBreaker +
                   "A=M+D" + lineBreaker +
                   "D=M" + lineBreaker +
                   "@SP" + lineBreaker +
                   "A=M" + lineBreaker +
                   "M=D" + lineBreaker +
                   "@SP" + lineBreaker +
                   "M=M+1" + lineBreaker;
            return res;
        } else if (line.find("argument") != string::npos) { // TODO:
            res = "@" + mem + lineBreaker +
                   "D=A" + lineBreaker +
                   "@ARG" + lineBreaker +
                   "A=M+D" + lineBreaker +
                   "D=M" + lineBreaker +
                   "@SP" + lineBreaker +
                   "M=M+1" + lineBreaker;
        }
    } else if(line.find("pop") != string::npos) {
        // pop function
        size_t found = line.find_last_of(' ');
        string mem = line.substr(found+1, size(line));
        if (line.find("local") != string::npos) { // TODO:
            res = "@" + mem + lineBreaker +
                  "D=A" + lineBreaker +
                  "@LCL" + lineBreaker +
                  "A=M+D" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@SP" + lineBreaker +
                  "A=M" + lineBreaker +
                  "M=D" + lineBreaker +
                  "@SP" + lineBreaker +
                  "M=M+1" + lineBreaker;
            return res;
        }
    }
    else if (line.find("add") != string::npos) {
//        cout << "line" << endl;
        res = "@SP" + lineBreaker +         // Stackpointer
               "M=M-1" + lineBreaker +      // Get last value in stack
               "A=M" + lineBreaker +
               "D=M" + lineBreaker +        // Save value in D reg
               "@SP" + lineBreaker +
               "M=M-1" + lineBreaker +      // count stackpointer one less
               "A=M" + lineBreaker +
               "MD=M+D" + lineBreaker +      // Save the value in memory
               "@SP" + lineBreaker +        // count stackpointer up
               "M=M+1" + lineBreaker;
        return res;
    }
    return res;
}


