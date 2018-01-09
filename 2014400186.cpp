#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;
int quant=100;  //time quant of the round robin

//Abstracts the processes
struct Process{

    string processName;  //Name of the process
    int arrivalTime; //Arrival time of the process to the CPU
    string programCodeFile; //Name of the code file that includes instructions.
    int instNum; //Last instruction number that executed

    Process(){};

    Process(string name, int arrival, string programCode){
        processName = name;
        arrivalTime= arrival;
        programCodeFile = programCode;
        instNum=0;
    }

};

//Reads instruction files and saves them to vectors.
void readCodes(vector<int> &code1, vector<int> & code2,vector<int>& code3,vector<int> & code4 ){
    string inst;
    int time;

    ifstream myFile;
    myFile.open("./1.code.txt");
    while(!myFile.eof()){
        myFile >> inst >> time;
        code1.push_back(time);
    }
    myFile.close();
    myFile.open("./2.code.txt");
    while(!myFile.eof()){
        myFile >> inst >> time;
        code2.push_back(time);
    }
    myFile.close();
    myFile.open("./3.code.txt");
    while(!myFile.eof()){
        myFile >> inst >> time;
        code3.push_back(time);
    }
    myFile.close();
    myFile.open("./4.code.txt");
    while(!myFile.eof()){
        myFile >> inst >> time;
        code4.push_back(time);
    }
    myFile.close();

}

//Reads definition.txt file, saves processes to Process vector, saves arrival times to another vector also.
void readDefinition(vector<Process>* p, vector<int> *arrival){
    //Information of the processes
    string process;
    string codeFile;
    int arrivalTime;

    ifstream infile;
    infile.open("./definition.txt");
    if (!infile) {
        cerr << "Unable to open file definition.txt";
        exit(1);   // call system to stop
    }

    while(!infile.eof()) // To get all the lines.
    {
        infile >> process >> codeFile >> arrivalTime ;
        p->push_back(Process(process, arrivalTime, codeFile)); //Creates processes and pushes them to a vector
        arrival->push_back(arrivalTime); //Fills another vector with the arrival times
    }
    infile.close();
}

//Prints the state of the queue in to "output.txt".
void printQueue(queue<Process> q, int time, ofstream * resultFile){
    *resultFile <<time << "::HEAD-";
    if(q.size()==0){
        *resultFile <<"-TAIL" << endl;
    }else {
        while (!q.empty()) {
            *resultFile << q.front().processName << "-";
            q.pop();
        }
        *resultFile << "TAIL" << endl;
    }
};

int main() {

    vector<int> code1; //Includes instruction times of "1.code.txt" file
    vector<int> code2; //Includes instruction times of "2.code.txt" file
    vector<int> code3; //Includes instruction times of "3.code.txt" file
    vector<int> code4; //Includes instruction times of "4.code.txt" file
    vector<int> arrivalTime; //Includes arrival times of the processes.
    int iterateArrival=1; //For keeping track of the arrival times.

    readCodes(code1, code2, code3, code4); //Reads code files and fills the code vectors.

    vector<Process> p ; //Includes all processes included in the definition.txt file
    readDefinition(&p, &arrivalTime); //Reads definition.txt file.

    //Creates output file
    ofstream resultFile;
    resultFile.open("output.txt");
    if(!resultFile.is_open()){
        cout<< "output file can not be opened." << endl;
    }

    int timer=0; //Current time of the simulation.
    queue<Process> mainQueue; //Round robin process queue
    int k=0; //Incase of multiple processes that arrive at time 0.
    while(arrivalTime[k]==0){
        mainQueue.push(p[k]); //Pushes the process that arrives at the time 0 into the queue.
        k++;
    }

    printQueue(mainQueue, timer, &resultFile); //Pirnts the queue at the time 0.
    while(!mainQueue.empty()){ //Assumed that CPU will never be idle.
        vector<int> currentInstSet; //Instruction set of current process.
        int processSum=0; //Shows the time that have been spened on that process in current time quant.
        Process current = mainQueue.front(); //Current process in the CPU.
        mainQueue.pop();

        //Initializes the current instruction set.
        if(current.programCodeFile=="1.code.txt"){
            currentInstSet = code1;
        }else if(current.programCodeFile=="2.code.txt"){
            currentInstSet = code2;
        }else if(current.programCodeFile=="3.code.txt"){
            currentInstSet = code3;
        }else if(current.programCodeFile=="4.code.txt"){
            currentInstSet = code4;
        }else{
            cout<< "instruction set can not found."<<endl;
            exit(1);
        }

        //Loops untill instruction set is over (process is over) or time quantum is over.
        while(processSum<quant && current.instNum<currentInstSet.size()){
            processSum += currentInstSet[current.instNum]; //Time jumps to the next instruction.
            current.instNum ++;
        }

        timer +=processSum; //Updates the simulation time when the process is about to live the CPU.

        //Checks for the new arrivals.
        while (arrivalTime[iterateArrival] <= timer && iterateArrival<p.size()){
            mainQueue.push(p[iterateArrival]);
            iterateArrival ++;
        }
        //Process is not finished entirely so oush it back to the queue.
        if(current.instNum<currentInstSet.size()){
            mainQueue.push(current);
        }

        //Prints updated version of the queue.
        printQueue(mainQueue,timer, &resultFile);

    }

    resultFile.close();

    return 0;
}


