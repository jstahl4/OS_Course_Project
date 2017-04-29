/*
Tom Kueny
James Stahl
*/

#ifndef FILE_H
#define FILE_H
#include <iostream>
using namespace std;

class File{
public:
    File(std::string fileName);
    File(std::string fileName, int blockSize, char* buffer = NULL);

    // operator overload
    bool operator==(File &) const;
    
    // destructor
    ~File();

    // accessors
    string get_name() const;
    int get_size() const;
    int get_block_size() const;
    char * get_data() const;

    // mutators
    void set_data(char*);
    void set_size(int);
    void set_block_size(int);
    void set_name(string);

    // calculators
    void calculate_size();

private:
    static int block_size;
    char* data;
    std::string name;
    int size;
};
#endif
