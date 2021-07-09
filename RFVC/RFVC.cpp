
#include <Windows.h>
#include <conio.h>
#include <stdio.h>

struct sField
{
	unsigned char iStart;
	unsigned char iEnd;
	struct sField* pmNext;
};

void showFields(struct sField *pmFields,unsigned long iValue)
{
	struct sField* pmField;
	pmField = pmFields;
	while (pmField)
	{
		unsigned char iDistance;
		iDistance = pmField->iStart - pmField->iEnd;
		printf("%2d ", pmField->iStart);
		if (iDistance)
		{
			unsigned char iCon;
			for (iCon = 0; iCon < iDistance; iCon++)
			{
				if (iCon == iDistance - 1) { printf("%2d ", pmField->iEnd); }
				else printf("   ");
			}
		}
		pmField = pmField->pmNext;
	}
	printf("\n");
	pmField = pmFields;
	while (pmField)
	{
		unsigned char iDistance;
		unsigned char iMask;
		iDistance = pmField->iStart - pmField->iEnd;
		iMask = (1 << (iDistance+1)) - 1;
		printf("%2X ", (iValue >> pmField->iStart)&iMask);
		unsigned char iCon;
		for (iCon = 0; iCon < iDistance; iCon++)
		{
			printf("   ");
		}
		pmField = pmField->pmNext;
	}
	printf("\n");

	unsigned char iConSep=0;
	pmField = pmFields;
	while (pmField)
	{
		unsigned char iDistance;
		unsigned char iMask;
		iDistance = pmField->iStart - pmField->iEnd;
		if (iDistance)
		{
			unsigned char iCon;
			printf("|__");
			for (iCon = 0; iCon < iDistance-1; iCon++)
			{
				printf("___");
			}
			printf("__|");
		}
		else
		{
			printf("--");
			if (iConSep % 4 == 3) { printf("|"); }
			else printf("-");
		}
		iConSep += (iDistance + 1);
		pmField = pmField->pmNext;
	}
	return;
}

int main(void)
{
	

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
	
	printf("%08X\n", iValue);
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
		printf("\n\n");
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
	printf("\n\n");
	showFields(pmFields, iValue);
	printf("\n\n");
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
	showFields(pmFields, iValue);
	printf("\n\n");
	HeapDestroy(hHeap);
}