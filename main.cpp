#include <iostream>
# include <vector>                              // vector
# include <iostream>                            // cin, cout, ...
# include <stdlib.h>							// atoi
# include <fstream>                             // ifstream
#include <sstream>
#include <cmath>

using namespace std;

struct Student {
    char sid[10];
    char sname[10];
    unsigned char score[6];
    float avg;
};

class Read{

public:
    void txtToBin(string fileName);
    void readF(string,vector<Student> & );
    void readBin(string fileName,vector<Student> &inputData);

};

class Quadratic{
private:
    struct HashMap {
        string sid;
        string sname;
        int score[6];
        float avg;
        int HashValue;
        bool isEmpty;
        HashMap() : sid(""), sname(""), avg(0.0), HashValue(-1) ,  isEmpty(false) {}
    };

    void resizeTable( int );
    void quadraticProbing ( vector<Student> inputData );
public:
    vector<HashMap> table;
    void hashFuction( int, vector<Student> inputData );



    void clearUp() {            // erase the object content
        table.clear();
    } // end clearUp


    ~Quadratic() { clearUp(); }      // destructor: destroy the objec

};


int main() {
    int command;
    string fileNumber;
    string fileName;
    Read read;
    vector<Student> inputData;
    Quadratic hash;

    while(true) {
        cout << endl << "******* Hash Table *****";
        cout << endl << "* 0. QUIT              *";
        cout << endl << "* 1. Quadratic probing *";
        cout << endl << "* 2. Double hashing    *";
        cout << endl << "************************";
        cout << endl << "Input a choice(0, 1, 2):";
        cin >> command; // get a command
        if (command == 1) {
            cout << endl << "Input a file number ([0] Quit): ";
            cin >> fileName;
            if ( fileName != "0" ) {
                read.readF( fileName, inputData );
                hash.hashFuction( inputData.size(), inputData );

            }

        }
        else if (command == 2) {

        }
        else if (command == 0) {
            break;
        }
        else {
            cout << endl << "Command does not exist!" << endl;
        }
    }

    system("pause");
    return 0;
}

// Quadratic -------------------------------------------------------------------------------------------------
void Quadratic ::hashFuction( int size, vector<Student> inputData ) {
    resizeTable(size);

}

void Quadratic::quadraticProbing(vector<Student> inputData) {

}

void Quadratic ::resizeTable(int size ) {
    size = ceil(size*1.15);
    bool find = true;
    if ( size < 2 )
        size = 2;
    else {
        while ( find ) {
            bool isPrime = true;

            for ( int i = 0; i < size ; i++ ) {
                if ( size % i == 0 ) {
                    isPrime = false;
                }
            }
            if ( isPrime )
                find = false;

            if ( !isPrime )
                size++;
        }
    }

    table.resize(size);
}

// READ -------------------------------------------------------------------------------------------

void Read ::readF(string fileName,vector<Student> &inputData ) {
    string txtFile = "input" + fileName + ".txt";
    string binFile = "input" + fileName + ".bin";
    fstream fileT;
    fstream fileB;
    fileT.open(txtFile.c_str(), fstream::in);
    fileB.open(binFile.c_str(), fstream::in);
    if ( !fileT.is_open() && !fileB.is_open() ) {
        cout << endl << "### " << binFile << " does not exist! ###" ;
        cout << endl << "### " << txtFile << " does not exist! ###" << endl;
    }
    else if ( fileT.is_open() && !fileB.is_open() ) {
        cout << endl << "### input301.bin does not exist! ###";
        txtToBin(fileName);
    }

    readBin(fileName, inputData);

}

void Read::txtToBin(std::string fileName) {
    string txtFileName = "input" + fileName + ".txt";
    string binFileName = "input" + fileName + ".bin";

    ifstream inFile(txtFileName.c_str(), ios::in);
    ofstream outFile(binFileName.c_str(), ios::out | ios::binary);
    if (!inFile) {
        cerr << "Cannot open the text file." << endl;
        return;
    }
    if (!outFile) {
        cerr << "Cannot create the binary file." << endl;
        return;
    }
    Student student;
    string line;

    while (getline(inFile, line)) {
        istringstream iss(line);

        // Parse student ID and name
        iss.get(student.sid, 10, '\t');
        iss.ignore(); // Skip tab
        iss.get(student.sname, 10, '\t');
        iss.ignore(); // Skip tab

        // Parse scores
        for (int i = 0; i < 6; ++i) {
            int score;
            iss >> score;
            student.score[i] = static_cast<unsigned char>(score);
            iss.ignore(); // Skip tab
        }

        // Parse average
        iss >> student.avg;

        // Write the student record to the binary file
        outFile.write(reinterpret_cast<char*>(&student), sizeof(student));
    }

    inFile.close();
    outFile.close();
}

void Read ::readBin( string fileName,vector<Student> &inputData ) {
    string binFile = "input" + fileName + ".bin";
    ifstream inFile(binFile.c_str(), ios::in | ios::binary);

    if (!inFile) {
        cerr << "Cannot open the binary file: " << binFile << endl;
        return;
    }
    Student student;
    size_t size = sizeof (student);
    while ( inFile.read(reinterpret_cast<char*>(&student), size) ) {
        inputData.push_back(student);
    }

    inFile.close();

}

