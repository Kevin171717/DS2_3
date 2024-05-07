// 資訊二甲 11127116 曾詮淳
// 資訊四乙 10942208 陳洺安

#include <iostream>
#include <vector>                              // vector
#include <fstream>                             // ifstream
#include <sstream>
#include <cmath>
#include <iomanip>
#include <iomanip>

using namespace std;

struct Student {
    char sid[10];  // STUD ID 
    char sname[10]; // STUD NAME
    unsigned char score[6];  // 
    float avg; // MEAN
};

class Read{

public:
    void txtToBin(string fileName);  // convert to binary
    bool readF(string,vector<Student> & );
    void readBin(string fileName,vector<Student> &inputData);

};

class doubleHashing {

private:

    struct HashMap {
        string sid;
        string sname;
        int score[6];
        float avg;
        int HashValue;
        bool isEmpty ;
        HashMap() : sid(""), sname(""), avg(0.0), HashValue(-1), isEmpty(true) {}
    };


    int tableSize;

    int step( int, long long );
    bool isPrime( int );
    int nextPrime( int );
    int highestStep(int);
    void resizeTable( int );
    void doubleHashProbing ( vector<Student> inputData, int &);
    int probNext(  int, int &, int );
    void write(string, int, vector<Student> );

public:
    vector<HashMap> table;
    void hashFuction( int, vector<Student> inputData, string );

    void clearUp() {            // erase the object content
        table.clear();
        tableSize = 0;
    } // end clearUp

    ~doubleHashing() { clearUp(); }      // destructor: destroy the objec

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
    int probNext( int, int &);
    void write(string, int, vector<Student> );
    int findUnsuccessful( );

public:
    vector<HashMap> table;
    void hashFuction( int, vector<Student> inputData, string );
    bool findLocation(string, int&);

    void clearUp() {            // erase the object content
        table.clear();
        tableSize = 0;
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
    doubleHashing doublehash ;
    int cm1_runned = false ;

    while(true) {
        cout << endl << "******* Hash Table *****";
        cout << endl << "* 0. QUIT              *";
        cout << endl << "* 1. Quadratic probing *";
        cout << endl << "* 2. Double hashing    *";
        cout << endl << "************************";
        cout << endl << "Input a choice(0, 1, 2):";
        cin >> command; // get a command
        if (command == 1) {
            inputData.clear();
            hash.clearUp();
            cout << endl << "Input a file number ([0] Quit): ";
            cin >> fileName;
            if ( fileName != "0" ) {
                if ( read.readF( fileName, inputData ) ) {
                    hash.hashFuction( inputData.size(), inputData, fileName );
                    cm1_runned = true ;
                }

            }

        }
        else if (command == 2 ) {
            if( cm1_runned ) {
                inputData.clear();
                doublehash.clearUp();

                if ( fileName != "0" ) {
                    if ( read.readF( fileName, inputData ) ) {
                        doublehash.hashFuction( inputData.size(), inputData, fileName );
                    } // if
                }//if
            }
            else {
                cout << "### Command 1 first. ###" << endl ;
            }

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


// doubleHashing ---------------------------------------------------------------------------------------------
void doubleHashing ::hashFuction( int size, vector<Student> inputData, string fileName ) {
    int totalSearch = 0;
    resizeTable(size);
    doubleHashProbing(inputData, totalSearch);
    write(fileName, totalSearch, inputData);

}
void doubleHashing::doubleHashProbing(vector<Student> inputData, int& totalSearch) {

    for ( int i = 0; i < inputData.size(); i++ ) {
        long long product = 1;
        long long product1 = 1;
        int step = 0 ;
        for ( char j : inputData[i].sid ) { //  
            if ( j <= '9' && j >= '0' ) {
                product = product*int(j);
                product = product%this->tableSize;
            } // if
        } // for

        for ( char j : inputData[i].sid ) {
            if ( j <= '9' && j >= '0' ) product1 = product1*int(j);
        } // for

        step = this->step(inputData.size(), product1);

        int location = product;

        totalSearch++;


        if ( !table[location].isEmpty ) // collision
            location = probNext( location, totalSearch, step );


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
    } // for

}

int doubleHashing ::highestStep(int size) {
    size = ceil(size/5);
    int prime = this->nextPrime(size) ;
    return prime ;
} // highestStep

bool doubleHashing ::isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i*i <= n; i+=2) {
        if (n % i == 0) return false;
    } // for
    return true;
} // isPrime()

int doubleHashing ::nextPrime(int num) {
    int prime = num+1 ;
    while (true) {
        if (isPrime(prime)) {
            return prime;
        }
        prime++;
    }
} // nextPrime()


int doubleHashing ::step(int size, long long product ) {
    return (this->highestStep(size)-(product%this->highestStep(size)));
} // 


int doubleHashing ::probNext(int location, int &totalSearch, int step ) { // step func
    int cur = location;
    int step_num = step ;
    while ( !table[cur].isEmpty ) {
        totalSearch++;
        cur = ( cur + step_num )%tableSize;
    }

    return cur;
} // probNext()

void doubleHashing ::resizeTable(int size ) {
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

void doubleHashing::write(string fileName, int totalSearch, vector<Student> inputData ) {
    fstream file ;
    string d = "double" + fileName + ".txt" ;
    file.open( d.c_str(), ios::out ) ;

    file <<  " --- Hash table created by Double probing ---" << endl ;
    for( int i = 0 ; i < tableSize ; i++ ) {
        file << "[" << i << "] " ;
        if( table[i].isEmpty != true ) {
            file << table[i].HashValue << ", " << table[i].sid ;
            file << ", " << table[i].sname << ", " << table[i].avg ;
        } // if
        file << endl ;

    }  // for

    file <<  " ----------------------------------------------- " << endl ;
    file.close();

    double successfulRate = static_cast<double>(totalSearch) / static_cast<double>(inputData.size());
    cout << fixed << setprecision(4);
    cout << endl << "Hash table has been successfully created by Double hashing" ;
    cout << endl << "successful search: " << successfulRate <<" comparisons on average"<< endl;

}


// Quadratic -------------------------------------------------------------------------------------------------
void Quadratic ::hashFuction( int size, vector<Student> inputData, string fileName ) {
    int totalSearch = 0;
    resizeTable(size);
    quadraticProbing(inputData, totalSearch);
    write(fileName, totalSearch, inputData);
    string key = "-1";
    bool isFind = false;
    int step = 0;
    while ( key != "0" ) {
        cout << endl << "Input student ID to search ([0] Quit) :";
        cin >> key;
        isFind = findLocation(key, step);
        if ( !isFind )
            cout << endl << key << " is not found after "  << step << " probes";
    }

}

bool Quadratic ::findLocation( string key, int &step ) {
    long long product = 1;
    for ( char i : key ) {
        if ( i <= '9' && i >= '0' ) {
            product = product*int(i);
            product = product%tableSize;
        }
    }
    int location = product;

    while ( !table[location].isEmpty ) {
        step++;
        location = product;
        location = (step*step + product)%tableSize;
        if ( key == table[location].sid ) {
            step++;
            cout << endl << "{ " << table[location].sid << ", " << table[location].sname << ", " << table[location].avg << " }";
            cout << "is found after " << step << " probes.";
            return true;
        }
    }

    step++;
    return false;
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


void Quadratic::write(string fileName, int totalSearch, vector<Student> inputData ) {
    fstream file ;
    string d = "quadratic" + fileName + ".txt" ;
    file.open( d.c_str(), ios::out ) ;

    file <<  " --- Hash table created by Quadratic probing ---" << endl ;
    for( int i = 0 ; i < tableSize ; i++ ) {
        file << "[" << i << "] " ;
        if( table[i].isEmpty != true ) {
            file << table[i].HashValue << ", " << table[i].sid ;
            file << ", " << table[i].sname << ", " << table[i].avg ;
        } // if
        file << endl ;

    }  // for

    file <<  " ----------------------------------------------- " << endl ;
    file.close();

    int unsuccessful = findUnsuccessful();
    double unsuccessfulRate = static_cast<double>(unsuccessful) / static_cast<double>(tableSize);
    double successfulRate = static_cast<double>(totalSearch) / static_cast<double>(inputData.size());
    cout << fixed << setprecision(4);
    cout << endl << "Hash table has been successfully created by Quadratic probing" << endl ;
    cout << "unsuccessful search: " << unsuccessfulRate <<" comparisons on average" << endl;
    cout << "successful search: " << successfulRate <<" comparisons on average" << endl;

}

int Quadratic::findUnsuccessful( ) {
    int total = 0;
    for ( int i = 0 ; i < tableSize; i++ ) {
        int cur = i;
        int step = 1;
        while ( !table[cur].isEmpty && step < tableSize ) {
            total++;
            cur = i;
            cur = ( cur + step*step )%tableSize;
            step++;
        }
    }

    return total;
}

// READ -------------------------------------------------------------------------------------------

bool Read ::readF(string fileName,vector<Student> &inputData ) {
    string txtFile = "input" + fileName + ".txt"; // read txt 
    string binFile = "input" + fileName + ".bin"; // read bin

    fstream fileT; // pointer to file txt 
    fstream fileB; // pointer to file bin

    fileT.open(txtFile.c_str(), fstream::in);
    fileB.open(binFile.c_str(), fstream::in);
    if ( !fileT.is_open() && !fileB.is_open() ) {
        cout << endl << "### " << binFile << " does not exist! ###" << endl;
        cout << endl << "### " << txtFile << " does not exist! ###" << endl;
        return false;
    }
    else if ( fileT.is_open() && !fileB.is_open() ) { // txt is exist and bin is not exist
        cout << endl << "### input301.bin does not exist! ###" << endl;
        txtToBin(fileName);
    }

    readBin(fileName, inputData);
    return true;

}

void Read::txtToBin(std::string fileName) { // convert txt to bin 
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
