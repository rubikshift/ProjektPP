#pragma once
#include "file.h"

char* inputFileName();
int inputSize(const char* TexttoPrint, int x, int y);
void openDefaultFile(file** openedFiles, int &actualFileId, int &fileCounter);
void openFile(file** openedFiles, int &actualFileId, int &fileCounter);
void openNewFile(file** actualFile, int &actualFileId, int &fileCounter);
void resizeFileTab(file** fileTab, int counter, int &multipler);