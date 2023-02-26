/* add.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
	// Gioi thieu thanh vien nhom
	PrintString("Thanh vien nhom:\n");
	PrintString("Dinh Cao Hong Phuoc 20127287\n");
	PrintString("Vo Thanh Lam 20127546\n");
	PrintString("Nguyen Hoai Man 20127561\n\n");
	// Mo ta chuong trinh in ra bang ascii
	PrintString(
		"Trong thu muc ascii.c, chay cau lenh: build.linux/nachos -x ascii\n"
		"In ra bang ascii tu 32 den 126 \n");
	// Mo ta chuong trinh sort
	PrintString(
		"Trong thu muc bsort.c, chay cau lenh: build.linux/nachos -x bsort\n");

	PrintString("\t- Nhap so phan tu cua mang (n) (So phan tu cua mang: 0 <= n <= 100)\n");
	PrintString("\t- Nhap cac phan tu cua mang\n");
	PrintString(
		"\t- Nhap 1 hoac 2 de sap xep theo chieu tang dan hoac giam dan (1: Sap xep theo chieu tang dan, "
		"2: Sap xep theo chieu giam dan)\n");
	PrintString(
		"\t- Chuong trinh se in ra mang da sap xep theo chieu ban da chon\n");

	PrintChar('\n');
	PrintChar('\n');

	Halt();
	/* not reached */
}
