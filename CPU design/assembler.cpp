#include<bits/stdc++.h>
using namespace std;

void opcodesGenerate(string arr[10]){
    arr[0]="0110011";// r-type
    arr[1]=arr[2]="0010011";// i-type
    arr[3]="0000011";//l-type
    arr[4]="0100011";//s-type
    arr[5]="1100011";//b-type
    arr[6]="1101111"; //jal
    arr[7]="1100111";//jalr
    arr[8]="0110111";//lui
    arr[9]="0010111";//auipc
}

void commandToType(unordered_map<string,int>& com){
    com["add"]=com["slt"]=com["sltu"]=com["and"]=com["or"]=com["xor"]=com["sll"]=com["srl"]=com["sra"]=com["sub"]=0;
    com["mul"]=com["mulh"]=com["mulhsu"]=com["mulhu"]=com["div"]=com["divu"]=com["rem"]=com["remu"]=0;
    com["addi"]=com["slti"]=com["sltiu"]=com["andi"]=com["ori"]=com["xori"]=1;
    com["slli"]=com["srli"]=com["srai"]=2;
    com["lw"]=com["ld"]=com["lh"]=com["lb"]=com["lwu"]=com["lhu"]=com["lbu"]=3;
    com["sd"]=com["sw"]=com["sh"]=com["sb"]=4;
    com["beq"]=com["bne"]=com["blt"]=com["bltu"]=com["bge"]=com["bgeu"]=5;
    com["jal"]=6;
    com["jalr"]=7;
    com["lui"]=8;
    com["auipc"]=9;
}


void registers(unordered_map<string,int>& register_set){
    register_set["zero"]=0;
    register_set["ra"]=1;
    register_set["sp"]=2;
    register_set["gp"]=3;
    register_set["tp"]=4;
    register_set["t0"]=5;
    register_set["t1"]=6;
    register_set["t2"]=7;
    register_set["s0"]=8;
    register_set["s1"]=9;
    register_set["a0"]=10;
    register_set["a1"]=11;
    register_set["a2"]=12;
    register_set["a3"]=13;
    register_set["a4"]=14;
    register_set["a5"]=15;
    register_set["a6"]=16;
    register_set["a7"]=17;
    register_set["s2"]=18;
    register_set["s3"]=19;
    register_set["s4"]=20;
    register_set["s5"]=21;
    register_set["s6"]=22;
    register_set["s7"]=23;
    register_set["s8"]=24;
    register_set["s9"]=25;
    register_set["s10"]=26;
    register_set["s11"]=27;
    register_set["t3"]=28;
    register_set["t4"]=29;
    register_set["t5"]=30;
    register_set["t6"]=31;
}


void func3Generate(unordered_map<string,int>& func3){
    //r-type
    func3["add"]=0;
    func3["sub"]=0;
    func3["xor"]=4;
    func3["or"]=6;
    func3["and"]=7;
    func3["sll"]=1;
    func3["srl"]=5;
    func3["sra"]=5;
    func3["slt"]=2;
    func3["sltu"]=3;
    //r-type m extension
    func3["mul"]=0;
    func3["mulh"]=1;
    func3["mulhsu"]=2;
    func3["mulhu"]=3;
    func3["div"]=4;
    func3["divu"]=5;
    func3["rem"]=6;
    func3["remu"]=7;
    //i-type
    func3["addi"]=0;
    func3["xori"]=4;
    func3["ori"]=6;
    func3["andi"]=7;
    func3["slli"]=1;
    func3["srli"]=5;
    func3["srai"]=5;
    func3["slti"]=2;
    func3["sltiu"]=3;
    //l-type
    func3["lb"]=0;
    func3["lh"]=1;
    func3["lw"]=2;
    func3["lbu"]=4;
    func3["lhu"]=5;
    func3["ld"]=3;
    func3["lwu"]=6;
    //s-type
    func3["sb"]=0;
    func3["sh"]=1;
    func3["sw"]=2;
    func3["sd"]=3;
    //b-type
    func3["beq"]=0;
    func3["bne"]=1;
    func3["blt"]=4;
    func3["bge"]=5;
    func3["bltu"]=6;
    func3["bgeu"]=7;
    //j type
    func3["jalr"]=0;
}

//command to fun7 mapping
void func7Generate(unordered_map<string,int>& func7){
    //r type
    func7["add"]=0;
    func7["sub"]=32;
    func7["xor"]=0;
    func7["or"]=0;
    func7["and"]=0;
    func7["sll"]=0;
    func7["srl"]=0;
    func7["sra"]=32;
    func7["slt"]=0;
    func7["sltu"]=0;
    //r-type m extension
    func7["mul"]=1;
    func7["mulh"]=1;
    func7["mulhsu"]=1;
    func7["mulhu"]=1;
    func7["div"]=1;
    func7["divu"]=1;
    func7["rem"]=1;
    func7["remu"]=1;
    //i type
    func7["slli"]=0;
    func7["srli"]=0;
    func7["srai"]=32;
}

string decimaltobinary(int n, int bits){
    string ans="";
    for(int i=0;i<bits;i++){
        if(n&1){
            ans+='1';
        }
        else{
            ans+='0';
        }
        n=n>>1;
    }
    reverse(ans.begin(),ans.end());
    return ans;
}

//function to trim leading and trailing spaces from a string
string trim(const string &str){
    size_t first = str.find_first_not_of(" \t");
    if(first== string::npos){
        return "";
    }
     size_t last = str.find_last_not_of(" \t");
     return str.substr(first, last-first+1);
}

//split the line into operation and parameters
vector<string> binarycommand(string& s){
    vector<string> ans;
    string command="";
    int n=s.size();
    int i=0;
    while(i<n && s[i]!=' '){
        command+=s[i];
        i++;
    }
    if(command=="mv") ans.push_back("addi");
    else if (command=="li") ans.push_back("addi");
    else ans.push_back(command);
    
    while(i<n && s[i]==' ') i++;
    string t="";
    while(i<n && s[i]!=','){
        t+=s[i];
        i++;
    }
    ans.push_back(t);

    if (command=="li") ans.push_back("zero");
    
    i++;
    string t1,t2;
    t1=""; t2="";
    while(i<n && !(s[i]==',' || s[i]=='(' || s[i]=='\0')){
        t1+=s[i];
        i++;
    }
    int j=i;
    if(s[i]!='\0') i++;
    while(i<n && !(s[i]=='\0' || s[i]==')')){
        t2+=s[i];
        i++;
    }
    if (s[j]==','){
        ans.push_back(t1);
        ans.push_back(t2);
    }
    else if (s[j]=='\0'){
        ans.push_back(t1);
        if(command=="mv") ans.push_back("0");
    }
    else {
        ans.push_back(t2);
        ans.push_back(t1);
    }
    return ans;
}

//generate the binary of the command with its parameter
string generateBinary(string& cmd,unordered_map<string,int>& register_set,string arr[10],unordered_map<string,int>& com,unordered_map<string,int>& func3,unordered_map<string,int>& func7){
    if (cmd=="") return cmd;
    vector<string> param=binarycommand(cmd);
    string binaryans="";
    string opcode=arr[com[param[0]]];
    string rd,rs1,rs2;
    int temp= register_set[param[1]];
    int type=com[param[0]];
    if(type==4){
        rs2=decimaltobinary(temp,5);
    }
    else if (type==5) {
        rs1=decimaltobinary(temp,5);
    }
    else{
        rd=decimaltobinary(temp,5);
    }
    
    if(type==0){
        rs2=decimaltobinary(register_set[param[3]],5);
        rs1=decimaltobinary(register_set[param[2]],5);
    }
    else if (type==5) {
        rs2=decimaltobinary(register_set[param[2]],5);
    }
    else if (!(type==6 || type==9 || type==8)){
        rs1=decimaltobinary(register_set[param[2]],5);
    }
    string immediate="";
    if (type==6 || type==9 || type==8){
        int imm=stoi(param[2]);
        if (type==6) immediate=decimaltobinary((imm/2),20);
        else immediate=decimaltobinary(imm,20);
    }
    else if(type!=0){
        int imm=stoi(param[3]);
        if(type==2) immediate= decimaltobinary(imm,5);
        else if (type==5) immediate=decimaltobinary((imm/2),12);
        else immediate= decimaltobinary(imm,12);
    }
    string f3,f7;
    f3="";f7="";
    if(!(type==6 || type==8 || type==9)){
        f3=decimaltobinary(func3[param[0]],3);
    }
    if(type==0|| type==2){
        f7=decimaltobinary(func7[param[0]],7);
    }
    
    //final ans generation
    if(type==0 || type==2){
        binaryans+=f7;
    }
    else if(type==5 || type==4){
        binaryans+=immediate.substr(0,7);
    }
    else{
        binaryans+=immediate;
    }
    if (type==2) binaryans+=immediate;
    
    if(type==0|| type==4|| type==5){
        binaryans+=rs2;
    }
    
    if(!(type==6 || type==8 || type==9)){
        binaryans+=rs1;
        binaryans+=f3;
    }
    
    if(type==4 || type==5){
        binaryans+=immediate.substr(7);
    }
    else{
        binaryans+=rd;
    }
    
    binaryans+=opcode;
    
    return binaryans;
}


vector<string> readAndTrimFile(const string &fileName){
    vector<string> lines;
    ifstream inputFile(fileName);
    
    if(!inputFile){
        cerr<< "Error opening file:"<< fileName<< endl;
        return lines;
    }
    
    string line;
    while (getline(inputFile, line)){
        lines.push_back(trim(line));
    }
    
    inputFile.close();
    return lines;
}

void assemblyCodeGenerator(const string& inputfile, const string& outputfile){
    unordered_map<string,int> register_set;
    string arr[10];
    unordered_map<string,int> com;
    unordered_map<string,int> func3;
    unordered_map<string,int> func7;
    
    opcodesGenerate(arr);
    commandToType(com);
    registers(register_set);
    func3Generate(func3);
    func7Generate(func7);
    
    vector<string> lines;
    lines=readAndTrimFile(inputfile);
    ofstream outFile(outputfile);
    if(!outFile){
        std::cerr<< "Error opening file for writing!"<< std:: endl;
        return;
    }
    
    string bin;
    for(string s:lines){
        bin=generateBinary(s,register_set,arr,com,func3,func7);
        outFile<< bin << endl;
    }
    
    outFile.close();
}

int main(){
    string inputFile="C:/Users/bshas/OneDrive/Desktop/COA Lab/COA lab@MV/inputAssemblyCode.txt";
    string outputFile="C:/Users/bshas/OneDrive/Desktop/COA Lab/COA lab@MV/outputBinaryCode.txt";
    
    assemblyCodeGenerator(inputFile,outputFile);
    
    cout<<"\n Binary code of given assembly code successfully generated\n";
    return 0;
}