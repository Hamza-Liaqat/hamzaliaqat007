// OOP Project.cpp : Defines the entry point for the console application.
//

//***************************************************************
//                   HEADER FILE USED IN PROJECT
//****************************************************************
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

//*************************************************************
//                   CLASS USED IN PROJECT
//*************************************************************
COORD coord = { 0, 0 }; // this is global variable
//center of axis is set to the top left cornor of the screen
void gotoxy(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
class product
{
	int pno;
	char name[50];
	float price, qty, tax, dis;
public:
	void create_product()
	{
		cout << "\nPlease Enter The Product No. of The Product: ";
		cin >> pno;
		cout << "\n\n\nPlease Enter The Name of The Product: ";
		cin >> name;
		cout << "\nPlease Enter The Price of The Product($): ";
		cin >> price;
		cout << "\nPlease Enter The Discount(%): ";
		cin >> dis;
	}

	void show_product()
	{
		cout << "\nThe Product No. of The Product: " << pno;
		cout << "\nThe Name of The Product: ";
		puts(name);
		cout << "\nThe Price of The Product: " << price;
		cout << "\nDiscount : " << dis;
		cin >> dis;
	}

	int  retpno()
	{
		return pno;
	}

	float retprice()
	{
		return price;
	}

	char* retname()
	{
		return name;
	}

	int retdis()
	{
		return dis;
	}

};         //class ends here



//******************************************************
//   Global declaration for stream object, object
//******************************************************

fstream fp;
product pr;


//******************************************************
//    Function to write in file
//******************************************************

void write_product()
{
	fp.open("Shop.dat", ios::out | ios::app);
	pr.create_product();
	fp.write((char*)&pr, sizeof(product));
	fp.close();
	cout << "\n\nThe Product Has Been Created. ";
	_getch();
}


//*********************************************************
//    Function to read all records from file
//*********************************************************


void display_all()
{
	system("cls");
	cout << "\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n";
	fp.open("Shop.dat", ios::in);
	while (fp.read((char*)&pr, sizeof(product)))
	{
		pr.show_product();
		cout << "\n\n====================================\n";
		_getch();
	}
	fp.close();
	_getch();
}


//***********************************************************
//    Function to read specific record from file
//***********************************************************


void display_sp(int n)
{
	int flag = 0;
	fp.open("Shop.dat", ios::in);
	while (fp.read((char*)&pr, sizeof(product)))
	{
		if (pr.retpno() == n)
		{
			system("cls");
			pr.show_product();
			flag = 1;
		}
	}
	fp.close();
	if (flag == 0)
		cout << "\n\nRecord not exist!";
	_getch();
}


//****************************************************
//    Function to modify record of file
//****************************************************


void modify_product()
{
	int no, found = 0;
	system("cls");
	cout << "\n\n\tTo Modify ";
	cout << "\n\n\tEnter The Product No. of The Product: ";
	cin >> no;
	fp.open("Shop.dat", ios::in | ios::out);
	while (fp.read((char*)&pr, sizeof(product)) && found == 0)
	{
		if (pr.retpno() == no)
		{
			pr.show_product();
			cout << "\nPlease Enter The New Details of Product: " << endl;
			pr.create_product();
			int pos = 1 * sizeof(pr);
			fp.seekp(pos, ios::cur);
			fp.write((char*)&pr, sizeof(product));
			cout << "\n\n\t Record Updated";
			found = 1;
		}
	}
	fp.close();
	if (found == 0)
		cout << "\n\nRecord Not Found! ";
	_getch();
}


//*****************************************************
//    Function to delete record of file
//*****************************************************


void delete_product()
{
	int no;
	system("cls");
	cout << "\n\n\n\tDelete Record";
	cout << "\n\nPlease Enter The product no. of The Product You Want To Delete: ";
	cin >> no;
	fp.open("Shop.dat", ios::in | ios::out);
	fstream fp2;
	fp2.open("Temp.dat", ios::out);
	fp.seekg(0, ios::beg);
	while (fp.read((char*)&pr, sizeof(product)))
	{
		if (pr.retpno() != no)
		{
			fp2.write((char*)&pr, sizeof(product));
		}
	}
	fp2.close();
	fp.close();
	remove("Shop.dat");
	rename("Temp.dat", "Shop.dat");
	cout << "\n\n\tRecord Deleted... ";
	_getch();
}


//************************************************************
//    Function to display all products price list
//************************************************************

void menu()
{
	system("cls");
	fp.open("Shop.dat", ios::in);
	if (!fp)
	{
		cout << "ERROR!!! FILE COULD NOT BE OPEN\n\n\n Go To Admin Menu to create File";
		cout << "\n\n\nProgram is closing... ";
		_getch();
		exit(0);
	}

	cout << "\n\n\t\tProduct MENU\n\n";
	cout << "=====================================\n";
	cout << "P.NO.\t\tNAME\t\tPRICE\n";
	cout << "=====================================\n";

	while (fp.read((char*)&pr, sizeof(product)))
	{
		cout << pr.retpno() << "\t\t" << pr.retname() << "\t\t" << pr.retprice() << endl;
	}
	fp.close();
}




//****************************************************************
//    Function to place order and generating bill for Products
//****************************************************************

void place_order()
{
	int  order_arr[50], quan[50], c = 0;
	float amt, damt, total = 0;
	char ch = 'Y';
	menu();
	cout << "\n============================";
	cout << "\n    PLACE YOUR ORDER";
	cout << "\n============================\n";
	do
	{
		cout << "\n\nEnter The Product No. Of The Product: ";
		cin >> order_arr[c];
		cout << "\nQuantity in number: ";
		cin >> quan[c];
		c++;
		cout << "\nDo You Want To Order Another Product? (y/n)";
		cin >> ch;
	} while (ch == 'y' || ch == 'Y');
	cout << "\n\nThank You For Placing The Order. ";
	_getch();
	system("cls");
	cout << "\n\n******************INVOICE*******************\n";
	cout << "\nPr No.\tPr Name\t    Quantity\t  Price\t\tAmount\tAmount after discount\n";
	for (int x = 0; x <= c; x++)
	{
		fp.open("Shop.dat", ios::in);
		fp.read((char*)&pr, sizeof(product));
		while (!fp.eof())
		{
			if (pr.retpno() == order_arr[x])
			{
				amt = pr.retprice()*quan[x];
				damt = amt - (amt*pr.retdis() / 100);
				cout << "\n" << order_arr[x] << "\t" << pr.retname()
					<< "\t       " << quan[x] << "\t  " << pr.retprice()
					<< "\t\t" << amt << "\t\t" << damt;
				total += damt;
			}
			fp.read((char*)&pr, sizeof(product));
		}

		fp.close();
	}
	cout << "\n\n\n\n\t\t\t\t\tTOTAL = " << total;
	_getch();
}

//************************************************
//    	INTRODUCTION FUNCTION
//************************************************

void intro()
{
	system("cls");
	gotoxy(38, 9);
	cout << "SUPER MARKET";
	gotoxy(40, 10);
	cout << "BILLING";
	gotoxy(40, 11);
	cout << "PROJECT";
	cout << "\n\n\n\nMADE BY: Hamza Liaqat";
	cout << "\n\nInstitute: Bahria University Islamabad.";
	cout << "\n\nDepartment: Computer Engineering";
	_getch();

}




//*************************************************
//    	ADMINSTRATOR MENU FUNCTION
//*************************************************
void admin_menu()
{
	system("cls");
	char ch2;
	cout << "\n\n\n\tADMIN MENU";
	cout << "\n\n1.Create Product";
	cout << "\n\n2.Display All Products";
	cout << "\n\n3.Query ";
	cout << "\n\n4.Modify Product";
	cout << "\n\n5.Delete Product";
	cout << "\n\n6.View Product Menu";
	cout << "\n\n7.Back to MAIN MENU";
	cout << "\n\nEnter Your Choice from 1-7: ";
	ch2 = _getche();
	switch (ch2)
	{
	case '1':
		system("cls");
		write_product();
		break;
	case '2':
		display_all(); break;
	case '3':
		int num;
		system("cls");
		cout << "\n\n\tPlease Enter The Product No. ";
		cin >> num;
		display_sp(num);
		break;
	case '4':
		modify_product(); break;
	case '5':
		delete_product(); break;
	case '6':
		menu();
		_getch();
	case '7':
		break;
	default:
		cout << "\a"; admin_menu();
	}
}


//********************************************************
//    	THE MAIN FUNCTION OF PROGRAM
//********************************************************


int main()
{
	char ch;
	intro();
	do
	{
		system("cls");
		cout << "\n\n\n\tMAIN MENU";
		cout << "\n\n1. Customer";
		cout << "\n\n2. Administrator";
		cout << "\n\n3. EXIT";
		cout << "\n\nPlease select the option from 1-3: ";
		ch = _getche();
		switch (ch)
		{
		case '1':
			system("cls");
			place_order();
			_getch();
			break;
		case '2': admin_menu();
			break;
		case '3':exit(0);
		default:cout << "\a";
		}
	} while (ch != '3');
}

//**************************************************
//    			END OF PROJECT
//**************************************************


