#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem> 
#include "../DTO/SinhVien.h"
#include "../DTO/Khoa.h"

using namespace std;
namespace fs = std::filesystem;

class DataAccess{
    public:
        //load all files in folder via way of loading every single files in the folder (depend on LoadSingleData)
        static vector<SinhVien> LoadAllData(const string &folder){
            vector<SinhVien> All_SinhVien;

            //folder doesnt exist (fail)
            if (!fs::exists(folder) || !fs::is_directory(folder)) {
                return All_SinhVien;
            }

            //each entry is a file
            for (const auto &entry : fs::directory_iterator(folder)){
                if (entry.path().extension() == ".txt"){
                    vector<SinhVien> FileSinhVien = LoadSingleData(entry.path().string());
                    All_SinhVien.insert(All_SinhVien.end(), FileSinhVien.begin(), FileSinhVien.end());
                }
            }
            return All_SinhVien;
        }

        //load each and every single file, read thingies
        static vector<SinhVien> LoadSingleData(const string &filepath){
            vector<SinhVien> Single_SinhVien;
            ifstream file(filepath);
            if (!file.is_open()) return Single_SinhVien;

            string line;
            while (getline(file, line)){
                stringstream ss(line);
                string segment;
                vector<string> data;

                while (getline(ss, segment, '|')) {
                    data.push_back(segment);
                }

                //crack ts up shouja boy
                if (data.size() >= 8){
                    string FullName = data[1];
                    string FirstName = "", Name = "";
                    size_t LastSpace = FullName.find_first_of(" ");
                    if (LastSpace != string::npos){
                        FirstName = FullName.substr(0, LastSpace);
                        Name = FullName.substr(LastSpace + 1);
                    }
                    else{
                        Name = FullName;
                    }

                    SinhVien sv(FirstName, Name, data[2], data[3], data[4], data[6], data[7]);
                    sv.Set_Email(data[0]);
                    sv.Set_Email(data[5]);
                    Single_SinhVien.push_back(sv);
                }
            }
            file.close();
            return Single_SinhVien;
        }


};