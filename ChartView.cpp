// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChartView.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#if __CODEGEARC__ == 0x0630
	#pragma link "Chart"
	#pragma link "Series"
	#pragma link "TeEngine"
	#pragma link "TeeProcs"
#endif

#pragma resource "*.dfm"

TFormChart *FormChart;

// ---------------------------------------------------------------------------
__fastcall TFormChart::TFormChart(TComponent *Owner) : TForm(Owner){}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::ListViewScanDateChange(TObject *Sender, TListItem *Item, TItemChange Change){;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::UpdateListViewScanDate(){
	sysVCL::ClearStringGrid(StringGridScanDate);

	if (!db){
		return;
	}

	double scan_date;
	TDateTime dt;

	char sSQL[] = "SELECT DISTINCT scan_date FROM file";
	sqlite3_stmt *stmtSelectScanDate;

	sqlite3_prepare(db, sSQL, strlen(sSQL), &stmtSelectScanDate, NULL);

	while (sqlite3_step(stmtSelectScanDate) != SQLITE_DONE){
		scan_date = sqlite3_column_double(stmtSelectScanDate, 0);
		dt.Val = scan_date;
		StringGridScanDate->Cells[0][StringGridScanDate->RowCount - 1] = dt.FormatString("yyyy-mm-dd hh:nn:ss");
		StringGridScanDate->RowCount += 1;
	}

	sqlite3_finalize(stmtSelectScanDate);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::PrepareStatments(){}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::SetDB(sqlite3 *db){
	this->db = db;
	PrepareStatments();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::UpdateColWidths(){
	StringGridScanDate->ColWidths[0] = StringGridScanDate->Width - 25;

	StringGridFiles->ColWidths[1] = StringGridFiles->Width - StringGridFiles->ColWidths[0] - 25;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::InitializeStringGridScanDate(){
	StringGridScanDate->RowCount = 2;
	StringGridScanDate->FixedRows = 1;

	StringGridScanDate->Cells[0][0] = "Дата сканирования";
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::InitializeChart(){
	// set min max window range as one week
//    double max = Series1->MaxXValue();
//	Chart1->BottomAxis->SetMinMax(max - 7, max);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::InitScrollBarBottomAxis(){
/*
	double min = Series1->MinXValue();
	double max = Series1->MaxXValue();
	double step = 7;
	ScrollBarBottomAxis->Max = (max - min) / step;
	ScrollBarBottomAxis->Min = 0;
	ScrollBarBottomAxis->Position = ScrollBarBottomAxis->Max;
*/
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::FormCreate(TObject *Sender){
	InitializeStringGridScanDate();
	InitializeChart();
	InitializeStringGridFiles();
	//InitScrollBarBottomAxis();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::InitializeStringGridFiles(){
	StringGridFiles->Cells[0][0] = "Дата создания";
	StringGridFiles->Cells[1][0] = "Файл";

	UpdateColWidths();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::FormShow(TObject *Sender){
	UpdateListViewScanDate();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::BuildChart(double scanDate){
	sqlite3_stmt *stmtSelect;

    double eps = sysTime::SEC2DAY;

	std::stringstream ss;
	ss.str("");
	ss.precision(std::numeric_limits<double>::digits10);
	ss << "SELECT created FROM file";
	ss << " WHERE scan_date > " << (scanDate - eps);
	ss << " AND scan_date < " << (scanDate + eps);

	sqlite3_prepare(db, ss.str().c_str(), ss.str().size(), &stmtSelect, NULL);

	int created;
	std::map<double, int>createdCount;

	while (sqlite3_step(stmtSelect) != SQLITE_DONE){
		created = sqlite3_column_double(stmtSelect, 0);
		if (createdCount.find(created) == createdCount.end()){
			createdCount[created] = 0;
		}
		createdCount[created] += 1;
	}

	sqlite3_finalize(stmtSelect);

	Series1->Clear();

	double minY = 0;
	double maxY = 0;

	for (std::map<double, int>::iterator it = createdCount.begin(); it != createdCount.end(); ++it){
		Series1->AddXY(it->first, it->second);

		if (maxY < it->second) {
			maxY = it->second;
		}
	}

	//inc maxY value by 10%
	if (maxY <= 20) {
		maxY = 20;
	}
	maxY += maxY * 0.1;

	Chart1->LeftAxis->SetMinMax(minY, maxY);

	wchar_t wTitle[255];
	TDateTime dt;
	dt.Val = scanDate;
	String sScanDate = dt.FormatString("yyyy-mm-dd");
	swprintf(wTitle, L"Количество найденных файлов за %s", sScanDate.c_str());
	Chart1->Title->Text->SetText(wTitle);

	UpdateLeftLabelAxis();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::UpdateLeftLabelAxis(){
	int maxY = Chart1->LeftAxis->Maximum;

	Chart1->LeftAxis->Items->Clear();
	//draw 10 horizontal grids
	int step = maxY / 20;
	for (int y = 0; y < maxY; y += step) {
		Chart1->LeftAxis->Items->Add(y, String(IntToStr(y)));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::BuildStringGridFiles(double scanDate) {
	sqlite3_stmt *stmtSelect;

    double eps = sysTime::SEC2DAY;

	std::stringstream ss;
	ss.str("");
	ss.precision(std::numeric_limits<double>::digits10);

	ss << "SELECT count(1) FROM file";
	ss << " WHERE scan_date > " << (scanDate - eps);
	ss << " AND   scan_date < " << (scanDate + eps);

	sqlite3_prepare(db, ss.str().c_str(), ss.str().size(), &stmtSelect, NULL);

	sqlite3_step(stmtSelect);

	int count = sqlite3_column_int(stmtSelect, 0);

	sqlite3_finalize(stmtSelect);

	//Prepare row count for string grid
	if (count) {
		StringGridFiles->RowCount = count + 1;
	} else {
    	StringGridFiles->RowCount = 2;
	}

	ss.str("");
	ss << "SELECT id, path, created, scan_date  FROM file";
	ss << " WHERE scan_date > " << (scanDate - eps);
	ss << " AND scan_date < " << (scanDate + eps);

	sqlite3_prepare(db, ss.str().c_str(), ss.str().size(), &stmtSelect, NULL);

	int id;
	char *path;
	double created;
	double scan_date;
	int itFile = 0;
	TDateTime dtCreated;

	while (sqlite3_step(stmtSelect) != SQLITE_DONE){
		id = sqlite3_column_int(stmtSelect, 0);
		path = sqlite3_column_text(stmtSelect, 1);
		created = sqlite3_column_double(stmtSelect, 2);

		dtCreated.Val = created;

		StringGridFiles->Cells[0][itFile + 1] = dtCreated.FormatString("yyyy.mm.dd hh:nn:ss");
		StringGridFiles->Cells[1][itFile + 1] = String(path);

		++itFile;
	}

	sqlite3_finalize(stmtSelect);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::StringGridScanDateSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect){
	if (!StringGridScanDate->Cells[ACol][ARow].IsEmpty()){
		String sScanDate = StringGridScanDate->Cells[0][ARow];
		DWORD year, month, day;
        DWORD hour, min, sec;
		swscanf(sScanDate.c_str(), L"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
		TDateTime dt(year, month, day, hour, min, sec, 0);

		BuildChart(dt.Val);

		//update StringGridFiles
		BuildStringGridFiles(dt.Val);

		//update found files caption
		LabelCount->Caption = String(L"Количество: ") + IntToStr(StringGridFiles->RowCount);
	} else {
		Series1->Clear();
		Chart1->Title->Text->SetText(L"Укажите дату сканирования");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::Chart1Zoom(TObject *Sender)
{
	//calculate min max Y value for current zoom
	double min = Chart1->BottomAxis->Minimum;
	double max = Chart1->BottomAxis->Maximum;

	int size = Series1->Count();
	double x, y;
	double minY = 0;
	double maxY = 0;
	for (int i = 0; i < size; i++) {
		x = Series1->XValue[i];
		y = Series1->YValue[i];

		if (min < x && x < max) {
			maxY = (maxY < y)? y: maxY;
		}
	}

	//inc maxY value by 10%
	if (maxY <= 20) {
		maxY = 20;
	}
	maxY += maxY * 0.1;

	Chart1->LeftAxis->SetMinMax(minY, maxY);

	UpdateLeftLabelAxis();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::FormResize(TObject *Sender) {
	UpdateColWidths();
}
//---------------------------------------------------------------------------


void __fastcall TFormChart::Chart1UndoZoom(TObject *Sender) {
	UpdateLeftLabelAxis();
}
//---------------------------------------------------------------------------

