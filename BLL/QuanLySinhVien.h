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
#include <ctime>
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
            ClearScreen();

            string namhoc, makhoa;
            Khoa* k = nullptr;
            NamHoc* n = nullptr;

            while(true){
                cout << "\t\t\t\t\t--- THEM SINH VIEN MOI ---\n\n";
                cout << "\t\t Nhap nam hoc (ex: " << currentYear << "): \n";
                cout << "Valid range is from 1995 to " << currentYear << endl;
                getline(cin, namhoc);

                //is it valid?
                //yes
                if (YearValidator(namhoc)){
                    break;
                }
                //no
                else{
                    SetColor(12);
                    cout << "\t\t [!] Nam hoc khong hop le! Vui long nhap lai.\n";
                    
                    //wait (value/1000) seconds so user can read the error message
                    Sleep(5000); 

                    //clear the errors + user error 
                    ClearLines(4);
                }
            }

            while(true){
                cout << "\t\t Nhap ma khoa (ex: 101): ";
                cout << "Valid values: 101, 102, 104, 105, 106, 107, 117, 118, 121. \n";
                getline(cin, makhoa);

                k = FindKhoa(makhoa);
                if (k != nullptr) {
                    //theres a valid khoa
                    //find namhoc next
                    n = k->get_NamHoc(namhoc);
                    if (n != nullptr){
                        //theres a valid namhoc
                        //this ngga not classless anymore, yay
                        break;
                        //success case
                    }
                    //got Khoa, no Lop
                    else{
                        SetColor(12);
                        cout << "\t\t [LOI] Nam hoc khong ton tai trong khoa nay!\n";
                        Sleep(5000);
                        ClearLines(4); 
                        continue;
                    }
                //neither found
                }
                SetColor(12); 
                cout << "\t\t [LOI] Khong tim thay khoa voi ma: " << makhoa << "\n";
                Sleep(1000); 
                ClearLines(4);
            }

            string ho, ten, gioitinh, ngaysinh, diachi;
            cout << "Ho va ten lot: "; getline(cin, ho);
            cout << "Ten: "; getline(cin, ten);
            cout << "Gioi tinh: "; getline(cin, gioitinh);
            cout << "Ngay sinh: "; getline(cin, ngaysinh);
            cout << "Dia chi: "; getline(cin, diachi);

            SinhVien sv(ho, ten, gioitinh, ngaysinh, diachi, "", "");

            bool added = false;

            for (auto& lop : n->get_DanhSachLop()) {
            // lop.Add_SinhVien returns TRUE if added, FALSE if full (MAX_SV)
            if (lop.Add_SinhVien(sv)) {
                    SetColor(10); // Green
                    cout << "\n\t\t [THANH CONG] Sinh vien da duoc xep vao lop: " << lop.Get_TenLop() << "\n";
                    added = true;
                    break; // Stop checking other classes
                }
            }

            if (!added) {
                SetColor(12);
                cout << "\n\t\t [THAT BAI] Tat ca cac lop trong khoa nay deu da day!\n";
            }
            Pause();    
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

        //get current year
            time_t t = time(nullptr);
            tm* now = localtime(&t);
            int currentYear = now->tm_year + 1900;

        //showing intro
        void intro() {
            ClearScreen();
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
                ClearScreen();
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
                    case 0: ClearScreen(); cout << "Thoat...\n"; break;
                    default: cout << "Lua user_choice khong hop le!\n"; Pause(); break;
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

        //
        Khoa* FindKhoa(const string& maKhoa) {
            for (auto& k : DanhSachKhoa) {
                if (k.get_MaKhoa() == maKhoa) return &k;
            }
            return nullptr;
        }

        //check for vampires/ time travellers
        bool YearValidator(string Year){
            int YearAsInt = stoi(Year);

            if (YearAsInt > 1995 || YearAsInt < currentYear){
                return true;
            }
            else{
                return false;
            }
        }

        //clear "lineCount" lines above and move up the same number of lines deleted
        void ClearLines(int lineCount) {
            for (int i = 0; i < lineCount; ++i) {
                cout << "\033[A";  // Move cursor UP one line
                cout << "\033[2K"; // Erase the entire line
            }
        }

        //clear the screen
        void ClearScreen(){
            system("cls");
        }

        //pause the screen
        void Pause(){
            system("pause");
        }


};  
