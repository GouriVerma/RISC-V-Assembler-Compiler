#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<bitset>
#include<fstream>
using namespace std;

#define ROp 51
#define IOp 19
#define LOp 3
#define SOp 35
#define BOp 99
#define JalOp 111
#define luiOp 55
#define AuipcOp 23

unordered_map<string,bitset<7>> funcToOpCodeBinary;
unordered_map<string,bitset<5>> registerToBinary;
vector<string> rType={"add","slt","sltu","and","or","xor","sll","srl","sub","sra"};
vector<string> iType={"addi","slti","sltiu","andi","ori","xori","slli","srli","srai"};
vector<string> lType={"lw","lh","lb","ld"};
vector<string> sType={"sw","sh","sb"};
vector<string> bType={"beq","blt","bge"};

unordered_map<string,int> rFuncToFunc3={{"add",0},{"sub",0},{"and",7},{"or",6},{"slt",2},{"sltu",3},{"sll",1},{"xor",4},{"srl",5},{"sra",5}};
unordered_map<string,int> iFuncToFunc3={{"addi",0},{"andi",7},{"ori",6},{"slli",1},{"xori",4},{"srli",5},{"srai",5}};
unordered_map<string,int> bFuncToFunc3={{"beq",0},{"bne",1},{"blt",4},{"bge",5}};
unordered_map<string,int> lFuncToFunc3={{"lw",2},{"lh",1},{"ld",3},{"lb",0}};
unordered_map<string,int> sFuncToFunc3={{"sw",2},{"sh",1},{"sd",3},{"sb",0}};


vector<string> IM;
//Some predefined Data Memory
vector<int> DM={10,3,13,1,34,11,500,22,98};
int GPR[32]={0};
int GPRSem[32]={0};



//Creates the Map for OpCode to function and initializes register values
void initialise(){
    for(auto str:rType){
        funcToOpCodeBinary[str]=bitset<7>("0110011");
    }
    
    for(auto str:iType){
        funcToOpCodeBinary[str]=bitset<7>("0010011");
    }
    
    for(auto str:lType){
        funcToOpCodeBinary[str]=bitset<7>("0000011");
    }
    
    for(auto str:bType){
        funcToOpCodeBinary[str]=bitset<7>("1100011");
    }
    
    funcToOpCodeBinary["jal"]=bitset<7>("1101111");
    funcToOpCodeBinary["jalr"]=bitset<7>("1100111");
    funcToOpCodeBinary["lui"]=bitset<7>("0110111");
    funcToOpCodeBinary["auipc"]=bitset<7>("0010111");
    
    
    for(auto str:sType){
        funcToOpCodeBinary[str]=bitset<7>("0100011");
    }
    
    for(int i=0;i<32;i++){
        string reg="x";
        reg=reg+to_string(i);
        registerToBinary[reg]=bitset<5>(i);
    }
}

//Converts hexadecimal constants to binary numbers
string convert_binary(string hexNum){
    int n=hexNum.length();
    int i;
    string bin;
    if(hexNum[0]=='-'){
        i=hexNum[2]=='x' || hexNum[2]=='X'? 3:1;
    }
    else{
        i=hexNum[1]=='x' || hexNum[1]=='X'? 2:0;
    }
    
    
    while(i<n){
        switch (hexNum[i]){
            case '0':
            bin.append("0000");
            break;
        case '1':
            bin.append("0001");
            break;
        case '2':
            bin.append("0010");
            break;
        case '3':
            bin.append("0011");
            break;
        case '4':
            bin.append("0100");
            break;
        case '5':
            bin.append("0101");
            break;
        case '6':
            bin.append("0110");
            break;
        case '7':
            bin.append("0111");
            break;
        case '8':
            bin.append("1000");
            break;
        case '9':
            bin.append("1001");
            break;
        case 'A':
        case 'a':
            bin.append("1010");
            break;
        case 'B':
        case 'b':
            bin.append("1011");
            break;
        case 'C':
        case 'c':
            bin.append("1100");
            break;
        case 'D':
        case 'd':
            bin.append("1101");
            break;
        case 'E':
        case 'e':
            bin.append("1110");
            break;
        case 'F':
        case 'f':
            bin.append("1111");
            break;
        default:
            cout<<"Invalid"<<endl;
        
        
        }
        i++;
        
    }
    if(hexNum[0]=='-'){
        bin.append("1");
    } else{
        bin.append("0");
    }
    return bin;
}

//class for Instruction Instr
class Instr{
    public:
    bitset<5> rd;
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<7> opCode;
    bitset<3> func3;
    bitset<7> func7;
    bitset<20> imm20;
    bitset<12> imm12;
    bitset<1> sign;
    string ans;
  //  bitset<5> imm5;
    
    Instr(vector<string> instr){
        this->opCode=funcToOpCodeBinary[instr[0]];
        switch(this->opCode.to_ulong()){
            
            case ROp: {
            
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: ");
                }
                cout<<"it is r type"<<endl;
                this->rd=bitset<5>(registerToBinary[instr[1]]);
                this->rs1=bitset<5>(registerToBinary[instr[2]]);
                this->rs2=bitset<5>(registerToBinary[instr[3]]);
                int idx=rFuncToFunc3[instr[0]];
                this->func3=bitset<3>(idx);
                this->func7=bitset<7>("0000000");
                if(instr[0]=="sra" || instr[0]=="sub"){
                    this->func7=bitset<7>("0100000");
                }
                this->sign=bitset<1>("0");
                ans=func7.to_string()+rs2.to_string()+rs1.to_string()+func3.to_string()+rd.to_string()+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                
                break;
            
            //i type
                
            case IOp: {
            
                cout<<"it is i type"<<endl;
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: ");
                }
                this->rd=bitset<5>(registerToBinary[instr[1]]);
                this->rs1=bitset<5>(registerToBinary[instr[2]]);
                auto it=find(iType.begin(),iType.end(),instr[0]);
                int idx=iFuncToFunc3[instr[0]];
                this->func3=bitset<3>(idx);
                
                //shift instructions
                if(instr[0]=="slli" || instr[0]=="srli" || instr[0]=="srai"){
                    string imm7="0000000";
                    if(instr[0]=="srai"){
                        imm7="0100000";
                    }
                    string immBin=convert_binary(instr[3]);
                    this->sign=bitset<1>(immBin[immBin.length()-1]);
                    immBin.pop_back();
                    int immLen=immBin.length()<5 ? immBin.length() : 5;
                    string imm5=immBin.substr(immBin.length()-immLen);
                    this->imm12=bitset<12>(imm7+imm5);
                    
                }
                else{
                    string immBin=convert_binary(instr[3]);
                    cout<<immBin<<endl;
                    this->sign=bitset<1>(immBin[immBin.length()-1]);
                    immBin.pop_back();
                    int immLen=immBin.length()<12 ? immBin.length() : 12;
                    this->imm12=bitset<12>(immBin.substr(immBin.length()-immLen));
                }
               
                ans=imm12.to_string()+rs1.to_string()+func3.to_string()+rd.to_string()+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                break;   
            
            //l type
            case LOp: {
            
                cout<<"it is l type"<<endl;
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: ");
                }
                this->rd=bitset<5>(registerToBinary[instr[1]]);
                this->rs1=bitset<5>(registerToBinary[instr[3]]); // after brackets
                

                int idx=lFuncToFunc3[instr[0]];
                this->func3=bitset<3>(idx);
                
                string immBin=convert_binary(instr[2]);
                this->sign=bitset<1>(immBin[immBin.length()-1]);
                immBin.pop_back();
                cout<<immBin<<endl;
                int immLen=immBin.length()<12 ? immBin.length() : 12;
                this->imm12=bitset<12>(immBin.substr(immBin.length()-immLen)); //if more than 12 bits so truncated to take only last 12 bits
                
                ans=imm12.to_string()+rs1.to_string()+func3.to_string()+rd.to_string()+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                break; 
                
                
            //b type
            case BOp: {
            
                cout<<"it is b type"<<endl;
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: ");
                }
                this->rs1=bitset<5>(registerToBinary[instr[1]]);
                this->rs2=bitset<5>(registerToBinary[instr[2]]); // after brackets
                
                int idx=bFuncToFunc3[instr[0]];
                this->func3=bitset<3>(idx);
                
                string immBin=convert_binary(instr[3]);
                this->sign=bitset<1>(immBin[immBin.length()-1]);
                immBin.pop_back();
                int immLen=immBin.length()<12 ? immBin.length() : 12;
                this->imm12=bitset<12>(immBin.substr(immBin.length()-immLen));
                
                ans=imm12.to_string().substr(0,7)+rs2.to_string()+rs1.to_string()+func3.to_string()+imm12.to_string().substr(7)+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                break;
                
            //jal, lui, auipc type
            case JalOp:
            case luiOp:
            case AuipcOp: {
            
                cout<<"it is lui or auipc or jal type"<<endl;
                if(instr.size()!=3){
                    throw runtime_error("Invalid Instruction: "+instr[0]);
                }
                this->rd=bitset<5>(registerToBinary[instr[1]]);
                string immBin=convert_binary(instr[2]);
                this->sign=bitset<1>(immBin[immBin.length()-1]);
                immBin.pop_back();
                
                if(instr[0]=="lui" || instr[0]=="auipc"){
                    
                    int immLen=immBin.length()<20 ? immBin.length() : 20;
                    string imm20Str=immBin.substr(0,immLen).append(20-immLen,'0');
                    this->imm20=bitset<20>(imm20Str);
                    
                }
                
                else{
                    int immLen=immBin.length()<20 ? immBin.length() : 20;
                    this->imm20=bitset<20>(immBin.substr(immBin.length()-immLen));
                }
                
                ans=imm20.to_string()+rd.to_string()+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                break;
                
            //jalr type
            case 103:  {
            
                cout<<"it is jalr type"<<endl;
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: "+instr[0]);
                }
                this->rd=bitset<5>(registerToBinary[instr[1]]);
                this->rs1=bitset<5>(registerToBinary[instr[3]]); // after brackets
                
                
                this->func3=bitset<3>(0);
                string immBin=convert_binary(instr[2]);
                this->sign=bitset<1>(immBin[immBin.length()-1]);
                immBin.pop_back();
                int immLen=immBin.length()<12 ? immBin.length() : 12;
                this->imm12=bitset<12>(immBin.substr(immBin.length()-immLen)); //if more than 12 bits so truncated to take only last 12 bits
                
                ans=imm12.to_string()+rs1.to_string()+func3.to_string()+rd.to_string()+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                
            break; 
            
            //s type
            case SOp: {
            
                cout<<"it is s type"<<endl;
                if(instr.size()!=4){
                    throw runtime_error("Invalid Instruction: ");
                }
                this->rs1=bitset<5>(registerToBinary[instr[3]]); // after brackets
                this->rs2=bitset<5>(registerToBinary[instr[1]]); 
                
                auto it=find(sType.begin(),sType.end(),instr[0]);
                int idx=sFuncToFunc3[instr[0]];
                this->func3=bitset<3>(idx);
                string immBin=convert_binary(instr[2]);
                this->sign=bitset<1>(immBin[immBin.length()-1]);
                immBin.pop_back();
                int immLen=immBin.length()<12 ? immBin.length() : 12;
                this->imm12=bitset<12>(immBin.substr(immBin.length()-immLen));
                
                ans=imm12.to_string().substr(0,7)+rs2.to_string()+rs1.to_string()+func3.to_string()+imm12.to_string().substr(7)+opCode.to_string()+sign.to_string();
                cout<<ans<<endl;
            }
                break;
            
            
        }
            
                
    }
    
    string getDecodedInstr(){
        return ans;
    }
    
    
};


class ControlWord{
    public:
    bool RegRead;
    bool RegWrite;
    bool ALUSrc;
    bool Branch;
    bool Jump;
    bool MemRead;
    bool MemWrite;
    bool Mem2Reg;
    bitset<2> ALUOp;
    
    ControlWord(bitset<7> opCode){
        
        
        if(opCode.to_ulong()==ROp || opCode.to_ulong()==IOp || opCode.to_ulong()==LOp){
            this->RegWrite=true;
        } else this->RegWrite=false;
        
        if(opCode.to_ulong()==ROp || opCode.to_ulong()==BOp){
            this->ALUSrc=false;
        } else this->ALUSrc=true;
        
        if(opCode.to_ulong()==BOp){
            this->Branch=true;
        } else this->Branch=false;
        
        if(opCode.to_ulong()==JalOp){
            this->Jump=true;
            this->RegRead=false;
        } else {
            this->Jump=false;
            this->RegRead=true;
        }
        
        if(opCode.to_ulong()==SOp){
            this->MemWrite=true;
        } else this->MemWrite=false;
        
        if(opCode.to_ulong()==LOp){
            this->MemRead=true;
            this->Mem2Reg=true;
        } else {
            this->MemRead=false;
            this->Mem2Reg=false;
        }
        
        
        //AlUOp
        switch(opCode.to_ulong()){
            case ROp:
                this->ALUOp=bitset<2>("10");
                break;
                
            
            case IOp:
                this->ALUOp=bitset<2>("11");
                break;
            
            case LOp:
            case SOp:
                this->ALUOp=bitset<2>("00");
                break;
                
            case BOp:
                this->ALUOp=bitset<2>("01");
                break;
            
            case JalOp:
            case luiOp:
            case AuipcOp:
                cout<<"Jump or luipc or Auipc"<<endl;
                this->ALUOp=bitset<2>("00");
                break;
            
            default:
                throw runtime_error("Invalid OpCode for ALUOp generation");
        }
    }
};

class ALU{
    public:
    
    bitset<4> ALUControl(bitset<2> ALUOp,bitset<3> func3,bitset<7> func7){
        if(ALUOp.to_string()=="00"){
            return bitset<4>("0010");
        }
        
        else if(ALUOp.to_string()=="01"){
            return bitset<4>("0110");
        }
        
        else if(ALUOp.to_string()=="10" || ALUOp.to_string()=="11"){
            if(ALUOp.to_string()=="10" && func7.to_string()[1]=='1'){
                return bitset<4>("0110");
            }
            
            else{
                if(func3.to_ulong()==0){
                    return bitset<4>("0010");
                }
                else if(func3.to_ulong()==6){
                    return bitset<4>("0001");
                }
                else if(func3.to_ulong()==7){
                    return bitset<4>("0000");
                }
                else if(func3.to_ulong()==4){
                    return bitset<4>("0011");
                }
                
            }
        }
        
        cout<<"Invalid ALUOp or func3 or func7"<<endl;
        throw runtime_error("Invalid ALUOp or func3 or func7 for ALUSelect Generation");
        return bitset<4>("1111");
        
        
    }
    
    int ALUOperation(bitset<4> ALUSelect,int inputData1, int inputData2){
        if(ALUSelect.to_string()=="0010"){
            return inputData1+inputData2;
        }
        else if(ALUSelect.to_string()=="0110"){
            return inputData1-inputData2;
        }
        else if(ALUSelect.to_string()=="0000"){
            return inputData1&inputData2;
        }
        else if(ALUSelect.to_string()=="0001"){
            return inputData1|inputData2;
        }
        else if(ALUSelect.to_string()=="0011"){
            return inputData1^inputData2;
        }
        else{
           throw runtime_error("Invalid ALUSelect for ALU Operation"); 
           return -1;
        }
    }
};


class IFReg{
    public:
    bool valid;
    bool stall;
    int PC;
    
    IFReg(){
        this->PC=0;
        this->valid=true;
        this->stall=false;
    }
};

class IFID{
    public:
    bool valid;
    bool stall;
    string IR;
    int DPC;
    int NPC;
    
    IFID(){
        this->valid=false;
        this->stall=false;
    }
};

class IDEX{
    public:
    bool valid;
    bool stall;
    int DPC;
    int JPC;
    ControlWord* CW;
    int rs1;
    int rs2;
    bitset<5> rsl1;
    bitset<5> rsl2;
    int imm12;
    bitset<3> func3;
    bitset<7> func7;
    bitset<5> rd;
    
    IDEX(){
        this->valid=false;
        this->stall=false;
    }
};

class EXMO{
    public:
    bool valid;
    bool stall;
    ControlWord* CW;
    int ALUResult;
    bool ALUZeroFlag;
    bitset<5> rd;
    bitset<5> rsl2;
    int rs2;
    int TPC;
    int DPC;
    
    EXMO(){
        this->valid=false;
        this->stall=false;
    }
};

class MOWB{
    public:
    bool valid;
    bool stall;
    ControlWord* CW;
    int ALUResult;
    int LDResult;
    bitset<5> rd;
    int DPC;
    
    MOWB(){
        this->valid=false;
        this->stall=false;
    }
};


class Pipeline{
    public:
    
    void fetch(IFReg* IFRegIns,IFID* IFIDIns){
        if(IFIDIns->stall) return;
        
        if(!IFRegIns->valid){
            IFIDIns->valid=false;
            return;
        }
        
        
        cout<<"Fetching from "<<IFRegIns->PC<<endl;
        
        if(IFRegIns->PC>=IM.size()){
            IFRegIns->valid=false;
            return;
        }
        
        IFIDIns->IR=IM[IFRegIns->PC];
        IFIDIns->DPC=IFRegIns->PC;
        IFIDIns->NPC=IFRegIns->PC+1;
        IFRegIns->PC=IFRegIns->PC+1;
        IFIDIns->valid=true;
        
        if(IFRegIns->PC>=IM.size()){
            IFRegIns->valid=false;
            return;
        }
        
    }
    
    
    void decode(IFID* IFIDIns,IDEX* IDEXIns){
        if(IDEXIns->stall) return;
         
        if(!IFIDIns->valid){
            IDEXIns->valid=false;
            return;
        } 
        
        cout<<"Decoding "<<IFIDIns->IR<<endl;
        IDEXIns->DPC=IFIDIns->DPC;
        
        bitset<1> signBit=bitset<1>(IFIDIns->IR.substr(32));
        if(signBit.to_ulong()==0){
            IDEXIns->JPC=IFIDIns->NPC+bitset<20>(IFIDIns->IR.substr(0,20)).to_ulong()/4;    
        }
        else{
            IDEXIns->JPC=IFIDIns->NPC-bitset<20>(IFIDIns->IR.substr(0,20)).to_ulong()/4;
        }
        
        
        bitset<7> opCode=bitset<7>(IFIDIns->IR.substr(25,7));
        
        //Immediate Generation
        bitset<12> imm12=bitset<12>(IFIDIns->IR.substr(0,12)); //for i type,ltype
        if(opCode.to_ulong()==BOp || opCode.to_ulong()==SOp){
            imm12=bitset<12>(IFIDIns->IR.substr(0,7)+IFIDIns->IR.substr(20,5));
        }
        IDEXIns->imm12=imm12.to_ulong();
        if(signBit.to_ulong()==1){
            IDEXIns->imm12=-IDEXIns->imm12;
            cout<<"updated -ve "<<IDEXIns->imm12<<endl;
        }
        
        IDEXIns->rd=bitset<5>(IFIDIns->IR.substr(20,5));
        IDEXIns->func3=bitset<3>(IFIDIns->IR.substr(17,3));
        IDEXIns->func7=bitset<7>(IFIDIns->IR.substr(0,7));
        bitset<5> rsl1=bitset<5>(IFIDIns->IR.substr(12,5));
        bitset<5> rsl2=bitset<5>(IFIDIns->IR.substr(7,5));
        
        IDEXIns->CW=new ControlWord(opCode);
        
        if(IDEXIns->CW->Jump){
            cout<<"Jumping to "<<IDEXIns->JPC<<endl;
        }
        
        //Decode continued
        cout<<"Register Semaphore values: ";
        for(auto i:GPRSem){
            cout<<i<<" ";
        }cout<<endl;
        
        if(IDEXIns->CW->RegRead && !IDEXIns->CW->Jump){
            if(GPRSem[rsl1.to_ulong()]!=0){
                IFIDIns->stall=true;
                cout<<"Stalling due to "<<rsl1.to_ulong()<<endl;
                return;
            }
            else{
                IDEXIns->rs1=GPR[rsl1.to_ulong()];     
            }
           
           
        }
        
        if(opCode.to_ulong()!=IOp && opCode.to_ulong()!=JalOp && IDEXIns->CW->RegRead){
            if(GPRSem[rsl2.to_ulong()]!=0){
                IFIDIns->stall=true;
                cout<<"Stalling due to "<<rsl2.to_ulong()<<endl;
                return;
            }
            else{
                IDEXIns->rs2=GPR[rsl2.to_ulong()];     
            }
           
            
        }
        
        //Semaphore for rd
        if(opCode.to_ulong()!=BOp && opCode.to_ulong()!=SOp && IDEXIns->rd.to_ulong()!=0){
            GPRSem[IDEXIns->rd.to_ulong()]--;
            cout<<"Updating Semaphore for "<<IDEXIns->rd.to_ulong()<<" to be "<<GPRSem[IDEXIns->rd.to_ulong()]<<endl;
        }
        
        cout<<"Decoded opCode "<<opCode.to_ulong()<<endl;
        
        IDEXIns->valid=true;
        IFIDIns->stall=false;
        IFIDIns->valid=false;
        
        
        
    }
    
    void execute(IDEX* IDEXIns, EXMO* EXMOIns, IFReg* IFRegIns, IFID* IFIDIns){
        if(EXMOIns->stall) return;
        
        if(!IDEXIns->valid){
            EXMOIns->valid=false;
            return;
        } 
        
        EXMOIns->rd=IDEXIns->rd;
        EXMOIns->CW=IDEXIns->CW;
        EXMOIns->rs2=IDEXIns->rs2;
        EXMOIns->DPC=IDEXIns->DPC;
        
        int inputData1=IDEXIns->rs1,inputData2=IDEXIns->rs2;
        
        if(IDEXIns->CW->ALUSrc){
            inputData2=IDEXIns->imm12;
            cout<<"Taking Immediate as operand "<<inputData2<<endl;
        }
        cout<<"data 1: "<<inputData1<<", data 2: "<<inputData2<<endl;
        
        //ALU Operation
        ALU* ALUUnit=new ALU();
        bitset<4> ALUSelect=ALUUnit->ALUControl(IDEXIns->CW->ALUOp,IDEXIns->func3,IDEXIns->func7);
        EXMOIns->ALUResult=ALUUnit->ALUOperation(ALUSelect,inputData1,inputData2);
        EXMOIns->ALUZeroFlag=(EXMOIns->ALUResult==0)?true:false;
        cout<<"ALU Result "<<EXMOIns->ALUResult<<endl;
        
        //Branch calculation
        if(IDEXIns->CW->Jump){
            EXMOIns->TPC=IDEXIns->JPC;
            IDEXIns->valid=false;
            IFIDIns->valid=false;
            IFRegIns->valid=false;
        }
        else if(IDEXIns->CW->Branch && EXMOIns->ALUZeroFlag){
            EXMOIns->TPC=IDEXIns->DPC+((IDEXIns->imm12)<<1)/4;
            IDEXIns->valid=false;
            IFIDIns->valid=false;
            IFRegIns->valid=false;
        }
        
        EXMOIns->valid=true;
        IDEXIns->stall=false;
        IDEXIns->valid=false;
        
        
    }
    
    void memoryAccess(EXMO* EXMOIns,MOWB* MOWBIns,IFReg* IFRegIns){
        if(MOWBIns->stall) return;
        
        if(!EXMOIns->valid){
            MOWBIns->valid=false;
            return;
        } 
        
        MOWBIns->rd=EXMOIns->rd;
        MOWBIns->CW=EXMOIns->CW;
        MOWBIns->ALUResult=EXMOIns->ALUResult;
        MOWBIns->DPC=EXMOIns->DPC;
        
         //Memory writeback
        if(EXMOIns->CW->MemWrite){
            cout<<"Writing to Memory "<<EXMOIns->rs2<<" at "<<EXMOIns->ALUResult<<endl;
            if(EXMOIns->ALUResult/4<DM.size()){
                DM[EXMOIns->ALUResult/4]=EXMOIns->rs2;
            } else{
                throw runtime_error("EA from ALUResult in store is out of range");
            }
            
            for(int i=0;i<DM.size();i++){
                cout<<DM[i]<<" ";
            } cout<<endl;
        }
        
        
        if(EXMOIns->CW->MemRead){
            cout<<"Reading from Memory at "<<EXMOIns->ALUResult<<endl;
            if(EXMOIns->ALUResult/4<DM.size()){
                MOWBIns->LDResult=DM[EXMOIns->ALUResult/4];
            } else{
                throw runtime_error("EA from ALUResult in load is out of range");
            }
            cout<<"Data Memory: ";
            for(int i:DM){
                cout<<i<<" ";
            }cout<<endl;
        }
        
        
        
        //PC Update
        if((EXMOIns->ALUZeroFlag && EXMOIns->CW->Branch) || (EXMOIns->CW->Jump)){
            cout<<"Updating PC to "<<EXMOIns->TPC<<endl;
            IFRegIns->PC=EXMOIns->TPC;
            IFRegIns->valid=true;
        }
        
        MOWBIns->valid=true;
        EXMOIns->stall=false;
        EXMOIns->valid=false;
        
        
    }
    
    void writeBack(MOWB* MOWBIns){
        if(!MOWBIns->valid) return;
        
        if(MOWBIns->CW->RegWrite){
            cout<<"Writing back to register "<<MOWBIns->rd.to_ulong()<<endl;
            if(MOWBIns->rd.to_ulong()==0){
                cout<<"Not allowed to write to register 0"<<endl;
                return;
            }
            if(MOWBIns->rd.to_ulong()<32){
                if(MOWBIns->CW->Jump){
                    cout<<"Storing PC+4 in Jump "<<MOWBIns->DPC+1<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->DPC+1;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                }
                else if(MOWBIns->CW->Mem2Reg){
                    cout<<"Load Result "<<MOWBIns->LDResult<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->LDResult;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                } else{
                    cout<<"ALU Result "<<MOWBIns->ALUResult<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->ALUResult;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                }
                
                cout<<"Current Register Value: ";
                for(int i:GPR){
                    cout<<i<<" ";
                }cout<<endl;
                cout<<"Current Register Lock Value: ";
                for(int i:GPRSem){
                    cout<<i<<" ";
                }cout<<endl;
            }
            else{
                throw runtime_error("rd range to writeback is out of range");
            }
        }
        
        MOWBIns->valid=false;
        
    }
    
};


//Operand Forewarding

class PipelineOpFw{
    public:
    
    void fetch(IFReg* IFRegIns,IFID* IFIDIns){
        if(IFIDIns->stall) return;
        
        if(!IFRegIns->valid){
            IFIDIns->valid=false;
            return;
        }
        
        
        cout<<"Fetching from "<<IFRegIns->PC<<endl;
        
        if(IFRegIns->PC>=IM.size()){
            IFRegIns->valid=false;
            return;
        }
        
        IFIDIns->IR=IM[IFRegIns->PC];
        IFIDIns->DPC=IFRegIns->PC;
        IFIDIns->NPC=IFRegIns->PC+1;
        IFRegIns->PC=IFRegIns->PC+1;
        IFIDIns->valid=true;
        
        if(IFRegIns->PC>=IM.size()){
            IFRegIns->valid=false;
            return;
        }
        
    }
    
    
    void decode(IFID* IFIDIns,IDEX* IDEXIns){
        if(IDEXIns->stall) return;
         
        if(!IFIDIns->valid){
            IDEXIns->valid=false;
            return;
        } 
        
        cout<<"Decoding "<<IFIDIns->IR<<endl;
        IDEXIns->DPC=IFIDIns->DPC;
        
        bitset<1> signBit=bitset<1>(IFIDIns->IR.substr(32));
        if(signBit.to_ulong()==0){
            IDEXIns->JPC=IFIDIns->NPC+bitset<20>(IFIDIns->IR.substr(0,20)).to_ulong()/4;    
        }
        else{
            IDEXIns->JPC=IFIDIns->NPC-bitset<20>(IFIDIns->IR.substr(0,20)).to_ulong()/4;
        }
        
        
        bitset<7> opCode=bitset<7>(IFIDIns->IR.substr(25,7));
        
        //Immediate Generation
        bitset<12> imm12=bitset<12>(IFIDIns->IR.substr(0,12)); //for i type,ltype
        if(opCode.to_ulong()==BOp || opCode.to_ulong()==SOp){
            imm12=bitset<12>(IFIDIns->IR.substr(0,7)+IFIDIns->IR.substr(20,5));
        }
        IDEXIns->imm12=imm12.to_ulong();
        if(signBit.to_ulong()==1){
            IDEXIns->imm12=-IDEXIns->imm12;
            cout<<"updated -ve "<<IDEXIns->imm12<<endl;
        }
        
        IDEXIns->rd=bitset<5>(IFIDIns->IR.substr(20,5));
        IDEXIns->func3=bitset<3>(IFIDIns->IR.substr(17,3));
        IDEXIns->func7=bitset<7>(IFIDIns->IR.substr(0,7));
        IDEXIns->rsl1=bitset<5>(IFIDIns->IR.substr(12,5));
        IDEXIns->rsl2=bitset<5>(IFIDIns->IR.substr(7,5));
        
        cout<<"Registers: "<<IDEXIns->rsl1.to_ulong()<<" "<<IDEXIns->rsl2.to_ulong()<<endl;
        
        IDEXIns->CW=new ControlWord(opCode);
        
        if(IDEXIns->CW->Jump){
            cout<<"Jumping to "<<IDEXIns->JPC<<endl;
        }
        
        //Decode continued
        cout<<"Register Semaphore values: ";
        for(auto i:GPRSem){
            cout<<i<<" ";
        }cout<<endl;
        
        if(IDEXIns->CW->RegRead && !IDEXIns->CW->Jump){
            if(GPRSem[IDEXIns->rsl1.to_ulong()]!=0){
                // IFIDIns->stall=true;
                // cout<<"Stalling due to "<<rsl1.to_ulong()<<endl;
                // return;
                IDEXIns->rs1=-1;
            }
            else{
                IDEXIns->rs1=GPR[IDEXIns->rsl1.to_ulong()];     
            }
           
           
        }
        
        if(opCode.to_ulong()!=IOp && opCode.to_ulong()!=LOp && IDEXIns->CW->RegRead){
            if(GPRSem[IDEXIns->rsl2.to_ulong()]!=0){
                // IFIDIns->stall=true;
                // cout<<"Stalling due to "<<rsl2.to_ulong()<<endl;
                // return;
                IDEXIns->rs2=-1;
            }
            else{
                IDEXIns->rs2=GPR[IDEXIns->rsl2.to_ulong()];     
            }
           
            
        }
        
        //Semaphore for rd
        if(opCode.to_ulong()!=BOp && opCode.to_ulong()!=SOp && IDEXIns->rd.to_ulong()!=0){
            GPRSem[IDEXIns->rd.to_ulong()]--;
            cout<<"Updating Semaphore for "<<IDEXIns->rd.to_ulong()<<" to be "<<GPRSem[IDEXIns->rd.to_ulong()]<<endl;
        }
        
        cout<<"Decoded opCode "<<opCode.to_ulong()<<endl;
        
        IDEXIns->valid=true;
        IFIDIns->stall=false;
        IFIDIns->valid=false;
        
        
        
    }
    
    void execute(IDEX* IDEXIns, EXMO* EXMOIns, IFReg* IFRegIns, IFID* IFIDIns, MOWB* MOWBIns){
        if(EXMOIns->stall) return;
        
        if(!IDEXIns->valid){
            EXMOIns->valid=false;
            return;
        } 
        
        int inputData1=IDEXIns->rs1,inputData2=IDEXIns->rs2;
        if(inputData1==-1){
            cout<<"Missing rs1"<<endl;
            if(MOWBIns->CW->RegWrite && MOWBIns->rd.to_ulong()!=0 && MOWBIns->rd==IDEXIns->rsl1){
                if(!MOWBIns->CW->Mem2Reg){
                    inputData1=MOWBIns->ALUResult;
                    cout<<"Got rs1 value from MOWB ALU "<<inputData1<<endl;
                }
                else{
                    inputData1=MOWBIns->LDResult;
                    cout<<"Got rs1 value from MOWB LD "<<inputData1<<endl;
                }
            }
            else{
                inputData1=GPR[IDEXIns->rsl1.to_ulong()];
                cout<<"Got rs1 value from register "<<inputData1<<endl;
            }
        }
        
        if(IDEXIns->CW->ALUSrc){
            inputData2=IDEXIns->imm12;
            cout<<"Taking Immediate as operand "<<inputData2<<endl;
        }
        else{
            if(inputData2==-1){
                cout<<"Missing rs2"<<endl;
                if(MOWBIns->CW->RegWrite && MOWBIns->rd.to_ulong()!=0 && MOWBIns->rd==IDEXIns->rsl2){
                    if(!MOWBIns->CW->Mem2Reg){
                        inputData2=MOWBIns->ALUResult;
                        cout<<"Got rs2 value from MOWB ALU "<<inputData2<<endl;
                    }
                    else{
                        inputData2=MOWBIns->LDResult;
                        cout<<"Got rs2 value from MOWB LD "<<inputData2<<endl;
                    }
                }
                else{
                    inputData2=GPR[IDEXIns->rsl2.to_ulong()];
                    cout<<"Got rs2 value from register "<<inputData2<<endl;
                }
            } 
        }
        
        EXMOIns->rd=IDEXIns->rd;
        EXMOIns->CW=IDEXIns->CW;
        EXMOIns->rs2=IDEXIns->rs2;
        EXMOIns->DPC=IDEXIns->DPC;
        EXMOIns->rsl2=IDEXIns->rsl2;
        
        
        
       
        cout<<"data 1: "<<inputData1<<", data 2: "<<inputData2<<endl;
        
        //ALU Operation
        ALU* ALUUnit=new ALU();
        bitset<4> ALUSelect=ALUUnit->ALUControl(IDEXIns->CW->ALUOp,IDEXIns->func3,IDEXIns->func7);
        EXMOIns->ALUResult=ALUUnit->ALUOperation(ALUSelect,inputData1,inputData2);
        EXMOIns->ALUZeroFlag=(EXMOIns->ALUResult==0)?true:false;
        cout<<"ALU Result "<<EXMOIns->ALUResult<<endl;
        
        //Branch calculation
        if(IDEXIns->CW->Jump){
            EXMOIns->TPC=IDEXIns->JPC;
            IDEXIns->valid=false;
            IFIDIns->valid=false;
            IFRegIns->valid=false;
        }
        else if(IDEXIns->CW->Branch && EXMOIns->ALUZeroFlag){
            EXMOIns->TPC=IDEXIns->DPC+((IDEXIns->imm12)<<1)/4;
            IDEXIns->valid=false;
            IFIDIns->valid=false;
            IFRegIns->valid=false;
        }
        
        EXMOIns->valid=true;
        IDEXIns->stall=false;
        IDEXIns->valid=false;
        
        
    }
    
    void memoryAccess(EXMO* EXMOIns,MOWB* MOWBIns,IFReg* IFRegIns){
        if(MOWBIns->stall) return;
        
        if(!EXMOIns->valid){
            MOWBIns->valid=false;
            return;
        } 
        
        
        
         //Memory writeback
        if(EXMOIns->CW->MemWrite){
            cout<<"Writing to Memory "<<EXMOIns->rs2<<" at "<<EXMOIns->ALUResult<<endl;
            if(EXMOIns->ALUResult/4<DM.size()){
                if(EXMOIns->rs2==-1){
                    if(MOWBIns->CW->RegWrite && MOWBIns->rd.to_ulong()!=0 && MOWBIns->rd==EXMOIns->rsl2){
                        if(!MOWBIns->CW->Mem2Reg){
                            EXMOIns->rs2=MOWBIns->ALUResult;
                            cout<<"Got rs2 value from MOWB ALU "<<EXMOIns->rs2<<endl;
                        }
                        else{
                            EXMOIns->rs2=MOWBIns->LDResult;
                            cout<<"Got rs2 value from MOWB LD "<<EXMOIns->rs2<<endl;
                        }
                    }
                    else{
                        EXMOIns->rs2=GPR[EXMOIns->rsl2.to_ulong()];
                        cout<<"Got rs2 value from register "<<EXMOIns->rs2<<endl;
                    }
                }
                DM[EXMOIns->ALUResult/4]=EXMOIns->rs2;
            } else{
                throw runtime_error("EA from ALUResult in store is out of range");
            }
            
            for(int i=0;i<DM.size();i++){
                cout<<DM[i]<<" ";
            } cout<<endl;
        }
        
        MOWBIns->rd=EXMOIns->rd;
        MOWBIns->CW=EXMOIns->CW;
        MOWBIns->ALUResult=EXMOIns->ALUResult;
        MOWBIns->DPC=EXMOIns->DPC;
        
        
        if(EXMOIns->CW->MemRead){
            cout<<"Reading from Memory at "<<EXMOIns->ALUResult/4<<endl;
            if(EXMOIns->ALUResult/4<DM.size()){
                MOWBIns->LDResult=DM[EXMOIns->ALUResult/4];
            } else{
                throw runtime_error("EA from ALUResult in load is out of range");
            }
            cout<<"Data Memory: ";
            for(int i:DM){
                cout<<i<<" ";
            }cout<<endl;
        }
        
        
        
        //PC Update
        if((EXMOIns->ALUZeroFlag && EXMOIns->CW->Branch) || (EXMOIns->CW->Jump)){
            cout<<"Updating PC to "<<EXMOIns->TPC<<endl;
            IFRegIns->PC=EXMOIns->TPC;
            IFRegIns->valid=true;
        }
        
        MOWBIns->valid=true;
        EXMOIns->stall=false;
        EXMOIns->valid=false;
        
        
    }
    
    void writeBack(MOWB* MOWBIns){
        if(!MOWBIns->valid) return;
        
        if(MOWBIns->CW->RegWrite){
            cout<<"Writing back to register "<<MOWBIns->rd.to_ulong()<<endl;
            if(MOWBIns->rd.to_ulong()==0){
                cout<<"Not allowed to write to register 0"<<endl;
                return;
            }
            if(MOWBIns->rd.to_ulong()<32){
                if(MOWBIns->CW->Jump){
                    cout<<"Storing PC+4 in Jump "<<MOWBIns->DPC+1<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->DPC+1;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                }
                else if(MOWBIns->CW->Mem2Reg){
                    cout<<"Load Result "<<MOWBIns->LDResult<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->LDResult;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                } else{
                    cout<<"ALU Result "<<MOWBIns->ALUResult<<endl;
                    GPR[MOWBIns->rd.to_ulong()]=MOWBIns->ALUResult;
                    GPRSem[MOWBIns->rd.to_ulong()]++;
                }
                
                cout<<"Current Register Value: ";
                for(int i:GPR){
                    cout<<i<<" ";
                }cout<<endl;
                cout<<"Current Register Lock Value: ";
                for(int i:GPRSem){
                    cout<<i<<" ";
                }cout<<endl;
            }
            else{
                throw runtime_error("rd range to writeback is out of range");
            }
        }
        
        MOWBIns->valid=false;
        
    }
    
};

void CPUPipelineImplementationOpFw(){
    IFReg* IFRegIns=new IFReg();
    IFID* IFIDIns=new IFID();
    IDEX* IDEXIns=new IDEX();
    EXMO* EXMOIns=new EXMO();
    MOWB* MOWBIns=new MOWB();
    
    PipelineOpFw* pipeline=new PipelineOpFw();
    int clock=1;
    
    while(true){
        cout<<".......................Clock"<<clock++<<"......................................"<<endl;
        
        cout<<"Writing Back: "<<endl;
        pipeline->writeBack(MOWBIns);
        cout<<endl;
        
        cout<<"Memory Access: "<<endl;
        pipeline->memoryAccess(EXMOIns,MOWBIns,IFRegIns);
        cout<<endl;
        
        
        cout<<"Execute: "<<endl;
        pipeline->execute(IDEXIns,EXMOIns,IFRegIns,IFIDIns,MOWBIns);
        cout<<endl;
        
        
        cout<<"Decode: "<<endl;
        pipeline->decode(IFIDIns,IDEXIns);
        cout<<endl;
        
        cout<<"Fetch: "<<endl;
        pipeline->fetch(IFRegIns,IFIDIns);
        cout<<endl;
        
        cout<<".................................................................."<<endl;
        
        if(!IFRegIns->valid && !IFIDIns->valid && !IDEXIns->valid && !EXMOIns->valid && !MOWBIns->valid){
            break;
        }
        
        
        
        
    }
    
    cout<<"Final Register Values: ";
    for(auto i:GPR){
        cout<<i<<" ";
    }cout<<endl;
    
    cout<<"Final Memory Values: ";
    for(auto i:DM){
        cout<<i<<" ";
    }cout<<endl;
}


void CPUPipelineImplementation(){
    IFReg* IFRegIns=new IFReg();
    IFID* IFIDIns=new IFID();
    IDEX* IDEXIns=new IDEX();
    EXMO* EXMOIns=new EXMO();
    MOWB* MOWBIns=new MOWB();
    
    Pipeline* pipeline=new Pipeline();
    int clock=1;
    
    while(true){
        cout<<".......................Clock"<<clock++<<"......................................"<<endl;
        
        cout<<"Writing Back: "<<endl;
        pipeline->writeBack(MOWBIns);
        cout<<endl;
        
        cout<<"Memory Access: "<<endl;
        pipeline->memoryAccess(EXMOIns,MOWBIns,IFRegIns);
        cout<<endl;
        
        
        cout<<"Execute: "<<endl;
        pipeline->execute(IDEXIns,EXMOIns,IFRegIns,IFIDIns);
        cout<<endl;
        
        
        cout<<"Decode: "<<endl;
        pipeline->decode(IFIDIns,IDEXIns);
        cout<<endl;
        
        cout<<"Fetch: "<<endl;
        pipeline->fetch(IFRegIns,IFIDIns);
        cout<<endl;
        
        cout<<".................................................................."<<endl;
        
        if(!IFRegIns->valid && !IFIDIns->valid && !IDEXIns->valid && !EXMOIns->valid && !MOWBIns->valid){
            break;
        }
        
        
        
        
    }
    
    cout<<"Final Register Values: ";
    for(auto i:GPR){
        cout<<i<<" ";
    }cout<<endl;
    
    cout<<"Final Memory Values: ";
    for(auto i:DM){
        cout<<i<<" ";
    }cout<<endl;
}







void CPUNonPipelineImplementation(){
    int PC=0;
    
    while(PC<IM.size()){
        
        //fetch
        string Instr=IM[PC];
        cout<<"Implementing PC "<<PC<<" "<<Instr<<endl;
        
        int NPC=PC+1;
        //Jump
        int JPC=NPC+bitset<20>(Instr.substr(0,20)).to_ulong()/4;
        
        //Decode
        bitset<7> opCode=bitset<7>(Instr.substr(25,7));
        bitset<5> rd=bitset<5>(Instr.substr(20,5));
        bitset<3> func3=bitset<3>(Instr.substr(17,3));
        bitset<5> rsl1=bitset<5>(Instr.substr(12,5));
        bitset<5> rsl2=bitset<5>(Instr.substr(7,5));
        bitset<7> func7=bitset<7>(Instr.substr(0,7));
        bitset<1> signImm=bitset<1>(Instr.substr(31));
        
        //Immediate Generation
        bitset<12> imm12=bitset<12>(Instr.substr(0,12)); //for i type,ltype
        if(opCode.to_ulong()==BOp || opCode.to_ulong()==SOp){
            imm12=bitset<12>(Instr.substr(0,7)+Instr.substr(20,5));
        }
        
        //ControlWord Generation
        ControlWord* CW=new ControlWord(opCode);
        
        
        //Decode Continued
        int rs1=0,rs2=0;
        if(CW->RegRead){
            rs1=GPR[rsl1.to_ulong()];
            rs2=GPR[rsl2.to_ulong()];
        }
        
        int inputData1=rs1;
        int inputData2=rs2;
        if(CW->ALUSrc){
            inputData2=imm12.to_ulong();
            cout<<"Taking Immediate as operand "<<inputData2<<endl;
        }
        
        
        //ALU Operation
        
        ALU* ALUUnit=new ALU();
        bitset<4> ALUSelect=ALUUnit->ALUControl(CW->ALUOp,func3,func7);
        int ALUResult=ALUUnit->ALUOperation(ALUSelect,inputData1,inputData2);
        bool ALUZeroFlag=(ALUResult==0)?true:false;
        cout<<"ALU Result "<<ALUResult<<endl;
        
        //Memory writeback
        if(CW->MemWrite){
            cout<<"Writing to Memory "<<rs2<<" at "<<ALUResult<<endl;
            if(ALUResult<DM.size()){
                for(int i=0;i<DM.size();i++){
                    cout<<DM[i]<<" ";
                } cout<<endl;
                DM[ALUResult]=rs2;
            } else{
                throw runtime_error("EA from ALUResult in store is out of range");
            }
            
            for(int i=0;i<DM.size();i++){
                cout<<DM[i]<<" ";
            } cout<<endl;
        }
        
        int LDResult=0;
        if(CW->MemRead){
            cout<<"Reading from Memory at "<<ALUResult<<endl;
            if(ALUResult<DM.size()){
                LDResult=DM[ALUResult];
            } else{
                throw runtime_error("EA from ALUResult in load is out of range");
            }
        }
        
        //Branch Calculation
        int BPC=PC+(imm12.to_ulong()<<1)/4;
        int TPC=NPC;
        
        if(CW->Branch && ALUZeroFlag){
            cout<<"Taking Branch to "<<BPC<<endl;
            TPC=BPC;
        }
        else if(CW->Jump){
            cout<<"Taking Jump to "<<JPC<<endl;
            TPC=JPC;
        }
        
        
        //Register Writeback
        if(CW->RegWrite){
            cout<<"Writing back to register "<<rd.to_ulong()<<endl;
            if(rd.to_ulong()<32){
                if(CW->Mem2Reg){
                    cout<<"Load Result "<<LDResult<<endl;
                    GPR[rd.to_ulong()]=LDResult;
                } else{
                    cout<<"ALU Result "<<ALUResult<<endl;
                    GPR[rd.to_ulong()]=ALUResult;
                }
            }
            else{
                throw runtime_error("rd range to writeback is out of range");
            }
        }
        
        PC=TPC;
        cout<<endl;
        
    }
}



vector<string> parseLine(string line){
    int startPos=0;
    int endPos=0;
    vector<string> tokens;
    string delimeters=",() ";
    
    
    while((endPos=line.find_first_of(delimeters,startPos))!=string::npos){
        
        if(line.substr(startPos,endPos-startPos).length()!=0){
            tokens.push_back(line.substr(startPos,endPos-startPos));    
        }
        
        startPos=endPos+1;
    }
    
    if(startPos!=line.length()){
        tokens.push_back(line.substr(startPos));
    }
    
    return tokens;
    
}


int main(){
    initialise();
    
    string line;
    ifstream file("fact.txt");
    // ofstream ofile("output.txt");
    
    if(file.is_open()){
        while(getline(file,line)){
            cout<<line<<"\n";
            vector<string> tokens=parseLine(line);
            if(tokens.size()>=3){
                Instr* sample=new Instr(tokens);
                string ans=sample->getDecodedInstr();
                cout<<endl;
                IM.push_back(ans);
                // if(ofile.is_open()){
                //     ofile<<ans<<endl;
                // }
            }
        }
        file.close();
        // ofile.close();
    }
    
    

    // CPUNonPipelineImplementation();
   // CPUPipelineImplementationOpFw();
     CPUPipelineImplementation();
    
    
}





