#include "DISK_API.cpp"

int main(){

    // create Disk object
    Disk * d = new Disk(10,10);

    // create new File object
    string fileName = "TESTFILE.txt";
    cout << "Disk successfully created \n";
    cout << "Creating file named " << fileName << " ...\n";
    d->Create(fileName);

    // open new File object
    File newFile = d->Open(fileName);
    cout << "File named " << newFile.get_name() << " was successfully created.\n";

    // write data to File object
    char* buffer = (char *) "Hi I'm writing data to the file.";
    newFile.set_data(buffer);
    d->Write(newFile, buffer);


    cout << newFile.get_data() << endl;
    cout << newFile.get_name() << " was written to successfully\n";
    cout << (string)d->disk[0]->data << (string)d->disk[1]->data << (string)d->disk[2]->data << (string)d->disk[3]->data << endl;
}