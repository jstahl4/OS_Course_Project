/*
 * Lead Author: James Stahl
 * Additional Authors: Tom Kueny
 *
 */

#ifndef FILE_H
#define FILE_H
#include <iostream>
using namespace std;

class File{
public:
    File(std::string fileName);
    File(std::string fileName, int blockSize, char* buffer = NULL);
    File(const File & f);

    // operator overload
    bool operator==(const File &) const;

    // destructor
    ~File();

    // accessors
    string get_name() const;
    int get_size() const;
    int get_block_size() const;
    int get_starting_block() const; //tell disk where file is located
    char * get_data() const;

    // mutators
    void set_data(char*);
    void set_size(int);
    void set_block_size(int);
    void set_starting_block(int); //set first block location of file
    void set_name(string);

    // calculators
    void calculate_size();

private:
    static int block_size;
    int starting_block;
    char* data;
    std::string name;
    int size;
};
#endif
