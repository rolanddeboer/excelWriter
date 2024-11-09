#include <cstdio>

#include "types.h"

// http://rapidjson.org/
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

class InputParser
{

  Document data;
  Input parsedInput;
  string boolYes, boolNo;

public:

  // InputParser( string json ) 
  // {
  //   data.Parse(json.c_str());
  // }

  // InputParser()
  // {
  //   //FILE* fp = fopen("testdata.json", "r");
  //   char readBuffer[65536];
  //   FileReadStream is(stdin, readBuffer, sizeof(readBuffer));
  //   data.ParseStream(is);
  //   // fclose(fp);
  // }

  InputParser( string filename )
  {
    FILE* fp = fopen(filename.c_str(), "r");
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    data.ParseStream(is);
    fclose(fp);
  }

  Input parseInput()
  {
    if (! data.HasMember("sheets") ||! data["sheets"].IsArray() ) return parsedInput;

    setBooleanWords();

    parsedInput.sheets.resize( data["sheets"].Size() );
    for (int sheetnr = 0; sheetnr < (int) parsedInput.sheets.size(); sheetnr++) {
      parsedInput.sheets[sheetnr].title = getTextProperty( "title", data["sheets"][sheetnr] );
      parsedInput.sheets[sheetnr].totalsTitle = getTextProperty( "totalsTitle", data["sheets"][sheetnr] );
      parsedInput.sheets[sheetnr].freezeColumns = getIntProperty( "freezeColumns", data["sheets"][sheetnr] );
      parsedInput.sheets[sheetnr].columns = move( getColumns( data["sheets"][sheetnr] ) );
      parsedInput.sheets[sheetnr].rowCount = getRowCount( data["sheets"][sheetnr] );
      addValuesToColumns( parsedInput.sheets[sheetnr].columns, data["sheets"][sheetnr] );
    }

    return parsedInput;
  }

  void setBooleanWords() 
  {
    if (! existsAndIsObject( "booleanWords", data )) {
      boolYes = "yes";
      boolNo = "no";
    }
    boolYes = getTextProperty( "true", data["booleanWords"] );
    boolNo = getTextProperty( "false", data["booleanWords"] );
  }

  vector<Column> getColumns( auto &sheet )
  {
    vector<Column> columns;
    
    if (! existsAndIsArray( "columns", sheet ) ) return columns;

    columns.resize( sheet["columns"].Size() );

    for (int col=0; col < (int) columns.size(); col++) {
      columns[col].title = getTextProperty( "title", sheet["columns"][col] );
      columns[col].format = getFormatType( getTextProperty( "format", sheet["columns"][col] ) );
      columns[col].hide = getBoolProperty( "hide", sheet["columns"][col] );
      columns[col].width = getIntProperty( "width", sheet["columns"][col] );
      columns[col].sum = getIntProperty( "sum", sheet["columns"][col] );
      columns[col].diff = getBoolProperty( "diff", sheet["columns"][col] );
      columns[col].showTotal = getBoolProperty( "showTotal", sheet["columns"][col] );
    }

    return columns;
  }

  int getRowCount( auto &sheet )
  {
    if (! existsAndIsArray( "rows", sheet ) ) return 0;
    return sheet["rows"].Size();
  }

  void addValuesToColumns( vector<Column> & columns, const auto &sheet )
  {
    if (! existsAndIsArray( "rows", sheet ) ) return;

    for (int row = 0; row < (int) sheet["rows"].Size(); row++) {
      for (int col = 0; col < (int) columns.size(); col++) {
        if ( columns[col].format == FORMAT_TEXT || columns[col].format == FORMAT_LONGTEXT ) {
          columns[col].stringValues.push_back( sheet["rows"][row][col].GetString() );
        } else if ( columns[col].format == FORMAT_BOOL ) {
          string value = 
            sheet["rows"][row][col].GetBool()
            ? boolYes
            : boolNo
          ;
          columns[col].stringValues.push_back( value );
        } else {
          columns[col].intValues.push_back( sheet["rows"][row][col].GetInt() );
        }
      }
    }
  }

  bool existsAndIsObject( string property, const auto &data )
  {
    return ( data.HasMember( property.c_str() ) && data[ property.c_str() ].IsObject() );
  }

  bool existsAndIsArray( string property, const auto &data )
  {
    return ( data.HasMember( property.c_str() ) && data[ property.c_str() ].IsArray() );
  }

  bool getBoolProperty( string property, const auto &data )
  {
    if ( data.HasMember( property.c_str() ) && data[ property.c_str() ].IsBool() ) {
      return data[ property.c_str() ].GetBool();
    } else {
      return false;
    }
  }

  int getIntProperty( string property, const auto &data )
  {
    if ( data.HasMember( property.c_str() ) && data[ property.c_str() ].IsInt() ) {
      return data[ property.c_str() ].GetInt();
    } else {
      return 0;
    }
  }

  string getTextProperty( string property, const auto &data ) 
  {
    if ( data.HasMember( property.c_str() ) && data[ property.c_str() ].IsString() ) {
      return data[ property.c_str() ].GetString();
    } else {
      return "";
    }
  }

  FormatType getFormatType( string format )
  {
    if ( format == "integer" ) return FORMAT_INT;
    if ( format == "date" ) return FORMAT_DATE;
    if ( format == "currency" ) return FORMAT_CURRENCY;
    if ( format == "longtext" ) return FORMAT_LONGTEXT;
    if ( format == "boolean" ) return FORMAT_BOOL;
    return FORMAT_TEXT;
  }

};