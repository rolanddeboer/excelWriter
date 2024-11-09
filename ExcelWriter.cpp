#include <math.h>
#include <cassert>

#include "types.h"

// https://libxlsxwriter.github.io
#include "xlsxwriter.h"

using namespace std;

class ExcelWriter
{
  lxw_workbook *workbook;
  TextStyle header, text;
  vector<lxw_format *> formats;
  vector<double> columnWidths;
  bool hasTotals;

  void init()
  {
    // header.fontname = "Georgia";
    header.fontname = "Calibri";
    header.fontsize = 11;
    header.color = 0xb16e3e;
    header.color = 0x000000;
    header.height = 48;
    header.backgroundcolor = 0xd8d1c8;

    text.fontname = "Calibri";
    text.fontsize = 11;
    text.height = 17;

    hasTotals = false;

    initFormats();
  }

  void initFormats()
  {
    // prepare empty formats
    for (int i = 0; i < 7; i++) {
      formats.push_back( workbook_add_format (workbook ) );
      format_set_font_name( formats[i], text.fontname.c_str() );
      format_set_font_size( formats[i], text.fontsize );
    }

    columnWidths.resize(6);
    columnWidths[FORMAT_TEXT] = 20;
    columnWidths[FORMAT_LONGTEXT] = 25;
    columnWidths[FORMAT_DATE] = 15;
    columnWidths[FORMAT_INT] = 10;
    columnWidths[FORMAT_CURRENCY] = 10;
    columnWidths[FORMAT_BOOL] = 10;
    
    // column headers
    format_set_bold( formats[FORMAT_HEADER] );
    format_set_font_color( formats[FORMAT_HEADER], header.color );
    format_set_font_name( formats[FORMAT_HEADER], header.fontname.c_str() ) ;
    format_set_font_size( formats[FORMAT_HEADER], header.fontsize );
    format_set_align( formats[FORMAT_HEADER], LXW_ALIGN_VERTICAL_TOP) ;
    format_set_text_wrap( formats[FORMAT_HEADER] );  
    format_set_bg_color( formats[FORMAT_HEADER], header.backgroundcolor ); 

    // long text
    format_set_align( formats[FORMAT_LONGTEXT], LXW_ALIGN_VERTICAL_JUSTIFY) ;
    format_set_text_wrap( formats[FORMAT_LONGTEXT] );   

    // currency. Setting this to the index number because xlsxwriter does not accept non-us decimal seperators.
    format_set_num_format_index( formats[FORMAT_CURRENCY], 0x04 ); 
    format_set_bg_color( formats[FORMAT_CURRENCY], header.backgroundcolor ); 

    // dates
    format_set_num_format( formats[FORMAT_DATE], "dd/mm/yyyy" );
  }

  double dateFromTimestamp( double timestamp )
  {
    if ( timestamp == 0 ) return 0;
    return 25569 + timestamp / 86400;
  }

  string excelColFromNumber(int number) 
  {
    int number1 = round( number / 26 );
    int number2 = number % 26 + 1;
    if ( number1 == 0 ) {
      return numberToLetter( number2 );
    } else {
      return numberToLetter( number1 ) + numberToLetter( number2 );
    }
  }

  string numberToLetter(int n)
  {
      assert(n >= 1 && n <= 26);
      char c = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n-1];
      return string(&c, 1);
  }

public:

  ExcelWriter( string filename )
  {
    workbook = workbook_new( filename.c_str() );
    init();
  }

  void close()
  {
    workbook_close( workbook );
  }

  void write( Input input )
  {
    for (int sheet = 0; sheet < (int) input.sheets.size(); sheet++) {
      writeSheet( input.sheets[sheet] );
    }
  }

  void writeSheet( Sheet &sheet )
  {
    lxw_worksheet *worksheet = workbook_add_worksheet( workbook, sheet.title.c_str() );
    // Set print orientation to landscape
    worksheet_set_landscape(worksheet);
    // make sure the column headers appear on top of every page when printed
    worksheet_repeat_rows( worksheet, 0, 0 );
    // Set autofilter
    worksheet_autofilter( worksheet, 0, 0, sheet.rowCount, sheet.columns.size()-1 );
    // Freeze the first row and the first columns
    worksheet_freeze_panes( worksheet, 1, sheet.freezeColumns );
    // Write column headers
    writeColumnHeaders( worksheet, sheet );
    // Write data
    writeRows( worksheet, sheet );
  }

  void writeColumnHeaders( auto worksheet, Sheet &sheet )
  {
  //   lxw_image_options options = {.x_offset = 0,  .y_offset = 0, .x_scale  = 0.4, .y_scale  = 0.4};
  //   worksheet_insert_image_opt(worksheet, 0, 0, "/var/www/html/ao2/server/public/images/shows/noordshow/logo.png", &options);

    worksheet_set_row(worksheet, 0, header.height, formats[FORMAT_HEADER]);
    int colpos = 0;
    for (int col = 0; col < (int) sheet.columns.size(); col++) {
      if ( sheet.columns[col].hide ) continue;
      sheet.columns[col].total = 0;
      if ( sheet.columns[col].showTotal ) hasTotals = true;
      worksheet_write_string(
        worksheet, 
        0, 
        colpos, 
        sheet.columns[col].title.c_str(), 
        NULL
      );
      int width = sheet.columns[col].width;
      if ( width == 0 ) width = columnWidths[ sheet.columns[col].format ];
      worksheet_set_column( 
        worksheet, 
        colpos, 
        colpos, 
        width, 
        NULL
      );
      colpos++;
    }
    if ( hasTotals ) {
      worksheet_write_string(
        worksheet, 
        sheet.rowCount + 2, 
        0, 
        sheet.totalsTitle.c_str(), 
        formats[FORMAT_HEADER]
      );
    }
  }

  void writeRows( auto worksheet, Sheet &sheet )
  {
    int colpos = 0;
    for (int col = 0; col < (int) sheet.columns.size(); col++) {
      if ( sheet.columns[col].hide ) continue;
      int row;
      for (row = 0; row < sheet.rowCount; row++) {
        writeField( worksheet, sheet, row, col, colpos );
      }
      if ( sheet.columns[col].showTotal ) {
        writeTotal( worksheet, sheet, row, col, colpos );       
      }
      colpos++;
    }
  }
  
  void writeField(auto worksheet, Sheet &sheet, int row, int col, int colpos = -1) {
    if (colpos == -1) colpos = col;

    // Text content
    if ( 
      sheet.columns[col].format == FORMAT_TEXT 
      || sheet.columns[col].format == FORMAT_LONGTEXT 
      || sheet.columns[col].format == FORMAT_BOOL 
    ) {
      worksheet_write_string( 
        worksheet, 
        row + 1, 
        colpos, 
        sheet.columns[col].stringValues[row].c_str(), 
        colpos == 0 ? formats[FORMAT_HEADER] : NULL
      );

    // Sum formula
    } else if ( sheet.columns[col].sum > 0 ) {
      double value = 0;
      for (int i = 1; i <= sheet.columns[col].sum; i++) {
        value += sheet.columns[col-i].intValues[row];
      }
      if ( sheet.columns[col].format == FORMAT_CURRENCY ) value = value / 100;
      string formula = 
        "=SUM(" 
        + excelColFromNumber( col - sheet.columns[col].sum )
        + to_string( row + 2 )
        + ":" + excelColFromNumber( col - 1 )
        + to_string( row + 2 )
        + ")"
      ;
      worksheet_write_formula_num( 
        worksheet, 
        row + 1, 
        colpos, 
        formula.c_str(), 
        NULL,
        value
      );

    // Difference formula
    } else if ( sheet.columns[col].diff ) {
      string formula = 
        "=" 
        + excelColFromNumber( col - 2 )
        + to_string( row + 2 )
        + "-" + excelColFromNumber( col - 1 )
        + to_string( row + 2 )
      ;
      double value = sheet.columns[col-2].intValues[row] - sheet.columns[col-1].intValues[row];
      if ( sheet.columns[col].format == FORMAT_CURRENCY ) value = value / 100;
      worksheet_write_formula_num( 
        worksheet, 
        row + 1, 
        colpos, 
        formula.c_str(), 
        NULL,
        value
      );

    // Number content
    } else {
      double value = sheet.columns[col].intValues[row];
      if ( sheet.columns[col].showTotal ) sheet.columns[col].total += value;
      if ( sheet.columns[col].format == FORMAT_CURRENCY ) value = value / 100;
      if ( sheet.columns[col].format == FORMAT_DATE ) value = dateFromTimestamp( value );
      if ( value == 0 ) {
        worksheet_write_blank( 
          worksheet, 
          row + 1, 
          colpos, 
          formats[sheet.columns[col].format]
        );
      } else {
        worksheet_write_number( 
          worksheet, 
          row + 1, 
          colpos, 
          value, 
          formats[sheet.columns[col].format]
        );
      }
    }
  }

  void writeTotal(auto worksheet, Sheet &sheet, int row, int col, int colpos) 
  {
    lxw_format *format = workbook_add_format (workbook );
    format_set_bold( format );
    format_set_font_name( format, text.fontname.c_str() );
    format_set_font_size( format, text.fontsize );
    if ( sheet.columns[col].format == FORMAT_CURRENCY ) {
      sheet.columns[col].total = sheet.columns[col].total / 100;
      format_set_num_format_index( format, 0x04 ); 
      format_set_bg_color( format, header.backgroundcolor );
    }

    string colLetter = excelColFromNumber( colpos );
    string formula = 
      "=SUM(" 
      + colLetter
      + "2"
      + ":" + colLetter
      + to_string( row + 1 )
      + ")"
    ;
    worksheet_write_formula_num( 
      worksheet, 
      row + 2, 
      colpos, 
      formula.c_str(), 
      format,
      sheet.columns[col].total
    );
  }

};