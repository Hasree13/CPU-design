#include <bits/stdc++.h>
using namespace std;

int binaryToDecimal(const string& b,bool sign){
    if (b.empty()) return 0;
    // sign is true means signed representation
    // sign is false means unsigned representation
    if (sign==false) return stoi(b,nullptr,2);
    int s=b.size();
    int imm=stoi(b,nullptr,2);
    if (b[0]=='1'){
        imm-=(1<<s);
    }
    return imm;
}
       
class instructionMem{
    vector<string> ins;
    public:
        int finalpc;
        instructionMem(){}
        instructionMem(const string& fileName){
            ifstream file(fileName);
            string line;
            if (!file) {
                cerr<<"File could not be opened\n";
            }
            while (getline(file,line)){
                ins.push_back(line);
            }
            finalpc=(ins.size()-1)*4;
        }
        string getins(int pc){
            pc/=4;
            return ins[pc];
        }
};

class gpr {
    public:
        int data;
        int insid;
};

class registers{
    gpr regs[32];
    public:
        registers(){
            regs[0].data=0;
            regs[1].data=200;
            regs[2].data=100;
            regs[8].data=300;
            for (int i=0;i<32;i++){
                regs[i].insid=-1;
            }
        }
        int readreg(int reg){
            if (reg>=32) return -1;
            return regs[reg].data;
        }
        void writereg(int reg,int data){
            if (reg>=32) return;
            regs[reg].data=data;
        }
        bool islocked(int reg){
            return (regs[reg].insid!=-1);
        }
        void lockreg(int reg,int pc){
            if (reg>=32) return;
            regs[reg].insid=pc;
        }
        void releaselock(int reg,int pc){
            if (reg>=32) return;
            if (regs[reg].insid==pc)
                regs[reg].insid=-1;
        }
        int lockedreg(int reg) {
            if (reg>=32) return -1;
            return regs[reg].insid;
        }
};

class dataMemory{
    unordered_map<unsigned int,int> dataMem;
    public:
        int readMem(unsigned int addr){
            return dataMem[addr];
        }
        void writeMem(unsigned int addr,int data){
            dataMem[addr]=data;
        }
        void printMemory(){
            cout<<"MEMORY CONTENTS\n";
            for (auto i:dataMem) {
                cout<<i.first<<" "<<i.second<<"\n";
            }
            cout<<"\n\n";
        }
};

class MUX{
    int controlsignal;
    int input0;
    int input1;
    public:
        void setmux(int control,int i0,int i1){
            controlsignal=control;
            input0=i0;
            input1=i1;
        }
        int mux(){
            if (controlsignal){
                return input1;
            }
            else {
                return input0;
            }
        }
};

class ALU{
    public:
        bool flag;
        int alures(int alusrc1,int alusrc2,int aluselect){
            int result;
            long long r;
                if (aluselect==0){
                    result=alusrc1+alusrc2;
                }
                else if (aluselect==1){
                    result=alusrc1-alusrc2;
                }
                else if (aluselect==2){
                    result=alusrc1*alusrc2;
                }
                else if (aluselect==3){
                    r=((long long)alusrc1)*((long long)alusrc2);
                    result=r>>32;
                }
                else if (aluselect==4){
                    r=((long long)alusrc1)*((long long unsigned)alusrc2);
                    result=r>>32;
                }
               else if (aluselect==5){
                    r=((long long unsigned)alusrc1)*((long long unsigned)alusrc2);
                    result=r>>32;
                }
                else if (aluselect==6){
                    result=alusrc1/alusrc2;
                }
                else if (aluselect==7){
                    result=((unsigned)alusrc1/(unsigned)alusrc2);
                }
                else if (aluselect==8){
                    result=alusrc1%alusrc2;
                }
                else if (aluselect==9){
                    result=((unsigned)alusrc1%(unsigned)alusrc2);
                }
                else if (aluselect==10){
                    result=alusrc1 & alusrc2;
                }
                else if (aluselect==11){
                    result=alusrc1 | alusrc2;
                }
                else if (aluselect==12){
                    result=alusrc1 ^ alusrc2;
                }
                else if (aluselect==13){
                    result=(int32_t)alusrc1<(int32_t)alusrc2;
                }
                else if (aluselect==14){
                    result=(uint32_t)alusrc1<(uint32_t)alusrc2;
                }
                else if (aluselect==15){
                    result=alusrc1<<(alusrc2&31);
                }
                else if (aluselect==16){
                    result=(uint32_t)alusrc1>>(alusrc2&31);
                }
                else if (aluselect==17){
                    result=(int32_t)alusrc1>>(alusrc2&31);
                }
                else if (aluselect==18){
                    flag=(alusrc1==alusrc2);
                }
                else if (aluselect==19){
                    flag=(alusrc1!=alusrc2);
                }
                else if (aluselect==20){
                    flag=(alusrc1<alusrc2);
                }
                else if (aluselect==21){
                    flag=(alusrc1>=alusrc2);
                }
                else if (aluselect==22){
                    flag=((unsigned)alusrc1<(unsigned)alusrc2);
                }
                else if (aluselect==23){
                    flag=((unsigned)alusrc1>=(unsigned)alusrc2);
                }
            return result;
        }
};

class Adder{
    public:
        int add(int src1,int src2){
            return src1+src2;
        }
};

class controlUnit{
    public:
        bool alusrc;
        bool regRead;
        bool regWrite;
        bool memRead;
        bool memWrite;
        bool memToReg;
        bool branch;
        bool jump;
        int aluop;
        bool jalr;

    controlUnit(){
        alusrc=regRead=regWrite=memRead=memWrite=memToReg=branch=jump=jalr=0;
        aluop=0;
    }
       
    void setcontrolsignals(int opcode){
        if(opcode==51){//r-type
            alusrc=0;
            regRead=1;
            regWrite=1;
            memRead=0;
            memWrite=0;
            memToReg=0;
            branch=0;
            jump=0;
            jalr=0;
            aluop=0;
        }    
        if(opcode==19){// I-type
            alusrc=1;
            regRead=1;
            regWrite=1;
            memRead=0;
            memWrite=0;
            memToReg=0;
            branch=0;
            jump=0;
            jalr=0;
            aluop=1;
        }
        if(opcode==3){//l-type
            alusrc=1;
            regRead=1;
            regWrite=1;
            memRead=1;
            memWrite=0;
            memToReg=1;
            branch=0;
            jump=0;
            jalr=0;
            aluop=2;
        }
        if(opcode==35){//s-type
            alusrc=1;
            regRead=1;
            regWrite=0;
            memRead=0;
            memWrite=1;
            memToReg=0;
            branch=0;
            jump=0;
            jalr=0;
            aluop=2;
        }
        if(opcode==99){// b-type
            alusrc=0;
            regRead=1;
            regWrite=0;
            memRead=0;
            memWrite=0;
            memToReg=0;
            branch=1;
            jump=0;
            jalr=0;
            aluop=3;
        }
        if(opcode==111){// jal
            alusrc=0;
            regRead=0;
            regWrite=1;
            memRead=0;
            memWrite=0;
            memToReg=0;
            branch=0;
            jump=1;
            jalr=0;
            aluop=1;
        }
        if(opcode==103){// jalr
            alusrc=0;
            regRead=1;
            regWrite=1;
            memRead=0;
            memWrite=0;
            memToReg=0;
            branch=0;
            jump=0;
            jalr=1;
            aluop=1;
        }
    }
};

class aluControlUnit{
    public:
        int aluselect;
        void setcontrolsignal(int func3,int func7,int aluop){
            if (aluop==0) {
                switch (func3) {
                    case 0:
                        if (func7==0) {
                            aluselect=0;
                        }
                        else if (func7==1) {
                            aluselect=2;
                        }
                        else if (func7==32) {
                            aluselect=1;
                        }
                        break;
                    case 1:
                        if (func7==0) {
                            aluselect=15;
                        }
                        else {
                            aluselect=3;
                        }
                        break;
                    case 2:
                        if (func7==0) {
                            aluselect=13;
                        }
                        else {
                            aluselect=4;
                        }
                        break;
                    case 3:
                        if (func7==0) {
                            aluselect=14;
                        }
                        else {
                            aluselect=5;
                        }
                        break;
                    case 4:
                        if (func7==0) {
                            aluselect=12;
                        }
                        else {
                            aluselect=6;
                        }
                        break;
                    case 5:
                        if (func7==0) {
                            aluselect=16;
                        }
                        else if (func7==1){
                            aluselect=7;
                        }
                        else if (func7==32) {
                            aluselect=17;
                        }
                        break;
                    case 6:
                        if (func7==0) {
                            aluselect=11;
                        }
                        else {
                            aluselect=8;
                        }
                        break;
                    case 7:
                        if (func7==0) {
                            aluselect=10;
                        }
                        else {
                            aluselect=9;
                        }
                        break;
                }
            }
            else if (aluop==1) {
                switch (func3) {
                    case 0:
                        aluselect=0;
                        break;
                    case 1:
                    //change
                        aluselect=15;
                        break;
                    case 2:
                        aluselect=13;
                        break;
                    case 3:
                        aluselect=14;
                        break;
                    case 4:
                        aluselect=12;
                        break;
                    case 5:
                        if (func7==0){                           
                            aluselect=16;
                        }
                        else if (func7==32) {
                            aluselect=17;
                        }
                        break;
                    case 6:
                        aluselect=11;
                        break;
                    case 7:
                        aluselect=10;
                        break;
                }
            }
            else if (aluop==2) {
                aluselect=0;
            }
            else if (aluop==3) {
                switch (func3) {
                    case 0:
                        aluselect=18;
                        break;
                    case 1:
                        aluselect=19;
                        break;
                    case 4:
                        aluselect=20;
                        break;
                    case 5:
                        aluselect=21;
                        break;
                    case 6:
                        aluselect=22;
                        break;
                    case 7:
                        aluselect=23;
                        break;
                }
            }
        }
};

class immGenerator{
    public:
        int generateimm(const string& ins,int opcode,int func3){
            string binary;
            bool sign=true;
            if (opcode==51){ //r type
                return 0;
            }
            if (opcode==19){
                if (func3==1 || func3==5){ //i type
                    binary=ins.substr(7,5);
                }
                else { //i (shift) type
                    binary=ins.substr(0,12);
                }
                if (func3==3) sign=false;
            }
            else if (opcode==3){ //l type
                binary=ins.substr(0,12);
            }
            else if (opcode==35){ //s type
                binary=ins.substr(0,7)+ins.substr(20,5);

            }
            else if (opcode==99){ //b type
                binary=ins.substr(0,7)+ins.substr(20,5);
            }
            else if (opcode==111){ //jal
                binary=ins.substr(0,20);
            }
            else if (opcode==103){ //jalr
                binary=ins.substr(0,12);
            }
            else if (opcode==55 || opcode==39){ //lui, auipc
                binary=ins.substr(0,20);
            }
            return binaryToDecimal(binary,sign);
        }
};

class PCcontrolUnit{
    bool branch,jump,jalr;
    int npc,jpc,bpc,alures;
    public:
        PCcontrolUnit(){
            branch=jump=jalr=0;
            npc=jpc=bpc=alures=0;
        }
        PCcontrolUnit(bool branch, bool jump,bool jalr, int npc, int jpc, int bpc,int alures){
            this->branch= branch;
            this->jump= jump;
            this->jalr=jalr;
            this->npc= npc;
            this->jpc= jpc;
            this->bpc= bpc;
            this->alures=alures;
    }
    int mux(){
    if(branch){
        return bpc;}
    else if(jump){
        return jpc;
        }
    else if (jalr){
        return alures;}
    else
    return npc;
    }
};

class oprndForwarder {
    int IDEXrsl,EXMOrdl,MOWBrdl;
    bool RSvalid;
    controlUnit EXMOcw,MOWBcw;
    int rs,EXMOaluout,MOWBres,EXMOdpc,MOWBdpc;
    public:
        oprndForwarder(){}
        oprndForwarder(int& IDEXrsl1,int& EXMOrdl,int& MOWBrdl,bool& RS1valid,controlUnit& EXMOcw,controlUnit& MOWBcw,int& rs1,int& EXMOaluout,int& MOWBres,int& EXMOdpc,int& MOWBdpc){
            this->IDEXrsl=IDEXrsl1;
            this->EXMOrdl=EXMOrdl;
            this->MOWBrdl=MOWBrdl;
            this->RSvalid=RS1valid;
            this->EXMOcw=EXMOcw;
            this->MOWBcw=MOWBcw;
            this->rs=rs1;
            this->EXMOaluout=EXMOaluout;
            this->MOWBres=MOWBres;
            this->EXMOdpc=EXMOdpc;
            this->MOWBdpc=MOWBdpc;
        }
        int mux() {
            if (RSvalid) return rs;
            if (EXMOrdl==IDEXrsl && EXMOcw.regWrite){
                if (EXMOcw.jump || EXMOcw.jalr){
                    return (EXMOdpc+4);
                }
                else if (!EXMOcw.memToReg) {
                    return EXMOaluout;
                }
            }
            if (MOWBrdl==IDEXrsl && MOWBcw.regWrite){
                if (MOWBcw.jump || MOWBcw.jalr){
                    return (MOWBdpc+4);
                }
                else {
                    return MOWBres;
                }
            }
            return 0;
        }
};

class MOWBbuffer {
    public:
    controlUnit cw;
    int dpc;
    int rdl;
    int MOWBres;
};

class startpc {
    public:
        int PC;
        bool VALID;
        startpc(){
            PC=0;
            VALID=true;
        }
};

class ifid {
    public:
        int DPC;
        string IR;
        bool STALL,VALID;
        ifid(){
            STALL=false;
            VALID=false;
        }
};

class idex {
    public:
        int DPC;
        int JPC;
        int IMM;
        int RS1;
        int RS2;
        bool RS1VALID;
        bool RS2VALID;
        int FUNC3,FUNC7;
        int RDL;
        int RSL1;
        int RSL2;
        controlUnit CW;
        bool STALL,VALID;
        idex(){
            STALL=false;
            VALID=false;
        }
};

class exmo {
    public:
        int DPC;
        int JPC;
        int BPC;
        int ALUOUT;
        int RS2;
        int RDL;
        controlUnit CW;
        bool STALL,VALID;
        exmo(){
            STALL=false;
            VALID=false;
        }
};

class mowb {
    public:
        int DPC;
        int JPC;
        int BPC;
        int ALUOUT;
        int LDOUT;
        int RDL;
        controlUnit CW;
        bool STALL,VALID;
        mowb(){
            DPC=JPC=BPC=0;
            STALL=false;
            VALID=false;
        }
};

void maincode(const string& binaryins){
    instructionMem insmem(binaryins);
    registers regfile;
    dataMemory datamemory;
    ALU alu;
    Adder adder;
    controlUnit controlunit;
    aluControlUnit alucontrolunit;
    immGenerator immgenerator;
    MUX wbMUX1;
    MUX wbMUX2;
    MUX alusrcselector;
    MOWBbuffer mowbbuffer; //change
   
    startpc STARTPC;
    ifid IFID;
    idex IDEX;
    exmo EXMO;
    mowb MOWB;
    int npc=0;
    int finalpc=insmem.finalpc;

    cout<<"final pc: "<<finalpc<<"\n\n"; 
    cout<<"REGISTERS CONTENTS\n";
    for (int i=0;i<32;i++) {
        cout<<"reg "<<i<<" : "<<regfile.readreg(i)<<"\n";
    }
    cout<<"\n\n";

    int cycles=0;


    while(1){

        cycles++;

        // write back
        if (MOWB.VALID){
        wbMUX1.setmux(MOWB.CW.memToReg,MOWB.ALUOUT,MOWB.LDOUT);
        // for memtoreg=0, aluresult is selected; memtoreg=1, ldout is selected
        int wbMUX1result=wbMUX1.mux();
        wbMUX2.setmux((MOWB.CW.jump || MOWB.CW.jalr),wbMUX1result,(MOWB.DPC+4));
        int wbMUX2result= wbMUX2.mux();
        if(MOWB.CW.regWrite){
            regfile.writereg(MOWB.RDL,wbMUX2result);
            regfile.releaselock(MOWB.RDL,MOWB.DPC);
        }
        mowbbuffer.cw=MOWB.CW;
        mowbbuffer.dpc=MOWB.DPC;
        mowbbuffer.MOWBres=wbMUX2result;
        mowbbuffer.rdl=MOWB.RDL;
        MOWB.STALL=false;
        MOWB.VALID=false;
        if (MOWB.DPC==finalpc){
            break;
        }
        }

        //memory operation
        if (EXMO.VALID && !MOWB.STALL){
        if(EXMO.CW.memRead){
        MOWB.LDOUT=datamemory.readMem(EXMO.ALUOUT);
        }
        if(EXMO.CW.memWrite){
        datamemory.writeMem(EXMO.ALUOUT,EXMO.RS2);
        }
        MOWB.DPC=EXMO.DPC;
        MOWB.JPC=EXMO.JPC;
        MOWB.BPC=EXMO.BPC;
        MOWB.CW=EXMO.CW;
        MOWB.ALUOUT=EXMO.ALUOUT;
        MOWB.RDL=EXMO.RDL;
        EXMO.STALL=false;
        MOWB.VALID=true;
        EXMO.VALID=false;
        }

        //instruction execution
        if (IDEX.VALID && !EXMO.STALL){
        oprndForwarder oprndforwarder1(IDEX.RSL1,EXMO.RDL,mowbbuffer.rdl,IDEX.RS1VALID,EXMO.CW,mowbbuffer.cw,IDEX.RS1,EXMO.ALUOUT,mowbbuffer.MOWBres,EXMO.DPC,mowbbuffer.dpc);
        oprndForwarder oprndforwarder2(IDEX.RSL2,EXMO.RDL,mowbbuffer.rdl,IDEX.RS2VALID,EXMO.CW,mowbbuffer.cw,IDEX.RS2,EXMO.ALUOUT,mowbbuffer.MOWBres,EXMO.DPC,mowbbuffer.dpc);
        int rs1=oprndforwarder1.mux();
        int rs2=oprndforwarder2.mux();
        int alusrc1,alusrc2,zeroflag;
        alusrc1=rs1;
        alusrcselector.setmux(IDEX.CW.alusrc,rs2,IDEX.IMM);
        //if alusrc is 0, it is rs2, else imm
        alusrc2= alusrcselector.mux();
        alucontrolunit.setcontrolsignal(IDEX.FUNC3,IDEX.FUNC7,IDEX.CW.aluop);
        int aluout=alu.alures(alusrc1,alusrc2,alucontrolunit.aluselect);
        EXMO.ALUOUT=aluout;
        zeroflag= alu.flag;
        int bpc=adder.add(IDEX.DPC,(IDEX.IMM<<1));
        EXMO.BPC = bpc;
        EXMO.CW=IDEX.CW;
        bool branch=IDEX.CW.branch&zeroflag;
        EXMO.CW.branch=branch;
        EXMO.DPC=IDEX.DPC;
        EXMO.JPC=IDEX.JPC;
        EXMO.RS2=rs2;
        EXMO.RDL=IDEX.RDL;
        PCcontrolUnit pccontrolunit(branch,IDEX.CW.jump,IDEX.CW.jalr,npc,IDEX.JPC,bpc,aluout);
        STARTPC.PC = pccontrolunit.mux();
        STARTPC.VALID=true;

        if (branch || IDEX.CW.jump || IDEX.CW.jalr) {
            IFID.VALID=false;
        }

        EXMO.VALID=true;
        IDEX.STALL=false;
        IDEX.VALID=false;
        }
        else {
            STARTPC.PC = npc;
            STARTPC.VALID=true;
        }

        //instruction decode
        if (IFID.VALID && !IDEX.STALL){
        int opcode,func3,func7,rsl1,rsl2,imm;
        opcode= binaryToDecimal(IFID.IR.substr(25,7),false);
        IDEX.FUNC3=func3= binaryToDecimal(IFID.IR.substr(17,3),false);
        IDEX.FUNC7=func7= binaryToDecimal(IFID.IR.substr(0,7),false);
        rsl1= binaryToDecimal(IFID.IR.substr(12,5),false);
        rsl2= binaryToDecimal(IFID.IR.substr(7,5),false);
        IDEX.RDL= binaryToDecimal(IFID.IR.substr(20,5),false);
        IDEX.IMM=imm=immgenerator.generateimm(IFID.IR,opcode,func3);
        controlunit.setcontrolsignals(opcode);

        bool shldStall;

        if (controlunit.regRead && regfile.islocked(rsl1)){
            shldStall=EXMO.VALID && EXMO.RDL==rsl1 && EXMO.CW.memToReg;
            if (shldStall) {
                IFID.STALL=true;
                IDEX.VALID=false;
                goto instruction_fetch;
            }
            else {
                IDEX.RS1VALID=false;
            }            
        }
        else {
            if(controlunit.regRead){
                IDEX.RS1= regfile.readreg(rsl1);
                IDEX.RS1VALID=true;
            }
        }

        if ((opcode==51 || opcode==35 || opcode==99) && regfile.islocked(rsl2)){
            shldStall=EXMO.VALID && EXMO.RDL==rsl2 && EXMO.CW.memToReg;
            if (shldStall) {
                IFID.STALL=true;
                IDEX.VALID=false;
                goto instruction_fetch;
            }
            else {
                IDEX.RS2VALID=false;
            }    
        }
        else {
            if(controlunit.regRead){
                IDEX.RS2= regfile.readreg(rsl2);
                IDEX.RS2VALID=true;
            }
        }

        IDEX.RSL1=rsl1;
        IDEX.RSL2=rsl2;
        IDEX.JPC=adder.add(IFID.DPC,(imm<<1));
        IDEX.DPC=IFID.DPC;
        IDEX.CW=controlunit;
        IFID.STALL=false;
        IDEX.VALID=true;
        IFID.VALID=false;

        if (controlunit.regWrite){
            regfile.lockreg(IDEX.RDL,IFID.DPC);
        }    
        }

        instruction_fetch:
 
        //instruction fetch
        if (STARTPC.VALID && !IFID.STALL){
        if (STARTPC.PC<=finalpc) {
        IFID.IR = insmem.getins(STARTPC.PC);
        npc = adder.add(STARTPC.PC,4);
        IFID.DPC=STARTPC.PC;
        IFID.VALID=true;
        STARTPC.VALID=false;
        }
        }
    }

    cout<<"REGISTERS CONTENTS\n";
    for (int i=0;i<32;i++) {
        cout<<"reg "<<i<<" : "<<regfile.readreg(i)<<"\n";
    }
    cout<<"\n\n";
    datamemory.printMemory();

    cout<<"\n\nCycles: "<<cycles<<"\n";
}

int main() {
    string fileName="C:/Users/bshas/OneDrive/Desktop/COA Lab/COA lab@MV/outputBinaryCode.txt";
    maincode(fileName);
    return 0;
}