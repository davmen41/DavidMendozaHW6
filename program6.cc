/*
 * David Mendoza
 * CS 3377
 * dxm140430@utdallas.edu
 */

#include <iostream>
#include <fstream>
#include "cdk.h"
#include "stdint.h"
#include <string>
#include <sstream>
#include <algorithm>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryHeaderFile{
  
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

  const int maxRecordStringLength = 25; 

 class BinaryFileRecord{

 public:
   uint8_t strLength;
   char stringBuffer[maxRecordStringLength];
 };

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix
  
  stringstream ss, hexstream;
  string cellMsg;
  string filename = "cs3377.bin";    //binary file
  BinaryFileRecord *binRecord = new BinaryFileRecord();
  BinaryHeaderFile *binHeader = new BinaryHeaderFile();

  // Sets up the array, the first values are null strings

  const char 		*rowTitles[] = {"null", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"null", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  //open up the binary file
  ifstream binFile (filename.c_str(), ios::in | ios::binary);

  //read headers from the file
  binFile.read((char*) binHeader, sizeof(BinaryHeaderFile));
  
  //read in feedface and add it into the first cell
  hexstream << hex << binHeader->magicNumber;
  string upperStr = hexstream.str();
  transform(upperStr.begin(), upperStr.end(), upperStr.begin(), (int (*)(int))toupper);
  cellMsg = "Magic: 0x" + upperStr;
  setCDKMatrixCell(myMatrix, 1, 1, cellMsg.c_str());
  hexstream.str("");

  //add version number to the gui
  ss << binHeader->versionNumber;
  cellMsg ="Version: " + ss.str(); 
  setCDKMatrixCell(myMatrix, 1, 2, cellMsg.c_str());
  ss.str("");

  //add the number of records to be read
  ss << binHeader->numRecords;
  cellMsg = "numRecords: " + ss.str();
  setCDKMatrixCell(myMatrix, 1, 3, cellMsg.c_str());
  ss.str("");

  for(int i = 2; i <= (int)binHeader->numRecords+1; i++){
 
    //read in a line(record) from the binary file
    binFile.read((char*)binRecord, sizeof(BinaryFileRecord));
    
    //add in the stringlength to its respective cell on the first columns
    ss << (int)binRecord->strLength;
    cellMsg = "strlen: " + ss.str();
    setCDKMatrixCell(myMatrix, i, 1, cellMsg.c_str());
    ss.str("");

    //add in the word read from the binary file to the cells in the second column
    setCDKMatrixCell(myMatrix, i, 2, binRecord->stringBuffer);
    
  }
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
  binFile.close();
}
