#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>

static int varCounter = 0;
static std::string lineBreaker = "\r\n";

std::string splitInstruction( std::string&);
std::string popInstruction(std::string&);
std::string pushInstruction(std::string&);
std::string removeWhiteSpaceTrail(std::string&);

using namespace std;
int main() {
    //test
//    string lineWithTrails[] = {"string  ", "oter ", "hello worldQ "};
//    for (string line : lineWithTrails){
//        cout << removeWhiteSpaceTrail(line) << endl;
//    }


    string workDir = "/home/marc/Documents/School/nand2tetris/nand2tetris/projects/08/";
    string foldername = "Output/FibonacciSeries/";
    string VMName = "FibonacciSeries.vm";
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
    cout << "END PROGRAM" << endl;
    return 0;
}

string splitInstruction(string& line) {
    string res;
    line = removeWhiteSpaceTrail(line);
    if (line.find("push") != string::npos) {
        return pushInstruction(line);
    } else if(line.find("pop") != string::npos) {
        return popInstruction(line);
    } else if(line.find("function") != string::npos) { // TODO:
        // find function label
        size_t found = line.find_last_of(' ');
        string mem = line.substr(found+1, size(line));
        fstream ss(mem);
        int val = 0;
        ss >> val;
        string label = line.substr(line.find(' ')+1, line.substr(line.find(' ')+1, line.size()).find(' '));
        label = (label.substr(0, label.find('.'))+"$"+label.substr(label.find('.')+1, label.size()));
        // return label "@RETURNLABEL" + varCounter + lineBreaker;
        res =
             "("+label+")" + lineBreaker;
        for (int locals = 0; locals <= val+1; ++locals) { // Adds the local spaces as defined
            res += "@SP\n"
                   "M=M+1\n"
                   "A=M\n"
                   "M=0\n";
        }
        return res;
    } else if(line.find("call") != string::npos) {
// find nArgs

    } else if (line.find("goto") != string::npos) {
        string gotoLabel = line.substr(line.find(' ')+1, line.size());
        res = "@" +gotoLabel+ lineBreaker +// TODO: test
              "0;JMP\n";
        return res;
    }

    else if(line.find("return") != string::npos){
// Take the last pushed value on stack
        // Add the value to ARG[0]
        // then, using the LCL pointer, recall the values of the other pointers
        res = "@ARG\n"        // saving ARG[0]
              "D=M\n"
              "@R13\n"
              "M=D\n"
              "@SP\n"
              "AM=M-1\n"
              "D=M\n"
              "@ARG\n"
              "A=M\n"
              "M=D\n"       // Calculated value is saved in ARG[0]
              "@LCL\n"
              "A=M-1\n"       // saved THAT pointer
              "D=M\n"
              "@THAT\n"
              "M=D\n"         // Reset THAT pointer
              "@2\n"
              "D=A\n"
              "@LCL\n"
              "A=M-D\n"       // saved THIS pointer
              "D=M\n"
              "@THIS\n"
              "M=D\n"         // Reset THIS pointer
              "@3\n"
              "D=A\n"
              "@LCL\n"
              "A=M-D\n"       // saved ARG pointer
              "D=M\n"
              "@ARG\n"
              "M=D\n"         // Reset ARG pointer
              "@4\n"
              "D=A\n"
              "@LCL\n"
              "A=M-D\n"       // Saved LCL pointer
              "D=M\n"
              "@LCL\n"
              "M=D\n"         // Reset LCL pointer
              "@R13\n"
              "D=M\n"
              "@SP\n"
              "M=D+1\n"         // Point SP to return value (ARG[0])
                ;
        return res;
    }

    else if(line.find("label") != string::npos){
        // label instr
        return  "("+line.substr(line.find(' ')+1, line.size())+")" + lineBreaker;
    }
    else if(line.find("if-goto") != string::npos){
        res = "@SP" + lineBreaker+
              "A=M-1\n"
              "D=M\n"
              "@" +line.substr(line.find(' ')+1, line.size())+ lineBreaker+
              "D;JGE" + lineBreaker;
    }
    else if (line.find("add") != string::npos) {
//        cout << "line" << endl;
        res = "@SP\n"         // Stackpointer
              "M=M-1\n"      // Get last value in stack
              "A=M\n"
              "D=M\n"        // Save value in D reg
              "@SP\n"
              "M=M-1\n"      // count stackpointer one less
              "A=M\n"
              "MD=M+D\n"      // Save the value in memory
              "@SP\n"        // count stackpointer up
              "M=M+1\n";
        return res;
    } else if (line.find("sub") != string::npos) {
        res = "@SP\n"
              "AM=M-1\n"
              "D=M\n"
              "@SP\n"
              "A=M-1\n"
              "M=M-D\n";
        return res;
    } else if(line.find("not") != string::npos){ // binary not
        res = "@SP\n"         // Stackpointer
              "M=M-1\n"
              "A=M\n"
              "M=!M\n"
              "@SP\n"
              "M=M+1\n";
        return res;
    } else if(line.find("neg") != string::npos){
        res = "@SP\n"         // Stackpointer
              "M=M-1\n"
              "A=M\n"
              "M=!M\n"
              "M=M+1\n"
              "@SP\n"
              "M=M+1\n";
        return res;
    } else if(line.find("eq") != string::npos){
        stringstream ss;
        ss << varCounter;
        res = "@SP\n"
              "AM=M-1\n"       // SP one back to retrieve 1. arg
              "D=M\n"         // Save 1. arg in D
              "A=A-1\n"
              "D=M-D\n"       // minus the two args
              "@NOT_EQUAL_" + ss.str() + lineBreaker +   // point to label
              "D;JNE\n"       // Test if D == 0
              "@SP\n"
              "A=M-1\n"
              "M=-1\n"
              "@CONTINUE_" + ss.str() + lineBreaker +
              "0;JMP\n"
              "(NOT_EQUAL_"+ss.str()+")"+lineBreaker +
              "@SP\n"
              "A=M-1\n"
              "M=0\n"
              "(CONTINUE_" + ss.str()+")"+lineBreaker;
        varCounter++;
        return res;
    } else if(line.find("lt") != string::npos){
        stringstream ss;
        ss << varCounter;
        res = "@SP\n"
              "AM=M-1" + lineBreaker+        // SP one back to retrieve 1. arg
              "D=M\n"         // Save 1. arg in D
              "A=A-1\n"
              "D=M-D\n"       // minus the two args
              "@NOT_LESS_" + ss.str() + lineBreaker +   // point to label
              "D;JGE\n"       // Test if D == 0
              "@SP\n"
              "A=M-1\n"
              "M=-1\n"
              "@CONTINUE_" + ss.str() + lineBreaker +
              "0;JMP\n"
              "(NOT_LESS_"+ss.str()+")"+lineBreaker +
              "@SP\n"
              "A=M-1\n"
              "M=0\n"
              "(CONTINUE_" + ss.str()+")"+lineBreaker;
        varCounter++;
        return res;
    } else if(line.find("gt") != string::npos){
        stringstream ss;
        ss << varCounter;
        res = "@SP\n"
              "AM=M-1" + lineBreaker+        // SP one back to retrieve 1. arg
              "D=M\n"         // Save 1. arg in D
              "A=A-1\n"
              "D=M-D\n"       // minus the two args
              "@NOT_GREATER_" + ss.str() + lineBreaker +   // point to label
              "D;JLE\n"       // Test if D == 0
              "@SP\n"
              "A=M-1\n"
              "M=-1\n"
              "@CONTINUE_" + ss.str() + lineBreaker +
              "0;JMP\n"
              "(NOT_GREATER_"+ss.str()+")"+lineBreaker +
              "@SP\n"
              "A=M-1\n"
              "M=0\n"
              "(CONTINUE_" + ss.str()+")"+lineBreaker;
        varCounter++;
        return res;
    }else if(line.find("or") != string::npos){ // bitwise or ?
        res = "@SP\n"
              "M=M-1" + lineBreaker+
              "A=M\n"
              "D=M\n"
              "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "M=M|D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    }else if(line.find("and") != string::npos){ // Bitwise and?
        res = "@SP\n"
              "M=M-1" + lineBreaker+
              "A=M\n"
              "D=M\n"
              "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "M=M&D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    }
    return "";
}

string pushInstruction(string& line){
    string res;
    // push method
    size_t found = line.find_last_of(' ');
    string mem = line.substr(found+1, size(line));
    if (line.find("constant") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("local") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@LCL\n"
              "A=M+D\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("argument") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@ARG\n"
              "A=M+D\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("temp") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@5\n"
              "A=A+D\n"
              "D=M"+lineBreaker+
              "@SP"+lineBreaker +
              "A=M" +lineBreaker+
              "M=D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("static") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@16\n"
              "A=A+D\n"
              "D=M"+lineBreaker+
              "@SP"+lineBreaker +
              "A=M" +lineBreaker+
              "M=D\n"
              "@SP\n"
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("this") != string::npos) { // Take what's in THIS-pointer plus number and put it in SPmemory//
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@THIS\n"
              "A=M+D\n"
              "D=M"+lineBreaker+
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP" +lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    } else if (line.find("that") != string::npos) { // Take what's in THAT-pointer plus number and put it in SPmemory//
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@THAT\n"
              "A=M+D\n"
              "D=M"+lineBreaker+
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP" +lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    }  else if (line.find("pointer 0") != string::npos) {
        res = "@THIS\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP"+ lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    }  else if (line.find("pointer 1") != string::npos) {
        res = "@THAT\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP"+ lineBreaker +
              "M=M+1" + lineBreaker;
        return res;
    }
    return nullptr;
}
string popInstruction(string& line){
    // pop function
    string res;
    size_t found = line.find_last_of(' ');
    string mem = line.substr(found+1, size(line));
    if (line.find("local") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@LCL\n"
              "D=M+D\n"
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+           // Pointer to local+memorylocation
              "@SP" +lineBreaker +          // Next get value of SP
              "M=M-1\n"
              "A=M\n"
              "D=M\n"         // Value saved in D
              "@R13\n"
              "A=M\n"
              "M=D" + lineBreaker;
        return res;
    } else if (line.find("argument") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@ARG\n"
              "D=M+D\n"
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+           // Pointer to local+memorylocation
              "@SP" +lineBreaker +          // Next get value of SP
              "M=M-1\n"
              "A=M\n"
              "D=M\n"         // Value saved in D
              "@R13\n"
              "A=M\n"
              "M=D" + lineBreaker;
        return res;
    } else if (line.find("static") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@16\n"
              "D=A+D\n"
              "@R13\n"
              "M=D" +lineBreaker +
              "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "D=M\n"
              "@R13\n"
              "A=M" + lineBreaker+
              "M=D" + lineBreaker;
        return res;
    } else if (line.find("temp") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@5\n"
              "D=A+D\n"
              "@R13\n"
              "M=D" +lineBreaker +
              "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "D=M\n"
              "@R13\n"
              "A=M" + lineBreaker+
              "M=D" + lineBreaker;
        return res;
    } else if (line.find("this") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@THIS\n"
              "D=M+D\n"
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+           // Pointer to local+memorylocation
              "@SP" +lineBreaker +          // Next get value of SP
              "AM=M-1\n"
              "D=M\n"         // Value saved in D
              "@R13\n"
              "A=M\n"
              "M=D" + lineBreaker;
        return res;
    } else if (line.find("that") != string::npos) {
        res = "@" + mem + lineBreaker +
              "D=A\n"
              "@THAT\n"
              "D=M+D\n"
              "@R13"+lineBreaker +
              "M=D" +lineBreaker+           // Pointer to local+memorylocation
              "@SP" +lineBreaker +          // Next get value of SP
              "AM=M-1\n"
              "D=M\n"         // Value saved in D
              "@R13\n"
              "A=M\n"
              "M=D" + lineBreaker;
        return res;
    }  else if (line.find("pointer 0") != string::npos) {
        res = "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "D=M\n"
              "@THIS\n"
              "M=D"+lineBreaker ;
        return res;
    } else if (line.find("pointer 1") != string::npos) {
        res = "@SP\n"
              "M=M-1\n"
              "A=M\n"
              "@THAT\n"
              "M=D"+lineBreaker ;
        return res;
    }
    return  nullptr;
}
string removeWhiteSpaceTrail(string& line){
    for (int i = line.size()-1; i > 0; --i) {
        if(!isspace(line.at(i)))
            return line;
        else
            line = line.substr(0, i);
    }
    return nullptr;
}