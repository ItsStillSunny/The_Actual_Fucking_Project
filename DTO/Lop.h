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

class Lop{
    //attributes
    private:
        std::string TenLop;
        std::string MaKhoa;
        std::string NamHoc;
        std::vector<SinhVien> ds_SV; 
    
    //methods
    public:
        //default constructor + actual constructor
        Lop() {}
        Lop(std::string tenlop, std::string makhoa, std::string namhoc) : TenLop(tenlop), MaKhoa(makhoa), NamHoc(namhoc) {}

        //set the maximum number of SinhVIen that can be in a singular Lop
        static const int MAX_SV = 50;

        static int GetCurrentYear() {
            time_t t = time(nullptr);
            tm now;
            localtime_s(&now, &t); 
            return now.tm_year + 1900;
        }

        //Assign MSSV to a Lop ONLY IF all SinhVien is sorted
        bool Assign_MSSV_To_Lop(int &StartCount){
            for (const SinhVien &sv: ds_SV){
                //failure
                if (sv.Is_Sorted() == false)
                {   
                    std::cout << "Lop chua duoc sap xep, chua the cap MSSV.";
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

                    std::string MaKhoa = sv.Get_MaKhoa();
                    std::string NamHoc = sv.Get_NamHoc();

                    std::string NamHoc_Formatted = "";
                    //only get the last 2 digts (2 digits starting from the 2nd digit) | ex: 2024 -> 24
                    if (NamHoc.length() >= 4){
                        NamHoc_Formatted = NamHoc.substr(2,2);
                    }
                    else{
                        NamHoc_Formatted = "00";
                    }

                    //edit it so it look like XXXX
                    char FormattedCount[5];
                    snprintf(FormattedCount, sizeof(FormattedCount), "%04d", StartCount);

                    //"102" + "24" + "0152"
                    std::string NewMSSV = MaKhoa + NamHoc_Formatted + FormattedCount;
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
                    std::cout << "Lop chua duoc cap MSSV, chua the cap Email.";
                    return false;
                }
            }

            for (SinhVien &sv : ds_SV){
                //skip those who already have Email
                if (sv.Has_Email() == true){
                    continue;
                }
                else{
                    std::string MSSV = sv.Get_MSSV();
                    std::string NamHoc = sv.Get_NamHoc();

                    int NamHoc_As_int = 0;
                    try{
                        NamHoc_As_int = std::stoi(NamHoc);
                    }
                    catch (...){
                        continue;
                    }

                    //Email = MSSV + "@sv?.dut.udn.vn"
                    //consistency purpose ()
                    int serverIndex = (NamHoc_As_int % 5) + 1; 

                    std::string NewEmail = MSSV + "@sv" + std::to_string(serverIndex) + ".dut.udn.vn";
                    sv.Set_Email(NewEmail);
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
                std::cout << "Max sinh vien limit reached, unable to add more.";
                return false;
            }

        }

        bool Delete_SinhVien_By_MSSV(const std::string& mssv) {
            
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
        SinhVien *Select_SinhVien_By_Name(const std::string &holot, const std::string &ten){
            for (SinhVien& sv : ds_SV) {
                if (sv.Get_HoLot() == holot && sv.Get_Ten() == ten) {
                    return &sv; 
                }
            }
        return nullptr;
        }
        

        //find the SinhVien based on MSSV
        SinhVien* Find_SinhVien_By_MSSV(const std::string& mssv) {
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
            std::sort(ds_SV.begin(), ds_SV.end(), [](const SinhVien &a, const SinhVien &b) {
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
            std::cout << ("\n\n\t\t\t\t\t\tTRUONG DAI HOC BACH KHOA - DAI HOC DA NANG\n");
            std::string MaKhoa = Get_MaKhoa();
            std::string TenLop = Get_TenLop();
            std::string NamHoc = Get_NamHoc();
            std::cout << "\n\n\t\t\t\t\t\tKHOA: " << MaKhoa << "\n";
            std::cout << "\n\n\t\t\t\t\t\tDANH SACH LOP: " << MaKhoa << " - " << "Khoa hoc " << NamHoc << "\n";

            std::cout << std::left;
            std::cout << std::setw(5) << "STT";
            std::cout << std::setw(12) << "MSSV";
            std::cout << std::setw(20) << "Ho Lot";
            std::cout << std::setw(10) << "Ten";
            std::cout << std::setw(10) << "Gioi Tinh";
            std::cout << std::setw(15) << "Ngay Sinh";
            std::cout << std::setw(35) << "Email" << std::endl;
            std::cout << std::string(107, '-') << std::endl;

            for (const SinhVien &sv: ds_SV){ 
                sv.Print_SinhVien();
            }
        }

        //How many SinhVien got MaKhoa?
        int Count_Assigned_SinhVien(const std::string& maKhoa) {
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
    std::string Get_MaKhoa() const {return MaKhoa;}
    std::string Get_TenLop() const {return TenLop;}
    std::string Get_NamHoc() const {return NamHoc;}


    //setters
    void Set_TenLop(const std::string& ten) { TenLop = ten; }
    void Set_MaKhoa(const std::string& ten) { MaKhoa = ten; }
    void Set_NamHoc(const std::string& nam) { NamHoc = nam; }

    //file handling
    size_t Add_DanhSach_Tu_File(const std::vector<SinhVien>& list) {
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
    const std::vector<SinhVien>& get_DanhSach() const { return ds_SV; }
};