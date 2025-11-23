#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../DTO/Khoa.h"
#include "../DTO/SinhVien.h"
#include "../DAL/DataAccess.h"

using namespace std;

class QuanLySinhVien {
private:
    vector<Khoa> danhSachKhoa;
    string dbFolder = "Data"; // Folder to store txt files

public:
    QuanLySinhVien() {
        // Initialize the Faculty (Khoa) Structure
        // You can add more faculties here if needed
        array<string, 3> prefixes = {"T_DT4", "T_DT5", "T_DT6"}; // Example class prefixes
        Khoa khoaDien("101", "Khoa Dien Tu", prefixes);
        danhSachKhoa.push_back(khoaDien);

        // Load data from DAL and Map it to the Tree Structure
        vector<SinhVien> rawData = DataAccess::LoadAllData(dbFolder);
        MapDataToModel(rawData);
    }

    // Helper: Distributes a flat list of students into the correct Lop inside Khoa
    void MapDataToModel(const vector<SinhVien>& svList) {
        for (const auto& sv : svList) {
            bool found = false;
            for (auto& k : danhSachKhoa) {
                if (k.get_MaKhoa() == sv.Get_MaKhoa()) {
                    for (auto& nh : k.get_DanhSachNamHoc()) {
                        if (nh.get_TenNamHoc() == sv.Get_NamHoc()) {
                            // Try to find class by matching filename pattern or logic
                            // Here we assume we simply add to the correct year/faculty
                            // Since we don't know the exact class name from SV object (it's not stored in SV explicitly in your SV.h),
                            // We might need to iterate all classes in that year to find where the student fits,
                            // OR your SV.h should have 'TenLop'. 
                            // *Assuming* we rely on the logic that SV belongs to one of the classes.
                            // For now, let's try to add to the first matching class or search logic.
                            
                            // IMPROVEMENT: Your SinhVien.h has "Lop" attribute but it's private/unused in constructor?
                            // Let's iterate all classes in this Year and try to add.
                            for (auto& l : nh.get_DanhSachLop()) {
                                // Simple check: If loading from file, we might not know the exact class unless it was stored.
                                // Current workaround: Add to the class that matches specific logic or if 'Lop' attribute was saved.
                                // Since 'Lop' attribute in SV is empty by default, we just add to the first valid class 
                                // or you need to save Class Name in the text file.
                                
                                // FIX for your Context: Assuming you just want to load them into memory:
                                l.Add_SinhVien(sv);
                                found = true;
                                break; // Added to one class (simplified)
                            }
                        }
                        if(found) break;
                    }
                }
                if(found) break;
            }
        }
    }

    void intro() {
        cout << "=== STUDENT MANAGEMENT SYSTEM ===\n";
    }

    void menu() {
        int choice;
        do {
            cout << "\n----------------------------------\n";
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
            cout << "Lua chon: ";
            cin >> choice;
            cin.ignore(); 

            switch (choice) {
                case 1: feature_ThemSinhVien(); break;
                case 2: feature_ThemTuFile(); break;
                case 3: feature_SapXep(); break;
                case 4: feature_SuaSinhVien(); break;
                case 5: feature_CapMSSV(); break;
                case 6: feature_CapEmail(); break;
                case 7: feature_TimKiem(); break;
                case 8: feature_XoaSinhVien(); break;
                case 9: feature_XuatLop(); break;
                case 10: feature_XuatToanTruong(); break;
                case 11: feature_XuatFileLop(); break;
                case 0: 
                    DataAccess::SaveData(dbFolder, danhSachKhoa); // Auto save on exit
                    cout << "Da luu du lieu va thoat.\n"; 
                    break;
                default: cout << "Lua chon khong hop le!\n";
            }
        } while (choice != 0);
    }

    // --- Feature Implementations ---

    // 1. Them Sinh Vien
    void feature_ThemSinhVien() {
        string ho, ten, gt, ns, dc, makhoa, nam;
        cout << "Nhap Ho Lot: "; getline(cin, ho);
        cout << "Nhap Ten: "; getline(cin, ten);
        cout << "Nhap Gioi Tinh: "; getline(cin, gt);
        cout << "Nhap Ngay Sinh: "; getline(cin, ns);
        cout << "Nhap Dia Chi: "; getline(cin, dc);
        cout << "Nhap Ma Khoa (ex: 101): "; getline(cin, makhoa);
        cout << "Nhap Nam Hoc (ex: 2024): "; getline(cin, nam);

        SinhVien sv(ho, ten, gt, ns, dc, makhoa, nam);
        
        // Find class to add
        Lop* l = SelectLop(makhoa, nam);
        if (l) {
            if(l->Add_SinhVien(sv)) cout << "Them thanh cong!\n";
        } else {
            cout << "Khong tim thay lop phu hop cho Khoa/Nam nay.\n";
        }
    }

    // 2. Them Tu File
    void feature_ThemTuFile() {
        string path;
        cout << "Nhap duong dan file: "; getline(cin, path);
        vector<SinhVien> imported = DataAccess::LoadSingleFile(path);
        MapDataToModel(imported);
        cout << "Da import " << imported.size() << " sinh vien.\n";
    }

    // 3. Sap Xep
    void feature_SapXep() {
        Lop* l = SelectLopUI();
        if (l) {
            l->Sort_SinhVien_In_Lop_By_Ten();
            cout << "Da sap xep lop.\n";
        }
    }

    // 5. Cap MSSV
    void feature_CapMSSV() {
        int globalCounter = 0; // Or manage logic to find max current MSSV
        for (auto& k : danhSachKhoa) {
            for (auto& nh : k.get_DanhSachNamHoc()) {
                for (auto& l : nh.get_DanhSachLop()) {
                    // Start count can be adjusted per logic
                    l.Assign_MSSV_To_Lop(globalCounter);
                }
            }
        }
        cout << "Hoan tat cap MSSV toan truong.\n";
    }

    // 6. Cap Email
    void feature_CapEmail() {
         for (auto& k : danhSachKhoa) {
            for (auto& nh : k.get_DanhSachNamHoc()) {
                for (auto& l : nh.get_DanhSachLop()) {
                    l.Assign_Email_To_Lop();
                }
            }
        }
        cout << "Hoan tat cap Email toan truong.\n";
    }

    // 7. Tim Kiem
    void feature_TimKiem() {
        string mssv;
        cout << "Nhap MSSV can tim: "; getline(cin, mssv);
        for (auto& k : danhSachKhoa) {
            for (auto& nh : k.get_DanhSachNamHoc()) {
                for (auto& l : nh.get_DanhSachLop()) {
                    SinhVien* sv = l.Find_SinhVien_By_MSSV(mssv);
                    if (sv) {
                        cout << "Tim thay:\n";
                        sv->Print_SinhVien();
                        return;
                    }
                }
            }
        }
        cout << "Khong tim thay sinh vien.\n";
    }

    // 9. Xuat Danh Sach Theo Lop
    void feature_XuatLop() {
        Lop* l = SelectLopUI();
        if (l) l->Xuat_ds_SV();
    }

    // 10. Xuat Toan Truong
    void feature_XuatToanTruong() {
        for (const auto& k : danhSachKhoa) {
            for (const auto& nh : k.get_DanhSachNamHoc()) {
                for (const auto& l : nh.get_DanhSachLop()) {
                    if (l.Get_SoLuongSV() > 0) l.Xuat_ds_SV();
                }
            }
        }
    }

    // 11. Xuat File Lop
    void feature_XuatFileLop() {
        Lop* l = SelectLopUI();
        if (l) {
            string filename = "Export_" + l->Get_TenLop() + ".txt";
            DataAccess::SaveLopToFile(filename, *l);
        }
    }

    // placeholders for other features...
    void feature_SuaSinhVien() { cout << "Tinh nang dang phat trien (Tuong tu Tim Kiem + Setter)\n"; }
    void feature_XoaSinhVien() { 
        string mssv;
        cout << "Nhap MSSV can xoa: "; getline(cin, mssv);
        // Logic: Iterate loop similar to search, call l.Delete_SinhVien_By_MSSV(mssv)
        for (auto& k : danhSachKhoa) {
            for (auto& nh : k.get_DanhSachNamHoc()) {
                for (auto& l : nh.get_DanhSachLop()) {
                    if (l.Delete_SinhVien_By_MSSV(mssv)){
                        cout << "Da xoa sinh vien " << mssv << endl;
                        return;
                    }
                }
            }
        }
        cout << "Khong tim thay de xoa.\n";
    }

    // Helpers
    Lop* SelectLop(string makhoa, string namhoc) {
        for (auto& k : danhSachKhoa) {
            if (k.get_MaKhoa() == makhoa) {
                for (auto& nh : k.get_DanhSachNamHoc()) {
                    if (nh.get_TenNamHoc() == namhoc) {
                        // User selects a class or we default to the first one available
                        if (!nh.get_DanhSachLop().empty()) {
                            return &nh.get_DanhSachLop()[0]; // Returning first class for simplicity
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    Lop* SelectLopUI() {
        string makhoa, namhoc, tenlop;
        cout << "Nhap Ma Khoa: "; getline(cin, makhoa);
        cout << "Nhap Nam Hoc: "; getline(cin, namhoc);
        // Ideally list classes here
        cout << "Nhap Ten Lop (vd: 24T_DT4): "; getline(cin, tenlop);

         for (auto& k : danhSachKhoa) {
            if (k.get_MaKhoa() == makhoa) {
                for (auto& nh : k.get_DanhSachNamHoc()) {
                    if (nh.get_TenNamHoc() == namhoc) {
                        Lop* l = nh.timLop(tenlop);
                        if (l) return l;
                    }
                }
            }
        }
        cout << "Khong tim thay lop.\n";
        return nullptr;
    }
};