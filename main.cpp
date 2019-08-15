#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>

int FileExists( char *filename);
std::string splitInstruction( std::string&);

using namespace std;
int main() {
    string workDir = "/home/marc/Documents/School/nand2tetris/nand2tetris/projects/07/";
    string foldername = "Output/StackTest/";
    string VMName = "StackTest.vm";
    string assOutput = VMName.substr(0, VMName.find('.')) + ".asm";

    string inputFile = workDir + foldername + VMName;
    bool write = true;

//    string instr[] = {"push constant 2", "//neg", "push constant 5454", "neg", "push constant 0", "neg"};
    string res;
//Reading from file:
    std::string line;
    std::ifstream file;
    cout << "Opening file:" << endl;
    cout << inputFile<< endl;


    file.open(inputFile);
    if(file.is_open()){
        while (getline(file, line)){
        line = line.substr(0, line.find('\r')); //
        line = line.substr( 0, line.find('/'));
            if(!line.empty())
                res += splitInstruction(line);
        }
        file.close();
        cout << res << endl;
    }
    else{
        std::cout << "File error" << std::endl;
    }
// Old code:
//    for ( string& line : instr){
//        line = line.substr(0, line.find('\r')); //
//        line = line.substr( 0, line.find('/'));
//        if(!line.empty()){
//            res += splitInstruction(line);
//            cout << splitInstruction(line) << endl;
//        }
//    }

// Create output file:
    if (write){
        std::string loc = workDir + foldername + assOutput;
        std::ofstream out(loc);
        if (out.is_open()){
            std::cout << "Outputting file to location: " << std::endl;
            std::cout << loc << std::endl;
            out << res;
            out << "(ENDPROGRAM)\r\n";
            out << "@ENDPROGRAM\r\n";
            out << "0;JMP";
            out.close();
        } else {
            std::cout <<  "File couldn't be created" << std::endl;
        }
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

string splitInstruction(string& line) {
    string res;
    std::string lineBreaker = "\r\n";

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
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("argument") != string::npos) {
            res = "@" + mem + lineBreaker +
                  "D=A" + lineBreaker +
                  "@ARG" + lineBreaker +
                  "A=M+D" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@SP" + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" +lineBreaker +
                  "M=D" +lineBreaker;
        } else if (line.find("temp") != string::npos) { //TODO: test
            res = "@SP" + mem + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@temp_" + mem + lineBreaker +
                  "M=D" + lineBreaker ;
            return res;
        } else if (line.find("static") != string::npos) { // TODO: test
            res = "@SP" + mem + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@static_" + mem + lineBreaker +
                  "M=D" + lineBreaker ;
            return res;
        } else if (line.find("this") != string::npos) { // TODO:
            res = "@SP" + mem + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@THIS" + mem + lineBreaker +
                  "M=D" + lineBreaker ;
            return res;
        } else if (line.find("that") != string::npos) { // TODO:
            res = "@SP" + mem + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@THAT" + mem + lineBreaker +
                  "M=D" + lineBreaker ;
            return res;
        }
    } else if(line.find("pop") != string::npos) {
        // pop function
        size_t found = line.find_last_of(' ');
        string mem = line.substr(found+1, size(line));
        if (line.find("local") != string::npos) { // TODO: test
            res = "@" + mem + lineBreaker +
                  "D=A" + lineBreaker +
                  "@LCL" + lineBreaker +
                  "D=M+D" + lineBreaker +
                  "@R13"+lineBreaker +
                  "M=D" +lineBreaker+           // Pointer to local+memorylocation
                  "@SP" +lineBreaker +          // Next get value of SP
                  "M=M-1" + lineBreaker +
                  "D=M" + lineBreaker +         // Value saved in D
                  "@R13" + lineBreaker +
                  "A=M" + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("argument") != string::npos) { // TODO: test
            res = "@" + mem + lineBreaker +
                  "D=A" + lineBreaker +
                  "@ARG" + lineBreaker +
                  "D=M+D" + lineBreaker +
                  "@R13"+lineBreaker +
                  "M=D" +lineBreaker+           // Pointer to local+memorylocation
                  "@SP" +lineBreaker +          // Next get value of SP
                  "M=M-1" + lineBreaker +
                  "D=M" + lineBreaker +         // Value saved in D
                  "@R13" + lineBreaker +
                  "A=M" + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("static") != string::npos) { // TODO: test
            res = "@SP" + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@static_" + mem + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("temp") != string::npos) { // TODO: test
            res = "@SP" + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@temp_" + mem + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("this") != string::npos) { // TODO: test
            res = "@SP" + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@THIS" + mem + lineBreaker +
                  "M=D" + lineBreaker;
            return res;
        } else if (line.find("that") != string::npos) { // TODO: test
            res = "@SP" + lineBreaker +
                  "M=M-1" + lineBreaker +
                  "A=M" + lineBreaker +
                  "D=M" + lineBreaker +
                  "@THAT" + mem + lineBreaker +
                  "M=D" + lineBreaker;
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
    } else if (line.find("sub") != string::npos) {
        res = "@SP" + lineBreaker +
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "D=M" + lineBreaker +
              "@SP" + lineBreaker +
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "MD=M-D" + lineBreaker +
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("not") != string::npos){ // binary not
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("neg") != string::npos){
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "M=M+1" + lineBreaker+
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("eq") != string::npos){ // JEQ TODO:
        res = "@SP" + lineBreaker +
              "M=M-1" + lineBreaker+        // SP one back to retrieve 1. arg
              "A=M" + lineBreaker +
              "D=M" + lineBreaker +         // Save 1. arg in D
              "@SP" + lineBreaker +
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +         // Stackpointer is now at second arg
              "D=M-D" + lineBreaker +       // minus the two args
              "@IS_EQUAL" + lineBreaker +   // point to label
              "D;JEQ" + lineBreaker +
              "A=M" + lineBreaker +;
        return res;
    } else if(line.find("gt") != string::npos){ // JGE TODO:
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "M=M+1" + lineBreaker+
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("lt") != string::npos){ // JLE TODO:
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "M=M+1" + lineBreaker+
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("and") != string::npos){ //  TODO:
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "M=M+1" + lineBreaker+
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if(line.find("or") != string::npos){ // TODO:
        res = "@SP" + lineBreaker +         // Stackpointer
              "M=M-1" + lineBreaker +
              "A=M" + lineBreaker +
              "M=!M" + lineBreaker +
              "M=M+1" + lineBreaker+
              "@SP" + lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    }
}