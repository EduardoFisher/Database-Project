//
// Created by lalof on 5/21/2019.
//

#include "database.h"

using namespace std;

const string ADD_ON = "_FieldList.txt";

DataBase::DataBase()
{
    //Do Nothing
}

void DataBase::run()
{
    //do everything.
    bool keep_going = true;
    string statement;
    MMap<string, string> mmap;
    int pos;
    while(keep_going == true)
    {
        pos = 0;
        mmap.clear();
        cout << ":";
        getline(cin, statement);
        sql_parser Parsed(statement);
        if(Parsed.set_MultiMap(mmap))
        {
            //If this is a VALID statement do one of these things.
            //cout << "\033[2J\033[1;H";
            system("CLS"); //For WINDOWS ONLY
            this->Loaded_Tables();
            if(mmap[END_P].at(0) == YES)
            {
                keep_going = false;
            }
            else if(mmap[COMMAND].at(0) == INSERT_P)
            {
                cout << "Inserting into " << mmap[TABLE_NAME].at(0) << endl;
                //When inserting into a file you must first load the file into this Database.
                if(this->Table_Loaded(mmap[TABLE_NAME].at(0), pos))
                {
                    _Tables.at(pos)->insert(mmap[FEILD_LIST]);
                }
                else
                    cout << "You must load the file first before you can insert into it!\n";

            }
            else if(mmap[COMMAND].at(0) == LOAD_P)
            {
                cout << "Loading " << mmap[TABLE_NAME].at(0) << endl;
                //When loading a file does it exist?
                if(Does_File_Exist(mmap[TABLE_NAME].at(0)))
                {
                    if(this->Table_Loaded(mmap[TABLE_NAME].at(0), pos))
                    {
                        cout << "You have already loaded this file\n";
                    }
                    else
                    {
                        cout << "File " << mmap[TABLE_NAME].at(0) << " was loaded!\n";
                        this->_Tables.push_back(new Table(mmap[TABLE_NAME].at(0)));
                    }
                }
                else
                    cout << "Error this file named " << mmap[TABLE_NAME].at(0)
                         << " does not exist and therefore cannot be loaded!\n";
            }
            else if(mmap[COMMAND].at(0) == CREATE_P)
            {
                //Note a created file is always loaded automaticlly;
                cout << "Creating a File named " << mmap[TABLE_NAME].at(0) << endl;
                if(Does_File_Exist(mmap[TABLE_NAME].at(0)))
                    cout << "This file name " << mmap[TABLE_NAME].at(0) <<
                         " already exist and we cannot create a file with the same name!\n";
                else
                {
                    cout << "Table Created\n";
                    _Tables.push_back(new Table(mmap[TABLE_NAME].at(0), mmap[FEILD_LIST]));
                    this->Create_File(mmap[TABLE_NAME].at(0));
                }

            }
            else if(mmap[COMMAND].at(0) == DROP_P)
            {
                cout << "Dropping " << mmap[TABLE_NAME].at(0) << endl;
                //When dropping a file does it exist?
                if(Does_File_Exist(mmap[TABLE_NAME].at(0)))
                {
                    cout << "File " << mmap[TABLE_NAME].at(0) << " was dropped!\n";
                    remove(mmap[TABLE_NAME].at(0).c_str());
                    remove((mmap[TABLE_NAME].at(0) + ADD_ON).c_str());
                }
                else
                    cout << "Error this file named " << mmap[TABLE_NAME].at(0)
                         << " does not exist and therefore cannot be dropped!\n";
            }
            else if(mmap[COMMAND].at(0) == PRINT_P)
            {
                if(this->Table_Loaded(mmap[TABLE_NAME].at(0), pos))
                {
                    cout << endl;
                    _Tables.at(pos)->show();
                }
                else
                    cout << "Error this file named " << mmap[TABLE_NAME].at(0)
                         << " has not been loaded yet please load the file first!\n";
            }
            else if(mmap[COMMAND].at(0) == SELECT)
            {
                //Find me and print the Table with the correct shit!
                if(this->Table_Loaded(mmap[TABLE_NAME].at(0), pos))
                {
                    cout << endl;
                    //(std::vector<std::string> field_names, std::string FieldName, std::string op, std::string value)
                    //Select all command
                    if(mmap[WHERE_P].at(0) == YES)
                    {
                        if(mmap.contains(ASTERISK_P))
                        {
                            if(_Tables.at(pos)->Check_Field_Comp(mmap[FEILD_COMP]))
                            {
//                                Table* f = _Tables.at(pos)->Select(mmap[ASTERISK_P], mmap[FEILD_COMP].at(0), mmap[OPERATOR_P].at(0), mmap[FIND_P].at(0));
                                Table* f = _Tables.at(pos)->Select(mmap[ASTERISK_P], mmap[FEILD_COMP], mmap[OPERATOR_P], mmap[FIND_P], mmap[AND_OR]);
                                f->show();
                                f->DeleteTemp();
                                delete f;
                            }
                            else
                            {
                                cout << "The Fields:" << mmap[FEILD_COMP] << " you entered do match the ones in table!\n";
                            }
                        }
                        else
                        {
                            if(_Tables.at(pos)->Check_Field_List(mmap[FEILD_LIST]))
                            {
                                if(_Tables.at(pos)->Check_Field_Comp(mmap[FEILD_COMP]))
                                {
//                                    Table* f = _Tables.at(pos)->Select(mmap[FEILD_LIST], mmap[FEILD_COMP].at(0), mmap[OPERATOR_P].at(0), mmap[FIND_P].at(0));
                                    Table* f = _Tables.at(pos)->Select(mmap[FEILD_LIST], mmap[FEILD_COMP], mmap[OPERATOR_P], mmap[FIND_P], mmap[AND_OR]);
                                    f->show();
                                    f->DeleteTemp();
                                    delete f;
                                }
                                else
                                {
                                    cout << "The Fields:" << mmap[FEILD_COMP] << " you entered do match the ones in table!\n";
                                }
                            }
                            else
                            {
                                cout << "The Fields:" << mmap[FEILD_LIST] << " you entered do match the ones in table!\n";
                            }
                        }
                    }
                    else
                    {
                        if(mmap.contains(ASTERISK_P))
                        {
                            Table* f = _Tables.at(pos)->Select_All(mmap[ASTERISK_P]);
                            f->show();
                            f->DeleteTemp();
                            delete f;
                        }
                        else
                        {
                            if(_Tables.at(pos)->Check_Field_List(mmap[FEILD_LIST]))
                            {
                                Table* f = _Tables.at(pos)->Select_All(mmap[FEILD_LIST]);
                                f->show();
                                f->DeleteTemp();
                                delete f;
                            }
                            else
                            {
                                cout << "The Fields:" << mmap[FEILD_LIST] << " you entered do match the ones in table!\n";
                            }
                        }
                    }
                }
                else
                    cout << "Error this file named " << mmap[TABLE_NAME].at(0)
                         << " has not been loaded yet please load the file first!\n";
            }
        }
        else
            cout << "Not a valid string please enter another one.\n";
    }
}

bool DataBase::Table_Loaded(string name, int& pos)
{
    for(unsigned int i = 0; i < this->_Tables.size(); i++)
    {
        if(name == _Tables.at(i)->Get_Name())
        {
            pos = i;
            return true;
        }
    }
    pos = 0;
    return false;
}

void DataBase::Loaded_Tables()
{
    cout << "The Tables you have Loaded are: ";
    for(unsigned int i = 0; i < this->_Tables.size(); i++)
        cout << _Tables.at(i)->Get_Name() << " ";
    cout << endl;
}

void DataBase::Create_File(string name)
{
    fstream os;
    //NOTE: THIS IS BUSTED ON LINUX THIS WORKS ON WINDOWS
    os.open(name, std::ios_base::app|std::fstream::out|std::fstream::binary);
    os.close();
}

bool DataBase::Does_File_Exist(string name)
{
    ifstream f(name.c_str());
    if(!f)
        return false;
    return true;
}
