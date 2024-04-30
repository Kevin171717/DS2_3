#include <iostream>
# include <vector>                              // vector
# include <iostream>                            // cin, cout, ...
# include <stdlib.h>							// atoi
# include <fstream>                             // ifstream
#include <sstream>
#include <cmath>
#include <iomanip>

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
    bool readF(string,vector<Student> & );
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
        bool isEmpty ;
        HashMap() : sid(""), sname(""), avg(0.0), HashValue(-1) ,  isEmpty(true) {}
    };
    int tableSize;

    void resizeTable( int );
    void quadraticProbing ( vector<Student> inputData, int &);
    int probNext( int, int & );
    void write(string, int );
public:
    vector<HashMap> table;
    void hashFuction( int, vector<Student> inputData, string );




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
                if ( read.readF( fileName, inputData ) ) {
                    hash.hashFuction( inputData.size(), inputData, fileName );

                    // cout << endl << "Hash table has been successfully created by Quadratic probing";
                }

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
void Quadratic ::hashFuction( int size, vector<Student> inputData, string fileName ) {
    int totalSearch = 0;
    resizeTable(size);
    quadraticProbing(inputData, totalSearch);
    write(fileName, totalSearch);
}

void Quadratic::quadraticProbing(vector<Student> inputData, int& totalSearch) {

    for ( int i = 0; i < inputData.size(); i++ ) {
        long long product = 1;
        for ( char j : inputData[i].sid ) {
            if ( j <= '9' && j >= '0' ) {
                product = product*int(j);
                product = product%tableSize;
            }
        }

        int location = product;
        totalSearch++;

        if ( !table[location].isEmpty )
            location = probNext( location, totalSearch );

        table[location].isEmpty = false;
        table[location].HashValue = product;
        table[location].sid = inputData[i].sid;
        table[location].sname = inputData[i].sname;
        table[location].avg = inputData[i].avg;
        int k = 0;
        for ( unsigned char j : inputData[i].score ) {
            table[location].score[k] = j;
            k++;
        }
    }

}

int Quadratic ::probNext(int location, int &totalSearch ) {
    int cur = location;
    int step = 1;
    while ( !table[cur].isEmpty ) {
        totalSearch++;
        cur = location;
        cur = ( cur + step*step )%tableSize;
        step++;
    }
    return cur;

}

void Quadratic ::resizeTable(int size ) {
    size = ceil(size*1.15);
    bool find = true;
    if ( size < 2 )
        size = 2;
    else {
        while ( find ) {
            bool isPrime = true;

            for ( int i = 2; i < size ; i++ ) {
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

    tableSize = size;
    table.resize(size);
}

void Quadratic::write(string fileName, int totalSearch ) {
    fstream file ;
    string d = "quadratic" + fileName + ".txt" ;
    file.open( d.c_str(), ios::out ) ;

    file <<  " --- Hash table created by Quadratic probing ---" << endl ;
    for( int i = 0 ; i < tableSize ; i++ ) {
        file << "[" << setw(3) << i << "] " ;
        if( table[i].isEmpty != true ) {
            file << setw(10) << table[i].HashValue << ", " << setw(10) << table[i].sid ;
            file << ", " << setw(10) << table[i].sname << ", " << setw(10) << table[i].avg ;
        } // if
        file << endl ;

    }  // for

    file <<  " ----------------------------------------------- " << endl ;
    file.close();
    float unsuccessful = float (totalSearch)/float (tableSize);
    cout << endl << "unsuccessful search: " << unsuccessful <<" comparisons on average";

}

// READ -------------------------------------------------------------------------------------------

bool Read ::readF(string fileName,vector<Student> &inputData ) {
    string txtFile = "input" + fileName + ".txt";
    string binFile = "input" + fileName + ".bin";
    fstream fileT;
    fstream fileB;
    fileT.open(txtFile.c_str(), fstream::in);
    fileB.open(binFile.c_str(), fstream::in);
    if ( !fileT.is_open() && !fileB.is_open() ) {
        cout << endl << "### " << binFile << " does not exist! ###" ;
        cout << endl << "### " << txtFile << " does not exist! ###" << endl;
        return false;
    }
    else if ( fileT.is_open() && !fileB.is_open() ) {
        cout << endl << "### input301.bin does not exist! ###";
        txtToBin(fileName);
    }

    readBin(fileName, inputData);
    return true;

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
/*
    for ( int i = 0 ; i < inputData.size(); i++ ){
        cout << endl << inputData[i].sid;
    }
*/

    inFile.close();

}

