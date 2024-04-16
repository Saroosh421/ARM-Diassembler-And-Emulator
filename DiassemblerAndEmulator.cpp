#include<iostream>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

typedef uint32_t REGISTER;		// registers are unsigned 32 bit values
typedef uint32_t WORD;          // words are also unsigned 32 bit values
REGISTER reg[15];               // Array of registers


// function to convert hexadecimal to binary
void hexToBin(WORD instruction, int instruct_arr[]){
    int dummy_arr[32];
    for(int i = 0; i < 32; i++){
        if(instruction & (1 << i)){
            dummy_arr[i] = 1;
        }else{
            dummy_arr[i] = 0;
        }
    }
    for (int i = 0; i < 32; i++){
        instruct_arr[i] = dummy_arr[31 - i];
    }
}

// function to convert binary to decimal
int binToDec(int bin_arr[], int size){
    int dec = 0;
    for (int i = 0; i < size; i++){
        dec += bin_arr[i] * pow(2, size-i-1);
    }
    return dec;
}

// function to set initialize values of register
void setRegisters(WORD registers[15]){
    for (int i = 0; i < 16; i++){
        registers[i] = 0x00000000;
    }
}

// function to print content of registers
void printRegisters(WORD registers[15]){
    cout << "Current state of registers: " <<endl;
    for (int i = 0; i < 16; i++){
        cout << "r" << i << ": " << setfill('0') << setw(8) << registers[i] << endl;
    }
    cout << endl;
}


string Decoder(string opCode, string destReg, string sourceReg, string operand){
    string decodedInstruct = "";
    if (opCode == "AND"){
        //AND(destReg, sourceReg, operand);
        decodedInstruct = "AND " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "EOR"){
        //EOR(destReg, sourceReg, operand);
        decodedInstruct = "EOR " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "SUB"){
        //SUB(destReg, sourceReg, operand);
        decodedInstruct = "SUB " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "RSB"){
        //RSB(destReg, sourceReg, operand);
        decodedInstruct = "RSB " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "ADD"){
        //ADD(destReg, sourceReg, operand);
        decodedInstruct = "ADD " + destReg + ", " + sourceReg + ", " + operand;
        return decodedInstruct;
    }
    else if (opCode == "ADC"){
        //ADC(destReg, sourceReg, operand);
        decodedInstruct = "ADC " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "SBC"){
        //SBC(destReg, sourceReg, operand);
        decodedInstruct = "SBC " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "RSC"){
        //RSC(destReg, sourceReg, operand);
        decodedInstruct = "RSC " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "TST"){
        //TST(destReg, sourceReg, operand);
        decodedInstruct = "TST " + destReg + ", " + operand;
    }
    else if (opCode == "TEQ"){
        //TEQ(destReg, sourceReg, operand);
        decodedInstruct = "TEQ " + destReg + ", " + operand;
    }
    else if (opCode == "CMP"){
        //CMP(destReg, sourceReg, operand);
        decodedInstruct = "CMP " + destReg + ", " + operand;
    }
    else if (opCode == "CMN"){
        //CMN(destReg, sourceReg, operand);
        decodedInstruct = "CMN " + destReg + ", " + operand;
    }
    else if (opCode == "ORR"){
        //ORR(destReg, sourceReg, operand);
        decodedInstruct = "ORR " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "MOV"){
        //MOV(destReg, sourceReg, operand);
        decodedInstruct = "MOV " + destReg + ", " + operand;
        return decodedInstruct;
    }
    else if (opCode == "BIC"){
        //BIC(destReg, sourceReg, operand);
        decodedInstruct = "BIC " + destReg + ", " + sourceReg + ", " + operand;
    }
    else if (opCode == "MVN"){
        //MVN(destReg, sourceReg, operand);
        decodedInstruct = "MVN " + destReg + ", " + operand;
    }

}


int main(){
    int instruct_arr[32];
    int opCode_arr[4];
    string destReg_str, sourceReg_str, operand_str;
    int destReg_arr[4], sourceReg_arr[4], operand_arr[12];
    string destReg[15], sourceReg[15];
    WORD registers[15];
    string genOpCode = "";
    WORD instruction = 0xE0802001;

    string opCode[16];
    opCode[0] = "AND";
    opCode[1] = "EOR";
    opCode[2] = "SUB";
    opCode[3] = "RSB";
    opCode[4] = "ADD";
    opCode[5] = "ADC";
    opCode[6] = "SBC";
    opCode[7] = "RSC";
    opCode[8] = "TST";
    opCode[9] = "TEQ";
    opCode[10] = "CMP";
    opCode[11] = "CMN";
    opCode[12] = "ORR";
    opCode[13] = "MOV";
    opCode[14] = "BIC";
    opCode[15] = "MVN";

    for (int i = 0; i < 16; i++){
        destReg[i] = "r" + to_string(i);
        sourceReg[i] = "r" + to_string(i);
    }
    string temp = "";

    hexToBin(instruction, instruct_arr);

    cout<<"hexadecimal to binary is: ";
    for (int i = 0; i < 32; i++){
        cout << instruct_arr[i];
    }
    cout << endl;

    // extracting opCode from instruction
    for (int i = 0, j = 7; i < 4; i++, j++){
        opCode_arr[i] = instruct_arr[j];
    }

    // extracting destination register from instruction
    for (int i = 0, j = 16; i < 4; i++, j++){
        destReg_arr[i] = instruct_arr[j];
    }

    // extracting source register instruction
    for (int i = 0, j = 12; i < 4; i++, j++){
        sourceReg_arr[i] = instruct_arr[j];
    }

    // extracting operand from instruction
    for (int i = 0, j = 20; i < 12; i++, j++){
        operand_arr[i] = instruct_arr[j];
    }

    destReg_str = destReg[binToDec(destReg_arr, 4)];
    sourceReg_str = sourceReg[binToDec(sourceReg_arr, 4)];
    operand_str = to_string(binToDec(operand_arr, 12));
    genOpCode = opCode[binToDec(opCode_arr, 4)];

    cout << "Genrated OpCode is: " << genOpCode << endl;

    //cout << "Destination Register is: " << destReg_str << endl;

    //cout << "Source Register is: " << sourceReg_str << endl;

    //cout << "Operand is: " << operand_str << endl;

    temp = Decoder(genOpCode, destReg_str, sourceReg_str, operand_str);
    cout << temp << endl;

    setRegisters(registers);
    printRegisters(registers);

}

// E3 A0 00 01 MOV r0,#1
// E3 A0 10 02 MOV r1,#2
// E0 80 20 01 ADD r2,r0,r1
// E2 82 20 05 ADD r2,r2,#5
// E1 A0 00 02 MOV r0,#2
// E1 A0 10 03 MOV r1,#3
// E0 80 20 01 ADD r2,r0,r1
// E2 82 20 05 ADD r2,r2,#5
// E1 A0 00 02 MOV r0,#2
// E1 A0 10 03 MOV r1,#3
// E0 80 20 01 ADD r2,r0,r1
// E2 82 20 05 ADD r2,r2,#5
// E1 A0 00 02 MOV r0,#2
// E1 A0 10 03 MOV r1,#3
// E0 80 20 01 ADD r2,r0,r1
