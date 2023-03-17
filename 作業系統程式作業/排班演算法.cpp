#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <iomanip>
using namespace std;
int main()
{
	// 5 0 10 3 1 2 2 3 4 5 4 1 4 2 5 1
	// 5 3 10 4 2 12 5 4 7 1 0 1 3 1 3 2
	// p1,到達時間:3,CPU時間:10,優先權(名次):4
	// p2,到達時間:2,CPU時間:12,優先權(名次):5
	// p3,到達時間:4,CPU時間:7,優先權(名次):1
	// p4,到達時間:0,CPU時間:1,優先權(名次):3
	// p5,到達時間:1,CPU時間:3,優先權(名次:2
	int n = 0;
	cout << "請輸入行程數量: "; // 輸入行程資料數量
	cin >> n;
	int p[n][4];
	for (int i = 0; i < n; i++)
	{
		p[i][0] = i + 1;									// 行程的index
		cout << "請輸入行程 P" << p[i][0] << " 到達時間: "; // 輸入行程資料到達時間 p[i][1]
		cin >> p[i][1];
		cout << "請輸入行程 P" << p[i][0] << " CPU時間: "; // 輸入行程資料CPU時間 p[i][2]
		cin >> p[i][2];
		cout << "請輸入行程 P" << p[i][0] << " 優先權(名次): "; // 輸入行程資料優先權(名次) p[i][3]
		cin >> p[i][3];
	}
	cout << endl;

	int pSchedule[n][4];
	memcpy(pSchedule, p, sizeof(p));
	int pWaitTime[n] = {0};
	float averageWaitTime = 0;
	int flag = 0;
	vector<int> pUndo;
	vector<int>::iterator ptr;
	int cpuTime = 0;
	int workTime = 0;
	vector<int> GanttSchedule;
	vector<int> GanttTime;
	cout << endl
		 << endl
		 << endl;
	cout << "----------1:FIFS-----------------" << endl; // 1:FIFS
	while (true)
	{
		if (flag == 0 && workTime == 0)
		{
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
		}
		for (int i = 0; i < n; i++)
		{
			if (pSchedule[i][1] == cpuTime)
			{ // 到達時把行程放到等待裡(先來的在前面)
				pUndo.push_back(i);
			}
		}
		for (int i = 0; i < pUndo.size(); i++)
		{ // 從陣列前面的開始檢查
			if (flag == 1 && pSchedule[pUndo[i]][1] <= cpuTime)
			{ // 當有行程到達，到達時間 p[i][1]
				GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
				workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
				pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
				ptr = pUndo.begin() + i;
				pUndo.erase(ptr); // 從行程中移除
				flag = 0;		  // 忙碌中
			}
		}
		if (workTime >= 0)
		{
			workTime--;
		}
		cpuTime++;
		if (pUndo.size() == 0 && flag == 1)
		{
			break;
		}
	}

	// 甘特圖
	cout << "甘特圖:" << endl;
	for (int i = 0; i < GanttSchedule.size(); i++)
	{
		cout << " |"
			 << " P" << GanttSchedule[i];
	}
	cout << " |" << endl;
	for (int i = 0; i < GanttTime.size(); i++)
	{
		cout << setw(2) << GanttTime[i] << "   ";
	}
	cout << endl;

	cout << "---------------------------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "行程P" << pSchedule[i][0] << " 等待時間: " << pWaitTime[i] << endl;
		averageWaitTime += pWaitTime[i];
	}
	averageWaitTime = averageWaitTime / n;
	cout << "平均等待時間: " << averageWaitTime << endl;

	pUndo.clear();
	cpuTime = 0;
	workTime = 0;
	averageWaitTime = 0;
	flag = 0;
	for (int i = 0; i < n; i++)
	{
		pWaitTime[i] = 0;
	}
	memcpy(pSchedule, p, sizeof(p));
	GanttTime.clear();
	GanttSchedule.clear();
	cout << endl
		 << endl
		 << endl;
	cout << "----------2:SJF(不可搶先)---------" << endl; // 2.SJF(不可搶先)
	while (true)
	{
		if (flag == 0 && workTime == 0)
		{
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
		}
		for (int i = 0; i < n; i++)
		{
			if (pSchedule[i][1] == cpuTime)
			{ // 到達時把行程放到等待裡
				pUndo.push_back(i);
			}
			for (int j = 1; j < pUndo.size(); j++)
			{
				for (int k = 0; k < j; k++)
				{
					if (pSchedule[pUndo[j]][2] < pSchedule[pUndo[k]][2])
					{ // 按照CPU時間排序，越短的放越前面
						int temp;
						temp = pUndo[j];
						pUndo[j] = pUndo[k];
						pUndo[k] = temp;
					}
				}
			}
		}
		for (int i = 0; i < pUndo.size(); i++)
		{
			if (flag == 1 && pSchedule[pUndo[i]][1] <= cpuTime)
			{ // 當有行程到達，到達時間 p[i][1]
				GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
				workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
				pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
				ptr = pUndo.begin() + i;
				pUndo.erase(ptr); // 從行程中移除
				flag = 0;		  // 忙碌中
			}
		}
		if (workTime >= 0)
		{
			workTime--;
		}
		cpuTime++;
		if (pUndo.size() == 0 && flag == 1)
		{
			break;
		}
	}

	// 甘特圖
	cout << "甘特圖:" << endl;
	for (int i = 0; i < GanttSchedule.size(); i++)
	{
		cout << " |"
			 << " P" << GanttSchedule[i];
	}
	cout << " |" << endl;
	for (int i = 0; i < GanttTime.size(); i++)
	{
		cout << setw(2) << GanttTime[i] << "   ";
	}
	cout << endl;

	cout << "---------------------------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "行程P" << pSchedule[i][0] << " 等待時間: " << pWaitTime[i] << endl;
		averageWaitTime += pWaitTime[i];
	}
	averageWaitTime = averageWaitTime / n;
	cout << "平均等待時間: " << averageWaitTime << endl;

	pUndo.clear();
	cpuTime = 0;
	workTime = 0;
	averageWaitTime = 0;
	flag = 0;
	for (int i = 0; i < n; i++)
	{
		pWaitTime[i] = 0;
	}
	memcpy(pSchedule, p, sizeof(p));
	GanttTime.clear();
	GanttSchedule.clear();
	cout << endl
		 << endl
		 << endl;
	cout << "----------3:SJF(可搶先)---------" << endl; // 3.SJF(可搶先)
	int nowDoing;
	while (true)
	{
		if (flag == 0 && workTime == 0)
		{
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
		}
		for (int i = 0; i < n; i++)
		{
			if (pSchedule[i][1] == cpuTime)
			{ // 到達時把行程放到等待裡
				pUndo.push_back(i);
			}
			for (int j = 1; j < pUndo.size(); j++)
			{
				for (int k = 0; k < j; k++)
				{
					if (pSchedule[pUndo[j]][2] < pSchedule[pUndo[k]][2])
					{ // 按照CPU時間排序，越短的放越前面
						int temp;
						temp = pUndo[j];
						pUndo[j] = pUndo[k];
						pUndo[k] = temp;
					}
				}
			}
		}
		for (int i = 0; i < pUndo.size(); i++)
		{
			if (pSchedule[pUndo[i]][1] <= cpuTime)
			{ // 當有行程到達，到達時間 p[i][1]
				if (flag == 1)
				{
					GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
					workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
					pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
					nowDoing = pUndo[i];
					ptr = pUndo.begin() + i;
					pUndo.erase(ptr); // 從行程中移除
					flag = 0;		  // 忙碌中
				}
				if (flag == 0 && pSchedule[pUndo[i]][2] < pSchedule[nowDoing][2])
				{															 // 當有更短的行程進來
					pWaitTime[nowDoing] += cpuTime - pSchedule[nowDoing][1]; // 儲存被中斷的行程
					pSchedule[nowDoing][1] = cpuTime + 1;
					pSchedule[nowDoing][2] = workTime;
					GanttTime.push_back(cpuTime);
					GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
					workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
					pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
					nowDoing = pUndo[i];
					ptr = pUndo.begin() + i;
					pUndo.erase(ptr); // 從行程中移除
					flag = 0;		  // 忙碌中
				}
			}
		}
		if (workTime >= 0)
		{
			workTime--;
		}
		cpuTime++;
		if (pUndo.size() == 0 && flag == 1)
		{
			break;
		}
	}

	// 甘特圖
	cout << "甘特圖:" << endl;
	for (int i = 0; i < GanttSchedule.size(); i++)
	{
		cout << " |"
			 << " P" << GanttSchedule[i];
	}
	cout << " |" << endl;
	for (int i = 0; i < GanttTime.size(); i++)
	{
		cout << setw(2) << GanttTime[i] << "   ";
	}
	cout << endl;

	cout << "---------------------------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "行程P" << pSchedule[i][0] << " 等待時間: " << pWaitTime[i] << endl;
		averageWaitTime += pWaitTime[i];
	}
	averageWaitTime = averageWaitTime / n;
	cout << "平均等待時間: " << averageWaitTime << endl;

	pUndo.clear();
	cpuTime = 0;
	workTime = 0;
	nowDoing = -1;
	averageWaitTime = 0;
	flag = 0;
	for (int i = 0; i < n; i++)
	{
		pWaitTime[i] = 0;
	}
	memcpy(pSchedule, p, sizeof(p));
	GanttTime.clear();
	GanttSchedule.clear();
	cout << endl
		 << endl
		 << endl;
	cout << "----------4:Priority(可搶先)---------" << endl; // 4.Priority(可搶先)
	while (true)
	{
		if (flag == 0 && workTime == 0)
		{
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
		}
		for (int i = 0; i < n; i++)
		{
			if (pSchedule[i][1] == cpuTime)
			{ // 到達時把行程放到等待裡
				pUndo.push_back(i);
			}
			for (int j = 1; j < pUndo.size(); j++)
			{
				for (int k = 0; k < j; k++)
				{
					if (pSchedule[pUndo[j]][3] < pSchedule[pUndo[k]][3])
					{ // 按照優先度排序
						int temp;
						temp = pUndo[j];
						pUndo[j] = pUndo[k];
						pUndo[k] = temp;
					}
				}
			}
		}
		for (int i = 0; i < pUndo.size(); i++)
		{
			if (pSchedule[pUndo[i]][1] <= cpuTime)
			{ // 當有行程到達，到達時間 p[i][1]
				if (flag == 1)
				{
					GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
					workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
					pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
					nowDoing = pUndo[i];
					ptr = pUndo.begin() + i;
					pUndo.erase(ptr); // 從行程中移除
					flag = 0;		  // 忙碌中
				}
				if (flag == 0 && pSchedule[pUndo[i]][3] < pSchedule[nowDoing][3])
				{															 // 當有優先度更高的行程進來
					pWaitTime[nowDoing] += cpuTime - pSchedule[nowDoing][1]; // 儲存被中斷的行程
					pSchedule[nowDoing][1] = cpuTime + 1;
					pSchedule[nowDoing][2] = workTime;
					GanttTime.push_back(cpuTime);
					GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
					workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
					pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
					nowDoing = pUndo[i];
					ptr = pUndo.begin() + i;
					pUndo.erase(ptr); // 從行程中移除
					flag = 0;		  // 忙碌中
				}
			}
		}
		if (workTime >= 0)
		{
			workTime--;
		}
		cpuTime++;
		if (pUndo.size() == 0 && flag == 1)
		{
			break;
		}
	}

	// 甘特圖
	cout << "甘特圖:" << endl;
	for (int i = 0; i < GanttSchedule.size(); i++)
	{
		cout << " |"
			 << " P" << GanttSchedule[i];
	}
	cout << " |" << endl;
	for (int i = 0; i < GanttTime.size(); i++)
	{
		cout << setw(2) << GanttTime[i] << "   ";
	}
	cout << endl;

	cout << "---------------------------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "行程P" << pSchedule[1][0] << " 等待時間: " << pWaitTime[i] << endl;
		averageWaitTime += pWaitTime[i];
	}
	averageWaitTime = averageWaitTime / n;
	cout << "平均等待時間: " << averageWaitTime << endl;

	pUndo.clear();
	cpuTime = 0;
	workTime = 0;
	nowDoing = -1;
	averageWaitTime = 0;
	flag = 0;
	for (int i = 0; i < n; i++)
	{
		pWaitTime[i] = 0;
	}
	memcpy(pSchedule, p, sizeof(p));
	GanttTime.clear();
	GanttSchedule.clear();
	cout << endl
		 << endl
		 << endl;
	cout << "----------5:RR(q=2)---------" << endl; // 5:RR(q=2)
	int q = 2;
	int count = 0;
	while (true)
	{
		if (flag == 0 && workTime == 0)
		{
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
		}
		if (count == q && workTime != 0)
		{									  // 當CPU執行經過Q=2
			pSchedule[nowDoing][1] = cpuTime; // 儲存被中斷的行程
			pSchedule[nowDoing][2] = workTime;
			GanttTime.push_back(cpuTime);
			flag = 1; // 非忙碌
			count = 0;
		}
		for (int i = 0; i < n; i++)
		{
			if (pSchedule[i][1] == cpuTime)
			{ // 到達時把行程放到等待裡
				pUndo.push_back(i);
			}
			for (int j = 1; j < pUndo.size(); j++)
			{
				for (int k = 0; k < j; k++)
				{
					if (pSchedule[pUndo[j]][1] < pSchedule[pUndo[k]][1])
					{ // 按照CPU時間排序，越短的放越前面
						int temp;
						temp = pUndo[j];
						pUndo[j] = pUndo[k];
						pUndo[k] = temp;
					}
				}
			}
		}
		for (int i = 0; i < pUndo.size(); i++)
		{
			if (pSchedule[pUndo[i]][1] <= cpuTime)
			{ // 當有行程到達，到達時間 p[i][1]
				if (flag == 1)
				{ // 非忙碌時
					GanttSchedule.push_back(pSchedule[pUndo[i]][0]);
					workTime = pSchedule[pUndo[i]][2]; // CPU時間 p[i][2]
					pWaitTime[pUndo[i]] += cpuTime - pSchedule[pUndo[i]][1];
					nowDoing = pUndo[i];
					ptr = pUndo.begin() + i;
					pUndo.erase(ptr); // 從行程中移除
					flag = 0;		  // 忙碌中
					count = 0;
				}
			}
		}
		if (workTime >= 0)
		{
			workTime--;
			count++;
		}
		cpuTime++;
		if (pUndo.size() == 0 && flag == 1)
		{
			break;
		}
	}

	// 甘特圖
	cout << "甘特圖:" << endl;
	for (int i = 0; i < GanttSchedule.size(); i++)
	{
		cout << " |"
			 << " P" << GanttSchedule[i];
	}
	cout << " |" << endl;
	for (int i = 0; i < GanttTime.size(); i++)
	{
		cout << setw(2) << GanttTime[i] << "   ";
	}
	cout << endl;

	cout << "---------------------------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "行程P" << pSchedule[i][0] << " 等待時間: " << pWaitTime[i] << endl;
		averageWaitTime += pWaitTime[i];
	}
	averageWaitTime = averageWaitTime / n;
	cout << "平均等待時間: " << averageWaitTime << endl;

	pUndo.clear();
	cpuTime = 0;
	workTime = 0;
	nowDoing = 0;
	averageWaitTime = 0;
	flag = 0;
	for (int i = 0; i < n; i++)
	{
		pWaitTime[i] = 0;
	}
	memcpy(pSchedule, p, sizeof(p));
	GanttTime.clear();
	GanttSchedule.clear();
}