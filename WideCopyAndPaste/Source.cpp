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
	CloseClipboard(); 


}
int main()
{

	using namespace std;

	CStringW dataArr; // place save data from each CTRL + C pressing

	bool wasPressedC = false; // preventing copying while holding keys
	bool wasPressedV = false; // preventing copying while holding keys

	while (true)
	{
		if ((GetAsyncKeyState(VK_CONTROL) < 0))
		{
			// get ctrl + c processing  -> copy processing
			if ((GetAsyncKeyState(0x43) < 0) && !wasPressedC)
			{
				// wait 50ms to give time for program wich called ctrl+c write data to the clipboard
				Sleep(30);

				wasPressedC = true;

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
			else if (!(GetAsyncKeyState(0x43) & 0x8000)) // if we dismissed C
			{
				wasPressedC = false;
			}

			// get ctrl + V processing -> paste processing
			if ((GetAsyncKeyState(0x56) < 0) && !wasPressedV)
			{
				wasPressedV = true;

				// pause to give another program copy data from the clipboard
				Sleep(30);

				// clean data storage
				dataArr = "";
				writeDataToClipBoard(dataArr);

			}
			else if (!(GetAsyncKeyState(0x56) & 0x8000))// if we dismissed V
			{
				wasPressedV = false;
			}

			// get ctrl + B proessing
			if ((GetAsyncKeyState(0x42) < 0)) // press CTRL + B to close the program
			{
				break;
			}
		}
	}


	return 0; 
}
