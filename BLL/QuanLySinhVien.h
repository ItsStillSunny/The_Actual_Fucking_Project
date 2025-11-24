#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <map>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <windows.h>
#include "../DTO/Khoa.h"
#include "../DTO/SinhVien.h"
#include "../DAL/DataAccess.h"

using namespace std;

class QuanLySinhVien{
    //attributes
    private:
        vector<Khoa> DanhSachKhoa;
        string TargetFolder = "data";
    //methods
    public:
        //default constructor + real constructor
        QuanLySinhVien() {}
        

        //mill
        //showing intro
        void intro() {
            system("cls");
            cout << "============================================================\n";
            cout << "         CHAO MUNG DEN VOI HE THONG QUAN LY SINH VIEN \n";
            cout << "                 TRUONG DAI HOC ABC - CONSOLE APP\n";
            cout << "============================================================\n";
            cout << "  Tac gia: Huynh Hieu Nghia 24T_DT3 & Mai Nguyen Dat Nguyen 24T_DT4\n";
            cout << "\t\t Mo ta : Quan ly sinh vien \n";
            cout << "============================================================\n";
            cout << " Bam Enter de tiep tuc...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        //showing menu
        void menu() {
            int chon = -1;
            do {
                system("cls");
                cout << "\n===============================================\n";
                cout << "    CHUONG TRINH QUAN LY SINH VIEN\n";
                cout << "===============================================\n";
                cout << " 1 | Them sinh vien moi\n";
                cout << " 2 | Them sinh vien tu file\n";
                cout << " 3 | Sap xep danh sach sinh vien (lop)\n";
                cout << " 4 | Sua thong tin sinh vien\n";
                cout << " 5 | Cap MSSV (toan truong)\n";
                cout << " 6 | Cap Email (toan truong)\n";
                cout << " 7 | Tim kiem sinh vien\n";
                cout << " 8 | Xoa sinh vien\n";
                cout << " 9 | Xuat danh sach theo lop\n";
                cout << "10 | Xuat danh sach toan truong\n";
                cout << "11 | Xuat file lop (.txt)\n";
                cout << " 0 | Thoat\n";
                cout << "===============================================\n";
                cout << "Nhap lua chon: ";
                if (!(cin >> chon)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    chon = -1;
                } else cin.ignore();
                switch (chon) {
                    case 1: themSinhVien(); break;
                    case 2: themSinhVienTuFile(); break;
                    case 3: sapXepDanhSach(); break;
                    case 4: suaThongTin(); break;
                    case 5: capMSSV(); break;
                    case 6: capEmail(); break;
                    case 7: timKiem(); break;
                    case 8: xoaSinhVien(); break;
                    case 9: xuatTheoLop(); break;
                    case 10: xuatToanTruong(); break;
                    case 11: xuatFileLop(); break;
                    case 0: cout << "Thoat...\n"; break;
                    default: cout << "Lua chon khong hop le!\n"; system("pause"); break;
                }
            } while (chon != 0);
        }
};  
