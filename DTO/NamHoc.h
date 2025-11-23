#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Lop.h"

using namespace std;

class NamHoc {
private:
    string TenNamHoc; 
    vector<Lop> DanhSachLop;

public:
    //default constructor + actual constructor
    NamHoc() {}
    NamHoc(string nam) : TenNamHoc(nam) {}

    //Add another Lop
    void themLop(const Lop& lop) {
        DanhSachLop.push_back(lop);
    }

    //getters
    string get_TenNamHoc() const { return TenNamHoc; }
    vector<Lop>& get_DanhSachLop() { return DanhSachLop; }

    //read file
    const vector<Lop>& get_DanhSachLop() const { return DanhSachLop; }

    //search Lop
    Lop* timLop(string tenLop) {
        for (auto& l : DanhSachLop) {
            if (l.Get_TenLop() == tenLop) return &l;
        }
        return nullptr;
    }
};