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

struct KhoaLookUp {
    string MaKhoa;
    string TenKhoa;
    string Lop_PostFix; 
};

class QuanLySinhVien{
    //attributes
    private:
        vector<Khoa> DanhSachKhoa;
        string TargetFolder = "data";
    //methods
    public:
        //constructor gaming
        QuanLySinhVien() {
            for (const auto& [makhoa, info] : Khoa_To_Lop_Map) {
                // Pass the single postfix instead of the old array
                Khoa newKhoa(makhoa, info.TenKhoa, info.Lop_PostFix);
                DanhSachKhoa.push_back(newKhoa);
            }
        }

        //the thingy mazingies used for the 11 human souls

        // Them sinh vien moi
        void themSinhVien(){

        }   
        
        // Them sinh vien tu file
        void themSinhVienTuFile(){

        }

        // Sap xep danh sach sinh vien
        void sapXepDanhSach(){

        }

        // Sua thong tin sinh vien
        void suaThongTin(){

        }

        // Cap MSSV (toan truong)
        void capMSSV(){

        }

        // Cap Email (toan truong)
        void capEmail(){

        }

        // Tim kiem sinh vien
        void timKiem(){

        }

        // Xoa sinh vien
        void xoaSinhVien(){

        }

        // Xuat danh sach theo lop
        void xuatTheoLop(){

        }

        // Xuat danh sach toan truong
        void xuatToanTruong(){

        }

        // Xuat file lop (.txt)
        void xuatFileLop(){

        }

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
            int user_choice = -1;
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
                cout << "Nhap lua user_choice: ";
                if (!(cin >> user_choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    user_choice = -1;
                } else cin.ignore();
                switch (user_choice) {
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
                    case 0: system("cls"); cout << "Thoat...\n"; break;
                    default: cout << "Lua user_choice khong hop le!\n"; system("pause"); break;
                }
            } while (user_choice != 0);
        }

        //set color of text
        static void SetColor(int color) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        }

        //MaKhoa, TenKhoa and the according postfix
        inline static const map<string, KhoaLookUp> Khoa_To_Lop_Map = {
        { "101", { "Co khi",              "CDT"   } }, 
        { "102", { "Cong nghe thong tin", "T_DT"  } }, 
        { "104", { "Nhiet - Dien lanh",   "N"     } },
        { "105", { "Dien",                "D"     } },
        { "106", { "Dien tu - Vien thong","DT"    } },
        { "107", { "Hoa",                 "H"     } },
        { "117", { "Moi truong",          "MT"    } },
        { "118", { "Quan li du an",       "QLDA"  } },
        { "121", { "Kien truc",           "KT"    } }
        };

        // In BLL/QuanLySinhVien.h -> private:
        Khoa* FindKhoa(const string& maKhoa) {
            for (auto& k : DanhSachKhoa) {
                if (k.get_MaKhoa() == maKhoa) return &k;
            }
            return nullptr;
        }
};  
