#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <iomanip>
#include <random>

using namespace std;
// 假設系統記憶體大小為 1MB,分頁大小為4KB
int memorySize = 256; // 記憶體的總分頁數
int pageSize = 4096;  // page的大小(Byte)
vector<int> memory;
vector<int>::iterator ptr;
// 6 12345 23456 19999 25643 17331 45000
// 123 y 1234 y 12344 n
// 0 y 12337 y 23453 n
// 99 y 18879 y 12345 n
// 9847 y 1234 y 12345 n
// 1293 y 8765 y 15766 n
// 1230 y 34567 y 22333 y 44000 n
class Schedule
{
public:
	int index;			   // 新增每個行程的索引值
	int Pages;			   // 新增每個行程的分頁數
	int ByteSize;		   // 新增每個行程的檔案大小
	int pageTable[256][2]; // 新增每個行程的分頁表
	void inputPage()
	{
		cout << "請輸入行程 P" << index << " 檔案大小(Byte): ";
		cin >> ByteSize;
		if (ByteSize % pageSize == 0)
		{
			Pages = (ByteSize / pageSize);
		}
		else
		{
			Pages = (ByteSize / pageSize) + 1;
		}
		for (int i = 0; i < Pages; i++)
		{
			pageTable[i][0] = i + 1; // 設定分頁表pageTable[i][0]的index值
			pageTable[i][1] = -1;
		}
	}
	void outputPage()
	{ // 輸出分頁表
		cout << "--------行程" << index << "的分頁表----------------" << endl;
		for (int i = 0; i < Pages; i++)
		{
			cout << " | " << setw(3) << pageTable[i][0];
		}
		cout << " | " << endl;
		for (int i = 0; i < Pages; i++)
		{
			if (pageTable[i][1] == -1)
			{
				cout << " | " << setw(3) << " ";
			}
			else
			{
				cout << " | " << setw(3) << pageTable[i][1];
			}
		}
		cout << " | " << endl;
		cout << "----------------------------------------" << endl;
	}
	void randomPage(int pageIndex)
	{ // 使用亂數分配記憶體給分頁
		random_device rd;
		mt19937 rng = mt19937(rd());
		uniform_int_distribution<> dis(0, memory.size() - 1);
		int randPage = dis(rng);
		pageTable[pageIndex - 1][1] = memory[randPage];
		ptr = memory.begin() + randPage;
		memory.erase(ptr);
	}
	void showCalculate(int address)
	{ // 計算出該指令在系統記憶體的實體位址(含計算式)
		cout << "該指令在系統記憶體的實體位址:" << endl;
		int page = (address / pageSize);
		cout << "(" << pageTable[page][1] << "*" << pageSize << ")+(" << address << "%" << pageSize << ")=";
		cout << (pageTable[page][1] * pageSize) + (address % pageSize) << endl;
	}
};

int main()
{
	int n = 0;
	for (int i = 0; i < 256; i++)
	{
		memory.push_back(i); // 全部的分頁表
	}

	cout << "請輸入行程數量: "; // 輸入行程資料數量
	cin >> n;

	Schedule P[n];
	// 新增行程資料數量

	for (int i = 0; i < n; i++)
	{
		P[i].index = i + 1;
		P[i].inputPage();
	}
	// 使用亂數分配記憶體給這些行程的前3個分頁
	for (int i = 0; i < n; i++)
	{
		P[i].randomPage(1);
		P[i].randomPage(2);
		P[i].randomPage(3);
	}
	// 輸出每個行程的分頁表
	for (int i = 0; i < n; i++)
	{
		P[i].outputPage();
	}

	int address;
	for (int i = 0; i < n; i++)
	{
		cout << "---------------查詢的行程" << i + 1 << "--------------" << endl;
		while (true)
		{
			cout << "輸入欲查詢的執行指令在該行程" << i + 1 << "的邏輯位址: ";
			cin >> address;
			if (address <= P[i].ByteSize - 1 && address >= 0)
			{
				// 若該指令之分頁尚未載入則先行載入
				if (P[i].pageTable[address / pageSize][1] <= 0)
				{
					P[i].randomPage((address / pageSize) + 1);
					P[i].outputPage(); // 輸出該行程的分頁表
				}
				P[i].showCalculate(address); // 輸出該行程的計算式
			}
			else
			{
				cout << "輸入欲查詢的邏輯位址錯誤!!!" << endl;
				continue;
			}
			string request;
			cout << "繼續查詢?(y/n): ";
			cin >> request;
			if (!request.compare("n"))
			{
				break;
			}
		}
	}
};