#include <stdio.h>
#include "files_extractor.h"

int main() {
    char* baseFileName = "test_output";

    /* --- נתוני קוד ונתונים --- */
    int code[] = {341, 1023};     // לדוגמה: שתי הוראות מקודדות
    int data[] = {512, 15};       // לדוגמה: שני נתונים
    int codeLength = 2;
    int dataLength = 2;

    /* --- סמלי entry --- */
    char* entryLabels[] = {"START", "LOOP"};
    int entryAddresses[] = {100, 123};
    int entryCount = 2;

    /* --- סמלי extern --- */
    char* externLabels[] = {"EXT_FUNC", "EXT_VAR"};
    int externAddresses[] = {110, 145};
    int externCount = 2;

    /* --- יצירת הקבצים --- */
    generateObFile(baseFileName, code, codeLength, data, dataLength);
    generateEntryFile(baseFileName, entryLabels, entryAddresses, entryCount);
    generateExternFile(baseFileName, externLabels, externAddresses, externCount);

    return 0;
}
