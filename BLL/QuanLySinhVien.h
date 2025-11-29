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

        //the thingies mazingies used for the 11 human souls

        // Them sinh vien moi
        void themSinhVien(){
            ClearScreen();

            string namhoc, makhoa;
            Khoa* k = nullptr;
            NamHoc* n = nullptr;

            while(true){
                cout << "\t\t--- THEM SINH VIEN MOI ---\n";
                cout << "Nhap nam hoc (ex: " << currentYear << "): \n";
                cout << "Valid range is from 1995 to " << currentYear << endl;
                getline(cin, namhoc);

                //is it valid?
                //yes
                if (YearValidator(namhoc) == true){
                    break;
                }
                //no
                else{
                    SetColor(12);
                    cout << "\t\t [!] Nam hoc khong hop le! Vui long nhap lai.\n";
                    
                    //wait (value/1000) seconds so user can read the error message
                    Sleep(1500); 

                    //clear the errors + user error 
                    ClearLines(5);

                    //reset color
                    SetColor(7); 
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
                    n = k->Get_Or_Create_NamHoc(namhoc);
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
                        Sleep(1500);
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

            //standard stuff
            //yea i dont care anymore about dob, you can be a vampire or time traveller, whatever float your boat
            //EDIT: add dob protection against vampire and/or time traveller, fuck them, deadline 25/11/2025
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
            ClearScreen();
            cout << "\t\t\t\t\t\t--- THEM SINH VIEN TU FILE ---\n\n";

            string filePath;
            cout << "\t\t Nhap duong dan file: (absolute path | ex: D:/something/abc)";
            getline(cin, filePath);

            //DAL importing work
            vector<SinhVien> importedList = DataAccess::LoadSingleData(filePath);

            if (importedList.empty()){
                cout << "File doesn't exist/ is empty";
                Pause();
                return;
            }

            cout << "\n\t\t Dang xu ly: " << importedList.size() << " sinh vien.";

            int failed = 0;
            int success = 0;

            for (const auto &sv : importedList){
                // Find Faculty
                Khoa* k = FindKhoa(sv.Get_MaKhoa());
                if (!k) { failed++; continue; }

                // Find Year
                NamHoc* n = k->Get_Or_Create_NamHoc(sv.Get_NamHoc());
                if (!n) { failed++; continue; }

                // Auto-Assign Overflow (one class full, onto the next one)
                bool added = false;
                for (auto& lop : n->get_DanhSachLop()) {
                    if (lop.Add_SinhVien(sv)) {
                        success++;
                        added = true;
                        break; 
                    }
                }
                    //unable to add
                    if (!added) failed++;
            }

            cout << "Successfully added: " << success << " sinh vien.";
            if (failed > 0){
                cout << "Khong the them vao: " << failed << " sinh vien (loi file/loi du lieu)";
            }
            Pause();
        }

        // Sap xep danh sach sinh vien
        void sapXepDanhSach(){
            ClearScreen();
            cout << "\t\t\t\t\t\t--- SAP XEP SINH VIEN ---\n\n";

            Lop *lop = Select_Lop_UI();

            //invalid choice/ cancelled choice
            if (!lop){
                Pause();
                return;
            }

            //empty
            if (lop->Get_SoLuongSV() == 0){
                cout << "Lop khong co sinh vien." << endl;
            }
            else{
                lop->Sort_SinhVien_In_Lop_By_Ten();

                cout << "Da sap xep lop theo ho va ten. " << endl;

                lop->Xuat_ds_SV();
            }
            Pause();
        }

        // Sua thong tin sinh vien
        void suaThongTin(){
            system("cls");
            cout << "\t\t\t\t\t\t--- SUA THONG TIN SINH VIEN (CHUA CAP MSSV) ---\n\n";

            string tenLop, hoLot, ten;
                    
            cout << "\t\t Nhap Ten Lop cua sinh vien: ";
            getline(cin, tenLop);
            
            cout << "\t\t Nhap Ho Lot cua sinh vien: ";
            getline(cin, hoLot);
            
            cout << "\t\t Nhap Ten cua sinh vien: ";
            getline(cin, ten);

            //find the Lop that SinhVien is in
            Lop* targetLop = nullptr;
            
            // Iterate through all Faculties (Khoa)
            for (auto& khoa : DanhSachKhoa) {
                // Iterate through all Years (NamHoc) within that Faculty
                for (auto& nam : khoa.get_DanhSachNamHoc()) {
                    // Try to find the Lop in this Year
                    Lop* found = nam.timLop(tenLop);
                    if (found != nullptr) {
                        targetLop = found;
                        break;
                    }
                }
                if (targetLop != nullptr) break;
            }

            //if Lop wasnt found
            if (targetLop == nullptr){
                cout << "Khong tim thay lop chua sinh vien." << endl;
                Pause();
                return;
            }

            //Found Lop, start finding by SinhVien in that Lop now
            SinhVien* sv = targetLop->Select_SinhVien_By_Name(hoLot, ten);
            
            //cant find
            if (sv == nullptr){
                cout << "Khong tim thay sinh vien." << endl;
                Pause();
                return;
            }
            //can find, but already has MSSV
            else if (sv != nullptr && sv->Has_MSSV() == true){
                cout << "Tim thay sinh vien, sinh vien da co MSSV, khong the chinh sua.";
                Pause();
                return;
            }
            //fucking finally
            else{
                while (true){
                    ClearScreen();
                    cout << "\t\t\t--- THONG TIN SINH VIEN ---\n";
                    //print the SinhVien's information like this:
                    //-------------------------------
                    //blah blah blah
                    //-------------------------------
                    cout << string(60, '-') << endl;
                    sv->Print_SinhVien(); 
                    cout << string(60, '-') << endl;

                    //the list of options to change
                    cout << " 1. Sua Ho Lot\n";
                    cout << " 2. Sua Ten\n";
                    cout << " 3. Sua Gioi Tinh\n";
                    cout << " 4. Sua Ngay Sinh\n";
                    cout << " 5. Sua Dia Chi\n";
                    cout << " 0. Hoan tat\n";
                    cout << string(60, '-') << endl;
                    cout << " Nhap lua chon: ";
                    
                    int choice;
                    if (!(cin >> choice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    cin.ignore(); // Clear buffer

                    if (choice == 0) {
                        break;
                    }

                    string newVal;
                    cout << " Nhap gia tri moi: ";
                    getline(cin, newVal);

                    switch (choice) {
                        case 1: sv->Set_HoLot(newVal); break;
                        case 2: sv->Set_Ten(newVal); break;
                        case 3: sv->Set_GioiTinh(newVal); break;
                        case 4: sv->Set_NgaySinh(newVal); break;
                        case 5: sv->Set_DiaChi(newVal); break;
                        default: 
                            cout << " Lua chon khong hop le.\n"; 
                            Sleep(1000); 
                            continue;
                    }
                    
                    SetColor(10); // Green
                    cout << "\t\t [CAP NHAT THANH CONG]\n";
                    Sleep(1000);
                    SetColor(7); // Reset color
                    }
            }
        }

        // Cap MSSV (toan truong)
        void capMSSV(){
            ClearScreen();
            cout << "\t\t\t\t\t\t--- CAP MSSV TOAN TRUONG ---\n\n";

            for (auto &khoa : DanhSachKhoa){
                string currentMaKhoa = khoa.get_MaKhoa();

                for (auto &nam : khoa.get_DanhSachNamHoc()){
                    int StartCount = 0;
                    for (auto &lop : nam.get_DanhSachLop()){
                        StartCount += lop.Count_Assigned_SinhVien();
                    }

                    for (auto &lop : nam.get_DanhSachLop()){
                        //skip empty classes
                        if (lop.Get_SoLuongSV() == 0){
                            continue;
                        }

                        bool success_indicator = lop.Assign_MSSV_To_Lop(StartCount);

                        if (!success_indicator){
                            SetColor(12);
                            cout << "Lop: " << lop.Get_TenLop() << " chua duoc sap xep, khong the cap MSSV toan truong.";
                            Pause();
                            return;
                        }
                    }
                }
            }
            SetColor(10);
            cout << "Da cap MSSV cho toan truong thanh cong.";
            Pause();
        }

        // Cap Email (toan truong)
        void capEmail(){
            ClearScreen();
            cout << "\t\t\t\t\t\t--- CAP MSSV TOAN TRUONG ---\n\n";

            for (auto &khoa : DanhSachKhoa){
                for (auto &nam : khoa.get_DanhSachNamHoc()){


                    for (auto &lop : nam.get_DanhSachLop()){
                        //skip empty classes
                        if (lop.Get_SoLuongSV() == 0){
                            continue;
                        }

                        bool success_indicator = lop.Assign_Email_To_Lop();

                        if (!success_indicator){
                            SetColor(12);
                            cout << "Lop: " << lop.Get_TenLop << " chua duoc cap MSSV, khong the cap Email toan truong.";
                            Pause();
                            return;
                        }
                    }
                }
            }
        }
            SetColor(10);
            cout << "Da cap Email cho toan truong thanh cong.";
            Pause();
        }

        // Tim kiem sinh vien
        void timKiem(){
            system("cls");
            cout << "\t\t\t\t\t\t--- Tim kiem sinh vien dua tren MSSV ---\n\n";
            
            string MSSV_Cua_SinhVien_Can_Tim;
            cout << "Nhap MSSV cua sinh vien can tim: ";
            getline(cin, MSSV_Cua_SinhVien_Can_Tim);

            SinhVien *sv = FindSV(MSSV_Cua_SinhVien_Can_Tim);
            
            if (sv == nullptr){
                cout << "Khong the tim thay sinh vien dua tren MSSV da nhap." << endl;
            }
            else{
                cout << string(60, '-') << endl;
                sv->Print_SinhVien(); 
                cout << string(60, '-') << endl;
            }
        }

        // Xoa sinh vien
        void xoaSinhVien(){
            system("cls");
            cout << "\t\t\t\t\t\t--- Xoa sinh vien dua tren MSSV ---\n\n";

            string MSSV_Cua_SinhVien_Can_Xoa;
            cout << "Nhap MSSV cua sinh vien can xoa. ";
            getline(cin, MSSV_Cua_SinhVien_Can_Xoa);

            bool Found = false;

            for (auto &khoa : DanhSachKhoa) {
                for (auto &nam : khoa.get_DanhSachNamHoc()) {
                    for (auto &lop : nam.get_DanhSachLop()) {
                        SinhVien *sv = lop.Find_SinhVien_By_MSSV(MSSV_Cua_SinhVien_Can_Xoa);

                        if (sv != nullptr){
                            Found = true;
                            cout << "Da tim thay sinh vien: " << endl;
                            cout << string(60, '-') << endl;
                            sv->Print_SinhVien(); 
                            cout << string(60, '-') << endl;

                            //deleting time
                            cout << "Ban co chac chan ban muon xoa sinh vien nay? (Y de dong y, an nut bat ky de huy)" << endl;
                            char confirm;
                            if (!(cin >> confirm)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                confirm = 'n';
                            } else {
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
                            }

                            //confirmation
                            if (confirm == "Y" || confirm == "y"){
                                //it went well
                                if (lop.Delete_SinhVien_By_MSSV(MSSV_Cua_SinhVien_Can_Xoa)){
                                    cout << "Xoa thanh cong. " << endl;
                                }
                                //it went wrong
                                else{
                                    cout << "Da co loi xay ra. " << endl;
                                }
                            }
                            //backed out
                            else{
                                cout << "Xoa that bai. (Bi huy boi nguoi dung) " << endl;
                            }
                            Pause();
                            return;
                        }
                    }
                }
            }
            if (!Found)
            {
                cout << "Khong tim thay sinh vien voi MSSV da nhap. ";
                Pause();
            }
        }

        // Xuat danh sach theo lop
        void xuatTheoLop(){
            system("cls");
            cout << "\t\t\t\t\t\t--- Xuat danh sach lop ---\n\n";

            Lop *lop = Select_Lop_UI();
            
            if (lop != nullptr){
                if (lop->Get_SoLuongSV() == 0){
                    cout << "LOI: Lop rong." << endl;
                }
                else{
                    lop->Xuat_ds_SV();
                }
            }
            //couldnt find specified Lop
            else{
                cout << "Khong tim thay lop. " << endl;
            }

            Pause();
        }

        // Xuat danh sach toan truong
        void xuatToanTruong(){
            system("cls");
            cout << "\t\t\t\t\t\t--- Xuat danh sach sinh vien toan truong ---\n\n";

            bool has_SinhVien = false;

            for (auto &khoa : DanhSachKhoa){
                for (auto &nam : khoa.get_DanhSachNamHoc()){
                    for (auto &lop: nam.get_DanhSachLop()){

                        if (lop.Get_SoLuongSV() > 0){
                            has_SinhVien == true;
                            lop.Xuat_ds_SV();

                            cout << "\n" << string(107, '=') << "\n";
                        }
                    }
                }
            }

            if (!has_SinhVien) {
                cout << "\n\t\t [THONG BAO] He thong hien tai chua co du lieu sinh vien.\n";
            }
            Pause();
            }

        // Xuat file lop (.txt)
        void xuatFileLop(){
            ClearScreen();
            cout << "\t\t\t\t\t\t--- XUAT DU LIEU LOP RA FILE ---\n\n";

            Lop *lop = Select_Lop_UI();

            if (lop == nullptr){
                cout << "Lop khong ton tai. " << endl;
                Pause();
                return;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (lop->Get_SoLuongSV() == 0){
                cout << "Lop rong, khong the xuat file. " << endl;
                Pause();
                return;
            }

            string fileName = lop->Get_TenLop() + ".txt";
            string fullPath = TargetFolder + "/" + fileName;

            //ensure the folder exist
            if (!fs::exists(TargetFolder)) {
                fs::create_directory(TargetFolder);
            }

            // 4. Save using DataAccess
            cout << "\n\t\t Dang xuat file: " << fullPath << " ...\n";
            DataAccess::SaveLopToFile(fullPath, *lop);

            cout << "\t\t [THANH CONG] Da xuat file tai: " << fullPath << "\n";
            
            Pause();
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

        //find Khoa based on MaKhoa
        Khoa *FindKhoa(const string &maKhoa) {
            for (auto& khoa : DanhSachKhoa) {
                if (khoa.get_MaKhoa() == maKhoa){
                    return &khoa;
                } 
            }
            //failure: no Khoa found
            return nullptr;
        }

        //find SinhVien based on MSSV
        SinhVien *FindSV(const string &MSSV){
            //looper whooper 
            for (auto &khoa : DanhSachKhoa){
                for (auto &nam : khoa.get_DanhSachNamHoc()){

                    for (auto &lop : nam.get_DanhSachLop()){
                        //oh my, oh my, I have found you, nigga
                        SinhVien *FoundSV = lop.Find_SinhVien_By_MSSV(MSSV);
                        return FoundSV;
                        }

                    }
                }
            //failure: cant find SinhVien based on MSSV
            return nullptr;
        }


        //check for vampires/ time travellers in choosing their year of admission
        bool YearValidator(string Year){
            try{
                //stoi require size_t, dont ask
                size_t pos;
                int YearAsInt = stoi(Year, &pos);

                if (pos < Year.length()) {
                    return false; 
                }

                if (YearAsInt > 1995 && YearAsInt < currentYear){
                    return true;
                }
                else{
                    return false;
                }
            }
            //if it cant convert (aka there exist letter in the number, throw an error)
            catch (...){
                return false;
            }
        }

        //ask user for NamHoc -> Khoa -> Lop
        Lop *Select_Lop_UI() {
            SetColor(14);
            string namhoc, makhoa;

            // 1. Get Context
            cout << "\t\t Nhap nam hoc (ex: 2024): ";
            getline(cin, namhoc);
            cout << "\t\t Nhap ma khoa (ex: 101): ";
            getline(cin, makhoa);

            // 2. Find the Faculty & Year objects
            Khoa* k = FindKhoa(makhoa);
            if (!k) { /* Error handling... */ return nullptr; }
            
            NamHoc* n = k->timNamHoc(namhoc);
            if (!n) { /* Error handling... */ return nullptr; }

            // 3. Show the list of classes
            SetColor(11);
            cout << "\n\t\t\t--- DANH SACH LOP ---\n";
            auto& listLop = n->get_DanhSachLop();
            
            for (size_t i = 0; i < listLop.size(); ++i) {
                // Print: "1. 24CDT1 (45 sv)"
                cout << "\t\t\t" << (i + 1) << ". " << listLop[i].Get_TenLop() << "\n";
            }

            // 4. Return the selection
            int choice;
            cout << "\t\t\tChon lop (1-" << listLop.size() << "): ";
            cin >> choice;
            // ... validation ...
            
            return &listLop[choice - 1];
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

        //pause the program until any user input is detected
        void Pause(){
            system("pause");
        }
};  
