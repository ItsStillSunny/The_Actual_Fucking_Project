#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "NamHoc.h"
#include "Lop.h"

class Khoa{
    //attributes
    private:
        std::string MaKhoa;
        std::string TenKhoa;
        std::string Lop_PostFix;
        std::vector<NamHoc> DanhSachNamHoc;

    //methods
    public:
        //default constructor + actual constructor
        Khoa(){}
        Khoa(std::string makhoa, std::string tenkhoa, std::string postfix) : MaKhoa(makhoa), TenKhoa(tenkhoa) , Lop_PostFix(postfix) {}

        //try to find NamHoc, create if not found
        NamHoc* Get_Or_Create_NamHoc(std::string namHocStr) {
            //try to find
            for (auto& nh : DanhSachNamHoc) {
                if (nh.get_TenNamHoc() == namHocStr) {
                    return &nh;
                }
            }

            //cant find, create a new one
            NamHoc newYear(namHocStr);

            //generate the 5 default classes (5 years)
            if (namHocStr.length() >= 4) {
                std::string shortYearString = namHocStr.substr(2, 2); 
                //loop by 5 years
                for (int i = 1; i <= 5; i++) {
                    //24 + T_DT + 1 = 24T_DT1
                    std::string TenLop = shortYearString + Lop_PostFix + to_string(i);
                    Lop newLop(TenLop, MaKhoa, namHocStr);
                    newYear.themLop(newLop);
                }
            }

            DanhSachNamHoc.push_back(newYear);
            
            //newly added year is here
            return &DanhSachNamHoc.back();
        }


        //getters/setters
        std::string get_MaKhoa() const {return MaKhoa;}
        std::string get_TenKhoa() const {return TenKhoa;}
        


        //write (BLL)
        std::vector<NamHoc> &get_DanhSachNamHoc() { 
            return DanhSachNamHoc; 
        }

        //read (DAL)
        const std::vector<NamHoc> &get_DanhSachNamHoc() const { 
            return DanhSachNamHoc; 
        } 

        //search NamHoc
        NamHoc* get_NamHoc(std::string nam) {
            for (auto& nh : DanhSachNamHoc) {
                if (nh.get_TenNamHoc() == nam) return &nh;
            }
            return nullptr;
        }
};

