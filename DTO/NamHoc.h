#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Lop.h"


class NamHoc {
private:
    std::string TenNamHoc; 
    std::vector<Lop> DanhSachLop;

public:
    //default constructor + actual constructor
    NamHoc() {}
    NamHoc(std::string nam) : TenNamHoc(nam) {}

    //Add another Lop
    void themLop(const Lop& lop) {
        DanhSachLop.push_back(lop);
    }

    //getters
    std::string get_TenNamHoc() const { return TenNamHoc; }
    std::vector<Lop>& get_DanhSachLop() { return DanhSachLop; }

    //read file
    const std::vector<Lop>& get_DanhSachLop() const { return DanhSachLop; }

    //search Lop
    Lop* timLop(std::string tenLop) {
        for (auto& l : DanhSachLop) {
            if (l.Get_TenLop() == tenLop) return &l;
        }
        return nullptr;
    }
};