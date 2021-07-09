
#include <Windows.h>
#include <conio.h>
#include <stdio.h>

int main(void)
{
	struct sField
	{
		unsigned char iStart;
		unsigned char iEnd;
		struct sField *pmNext;
	};

	HANDLE hHeap;
	char cValue[32];
	unsigned long iValue;
	struct sField *pmFields=NULL;

goto PRO_NOW;

	{
		unsigned long iLoc=1;
		char cInput;
		printf("#Please Input the Hex Value:\n>");
		do
		{
			cInput = _getch();
			switch (cInput)
			{
			case 8:
				if (iLoc > 1) {
					iLoc--;
					printf("%c %c", 8, 8);
				}
				break;
			default:
				if ((cInput >= '0' && cInput <= '9') || (cInput >= 'a' && cInput <= 'f') || (cInput >= 'A' && cInput <= 'F')) {
					if (iLoc < 9) {
						cValue[iLoc - 1] = cInput;
						iLoc++;
						printf("%c", cInput);
					}
				}
				break;
			}
		} while (cInput != 13);
		cValue[iLoc-1] = 0;
		printf("\n");
	}
	{
		char *pcValue;
		iValue = 0;
		pcValue = cValue;
		while (*pcValue)
		{
			iValue *= 16;
			if (*pcValue >= '0' && *pcValue <= '9') { iValue += *pcValue - '0'; }
			else if (*pcValue >= 'a' && *pcValue <= 'f') { iValue += *pcValue - 'a' + 10; }
			else if (*pcValue >= 'A' && *pcValue <= 'F') { iValue += *pcValue - 'A' + 10; }
			pcValue++;
		}
		printf("#Value = [%08X]\n", iValue);
	}
	
PRO_NOW:
	iValue = 0x87654321;
	hHeap = HeapCreate(0, 0, 0);
	if (hHeap == INVALID_HANDLE_VALUE || hHeap == 0) { return -1; }
	
	{
		unsigned char iCon;
		struct sField *pmField;
		for (iCon = 0; iCon < 32; iCon++)
		{
			pmField = (struct sField *)HeapAlloc(hHeap, 0, sizeof(struct sField));
			if (pmField)
			{
				pmField->iStart = iCon;
				pmField->iEnd = iCon;
				pmField->pmNext = pmFields;
				pmFields = pmField;
			}
		}
	}

	{//Showing
		struct sField *pmField;
		pmField = pmFields;
		while (pmField)
		{
			printf("[%d %d] ", pmField->iStart, pmField->iEnd);
			pmField = pmField->pmNext;
		}
	}
	{//Merging
		unsigned char iStart;
		unsigned char iEnd;
		iStart = 18;
		iEnd = 10;
		struct sField* pmHead;
		struct sField* pmTail;
		struct sField* pmTemp;
		pmHead = pmFields;
		while (pmHead)
		{
			if (pmHead->iStart == iStart) { break; }
			pmHead = pmHead->pmNext;
		}
		pmTail = pmFields;
		while (pmTail)
		{
			if (pmTail->iStart == iEnd) { break; }
			pmTail = pmTail->pmNext;
		}
		pmTemp = pmHead->pmNext;
		pmHead->pmNext = pmTail->pmNext;
		pmHead->iEnd = pmTail->iEnd;
		while (pmTemp != pmTail)
		{
			struct sField* pmRemove;
			pmRemove = pmTemp;
			pmTemp = pmTemp->pmNext;
			HeapFree(hHeap, 0, pmRemove);
		} 
		HeapFree(hHeap, 0, pmTemp);
		printf("========\n");
	}
	{//Showing
		struct sField* pmField;
		pmField = pmFields;
		while (pmField)
		{
			printf("[%d %d] ", pmField->iStart, pmField->iEnd);
			pmField = pmField->pmNext;
		}
	}
	printf("\n>>>>\n");
	{//Seperate
		unsigned char iStart=18;
		unsigned char iEnd;
		unsigned char iCon;
		struct sField *pmField;
		pmField = pmFields;
		for (iCon = 0; iCon < 32; iCon++)
		{
			if (pmField->iStart == iStart) { break; }
			pmField = pmField->pmNext;
		}
		iEnd = pmField->iEnd;
		pmField->iEnd = pmField->iStart;
		for (iCon = 0; iCon < iStart-iEnd; iCon++)
		{
			struct sField *pmNew;
			pmNew = (struct sField *)HeapAlloc(hHeap, 0, sizeof(struct sField));
			pmNew->iStart = iEnd + iCon;
			pmNew->iEnd = iEnd + iCon;
			pmNew->pmNext = pmField->pmNext;
			pmField->pmNext = pmNew;
		}
	}
	printf("========\n");
	{//Showing
		struct sField* pmField;
		pmField = pmFields;
		while (pmField)
		{
			printf("[%d %d] ", pmField->iStart, pmField->iEnd);
			pmField = pmField->pmNext;
		}
	}

	HeapDestroy(hHeap);
}