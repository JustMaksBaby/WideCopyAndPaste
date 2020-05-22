#include <fstream>
#include <iostream>
#include <Windows.h>
#include <atlstr.h> // for Cstring

void getDataFromClipboard(CStringW& dataStorage)
{

	// readData
	CStringW fromClipboard; // temp storage for data from clipboard

	// get data from the clipboard
	HGLOBAL hData = GetClipboardData(CF_UNICODETEXT);
	LPWSTR chBuffer = LPWSTR(GlobalLock(hData));
	fromClipboard = chBuffer;
	GlobalUnlock(hData);

	if (fromClipboard.GetLength() > 0)
	{
		dataStorage += fromClipboard;
		dataStorage += "\n";
	}
	CloseClipboard();
}


void writeDataToClipBoard(CStringW& dataStorage)
{
	if (OpenClipboard(nullptr))
	{
		EmptyClipboard();
		HGLOBAL hgBuffer = GlobalAlloc(GMEM_MOVEABLE, (dataStorage.GetLength() + 1) * 2);

		memcpy(LPWSTR(GlobalLock(hgBuffer)), LPCWSTR(dataStorage), (dataStorage.GetLength() + 1) * 2); // GetLength count elements NOT bites

		GlobalUnlock(hgBuffer); // unlock data 

		SetClipboardData(CF_UNICODETEXT, hgBuffer); //write to clipboard
	}


}
int main()
{

	using namespace std;

	CStringW dataArr;
	bool cleanBuffer = false;
	while (true)
	{
		// get ctrl + c pressing
		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(0x43) < 0)
		{
			// wait 50ms to give time for program wich called ctrl+c write data to the clipboard
			Sleep(50);


			if (OpenClipboard(nullptr))
			{
				if (IsClipboardFormatAvailable(CF_UNICODETEXT))
				{

					getDataFromClipboard(dataArr); 
					writeDataToClipBoard(dataArr); 
		
				}
				CloseClipboard();
			}

		}
		
		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(0x56) < 0 && dataArr.GetLength() > 0 )
		{
			Sleep(20);
			dataArr = "";
			writeDataToClipBoard(dataArr); 

			// doesn`t work correctly
		}
		Sleep(100);


	}
}
