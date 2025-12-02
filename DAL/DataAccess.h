#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem> 
#include "../DTO/SinhVien.h"
#include "../DTO/Khoa.h"

namespace fs = std::filesystem;

class DataAccess{
    public:
        //load all files in folder via way of loading every single files in the folder (depend on LoadSingleData)
        static std::vector<SinhVien> LoadAllData(const std::string &folder){
            std::vector<SinhVien> All_SinhVien;

            //folder doesnt exist (fail)
            if (!fs::exists(folder) || !fs::is_directory(folder)) {
                return All_SinhVien;
            }

            //each entry is a file
            for (const auto &entry : fs::directory_iterator(folder)){
                if (entry.path().extension() == ".txt"){
                    std::vector<SinhVien> FileSinhVien = LoadSingleData(entry.path().string());
                    All_SinhVien.insert(All_SinhVien.end(), FileSinhVien.begin(), FileSinhVien.end());
                }
            }
            return All_SinhVien;
        }

        //load each and every single file, read thingies
        static std::vector<SinhVien> LoadSingleData(const std::string &filepath){
            std::vector<SinhVien> Single_SinhVien;
            std::ifstream file(filepath);
            if (!file.is_open()) return Single_SinhVien;

            std::string line;
            while (getline(file, line)){
                std::stringstream ss(line);
                std::string segment;
                std::vector<std::string> data;

                while (getline(ss, segment, '|')) {
                    data.push_back(segment);
                }

                //break up ho va ten correctly
                if (data.size() >= 8){
                    std::string FullName = data[1];
                    std::string FirstName = "", Name = "";
                    size_t LastSpace = FullName.find_last_of(" ");

                    if (LastSpace != std::string::npos){
                        FirstName = FullName.substr(0, LastSpace);
                        Name = FullName.substr(LastSpace + 1);
                    }
                    else{
                        Name = FullName;
                    }

                    SinhVien sv(FirstName, Name, data[2], data[3], data[4], data[6], data[7]);

                    //check if MSSV exists in file data
                    if (!data[0].empty()) {
                        sv.Set_MSSV(data[0]); 
                    }

                    //check if Email exists
                    if (!data[5].empty()) {
                        sv.Set_Email(data[5]);
                    }

                    //
                    if (data.size() > 8 && !data[8].empty()) {
                        sv.Set_Lop(data[8]); 
                    }
                    
                    Single_SinhVien.push_back(sv);
                }
            }
            file.close();
            return Single_SinhVien;
        }

        //save data
        static void SaveData(const std::string &DataFolder, const std::vector<Khoa> &database){
            //if folder (named DataFolder) doesnt exist, create it
            if (!fs::exists(DataFolder)){
                fs::create_directory(DataFolder);
            }

            //find where it is
            fs::path AbsolutePath = fs::absolute(DataFolder);
            std::cout << "[DAL] data is stored at: " << AbsolutePath << std::endl;

            //save all files within DataFolder

            //set file count for easier debug if shit went sour
            int FileCount = 0;
            //loop through everything in order of Khoa -> NamHoc -> Lop, yea no SinhVien because fuck 'em thats why
            for (const auto& khoa : database) {
                for (const auto& namHoc : khoa.get_DanhSachNamHoc()) {
                    for (const auto& lop : namHoc.get_DanhSachLop()) {
                        if (lop.Get_SoLuongSV() > 0) {
                            // Build path: Data/101_24CDT1.txt
                            std::string filename = DataFolder + "/" + khoa.get_MaKhoa() + "_" + lop.Get_TenLop() + ".txt";
                            SaveLopToFile(filename, lop);
                            FileCount++;
                        }
                    }
                }
            }

            if (FileCount == 0){
                std::cout << "No data to be saved.";
            }
            else{
                std::cout << "Successfully saved: " << FileCount << " files." << std::endl;
            }
        }

        //save just a single Lop
        static void SaveLopToFile(const std::string &FileName, const Lop &lop){
            std::ofstream file(FileName);
            if (file.is_open()){
                for (const auto &sv : lop.get_DanhSach()){
                    file << sv.to_File() << std::endl;
                }
                file.close();
            }
            else{
                std::cout << "[DAL] Can't write to file named: " << FileName << "." << std::endl;
            }
        }

};