#include <iostream>
#include <windows.h>
#include "BLL/QuanLySinhVien.h"
using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    QuanLySinhVien ql;
    ql.intro();
    ql.menu();
    return 0;
}

