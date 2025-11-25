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

#include "SinhVien.h"
using namespace std;

class Lop{
    //attributes
    private:
        string TenLop;
        string MaKhoa;
        string NamHoc;
        vector<SinhVien> ds_SV; 
        //get current year
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        int currentYear = now->tm_year + 1900;

    
    //methods
    public:
        //default constructor + actual constructor
        Lop() {}
        Lop(string tenlop, string makhoa, string namhoc) : TenLop(tenlop), MaKhoa(makhoa), NamHoc(namhoc) {}

        //set the maximum number of SinhVIen that can be in a singular Lop
        static const int MAX_SV = 50;

        //Assign MSSV to a Lop ONLY IF all SinhVien is sorted
        bool Assign_MSSV_To_Lop(int &StartCount){
            for (const SinhVien &sv: ds_SV){
                //failure
                if (sv.Is_Sorted() == false)
                {   
                    cout << "Lop chua duoc sap xep, chua the cap MSSV.";
                    return false;
                }
            }

            for (SinhVien &sv : ds_SV){
                //skip those who already have MSSV
                if (sv.Has_MSSV() == true){
                    continue;
                }
                else{
                    StartCount++;

                    string MaKhoa = sv.Get_MaKhoa();
                    string NamHoc = sv.Get_NamHoc();
                    //only get the last 2 digts (2 digits starting from the 2nd digit) | ex: 2024 -> 24
                    string NamHoc_Formatted = NamHoc.substr(2,2);

                    //edit it so it look like XXXX
                    char FormattedCount[5];
                    snprintf(FormattedCount, sizeof(FormattedCount), "%04d", StartCount);

                    //"102" + "24" + "0152"
                    string NewMSSV = MaKhoa + NamHoc_Formatted + FormattedCount;
                    sv.Set_MSSV(NewMSSV);
                }         
            }
            return true;
        }

        //Assign Email to a Lop ONLY IF all SinhVien have MSSV
        bool Assign_Email_To_Lop(){
            for (const SinhVien &sv: ds_SV){
                //failure
                if (sv.Has_MSSV() == false)
                {   
                    cout << "Lop chua duoc cap MSSV, chua the cap Email.";
                    return false;
                }
            }

            for (SinhVien &sv : ds_SV){
                //skip those who already have Email
                if (sv.Has_Email() == true){
                    continue;
                }
                else{
                    string MSSV = sv.Get_MSSV();
                    string NamHoc = sv.Get_NamHoc();

                    int NamHoc_As_int = stoi(NamHoc);

                    //Email = MSSV + "@sv?.dut.udn.vn"

                    if (NamHoc_As_int == currentYear){
                        string NewEmail = MSSV + "@sv1.dut.udn.vn";
                        sv.Set_Email(NewEmail);
                    }
                    else if (NamHoc_As_int == currentYear-1){
                        string NewEmail = MSSV + "@sv2.dut.udn.vn";
                        sv.Set_Email(NewEmail);
                    }
                    else if (NamHoc_As_int == currentYear-2){
                        string NewEmail = MSSV + "@sv3.dut.udn.vn";
                        sv.Set_Email(NewEmail);
                    }
                    else if (NamHoc_As_int == currentYear-3){
                        string NewEmail = MSSV + "@sv4.dut.udn.vn";
                        sv.Set_Email(NewEmail);
                    }
                    else if (NamHoc_As_int == currentYear-4){
                        string NewEmail = MSSV + "@sv5.dut.udn.vn";
                        sv.Set_Email(NewEmail);
                    }
                }
            }
            return true;
        }

        //add + delete functions
        
        bool Add_SinhVien(const SinhVien &sv){
            if (ds_SV.size() < MAX_SV){
                //make a copy and set MaKhoa + NamHoc again for safety reason
                SinhVien newSV = sv;
                newSV.Set_MaKhoa(MaKhoa);
                newSV.Set_NamHoc(NamHoc);
                ds_SV.push_back(newSV);
                return true;
            }
            else{
                cout << "Max sinh vien limit reached, unable to add more.";
                return false;
            }

        }

        bool Delete_SinhVien_By_MSSV(const string& mssv) {
            
            //find where SinhVien is
            auto sinhvien = std::find_if(ds_SV.begin(), ds_SV.end(), [&](const SinhVien& tempsv) 
            {
                return tempsv.Get_MSSV() == mssv;
            });

            //check if found, then delete
            if (sinhvien != ds_SV.end()) {
                ds_SV.erase(sinhvien);
                return true; 
            }

            //not found
            return false; 
        }

        //finders by name & by MSSV

        //find SinhVien by name
        SinhVien *Select_SinhVien_By_Name(const string &holot, const string &ten){
            for (SinhVien& sv : ds_SV) {
                if (sv.Get_HoLot() == holot && sv.Get_Ten() == ten) {
                    return &sv; 
                }
            }
        return nullptr;
        }
        

        //find the SinhVien based on MSSV
        SinhVien* Find_SinhVien_By_MSSV(const string& mssv) {
            //loop to find
            for (SinhVien& sv : ds_SV) {
                if (sv.Get_MSSV() == mssv) {
                    return &sv; 
                }
            }
            //not found
            return nullptr; 
        }

        //sort all SinhVien within a lop by alphabetic order
        void Sort_SinhVien_In_Lop_By_Ten(){
            sort(ds_SV.begin(), ds_SV.end(), [](const SinhVien &a, const SinhVien &b) {
                if (a.Get_Ten() != b.Get_Ten())
                    return a.Get_Ten() < b.Get_Ten();
                
                return a.Get_HoLot() < b.Get_HoLot();
            });
            
            for (SinhVien& sv : ds_SV) {
                sv.Set_checkSort(true);
            }
        }


        //show all SinhVien within a Lop
        void Xuat_ds_SV() const{
            cout << ("\n\n\t\t\t\t\t\tTRUONG DAI HOC BACH KHOA - DAI HOC DA NANG\n");
            string MaKhoa = Get_MaKhoa();
            string TenLop = Get_TenLop();
            string NamHoc = Get_NamHoc();
            cout << "\n\n\t\t\t\t\t\tKHOA: " << MaKhoa << "\n";
            cout << "\n\n\t\t\t\t\t\tDANH SACH LOP: " << MaKhoa << " - " << "Khoa hoc " << NamHoc << "\n";

            cout << left;
            cout << setw(5) << "STT";
            cout << setw(12) << "MSSV";
            cout << setw(20) << "Ho Lot";
            cout << setw(10) << "Ten";
            cout << setw(10) << "Gioi Tinh";
            cout << setw(15) << "Ngay Sinh";
            cout << setw(35) << "Email" << endl;
            cout << string(107, '-') << endl;

            for (const SinhVien &sv: ds_SV){ 
                sv.Print_SinhVien();
            }
        }

        //How many SinhVien got MaKhoa?
        int Count_Assigned_SinhVien(const string& maKhoa) {
        int count = 0;
        for (const SinhVien& sv : ds_SV) {
            if (sv.Get_MaKhoa() == maKhoa && sv.Has_MSSV()) {
                count++;
            }
        }
        return count;
    }


    //getters
    int Get_SoLuongSV() const {return ds_SV.size();}
    string Get_MaKhoa() const {return MaKhoa;}
    string Get_TenLop() const {return TenLop;}
    string Get_NamHoc() const {return NamHoc;}


    //setters
    void Set_TenLop(const string& ten) { TenLop = ten; }
    void Set_MaKhoa(const string& ten) { MaKhoa = ten; }
    void Set_NamHoc(const string& nam) { NamHoc = nam; }

    //file handling
    size_t Add_DanhSach_Tu_File(const vector<SinhVien>& list) {
    //added is a number representating the number of sinh vien added in this process (ex: da them thanh cong 45 sinh vien something something)
    size_t added = 0;
    for (const auto& sv : list) {
        //stop if the size is larger than MAX_SV
        if (ds_SV.size() >= MAX_SV) 
        {
            break;
        }  
        else{
            Add_SinhVien(sv); 
            added++;
        }
    }
    return added;
    }

    //read
    const vector<SinhVien>& get_DanhSach() const { return ds_SV; }
};