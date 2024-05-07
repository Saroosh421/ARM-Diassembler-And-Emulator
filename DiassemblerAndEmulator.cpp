#include<iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include <cmath>
#include <iomanip>
using namespace std;

// Define the data types
typedef uint32_t WORD;      // words are also unsigned 32 bit values
#define KB_RAM	(128)       // 128 KB of RAM
WORD ram[KB_RAM>>2];		// The >>2 calculates KB / 4, since each WORD is 4 bytes in size.
WORD programCounter;		// The program counter


// populate ram
void populateRam(){
    for (size_t i = 0; i < sizeof(ram) / sizeof(ram[0]); i++) {
        ram[i] = static_cast<WORD>(0);  // Each element is set 0
    }
}

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
    ss1 << hex << hex1;
    ss2 << hex << hex2;
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

// Function to compare two hexadecimal numbers
bool compareHexStrings(string& hex1, string& hex2) {
    // Convert hexadecimal strings to integers
    stringstream ss1, ss2;
    ss1 << hex << hex1;
    ss2 << hex << hex2;
    unsigned int num1, num2;
    ss1 >> num1;
    ss2 >> num2;
    // Compare the integers
    if (num1 > num2)
        return true; // hex1 is greater
    else
        return false; // hex2 is greater
}

bool compareHexStringsSub(string& hex1, string& hex2) {
    // Convert hexadecimal strings to integers
    stringstream ss1, ss2;
    ss1 << hex << hex1;
    ss2 << hex << hex2;
    unsigned int num1, num2;
    ss1 >> num1;
    ss2 >> num2;
    // Compare the integers
    if (num1 > num2)
        return true; // hex1 is greater
    else if (num1 == num2)
        return true;
    else
        return false; // hex2 is greater
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

// Check zero flag
bool zeroFlag(string destReg){
    if (destReg == "00000000"){
        return true;
    }
    return false;
}

// Check negative flag
bool negativeFlag(string destReg){
    if (destReg[0] != '0'){
        return true;
    }
    return false;
}

// Check carry flag for addition
bool carryFlagAdd(string destReg, string sourceReg, string operand){
    // Check if destReg is greater than sourceReg
    if (compareHexStrings(destReg, sourceReg)){
        // Check if destReg is greater than operand
        if (compareHexStrings(destReg, operand))
            return false;
    }
    return true;
}

// Check overflow flag for addition
bool overflowFlagAdd(string destReg, string sourceReg, string operand){
    // Check if destReg is equal to sourceReg
    if (sourceReg[0] == operand[0]){
        // Check if destReg is not equal to sourceReg
        if (destReg[0] != sourceReg[0])
            return true;
    }
    return false;
}

// Check carry flag for subtraction
bool carryFlagSub(string destReg, string sourceReg, string operand){
    // Check if destReg is greater or equal than sourceReg
    if (compareHexStringsSub(sourceReg, operand))
        return true;
    return false;
}

// Check overflow flag for subtraction
bool overflowFlagSub(string destReg, string sourceReg, string operand){
    // Check if destReg is not equal to operand
    if (sourceReg[0] != operand[0]){
        // Check if destReg is not equal to sourceReg
        if (destReg[0] != sourceReg[0])
            return true;
    }
    return false;
}

// function to set initialize values of register
void setRegisters(string registers[16]){
    for (int i = 0; i < 14; i++){
        registers[i] = "00000000";
    }
    registers[14] = "00010000";
    registers[15] = "00000100";
}

// function to print content of registers
void printRegisters(string registers[15]){
    cout << "\nCurrent state of registers: " <<endl;
    for (int i = 0; i < 16; i++){
        cout << "r" << i << ": " << registers[i] << endl;
    }
    cout << endl;
}

// check if regDest is PC
bool isPC(string regDest){
    if (regDest == "r15")
        return true;
    return false;
}

// MOV instruction implementation
void MOV(string destReg, string operand, string registers[16]){
    string hexVal = "";
    // Determine the destination register index
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    // Convert the operand to integer
    int op = stoi(operand);
    // Convert the operand to hexadecimal
    hexVal = decToHex(op);
    // Pad the hexadecimal value with zeros
    for (int i = hexVal.length(); i < 8; i++){
        hexVal = "0" + hexVal;
    }
    // Store the hexadecimal value in the destination register
    registers[desRegIndex] = hexVal;
    printRegisters(registers);
}

// ADD instruction implementation
void ADD(string destReg, string sourceReg, string operand, string registers[16]){
    // Convert the operand to integer
    int op = stoi(operand);
    // Convert the operand to hexadecimal
    string hexOp = decToHex(op);
    // Determine the destination register index
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    // Determine the source register index
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    // Get the value of the source register
    string sourceRegValue = registers[sourceRegIndex];
    // Perform the hexadecimal addition
    string hexAdd = hexAddition(sourceRegValue, hexOp);
    string hexVal = "";
    // Pad the hexadecimal value with zeros
    for (int i = hexAdd.length(); i < 8; i++){
        hexAdd = "0" + hexAdd;
    }
    // Store the hexadecimal value in the destination register
    registers[desRegIndex] = hexAdd;
    printRegisters(registers);

    // check flags
    /*
    cout << "Zero Flag: " << zeroFlag(registers[desRegIndex]) << endl;
    cout << "Negative Flag: " << negativeFlag(registers[desRegIndex]) << endl;
    cout << "Carry Flag: " << carryFlagAdd(registers[desRegIndex], registers[sourceRegIndex], registers[op]) << endl;
    cout << "Overflow Flag: " << overflowFlagAdd(registers[desRegIndex], registers[sourceRegIndex], registers[op]) << endl;
    */
}

// SUB instruction implementation
void SUB(string destReg, string sourceReg, string operand, string registers[16]){
    // Convert the operand to integer
    int op = stoi(operand);
    // Convert the operand to hexadecimal
    string hexOp = decToHex(op);
    // Determine the destination register index
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    // Determine the source register index
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    // Get the value of the source register
    string sourceRegValue = registers[sourceRegIndex];
    // Perform the hexadecimal subtraction
    string hexSub = hexSubtraction(sourceRegValue, hexOp);
    string hexVal = "";
    // Pad the hexadecimal value with zeros
    for (int i = hexSub.length(); i < 8; i++){
        hexSub = "0" + hexSub;
    }
    // Store the hexadecimal value in the destination register
    registers[desRegIndex] = hexSub;
    printRegisters(registers);

    // check flags
    /*
    cout << "Zero Flag: " << zeroFlag(registers[desRegIndex]) << endl;
    cout << "Negative Flag: " << negativeFlag(registers[desRegIndex]) << endl;
    cout << "Carry Flag: " << carryFlagSub(registers[desRegIndex], registers[sourceRegIndex], registers[op]) << endl;
    cout << "Overflow Flag: " << overflowFlagSub(registers[desRegIndex], registers[sourceRegIndex], registers[op]) << endl;
    */
}

// CMP instruction implementation
void CMP(string destReg, string sourceReg, string operand, string registers[16]){
    // Convert the operand to integer
    int op = stoi(operand);
    // Convert the operand to hexadecimal
    string hexOp = decToHex(op);
    // Determine the destination register index
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    // Determine the source register index
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    // Get the value of the source register
    string sourceRegValue = registers[sourceRegIndex];
    // Perform the hexadecimal subtraction
    string hexSub = hexSubtraction(sourceRegValue, hexOp);
    // Pad the hexadecimal value with zeros
    for (int i = hexSub.length(); i < 8; i++){
        hexSub = "0" + hexSub;
    }
    cout << "CMP: " << hexSub << endl;

    // check flags
    cout << "Zero Flag: " << zeroFlag(sourceRegValue) << endl;
    cout << "Negative Flag: " << negativeFlag(sourceRegValue) << endl;
    cout << "Carry Flag: " << carryFlagSub(sourceRegValue, operand, registers[op]) << endl;
    cout << "Overflow Flag: " << overflowFlagSub(registers[desRegIndex], sourceRegValue, operand) << endl; 
}

// Data Processing Instructions
void DataProcessingInstruction(string opCode, string destReg, string sourceReg, string operand, bool isRegister, string registers[16], int timeCycle){
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
    if (isPC(destReg)){
        timeCycle += 3;
    }
    else{
        timeCycle += 1;
    }
    cout << "Updated time cycle: " << timeCycle << endl;
}

// Fetch the next instruction based on the program counter
WORD fetchNextInstruction() {
    WORD next = ram[programCounter >> 2]; // Access memory word-wise
    programCounter += 4; // Move to next instruction (increment by 4 bytes)
    return next;
}

// SWI instruction implementation
void SWIImp(WORD instruction ,string registers[16], int timeCycle){
    // Link Register (commonly used to store return address)
    // cast the string to WORD
    WORD reg14 = static_cast<WORD>(stoul(registers[14]));
    // Assuming SWI is right before the return address
    WORD swiInstructionAddress = reg14 - 4;
    // Extract the SWI instruction directly if r14 points directly to the next instruction after SWI
    WORD swiInstruction = ram[swiInstructionAddress >> 2];
    // Extract the SWI code
    WORD interruptCode = instruction & 0x00FFFFFF;
    cout << "SWI #" << hex << interruptCode << endl;
    timeCycle += 5;
    cout << "Updated time cycle: " << timeCycle << endl;
}

void BAndBLImp(int BAndBLOffset[24], bool branchWithLink, string registers[16], int timeCycle){
    int PC = 15;
    string hex_val = "";
    // Save the return address in the link register
    string programCounter = registers[PC];
    // Next instruction address
    string nextIns = "4", add_val = "1";
    // link register
    int LR = 14;

    if (branchWithLink){
        registers[LR] = hexAddition(registers[PC], nextIns); // Save the return address in the link register
    }
    for (int i = registers[LR].length(); i < 8; i++){
        registers[LR] = "0" + registers[LR];
    }
    // Update the program counter to the target address
    int BAndBLOffsetVal = binToDec(BAndBLOffset, 24);
    //programCounter = programCounter + BAndBLOffsetVal;
    for (int i = 0; i < BAndBLOffsetVal*4 ; i++){
        programCounter = hexAddition(programCounter, add_val);
    }
    for (int i = programCounter.length(); i < 8; i++){
        programCounter = "0" + programCounter;
    }
    registers[PC] = programCounter;
    if (branchWithLink){
        cout << "After bl instruction, the program counter is at: " << registers[PC] << endl;
        cout << "Link Register stored the address: " << registers[LR] << endl;
    }
    else{
        cout << "After b instruction, the program counter is at: " << programCounter << endl;
    }
    timeCycle += 3;
    cout << "Updated time cycle: " << timeCycle << endl;
    printRegisters(registers);
}

// Single Data Transfer Instruction
void singleDataTransfer(int instruct_arr[32], string destReg, string sourceReg, string offsett, bool preOrPostBit, bool upOrDownBit, bool byteOrWordBit, bool writeBackBit, bool loadOrStoreBit, string registers[16], bool isRegister, int timeCycle){
    WORD effectiveAddressWordValue, destRegWordValue;
    // Determine the destination register index
    string desRegInd = destReg.substr(1);
    int desRegIndex = stoi(desRegInd);
    // Determine the source register index
    string sourceRegInd = sourceReg.substr(1);
    int sourceRegIndex = stoi(sourceRegInd);
    // Calculate Address
    string base = registers[sourceRegIndex];
    string effectiveAddress = "";

    // Check pre or post indexing
    if (instruct_arr[7] == 1){
        preOrPostBit = true;
    }
    else{
        preOrPostBit = false;
    }
    // Check up or down bit
    if (instruct_arr[8] == 1){
        upOrDownBit = true;
    }
    else{
        upOrDownBit = false;
    }
    // Check byte or word bit
    if (instruct_arr[9] == 1){
        byteOrWordBit = true;
    }
    else{
        byteOrWordBit = false;
    }
    // Check write back bit
    if (instruct_arr[10] == 1){
        writeBackBit = true;
    }
    else{
        writeBackBit = false;
    }
    // Check load or store bit
    if (instruct_arr[11] == 1){
        loadOrStoreBit = true;
    }
    else{
        loadOrStoreBit = false;
    }

    // Calculate effective address
    if (upOrDownBit){
        effectiveAddress = hexAddition(base, offsett);
    }
    else {
        effectiveAddress = hexSubtraction(base, offsett);
    }
    // Calculate effective address if pre or post indexing
    if (preOrPostBit){
        if (upOrDownBit){
            effectiveAddress = hexAddition(base, offsett);
        }
        else{
            effectiveAddress = hexSubtraction(base, offsett);
        }
    }
    // Check load or store
    if (loadOrStoreBit){
        // load byte
        if (byteOrWordBit){
            // Convert the effective address to WORD
            effectiveAddressWordValue = static_cast<WORD>(stoul(effectiveAddress, nullptr, 16));
            // Load the byte from the effective address
            registers[desRegIndex] = ram[effectiveAddressWordValue / 4] & 0xff;

        }
        // load word
        else{
            // Convert the effective address to WORD
            effectiveAddressWordValue = static_cast<WORD>(stoul(effectiveAddress, nullptr, 16));
            // Load the word from the effective address
            registers[desRegIndex] = ram[effectiveAddressWordValue / 4];
        }
    }
    // store
    else {
        // store byte
        if (byteOrWordBit){
            // Convert the effective address to WORD
            effectiveAddressWordValue = static_cast<WORD>(stoul(effectiveAddress, nullptr, 16));
            // Convert the destination register to WORD
            destRegWordValue = static_cast<WORD>(stoul(registers[desRegIndex], nullptr, 16));
            // Store the byte in the effective address
            ram[effectiveAddressWordValue / 4] = destRegWordValue & 0xff;

        }
        // store word
        else{
            // Convert the effective address to WORD
            effectiveAddressWordValue = static_cast<WORD>(stoul(effectiveAddress, nullptr, 16));
            // Convert the destination register to WORD
            destRegWordValue = static_cast<WORD>(stoul(registers[desRegIndex], nullptr, 16));
            // Store the word in the effective address
            ram[effectiveAddressWordValue / 4] = destRegWordValue;
        }
    }
    // Calculate effective address if pre or post indexing
    if (preOrPostBit){
        if(upOrDownBit){
            effectiveAddress = hexAddition(base, offsett);
        }
        else{
            effectiveAddress = hexSubtraction(base, offsett);
        }
    }
    // write back
    if (writeBackBit){
        // Update the source register with the effective address
        registers[sourceRegIndex] = effectiveAddress;
    }
    // Check if offset is register and if it is load or store
    if (isRegister){
        // Check if it is load or store
        if (loadOrStoreBit){
            if (isPC(destReg)){
                timeCycle += 5;
            }
            else{
                timeCycle += 3;
            }
            cout << "LDR " << destReg << " [ " << sourceReg << " + #" << effectiveAddress << " ]" << endl;
            cout << "Updated time cycle: " << timeCycle << endl; 
        }
        else{
            timeCycle += 2;
            cout << "STR " << destReg << " [ " << sourceReg << " + #" << effectiveAddress << " ]" << endl;
            cout << "Updated time cycle: " << timeCycle << endl; 
        }
    }
    else{
        // Check if it is load or store
        if (loadOrStoreBit){
            if (isPC(destReg)){
                timeCycle += 5;
            }
            else{
                timeCycle += 3;
            }
            cout << "LDR " << destReg << " [ " << sourceReg << " + r" << offsett << " ]" << endl;
            cout << "Updated time cycle: " << timeCycle << endl; 
        }
        else{
            timeCycle += 2;
            cout << "STR " << destReg << " [ " << sourceReg << " + r" << offsett << " ]" << endl;
            cout << "Updated time cycle: " << timeCycle << endl; 
        }
    }
}

int main(){
    // array to store instruction in binary
    int instruct_arr[32];
    // array to store opCode in binary
    int opCode_arr[4];
    // strings to store destination register, source register and operand
    string destReg_str, sourceReg_str, operand_str;
    // arrays to store destination register, source register and operand in binary
    int destReg_arr[4], sourceReg_arr[4], operand_arr[12];
    // arrays to store destination register, source register and operand in binary
    string destReg[16], sourceReg[16], registers[16];
    // arrays to store b and bl offset in binary
    int bAndBlOffset[24];
    // variables to store generated op code, decoded instruction
    string genOpCode = "", decodedInstruct = "";
    WORD instruction;
    programCounter = 0;
    // time cycle for instruction
    int timeCycle = 0;
    // flag for immediate or register
    bool isRegister = false;
    // flags for different types of instructions
    bool branchOrSWI = false, branch = false, branchWithLink = false, SWI = false, unSupInstr = false;
    // flags for single data transfer instruction
    bool singleDataBit = false, preOrPostBit = false, upOrDownBit = false, byteOrWordBit = false, writeBackBit = false, loadOrStoreBit = false;

    // array to store opCodes
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

    // populate ram
    populateRam();

    // initialize registers
    for (int i = 0; i < 16; i++){
        destReg[i] = "r" + to_string(i);
        sourceReg[i] = "r" + to_string(i);
    }

    // prompt instruction from user
    cout << "Enter the instruction in hexadecimal: ";
    cin >> hex >> instruction;

    // hexadecimal instruction to binary
    hexToBin(instruction, instruct_arr);

    cout<<"hexadecimal to binary is: ";
    for (int i = 0; i < 32; i++){
        cout << instruct_arr[i];
    }
    cout << endl;

    // extracting b and bl offset bits
    for (int i = 0, j = 8; i < 24; i++, j++){
        bAndBlOffset[i] = instruct_arr[j];
    }

    // extracting branch with link bit from instruction
    if (instruct_arr[7] == 1){
        branchWithLink = true;
    }
    else{
        branchWithLink = false;
    }

    // extracting immediate bit from instruction
    if (instruct_arr[6] == 0){
        isRegister = true;
    }
    else{
        isRegister = false;
    }

    // extracting branch or SWI bit from instruction
    if (instruct_arr[4] == 1){
        branchOrSWI = true;
    }
    else{
        branchOrSWI = false;
    }

    // extracting unSupported instruction bit from instruction
    if (instruct_arr[6] == 1){
        unSupInstr = false;
    }
    else{
        unSupInstr = true;
    }

    // extracting SWI bit from instruction
    if (instruct_arr[5] == 1){
        SWI = true;
    }
    else{
        SWI = false;
    }

    // cheching if instruction is branch instruction
    if (!SWI && branchOrSWI && !unSupInstr){
        branch = true;
    }
    else{
        branch = false;
    }

    // checking if instruction is single data transfer instruction
    if (instruct_arr[5] == 1){
        singleDataBit = true;
    }
    else{
        singleDataBit = false;
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

    // retrieving values of destination register, source register and operand
    destReg_str = destReg[binToDec(destReg_arr, 4)];
    sourceReg_str = sourceReg[binToDec(sourceReg_arr, 4)];
    operand_str = to_string(binToDec(operand_arr, 12));
    genOpCode = opCode[binToDec(opCode_arr, 4)];

    setRegisters(registers);
    printRegisters(registers);

    // Checking varitions of instructions
    if (branchOrSWI){
        if(!unSupInstr){
            if(SWI){
                SWIImp(instruction, registers, timeCycle);
            }
            else {
                BAndBLImp(bAndBlOffset, branchWithLink, registers, timeCycle);
            }
        }
        else{
            timeCycle += 1;
            cout << "Unsupported Instruction!" << endl;
            cout << "Updated time cycle: " << timeCycle << endl;
        }
    }
    else{
        if(singleDataBit){
            singleDataTransfer(instruct_arr, destReg_str, sourceReg_str, operand_str, preOrPostBit, upOrDownBit, byteOrWordBit, writeBackBit, loadOrStoreBit, registers, isRegister, timeCycle);
        }
        else{
            DataProcessingInstruction(genOpCode, destReg_str, sourceReg_str, operand_str, isRegister, registers, timeCycle);
        }
    }
}


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
// MOV r0,#1 0xE3A00001
// MOV r1,#2 0xE3A01002
// ADD r2,r0,r1 0xE0802001
// ADD r2,r2,#5 0xE2822005
// ADD r2, r2, 15 0xE282200F
// SUB r3, r3, 1 0xE2433001
// SUB r3, r2, r1 0xE2423001
// B 0xEA000000
// BL 0xEB000000
// SWI 0xEF000000
// LDR r0, [r1, #4] 0xE5900004
// LDR r0, [r1, r2] 0xE7900002
// STR r0, [r1, #4] 0xE5800004
// STR r0, [r1, r2] 0xE7800002
// LDR r0, [r1, r5] 0xE5900050