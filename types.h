#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

using namespace std;

  enum FormatType {
    FORMAT_TEXT,
    FORMAT_LONGTEXT,
    FORMAT_DATE,
    FORMAT_INT,
    FORMAT_CURRENCY,
    FORMAT_BOOL,
    FORMAT_HEADER
  };

  struct Column {
    FormatType format;
    double width;
    string title;
    int sum;
    bool diff;
    double total;
    bool showTotal;
    vector<int> intValues;
    vector<string> stringValues;
  };

  struct Table {
    string title;
    int columnsToFreeze;
    vector<Column> columns;
    vector<vector<string>> rows;
  };

  struct TextStyle {
    long int color;
    string fontname;
    int fontsize;
    int height;
  };

  struct Sheet {
    string title;
    vector<Column> columns;
    int rowCount;
    int freezeColumns;
    string boolYes;
    string boolNo;
    string totalsTitle;
  };

  struct Input {
    vector<Sheet> sheets;
  };

#endif