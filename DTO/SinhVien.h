#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

class SinhVien{
    //attributes
    private:
        std::string HoLot;
        std::string Ten;
        std::string GioiTinh;
        std::string NgaySinh;
        std::string DiaChi;
        int Stt;
        std::string Email;
        std::string MSSV;
        std::string MaKhoa;
        std::string TenKhoa;
        std::string Lop;
        std::string NamHoc;  
        bool checkSort; 
        bool checkMSSV; 
        bool checkEmail;
    //methods
    public:
        //default constructor + actual constructor
        SinhVien() {}
        SinhVien(std::string ho, std::string ten, std::string gioitinh, std::string ngaysinh, std::string diachi, std::string makhoa, std::string nam) :
            HoLot(ho),
            Ten(ten),
            GioiTinh(gioitinh),
            NgaySinh(ngaysinh),
            DiaChi(diachi),
            MaKhoa(makhoa),
            NamHoc(nam),
            
            Stt(0),
            Email(""),
            MSSV(""),
            TenKhoa(""), 
            Lop(""),
            checkSort(false),
            checkMSSV(false),
            checkEmail(false) {}


        //setters
        void Set_checkSort(bool status) {checkSort = status;}
        void Set_MSSV(const std::string &newMssv){
            MSSV = newMssv;
            checkMSSV = true;
        }
        void Set_Email(const std::string &newEmail){
            Email = newEmail;
            checkEmail = true;
        }

        void Set_HoLot(const std::string& ho) { HoLot = ho; }
        void Set_Ten(const std::string& ten) { Ten = ten; }
        void Set_GioiTinh(const std::string& gt) { GioiTinh = gt; }
        void Set_NgaySinh(const std::string& ns) { NgaySinh = ns; }
        void Set_DiaChi(const std::string& dc) { DiaChi = dc; }
        void Set_MaKhoa(const std::string& mk) { MaKhoa = mk; }
        void Set_NamHoc(const std::string& nh) { NamHoc = nh; }


        //getters
        std::string Get_MaKhoa() const {return MaKhoa;}
        std::string Get_NamHoc() const {return NamHoc;}
        std::string Get_MSSV() const {return MSSV;}
        std::string Get_Ten() const {return Ten;}
        std::string Get_HoLot() const {return HoLot;}

        std::string Get_HoVaTen() const{
            if (HoLot.empty()) return Ten;
        return HoLot + " " + Ten;
        }

        std::string Get_GioiTinh() const {return GioiTinh;}
        std::string Get_DiaChi() const {return DiaChi;}
        std::string Get_NgaySinh() const {return NgaySinh;}

        bool Is_Sorted() const {return checkSort;}
        bool Has_MSSV() const {return checkMSSV;}
        bool Has_Email() const {return checkEmail;}


        //displayer
        void Print_SinhVien(int Real_Stt) const {
            std::cout << std::left; 
            std::cout << std::setw(5)  << Real_Stt;
            std::cout << std::setw(12) << MSSV;
            std::cout << std::setw(20) << HoLot;
            std::cout << std::setw(10) << Ten;
            std::cout << std::setw(10) << GioiTinh;
            std::cout << std::setw(15) << NgaySinh;
            std::cout << std::setw(35) << Email << std::endl;
        }


        //format then save to file
        std::string to_File() const{
            return MSSV + "|" + Get_HoVaTen() + "|" + GioiTinh + "|" +
               NgaySinh + "|" + DiaChi + "|" + Email + "|" +
               MaKhoa + "|" + NamHoc;
        }
};
