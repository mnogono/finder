//---------------------------------------------------------------------------

#ifndef ChartViewH
#define ChartViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <stdio.h>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

#if __CODEGEARC__ == 0x0670
	//0x0670 for C++Builder XE5 (BCC32, BCC64, BCCOSX, and BCCIOSARM).
	#include <VCLTee.Chart.hpp>
	#include <VclTee.TeeGDIPlus.hpp>
	#include <VCLTee.TeEngine.hpp>
	#include <VCLTee.TeeProcs.hpp>
	#include <VCLTee.Series.hpp>
	#include <Vcl.Grids.hpp>
#elif __CODEGEARC__ == 0x0630
	#include "Series.hpp"
	#include "TeEngine.hpp"
	#include "TeeProcs.hpp"
	#include "Chart.hpp"
#endif

#include "ChartView.h"


#include "sqlite3.h"
#include "sysVCL.h"
#include "sysTime.h"
#include <sstream>
#include <map>
#include <limits>
//---------------------------------------------------------------------------
class TFormChart : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel2;
	TChart *Chart1;
	TBarSeries *Series1;
	TStringGrid *StringGridScanDate;
	TStringGrid *StringGridFiles;
	TPanel *Panel1;
	TLabel *LabelCount;
	void __fastcall ListViewScanDateChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall StringGridScanDateSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall Chart1Zoom(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Chart1UndoZoom(TObject *Sender);

private:	// User declarations
	void __fastcall PrepareStatments();
public:		// User declarations
	sqlite3 *db;

	__fastcall TFormChart(TComponent* Owner);
	void __fastcall UpdateListViewScanDate();
	void __fastcall SetDB(sqlite3 *db);
	void __fastcall InitializeStringGridScanDate();
	void __fastcall BuildChart(double scanDate);
	void __fastcall InitializeChart();
	void __fastcall InitScrollBarBottomAxis();
	void __fastcall InitializeStringGridFiles();
	void __fastcall BuildStringGridFiles(double scanDate);
	void __fastcall UpdateColWidths();
	void __fastcall UpdateLeftLabelAxis();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormChart *FormChart;
//---------------------------------------------------------------------------
#endif

