#include "DISK_API.cpp"

int main()
{

    // create Disk object
    Disk *d = new Disk(10, 10);

    // create new File object
    string fileName = "TESTFILE.txt";
    cout << "Disk successfully created \n";
    cout << "Creating file named " << fileName << " ...\n";
    d->Create(fileName);

    // open new File object
    File newFile = d->Open(fileName);
    cout << "File named " << newFile.get_name() << " was successfully created.\n";

    // write data to File object
    char *buffer = (char *) "Hi I'm writing data to the file.";
    newFile.set_data(buffer);
    d->Write(newFile, buffer);

    // output what was written to disk
    cout << newFile.get_data() << endl;
    cout << newFile.get_name() << " was written to successfully\n";
    cout << (string) d->disk[0]->data << (string) d->disk[1]->data << (string) d->disk[2]->data
         << (string) d->disk[3]->data << endl;
    cout << "Lets write another file called TEST2.txt\n";
    char *buffer2 = (char *) "New data yo.";
    string file2 = "TEST2.txt";
    d->Create(file2);
    File newFile2 = d->Open(file2);
    newFile2.set_data(buffer2);
    cout << buffer2 << endl;
    d->Write(newFile2, buffer2);
    for(int i = 0; i < 10; i++){
        cout << (string)d->disk[i]->data << endl;
    }
    cout << "Deleting " << newFile.get_name() << " ...\n";
    d->Delete("TESTFILE.txt");
    cout << (string) d->disk[0]->data << (string) d->disk[1]->data << (string) d->disk[2]->data
         << (string) d->disk[3]->data << endl;
    cout << "If nothing from the file has been repeated in the previous line, file was deleted successfully \n";

    cout << "Now lets write the file again and see if we can read it...\n";
    d->Write(newFile, buffer);
    char* readBuffer = d->Read("TESTFILE.txt");
    cout << (string) readBuffer << endl;

}