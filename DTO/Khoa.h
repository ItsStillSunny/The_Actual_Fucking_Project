#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <ctime>   
#include "NamHoc.h"
#include "Lop.h"
using namespace std;

class Khoa{
    //attributes
    private:
        string MaKhoa;
        string TenKhoa;
        vector<NamHoc> DanhSachNamHoc;

    //methods
    public:
        //default constructor + actual constructor
        Khoa(){}
        Khoa(string makhoa, string tenkhoa, const array<string, 3>& prefix_of_khoa) : MaKhoa(makhoa), TenKhoa(tenkhoa) {

            //get current year
            time_t t = time(nullptr);
            tm* now = localtime(&t);
            int currentYear = now->tm_year + 1900;
        
            //only have classes from year 2000 -> current year
            for (int earliestYear = 2000; earliestYear <= currentYear; ++earliestYear){
                string yearAsString = to_string(earliestYear);
                NamHoc newYear(yearAsString);

                //2024 -> 24
                string shortYearString = yearAsString.substr(2,2);

                //       24       + T_DT4  = 24T_DT4
                //shortYearString + prefix = TenLop
                for (const string &prefix : prefix_of_khoa){
                    string TenLop = shortYearString + prefix;

                    //create Lop based on those information
                    Lop newLop(TenLop, MaKhoa, yearAsString);
                    newYear.themLop(newLop);
                }
                DanhSachNamHoc.push_back(newYear);
            }
        }
        //getters/setters
        string get_MaKhoa() const {return MaKhoa;}
        string get_TenKhoa() const {return TenKhoa;}

        //write (BLL)
        vector<NamHoc> &get_DanhSachNamHoc() { 
            return DanhSachNamHoc; 
        }

        //read (DAL)
        const vector<NamHoc> &get_DanhSachNamHoc() const { 
            return DanhSachNamHoc; 
        } 
};

