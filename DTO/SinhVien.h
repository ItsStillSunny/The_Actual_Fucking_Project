#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

class SinhVien{
    //attributes
    private:
        string HoLot;
        string Ten;
        string GioiTinh;
        string NgaySinh;
        string DiaChi;
        int Stt;
        string Email;
        string MSSV;
        string MaKhoa;
        string TenKhoa;
        string Lop;
        string NamHoc;  
        bool checkSort; 
        bool checkMSSV; 
        bool checkEmail;
    //methods
    public:
        //default constructor + actual constructor
        SinhVien() {}
        SinhVien(string ho, string ten, string gioitinh, string ngaysinh, string diachi, string makhoa, string nam) :
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
        void Set_MSSV(const string &newMssv){
            MSSV = newMssv;
            checkMSSV = true;
        }
        void Set_Email(const string &newEmail){
            Email = newEmail;
            checkEmail = true;
        }

        void Set_HoLot(const string& ho) { HoLot = ho; }
        void Set_Ten(const string& ten) { Ten = ten; }
        void Set_GioiTinh(const string& gt) { GioiTinh = gt; }
        void Set_NgaySinh(const string& ns) { NgaySinh = ns; }
        void Set_DiaChi(const string& dc) { DiaChi = dc; }
        void Set_MaKhoa(const string& mk) { MaKhoa = mk; }
        void Set_NamHoc(const string& nh) { NamHoc = nh; }


        //getters
        string Get_MaKhoa() const {return MaKhoa;}
        string Get_NamHoc() const {return NamHoc;}
        string Get_MSSV() const {return MSSV;}
        string Get_Ten() const {return Ten;}
        string Get_HoLot() const {return HoLot;}

        string Get_HoVaTen() const{
            if (HoLot.empty()) return Ten;
        return HoLot + " " + Ten;
        }

        string Get_GioiTinh() const {return GioiTinh;}
        string Get_DiaChi() const {return DiaChi;}
        string Get_NgaySinh() const {return NgaySinh;}

        bool Is_Sorted() const {return checkSort;}
        bool Has_MSSV() const {return checkMSSV;}
        bool Has_Email() const {return checkEmail;}


        //displayer
        void Print_SinhVien() const {
            cout << left; 
            cout << setw(5) << Stt;
            cout << setw(12) << MSSV;
            cout << setw(20) << HoLot;
            cout << setw(10) << Ten;
            cout << setw(10) << GioiTinh;
            cout << setw(15) << NgaySinh;
            cout << setw(35) << Email << endl;
        }


        //format then save to file
        string toFile() const{
            return MSSV + "|" + Get_HoVaTen() + "|" + GioiTinh + "|" +
               NgaySinh + "|" + DiaChi + "|" + Email + "|" +
               MaKhoa + "|" + NamHoc;
        }
};
