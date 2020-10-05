#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
//#include <boost/filesystem.hpp>

using namespace std;

class LocalStorage
{
private:
    ofstream db_writer;
    ifstream db_reader;

public:
    string get(string);
    bool set(string, string);
    bool del(string);
    LocalStorage();
};

LocalStorage::LocalStorage()
{
    //boost::filesystem::create_directories("/local_storage");
}

string LocalStorage::get(string k)
{
    string line;
    string result;
    this->db_reader.open(k);
    if (this->db_reader.is_open())
    {
        while (getline(this->db_reader, line))
        {
            result += line;
        }
    }
    this->db_reader.close();
    return result;
}

bool LocalStorage::set(string k, string d)
{
    this->db_writer.open(k);

    if (this->db_writer.is_open())
    {
        this->db_writer << d << endl;
    }

    this->db_writer.close();

    return true;
}

bool LocalStorage::del(string k)
{
    const int n = k.length();

    // declaring character array
    char char_array[50];

    // copying the contents of the
    // string to char array
    strcpy(char_array, k.c_str());

    if (remove(char_array) != 0)
    {
        perror("Error deleting file");
        return false;
    }
    else
    {
        return true;
    }
}
