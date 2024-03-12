#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "xlsxwriter.h"
#include "InputParser.cpp"
#include "ExcelWriter.cpp"
#include "types.h"

using namespace std;

int main(int argc, char *argv[]) 
{
  InputParser* inputParser;
  ExcelWriter* excelWriter;
  string line;

  inputParser = new InputParser(argv[1]);

   // The compiler gives a warning about tmpnam. The reason why its use is discouraged is that even though the filename is unique at the time of calling tmpnam, it might not be unique any more by the time the file is actually created. Mkstemp is recommended because it actually creates the file. But because we are not actually in control of the file creation (Libxlxswriter is), we can't make use of that.
   char filename[L_tmpnam];
   if (! tmpnam(filename) ) {};

   excelWriter = new ExcelWriter( filename );
   excelWriter->write( inputParser->parseInput() );
   excelWriter->close();

   ifstream f( filename );
   if (f.is_open()) cout << f.rdbuf();

   remove ( filename );

   delete excelWriter;
   delete inputParser;

   return 0;
}