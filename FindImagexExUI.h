//---------------------------------------------------------------------------

#ifndef FindImagexExUIH
#define FindImagexExUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <vector>
#include <string>
#include <DBXJSON.hpp>
#include <winnt.h>
#include <jpeg.hpp>

#include "sysApp.h"
#include "sysVCL.h"
#include "sysFile.h"
#include "sysFind.h"
#include "sqlite3.h"
#include "logger.h"

//---------------------------------------------------------------------------
struct TFile{
	double created;
	std::wstring sCreated;
	std::wstring sFilePath;

    TFile() {}

	TFile(std::wstring sCreated, std::wstring sFilePath, double created) {
		this->sCreated = sCreated;
		this->sFilePath = sFilePath;
        this->created = created;
	}
};

//---------------------------------------------------------------------------
class TFinderThread : public TThread, sysFile::IScanIterator {
	private:
    	//std::wstring   scanLogFile;
        //std::ofstream *scanLogStream;
        //char sBuffer[MAX_PATH];
	public:
		LONG volatile isWorking;
		LONG volatile isFinished;
		LONG volatile lScanFiles;

    	sysFind::TXMLSettings *xmlSettings;

    	std::vector<TFile> findFilesBuffer;

        __fastcall virtual ~TFinderThread(void);

    	TFinderThread(bool CreateSuspended, sysFind::TXMLSettings *xmlSettings);

        //void SetLogFile(const wchar_t *wLogFile);

		virtual void __fastcall Execute(void);

        void StartWorking();

        void DoWork();

        void StopWorking();

        void AbortWorking();

        bool ScanIterateCallback(const wchar_t *folder, const WIN32_FIND_DATAW &ffd, void *bFilePass = NULL);
};
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel3;
	TLabel *Label1;
	TButton *ButtonIncludeAdd;
	TEdit *EditIncludeFolder;
	TStringGrid *StringGridExcludes;
	TPanel *Panel4;
	TLabel *Label2;
	TEdit *EditExcludeFolder;
	TButton *ButtonExcludeAdd;
	TStringGrid *StringGridIncludes;
	TPanel *Panel5;
	TLabel *Label3;
	TPanel *Panel6;
	TLabel *Label123;
	TEdit *EditExtensions;
	TPanel *Panel8;
	TLabel *Label5;
	TEdit *EditLogFile;
	TSaveDialog *SaveDialog1;
	TDateTimePicker *DateTimePickerCreated;
	TPopupMenu *PopupMenuInclude;
	TMenuItem *DeleteInclude;
	TPopupMenu *PopupMenuExclude;
	TMenuItem *DeleteExclude;
	TStatusBar *StatusBar;
	TPanel *Panel9;
	TButton *ButtonStartSearch;
	TButton *ButtonStopSearch;
	TTimer *TimerSyncFindBuffer;
	TPopupMenu *PopupMenuStringGrid;
	TMenuItem *PMOpenFolder;
	TMenuItem *PMOpenFile;
	TImage *Image1;
	TButton *ButtonExcludeDel;
	TButton *ButtonIncludeDel;
    TStringGrid *StringGridResults;
	TMainMenu *MainMenu1;
	TMenuItem *MMChart;
	void __fastcall ButtonStartSearchClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonIncludeAddClick(TObject *Sender);
	void __fastcall ButtonExcludeAddClick(TObject *Sender);
	void __fastcall DeleteIncludeClick(TObject *Sender);
	void __fastcall DeleteExcludeClick(TObject *Sender);
	void __fastcall Splitter1Moved(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall ButtonStopSearchClick(TObject *Sender);
	void __fastcall TimerSyncFindBufferTimer(TObject *Sender);
	void __fastcall PMOpenFileClick(TObject *Sender);
	void __fastcall PMOpenFolderClick(TObject *Sender);
	void __fastcall StringGridResultsSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall ButtonExcludeDelClick(TObject *Sender);
	void __fastcall ButtonIncludeDelClick(TObject *Sender);
	void __fastcall ButtonSaveLogFileClick(TObject *Sender);
	void __fastcall MMChartClick(TObject *Sender);
private:	// User declarations
	void __fastcall PrepareStatments();
public:		// User declarations
	std::vector<TFile> findFilesBuffer;
	TFinderThread *finderThread;
	sysFind::TXMLSettings xmlSettings;

    sqlite3 *db;
    sqlite3_stmt *stmtInsertFile;
    sqlite3_stmt *stmtDeleteFile;

	__fastcall TForm2(TComponent* Owner);
    void __fastcall SaveUserSettings();
    void __fastcall LoadUserSettings();
    void __fastcall UpdateColWidths();
    void __fastcall InitializeFinderThread();
	void __fastcall InitializeStringGridResults();
	void __fastcall ParseCommandLine(wchar_t *commandLine);
    void __fastcall SaveResultsToLogFile(const String &file);
    void __fastcall InitializeSQLiteDB();
    void __fastcall CloseDB();
    void __fastcall DeleteDBFile(const TFile *file, double scanDate);
    void __fastcall AddDBFile(const TFile *file, double scanDate);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif

