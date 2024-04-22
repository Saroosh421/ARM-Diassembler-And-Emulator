#include<iostream>
#include <string>
#include <cstring>
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

// function to convert deciaml to hexadecimal
string decToHex(int dec){
    string hex = "";
    while (dec != 0){
        int rem = dec % 16;
        if (rem < 10){
            hex = to_string(rem) + hex;
        }
        else{
            hex = (char)(rem + 55) + hex;
        }
        dec = dec / 16;
    }
    return hex;
}

//function to perform hexadecimal addition
string hexAddition(string& hex1, string& hex2) {
    // Convert hexadecimal strings to integers
    stringstream ss1, ss2;
    ss1 << std::hex << hex1;
    ss2 << std::hex << hex2;
    unsigned int num1, num2;
    ss1 >> num1;
    ss2 >> num2;

    // Perform addition
    unsigned int sum = num1 + num2;

    // Convert sum back to hexadecimal string
    stringstream result_ss;
    result_ss << hex << sum;
    string result = result_ss.str();

    return result;
}


// function to perform hexadecimal subtraction
string hexSubtraction(string& hex1, const string& hex2) {
    // Convert hexadecimal strings to integers
    stringstream ss1, ss2;
    ss1 << hex << hex1;
    ss2 << hex << hex2;
    unsigned int num1, num2;
    ss1 >> num1;
    ss2 >> num2;

    // Perform subtraction
    unsigned int difference = num1 - num2;

    // Convert difference back to hexadecimal string
    stringstream result_ss;
    result_ss << hex << difference;
    string result = result_ss.str();

    return result;
}


// function to set initialize values of register
void setRegisters(string registers[15]){
    for (int i = 0; i < 16; i++){
        registers[i] = "00000000";
    }
}

// function to print content of registers
void printRegisters(string registers[15]){
    cout << "\nCurrent state of registers: " <<endl;
    for (int i = 0; i < 16; i++){
        cout << "r" << i << ": " << registers[i] << endl;
    }
    cout << endl;
}

// MOV instruction implementation
void MOV(string destReg, string operand, string registers[16]){
    string hexVal = "";
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    int op = stoi(operand);
    hexVal = decToHex(op);
    for (int i = hexVal.length(); i < 8; i++){
        hexVal = "0" + hexVal;
    }
    registers[desRegIndex] = hexVal;
    printRegisters(registers);
}

// ADD instruction implementation
void ADD(string destReg, string sourceReg, string operand, string registers[16]){
    int op = stoi(operand);
    string hexOp = decToHex(op);
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    string sourceRegValue = registers[sourceRegIndex];
    string hexAdd = hexAddition(sourceRegValue, hexOp);

    string hexVal = "";
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    for (int i = hexAdd.length(); i < 8; i++){
        hexAdd = "0" + hexAdd;
    }
    registers[desRegIndex] = hexAdd;
    printRegisters(registers);
}

// SUB instruction implementation
void SUB(string destReg, string sourceReg, string operand, string registers[16]){
    int op = stoi(operand);
    string hexOp = decToHex(op);
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    string sourceRegValue = registers[sourceRegIndex];
    string hexSub = hexSubtraction(sourceRegValue, hexOp);

    string hexVal = "";
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    for (int i = hexSub.length(); i < 8; i++){
        hexSub = "0" + hexSub;
    }
    registers[desRegIndex] = hexSub;
    printRegisters(registers);
}

// CMP instruction implementation
void CMP(string destReg, string sourceReg, string operand, string registers[16]){
    int op = stoi(operand);
    string hexOp = decToHex(op);
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    string sourceRegValue = registers[sourceRegIndex];
    string hexSub = hexSubtraction(sourceRegValue, hexOp);

    for (int i = hexSub.length(); i < 8; i++){
        hexSub = "0" + hexSub;
    }
    cout << "CMP: " << hexSub << endl;
}


void Decoder(string opCode, string destReg, string sourceReg, string operand, bool isRegister, string registers[16]){
    string decodedInstruct = "";
    if (opCode == "AND"){
        //AND(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "AND " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "AND " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "EOR"){
        //EOR(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "EOR " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "EOR " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "SUB"){
        //SUB(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "SUB " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "SUB " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
        SUB(destReg, sourceReg, operand, registers);
    }
    else if (opCode == "RSB"){
        //RSB(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "RSB " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "RSB " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "ADD"){
        //ADD(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "ADD " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "ADD " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
        ADD(destReg, sourceReg, operand, registers);
    }
    else if (opCode == "ADC"){
        //ADC(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "ADC " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "ADC " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "SBC"){
        //SBC(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "SBC " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "SBC " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "RSC"){
        //RSC(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "RSC " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "RSC " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "TST"){
        //TST(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "TST " + destReg + ", r" + operand;
        else
            decodedInstruct = "TST " + destReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "TEQ"){
        //TEQ(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "TEQ " + destReg + ", r" + operand;
        else
            decodedInstruct = "TEQ " + destReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "CMP"){
        //CMP(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "CMP " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "CMP " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
        CMP(destReg, sourceReg, operand, registers);
    }
    else if (opCode == "CMN"){
        //CMN(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "CMN " + destReg + ", r" + operand;
        else
            decodedInstruct = "CMN " + destReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "ORR"){
        //ORR(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "ORR " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "ORR " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "MOV"){
        //MOV(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "MOV " + destReg + ", r" + operand;
        else
            decodedInstruct = "MOV " + destReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
        MOV(destReg, operand, registers);
    }
    else if (opCode == "BIC"){
        //BIC(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "BIC " + destReg + ", " + sourceReg + ", r" + operand;
        else
            decodedInstruct = "BIC " + destReg + ", " + sourceReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }
    else if (opCode == "MVN"){
        //MVN(destReg, sourceReg, operand);
        if (isRegister)
            decodedInstruct = "MVN " + destReg + ", r" + operand;
        else
            decodedInstruct = "MVN " + destReg + ", #" + operand;
        cout << "Decoded Instruction: " << decodedInstruct << endl;
    }

}


int main(){
    int instruct_arr[32];
    int opCode_arr[4];
    string destReg_str, sourceReg_str, operand_str;
    int destReg_arr[4], sourceReg_arr[4], operand_arr[12];
    string destReg[16], sourceReg[16];
    string registers[16];
    string genOpCode = "", decodedInstruct = "";
    WORD instruction = 0xE1A00001;
    bool isRegister = false;

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
    //string temp = "";

    hexToBin(instruction, instruct_arr);

    cout<<"hexadecimal to binary is: ";
    for (int i = 0; i < 32; i++){
        cout << instruct_arr[i];
    }
    cout << endl;

    // extracting immediate bit from instruction

    if (instruct_arr[6] == 0){
        isRegister = true;
    }
    else{
        isRegister = false;
    }

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

    setRegisters(registers);
    printRegisters(registers);

    Decoder(genOpCode, destReg_str, sourceReg_str, operand_str, isRegister, registers);

}

// E3A00001 MOV r0,#1
// E3A01002 MOV r1,#2
// E0802001 ADD r2,r0,r1
// E2822005 ADD r2,r2,#5
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
// MOV r0, r1 0xE1A00001
// MOV r2, 4 0xE3A02004
// SUB r2, r2, 1 0xE2412001
// ADD r2, r2, 15 0xE282200F
// SUB r3, r3, 1 0xE2433001
// SUB r3, r2, r1 0xE2423001
// CMP r2, r1 0xE1520001
// CMP r0, r1 0xE1500001
// CMP r1, r2 0xE1521002
// CMP r2, 1 0xE3522001
// CMP r1, 2 0xE3521002
// CMP r0, 15 0xE350000F
// B and BL instructions
// B 0xEA000000
// BL 0xEB000000
