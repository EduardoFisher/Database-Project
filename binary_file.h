//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_BINARY_FILE_H
#define MYDB_BINARY_FILE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
//#include <vector>

using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(std::fstream& f, const char filename[]) throw(char*);
void open_fileW(std::fstream& f, const char filename[]);


class Record{
public:
    Record(int size = 0)
    {
        _size = size;
    }

    Record(std::vector<std::string> v, int size)
    {
        _size = size;
        for(unsigned int i = 0; i < v.size(); i++)
        {
            strcpy(record[i], v.at(i).c_str());
        }
    }
    void Size_setter(int size)
    {
        _size = size;
    }

//    Record(const char str[]){
//        strcpy(record, str);
//    }
    streampos write(std::fstream& outs);
    long read(std::fstream& ins, long recno);

    int size()
    {
        return _size;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const Record& r);

    friend bool operator ==(const Record& lhs, const Record& rhs);

    friend bool operator >=(const Record& lhs, const Record& rhs);

    char* getRow(int i)
    {
        return this->record[i];
    }

private:
    static const int MAX = 20;
    int _size;
    int recno;
    char record[MAX][MAX];
};

streampos Record::write(std::fstream &outs){
    //write to the end of the file.
    streampos pos = outs.tellp();
    outs.write(&record[0][0], sizeof(record));
    return pos / sizeof(record); // divide by the size of the records.
}
long Record::read(std::fstream &ins, long recno){
    long pos = recno * sizeof(record);
    ins.seekg(pos, std::ios_base::beg);

    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();

}
std::ostream& operator<<(std::ostream& os,
                         const Record& r)
{
//    os << ":" << r._size << endl;
    os << "|";
    os << setiosflags(ios::left);
    for(int i = 0; i < r._size; i++)
    {
        os << setw(12) << setfill(' ') << r.record[i];
    }
    os << resetiosflags(ios::left);
    os /*<< setfill('\0')*/ << "|";
    return os;
}

bool operator==(const Record &lhs, const Record &rhs) {
    bool equal = true;
    for(int i = 0; i < 3; i++)
    {
        if(lhs.record[i] != rhs.record[i])
            equal = false;
    }
    return equal;
}

bool operator>=(const Record &lhs, const Record &rhs) {
    return (lhs.record[0] >= rhs.record[0]);
}

//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    std::fstream ff;
    ff.open (filename, std::ios_base::app|
                       std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) std::cout<<"file_exists: File does NOT exist: "<<filename<<std::endl;
        return false;
    }
    if (debug) std::cout<<"file_exists: File DOES exist: "<<filename<<std::endl;
    return true;
}

void open_fileRW(std::fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename))
    {
        f.open(filename, std::ios_base::app | std::fstream::out|std::fstream::binary);
        if (f.fail()){
            std::cout<<"file open (RW) failed: with out|"<<filename<<"]"<<std::endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) std::cout<<"open_fileRW: file created successfully: "<<filename<<std::endl;
        }
    }
    else{
        f.open (filename,
                std::fstream::in | std::fstream::out| std::ios_base::app |std::fstream::binary );
        if (f.fail()){
            std::cout<<"file open (RW) failed. ["<<filename<<"]"<<std::endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(std::fstream& f, const char filename[]){
    f.open (filename, std::ios_base::app|
                      std::fstream::out| std::fstream::binary );
    if (f.fail()){
        std::cout<<"file open failed: "<<filename<<std::endl;
        throw("file failed to open.");
    }

}
void save_list(Record list[], int count, std::string File){
    std::fstream f;
    open_fileW(f, File.c_str());
    for (int i = 0; i<count; i++){
        list[i].write(f);
    }
    f.close();

}

#endif //MYDB_BINARY_FILE_H
