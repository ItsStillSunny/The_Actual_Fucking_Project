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

class DataAccess {
public:
    // [Feature 2 & Startup] Load all data or single file
    static vector<SinhVien> LoadAllData(const string& folder) {
        vector<SinhVien> allStudents;
        if (!fs::exists(folder) || !fs::is_directory(folder)) {
            try { fs::create_directory(folder); } catch (...) {}
            return allStudents;
        }

        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.path().extension() == ".txt") {
                vector<SinhVien> fileStudents = LoadSingleFile(entry.path().string());
                allStudents.insert(allStudents.end(), fileStudents.begin(), fileStudents.end());
            }
        }
        return allStudents;
    }

    // Made public for [Feature 2] "Them sinh vien tu file"
    static vector<SinhVien> LoadSingleFile(const string& filePath) {
        vector<SinhVien> students;
        ifstream file(filePath);
        if (!file.is_open()) return students;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            vector<string> data;

            while (getline(ss, segment, '|')) data.push_back(segment);

            if (data.size() >= 8) {
                // Parse Name logic...
                string fullName = data[1];
                string hoLot = "", ten = "";
                size_t lastSpace = fullName.find_last_of(" ");
                if (lastSpace != string::npos) {
                    hoLot = fullName.substr(0, lastSpace);
                    ten = fullName.substr(lastSpace + 1);
                } else { ten = fullName; }

                SinhVien sv(hoLot, ten, data[2], data[3], data[4], data[6], data[7]);
                sv.Set_MSSV(data[0]);
                sv.Set_Email(data[5]);
                students.push_back(sv);
            }
        }
        file.close();
        return students;
    }

    //save all data
    static void SaveData(const string& folder, const vector<Khoa>& database) {
        if (!fs::exists(folder)) fs::create_directory(folder);

        for (const auto& khoa : database) {
            for (const auto& namHoc : khoa.get_DanhSachNamHoc()) {
                for (const auto& lop : namHoc.get_DanhSachLop()) {
                    if (lop.Get_SoLuongSV() > 0) {
                        string filename = folder + "/" + khoa.get_MaKhoa() + "_" + lop.Get_TenLop() + ".txt";
                        SaveLopToFile(filename, lop);
                    }
                }
            }
        }
    }

    //save just a singular class
    static void SaveLopToFile(const string& filename, const Lop& lop) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& sv : lop.getDanhSach()) {
                file << sv.toFile() << endl;
            }
            file.close();
            cout << " -> Da luu file: " << filename << endl;
        } else {
            cout << " -> Loi: Khong the ghi file " << filename << endl;
        }
    }
};