/*
Tom Kueny
James Stahl
*/
#include <iostream>
class File{
public:
    File(std::string fileName);
    File(std::string fileName, int blockSize, char* buffer = NULL);

    // accessors
    string get_name();
    int get_size();
    char * get_data();

    // mutators
    void set_data(char*);
    void set_size(int);
    static void set_block_size(int);
    void set_name(string);

    // calculators
    void calculate_size();

private:
    static int block_size;
    char* data;
    std::string name;
    int size;
};
