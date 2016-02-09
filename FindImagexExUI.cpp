// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FindImagexExUI.h"
#include <FileCtrl.hpp>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;

CRITICAL_SECTION csSycnFindBuffer;
HANDLE hStartFinder;
HANDLE hFinderThreadTerminated;

__fastcall TFinderThread::~TFinderThread(void){
}

// ---------------------------------------------------------------------------
TFinderThread::TFinderThread(bool CreateSuspended, sysFind::TXMLSettings *xmlSettings) : TThread(CreateSuspended){
	this->xmlSettings = xmlSettings;

	isWorking = 0;
	isFinished = 0;
	lScanFiles = 0;
}

// ---------------------------------------------------------------------------
void TFinderThread::DoWork(){
	sysFind::Find(*xmlSettings, *this);
}

// ---------------------------------------------------------------------------
void TFinderThread::StartWorking(){
	InterlockedExchange(&isWorking, 1);
	InterlockedExchange(&lScanFiles, 0);
}

// ---------------------------------------------------------------------------
void TFinderThread::StopWorking(){
	InterlockedExchange(&isWorking, 0);
	InterlockedExchange(&isFinished, 1);
}

// ---------------------------------------------------------------------------
void TFinderThread::AbortWorking(){
	for (size_t itOperation = 0; itOperation < xmlSettings->operations.size(); ++itOperation){
		xmlSettings->operations[itOperation].AbortOperation();
	}
}

// ---------------------------------------------------------------------------
bool TFinderThread::ScanIterateCallback(const wchar_t *folder, const WIN32_FIND_DATAW &ffd, void *bFilePass){
	InterlockedIncrement(&lScanFiles);

	if (bFilePass){
		String sFilePath = String(folder) + String(ffd.cFileName);

		EnterCriticalSection(&csSycnFindBuffer);

		SYSTEMTIME st;
		FileTimeToSystemTime(&ffd.ftCreationTime, &st);
		wchar_t wDateBuffer[11];
		int r = GetDateFormat(LOCALE_INVARIANT, NULL, &st, L"yyyy-MM-dd", wDateBuffer, 11);
		int le = GetLastError();
        TDateTime dt = sysTime::SystemTimeToDateTime(&st);
		findFilesBuffer.push_back(TFile(wDateBuffer, sFilePath.c_str(), dt.Val));

		LeaveCriticalSection(&csSycnFindBuffer);
	}

	return false;
}

// ---------------------------------------------------------------------------
void __fastcall TFinderThread::Execute(){
	while (!Terminated){
    	DWORD result;
        result = WaitForSingleObject(hStartFinder, 1000);
		if (result == WAIT_TIMEOUT){
			continue;
		} else if (result == WAIT_OBJECT_0){
            StartWorking();

            DoWork();

            StopWorking();
        }
	}

	SetEvent(hFinderThreadTerminated);
}

// ---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent *Owner) : TForm(Owner){}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStartSearchClick(TObject *Sender){
	if (InterlockedCompareExchange(&finderThread->isWorking, finderThread->isWorking, finderThread->isWorking) == 1){
		MessageBox(NULL, L"Поск все еще выплняется", L"Сообщение", MB_OK);
	} else {

		InitializeStringGridResults();

		xmlSettings.Clear();

		std::vector<std::wstring>extensions;
		sysStr::Split(EditExtensions->Text.c_str(), L",", extensions);

		sysFind::TXMLOperation operation;
		operation.entity = sysFind::TXMLOperation::Entity::_file;
		operation.type = sysFind::TXMLOperation::Operation::_find;

		for (int itRow = 0; itRow < StringGridIncludes->RowCount; ++itRow){
			String sFolder = StringGridIncludes->Cells[0][itRow];
			if (sFolder.IsEmpty()){
				continue;
			}

			sysFind::TXMLInclude xmlInclude;
			xmlInclude.folder = sFolder;

			operation.includes.push_back(xmlInclude);
		}

		if (operation.includes.size() == 0){
			MessageBox(NULL, L"Не указано где искать файлы", L"Сообщение", MB_OK);
			return;
		}

		for (int itRow = 0; itRow < StringGridExcludes->RowCount; ++itRow){
			String sFolder = StringGridExcludes->Cells[0][itRow];
			if (sFolder.IsEmpty()){
				continue;
			}

			sysFind::TXMLExclude xmlExclude;
			xmlExclude.folder = sFolder;

			operation.excludes.push_back(xmlExclude);
		}

		sysFind::TXMLCriteria criteriaCreated;
		criteriaCreated.name = sysFind::TXMLCriteria::Criteria::_created;
		SYSTEMTIME st = sysTime::DateTimeToSystemTime(DateTimePickerCreated->DateTime);
		criteriaCreated.msecCreatedLocalTime = sysTime::SystemTimeToMSec(st);
		criteriaCreated.group = sysFind::TXMLCriteria::Group::_and;
		criteriaCreated.op = sysFind::TXMLCriteria::Operation::_lt;

		operation.criteriesAnd.push_back(criteriaCreated);

		for (size_t itExt = 0; itExt < extensions.size(); ++itExt){
			sysFind::TXMLCriteria criteriaExt;
			criteriaExt.name = sysFind::TXMLCriteria::Criteria::_ext;
			criteriaExt.ext = String(extensions[itExt].c_str());
			criteriaExt.group = sysFind::TXMLCriteria::Group::_or;
			criteriaExt.op = sysFind::TXMLCriteria::Operation::_eq;

			operation.criteriesOr.push_back(criteriaExt);
		}

		sysFind::TXMLCriteria criteriaFileName;
		criteriaFileName.name = sysFind::TXMLCriteria::Criteria::_name;
		criteriaFileName.fileName = "парол";
		criteriaFileName.op = sysFind::TXMLCriteria::Operation::_like;
		criteriaFileName.group = sysFind::TXMLCriteria::Group::_or;

		operation.criteriesOr.push_back(criteriaFileName);

		if (operation.criteriesOr.size() == 0){
			MessageBox(NULL, L"Не указаны расширения файлов для поиска", L"Сообщение", MB_OK);
			return;
		}

		xmlSettings.operations.push_back(operation);

		StartFinder();
	}
}

// ---------------------------------------------------------------------------
wchar_t *StringGridToString(TStringGrid *stringGrid){
	std::vector<std::wstring>rows;
	for (int itRow = 0; itRow < stringGrid->RowCount; ++itRow){
		rows.push_back(std::wstring(stringGrid->Cells[0][itRow].c_str()));
	}

	return sysStr::JoinStrings(rows);
}

// ---------------------------------------------------------------------------
void StringToStringGrid(wchar_t *wStringGrid, TStringGrid *stringGrid){
	std::vector<std::wstring>rows;
	sysStr::Split(wStringGrid, L",", rows);
	stringGrid->RowCount = rows.size() + 1;
	for (size_t itRow = 0; itRow < rows.size(); ++itRow){
		stringGrid->Cells[0][itRow] = rows[itRow].c_str();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::LoadUserSettings(){
	EditIncludeFolder->Text = sysApp::GetSettingString(L"EditIncludeFolder", L"");

	std::vector<std::wstring>drivers = sysFile::GetDrives();
	std::wstring sDrivers = sysStr::JoinString(drivers.begin(), drivers.end(), L",");

	String sStringGridIncludes = sysApp::GetSettingString(L"StringGridIncludes", sDrivers.c_str());
	StringToStringGrid(sStringGridIncludes.c_str(), StringGridIncludes);

	EditExcludeFolder->Text = sysApp::GetSettingString(L"EditExcludeFolder", L"");

	String sStringGridExcludes = sysApp::GetSettingString(L"StringGridExcludes", L"");
	StringToStringGrid(sStringGridExcludes.c_str(), StringGridExcludes);

	// date filter search 2 or more days file age
	TDateTime dt;
	dt.Val = Now().Val - 2;
	DateTimePickerCreated->DateTime = dt;

	EditExtensions->Text = sysApp::GetSettingString(L"EditExtensions", L"bmp,jpg,jpeg,pdf,tiff");

	EditLogFolder->Text = sysApp::GetSettingString(L"EditLogFolder", L"C:\\ПнД");

	int ApplicationWidth = sysApp::GetSettingInt(L"ApplicationWidth", Form2->Width);
	int ApplicationHeight = sysApp::GetSettingInt(L"ApplicationHeight", Form2->Height);
	Form2->Width = ApplicationWidth;
	Form2->Height = ApplicationHeight;
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::SaveUserSettings(){
	sysApp::SetSetting(L"EditIncludeFolder", EditIncludeFolder->Text.c_str());

	wchar_t *wStringGridIncludes = StringGridToString(StringGridIncludes);
	sysApp::SetSetting(L"StringGridIncludes", wStringGridIncludes);
	delete []wStringGridIncludes;

	sysApp::SetSetting(L"EditExcludeFolder", EditExcludeFolder->Text.c_str());

	wchar_t *wStringGridExcludes = StringGridToString(StringGridExcludes);
	sysApp::SetSetting(L"StringGridExcludes", wStringGridExcludes);
	delete[]wStringGridExcludes;

	sysApp::SetSetting(L"EditExtensions", EditExtensions->Text.c_str());

	sysApp::SetSetting(L"EditLogFolder", EditLogFolder->Text.c_str());

	sysApp::SetSetting(L"ApplicationWidth", IntToStr(Width).c_str());
	sysApp::SetSetting(L"ApplicationHeight", IntToStr(Height).c_str());
}

void __fastcall TForm2::CloseDB(){
	sqlite3_finalize(stmtInsertFile);
	sqlite3_finalize(stmtDeleteFile);
    sqlite3_close(db);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action){
	if (InterlockedCompareExchange(&finderThread->isWorking, finderThread->isWorking, finderThread->isWorking)){
		Action = TCloseAction::caNone;
		if (isRunWithCommandLine) {
			finderThread->Terminate();
			finderThread->AbortWorking();
			WaitForSingleObject(hFinderThreadTerminated, 10000);
			Form2->Close();

		} else if (MessageBox(NULL, L"Поиск все еще работает, остановить поиск?", L"Вопрос", MB_YESNO) == ID_YES){
			finderThread->Terminate();
			finderThread->AbortWorking();
			WaitForSingleObject(hFinderThreadTerminated, 10000);
			Form2->Close();
		}
	} else {
		TimerSyncFindBuffer->Enabled = false;
		SaveUserSettings();
		DeleteCriticalSection(&csSycnFindBuffer);
        CloseDB();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::InitializeFinderThread(){
	hStartFinder = CreateEvent(NULL, 0, 0, NULL);
	hFinderThreadTerminated = CreateEvent(NULL, 0, 0, NULL);
	InitializeCriticalSection(&csSycnFindBuffer);

	finderThread = new TFinderThread(false, &xmlSettings);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::InitializeStringGridResults(){
	StringGridResults->RowCount = 2;
	StringGridResults->Cells[0][0] = "Создан";
	StringGridResults->Cells[1][0] = "Файл";

	StringGridResults->Cells[0][1] = "";
	StringGridResults->Cells[1][1] = "";

	dataSourceStringGridResult.clear();
}

void __fastcall TForm2::ParseCommandLine(wchar_t *commandLine){
	int arvc;
	wchar_t **argv = CommandLineToArgvW(commandLine, &arvc);

	if (TLogger::logger->CheckLogLevel(LOG_LEVEL_TRACE)) {
		String log = String(L"arvc = ") + IntToStr(arvc);
		TLogger::logger->Log(log.c_str(), LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
		TLogger::logger->Log(commandLine, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
	}

	if (arvc == 2){
		isRunWithCommandLine = true;
		xmlSettings.LoadFromFile(argv[1]);
		//SetEvent(hStartFinder);
		StartFinder();
	} else {
	    isRunWithCommandLine = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::PrepareStatments() {
	if (db == NULL) {
		return;
    }

    std::stringstream ss;

    //prepare insert file statment
    ss.str("");
    ss << "INSERT INTO file (path, created, scan_date) VALUES (?1, ?2, ?3)";

	sqlite3_prepare(db, ss.str().c_str(), ss.str().size(), &stmtInsertFile, NULL);

    //prepare delete file statment
    ss.str("");
    ss << "DELETE FROM file where path = ?1 and created = ?2 and scan_date = ?3";

    sqlite3_prepare(db, ss.str().c_str(), ss.str().size(), &stmtDeleteFile, NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::InitializeSQLiteDB() {
	db = NULL;
    sqlite3_open("db.db", &db);
    if (db == NULL) {
		return;
    }

    char *err;
    std::stringstream ss;
    ss.str("");

    //create table file
    ss << "CREATE TABLE IF NOT EXISTS file ("
		<< " id        INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL"
        << ",path      VARCHAR(1024)                         NOT NULL"
        << ",created   REAL                                  NOT NULL"
        << ",scan_date REAL                                  NOT NULL);";

    sqlite3_exec(db, ss.str().c_str(), NULL, NULL, &err);
    if (err != NULL) {
		TLogger::logger->Error(ss.str().c_str(), __FILE__, __FUNCTION__, __LINE__);
        TLogger::logger->Error(err, __FILE__, __FUNCTION__, __LINE__);
		sqlite3_free(err);
    }

    /*
    //create table scan
    ss.str("");
    ss << "CREATE TABLE IF NOT EXISTS scan ("
		<< " id       INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL"
        << ",log_file VARCHAR(1024)                         NOT NULL"
        << ",date     REAL                                  NOT NULL);";

    sqlite3_exec(db, ss.str().c_str(), NULL, NULL, &err);
    if (err != NULL) {
        TLogger::logger->Error(ss.str().c_str(), __FILE__, __FUNCTION__, __LINE__);
        TLogger::logger->Error(err, __FILE__, __FUNCTION__, __LINE__);
	    sqlite3_free(err);
    }
	*/

    PrepareStatments();
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::FormCreate(TObject *Sender){
	setlocale(LC_ALL, ".1251");

    TLogger::logger = new TLoggerFile(LOG_LEVEL_DISABLE);

	LoadUserSettings();
	UpdateColWidths();
	InitializeFinderThread();
	InitializeStringGridResults();
    InitializeSQLiteDB();

	ParseCommandLine(GetCommandLine());
}

// ---------------------------------------------------------------------------
void AddFolderToStringGrid(String sFolder, TStringGrid *stringGrid){
	if (sFolder.IsEmpty()){
		return;
	}

	int iRow = sysVCL::FindStringGridRowByCellValue(stringGrid, 0, sFolder);
	if (iRow != -1){
		return;
	}

	stringGrid->Cells[0][stringGrid->RowCount - 1] = sFolder;
	stringGrid->RowCount += 1;
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonIncludeAddClick(TObject *Sender){
	String sIncludeFolder;
	if (SelectDirectory("Include Folder", "", sIncludeFolder, TSelectDirExtOpts() << sdShowEdit, 0)){
		EditIncludeFolder->Text = sIncludeFolder;

		if (EditIncludeFolder->Text.LastDelimiter(L"\\") != EditIncludeFolder->Text.Length()){
			EditIncludeFolder->Text = EditIncludeFolder->Text + String(L"\\");
		}
		AddFolderToStringGrid(EditIncludeFolder->Text, StringGridIncludes);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonExcludeAddClick(TObject *Sender){
	String sExcludeFolder;
	if (SelectDirectory("Exclude Folder", "", sExcludeFolder, TSelectDirExtOpts() << sdShowEdit, 0)){
		EditExcludeFolder->Text = sExcludeFolder;

		if (EditExcludeFolder->Text.LastDelimiter(L"\\") != EditExcludeFolder->Text.Length()){
			EditExcludeFolder->Text = EditExcludeFolder->Text + String(L"\\");
		}

		AddFolderToStringGrid(EditExcludeFolder->Text, StringGridExcludes);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::DeleteIncludeClick(TObject *Sender){
	sysVCL::DeleteStringGridRow(StringGridIncludes, StringGridIncludes->Row);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::DeleteExcludeClick(TObject *Sender){
	sysVCL::DeleteStringGridRow(StringGridExcludes, StringGridExcludes->Row);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::UpdateColWidths(){
	StringGridIncludes->ColWidths[0] = StringGridIncludes->Width - 25;
	StringGridExcludes->ColWidths[0] = StringGridExcludes->Width - 25;
	StringGridResults->ColWidths[1] = StringGridResults->Width - StringGridResults->ColWidths[0] - 25;
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::Splitter1Moved(TObject *Sender){
	UpdateColWidths();
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::FormResize(TObject *Sender){
	UpdateColWidths();
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::Button6Click(TObject *Sender){
	int top = StringGridResults->Selection.Top;
	int bottom = StringGridResults->Selection.Bottom;
	int count = 0;

	if (isRunWithCommandLine) {
		for (int itRow = bottom; itRow >= top; --itRow){
			if (sysFile::DeleteFile(StringGridResults->Cells[1][itRow].c_str())){
				sysVCL::DeleteStringGridRow(StringGridResults, itRow);
				++count;
			}
		}
	} else {
		if (MessageBox(NULL, L"Выбранные файлы будут удалены, продолжить?", L"Удаление", MB_YESNO) == ID_YES){
			for (int itRow = bottom; itRow >= top; --itRow){
				if (sysFile::DeleteFile(StringGridResults->Cells[1][itRow].c_str())){
					sysVCL::DeleteStringGridRow(StringGridResults, itRow);
					++count;
				}
			}

			String msg;
			msg.sprintf(L"Удалено %d файлов", count);
			MessageBox(NULL, msg.c_str(), L"Сообщение", MB_OK);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStopSearchClick(TObject *Sender){
	if (finderThread){
		if (InterlockedCompareExchange(&finderThread->isWorking, finderThread->isWorking, finderThread->isWorking) == 1){
			if (MessageBox(NULL, L"Поиск все еще продолжается, остановить поиск?", L"Вопрос", MB_YESNO) == ID_YES){
				finderThread->AbortWorking();
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::DeleteDBFile(const TFile *file, double scanDate) {
	//delete exisiting file from db
    char *sPath;
    int iPathSize;
	char log[255];
    int errCode;

    sysStr::WideCharToUTF8(
        file->sFilePath.c_str(),
        file->sFilePath.size(),
        sPath,
        iPathSize);


	/*
	prepared statement:
	"DELETE FROM file where path = '?' and created = ? and scan_date = ?";
	*/
    errCode = sqlite3_bind_text(
        stmtDeleteFile,
        1,
        sPath,
        iPathSize,
        NULL);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
    }

    errCode = sqlite3_bind_double(
        stmtDeleteFile,
        2,
		file->created);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
    }

    errCode = sqlite3_bind_double(
        stmtDeleteFile,
        3,
        scanDate);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
    }

    if (sqlite3_step(stmtDeleteFile) != SQLITE_DONE) {
        TLogger::logger->Error("stmtDeleteFile != SQLITE_DONE", __FILE__, __FUNCTION__, __LINE__);
    }

    sqlite3_reset(stmtDeleteFile);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::AddDBFile(const TFile *file, double scanDate) {
    //add new row in db
    char *sPath;
    int iPathSize;
	char log[255];
    int errCode;

    //"INSERT INTO file (path, created, scan_date) VALUES ('?1', ?2, ?3)";
    sysStr::WideCharToUTF8(
        file->sFilePath.c_str(),
        file->sFilePath.size(),
        sPath,
        iPathSize);

    errCode = sqlite3_bind_text(
        stmtInsertFile,
        1,
        sPath,
        iPathSize,
        NULL);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
    }

    errCode = sqlite3_bind_double(
        stmtInsertFile,
        2,
        file->created);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
	}

    errCode = sqlite3_bind_double(
        stmtInsertFile,
        3,
        scanDate);

    if (errCode != SQLITE_OK) {
        sprintf(log, "error code: %d", errCode);
        TLogger::logger->Error(log, __FILE__, __FUNCTION__, __LINE__);
    }

    if (sqlite3_step(stmtInsertFile) != SQLITE_DONE) {
        TLogger::logger->Error("stmtInsertFile != SQLITE_DONE", __FILE__, __FUNCTION__, __LINE__);
    }

    sqlite3_reset(stmtInsertFile);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::TimerSyncFindBufferTimer(TObject *Sender){
	if (!finderThread){
		return;
	}

    std::stringstream ss;

	EnterCriticalSection(&csSycnFindBuffer);

	if (finderThread->findFilesBuffer.size()) {
		//double scanDate;
		//TDateTime dtNow = Now();
        //scanDate = dtNow.Val;

        String sFilePath;
        TFile *file;
        char *errMsg;

        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errMsg);
        if (errMsg != NULL) {
            sqlite3_free(errMsg);
            TLogger::logger->Error(errMsg, __FILE__, __FUNCTION__, __LINE__);
        }

        // display all buffers in String Grid Results
        for (size_t itFile = 0; itFile < finderThread->findFilesBuffer.size(); ++itFile){
			file = &finderThread->findFilesBuffer[itFile];

			dataSourceStringGridResult.push_back(*file);

			StringGridResults->Cells[0][StringGridResults->RowCount - 1] = String(file->sCreated.c_str());
			StringGridResults->Cells[1][StringGridResults->RowCount - 1] = String(file->sFilePath.c_str());
			StringGridResults->RowCount += 1;

			if (db != NULL) {
				AddDBFile(file, scanDateTime);
            }
        }

        sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errMsg);
        if (errMsg != NULL) {
            sqlite3_free(errMsg);
            TLogger::logger->Error(errMsg, __FILE__, __FUNCTION__, __LINE__);
        }

        // Clear finder files buffer
        finderThread->findFilesBuffer.clear();
    }

	LeaveCriticalSection(&csSycnFindBuffer);

	if (InterlockedCompareExchange(&finderThread->isWorking, finderThread->isWorking, finderThread->isWorking) == 0){
		if (InterlockedCompareExchange((LONG *) &finderThread->isFinished, (LONG)false, (LONG)true) == true){
			TDateTime dt(scanDateTime);
			String sDate = dt.FormatString("yyyy.mm.dd hh nn ss");
			String sLogFolder = EditLogFolder->Text;
			String sLogFile = sLogFolder + String(L"\\") + sDate + String(L"_Log.csv");
			if (!sysFile::IsFolderExist(sLogFolder.c_str())) {
				sysFile::CreateFolder(sLogFolder.c_str());
			}
			SaveResultsToLogFile(sLogFile);
			if (isRunWithCommandLine) {
				//delete all find files automatically
				Button5Click(NULL); // select all
				Button6Click(NULL); // delete selected
				Close();

			} else {
				MessageBox(NULL, L"Поиск завершен", L"Сообщение", MB_OK);
			}
		}
	}

	String statusText;
	statusText = statusText.sprintf(L"Файлов проверено: %d Файлов найдено: %d", InterlockedExchange(&finderThread->lScanFiles, finderThread->lScanFiles),
		StringGridResults->RowCount - 2);
	Form2->StatusBar->SimpleText = statusText;
}
// ---------------------------------------------------------------------------

void __fastcall TForm2::PMOpenFileClick(TObject *Sender){
	String sFilePath = StringGridResults->Cells[1][StringGridResults->Row];
	ShellExecute(NULL, L"open", sFilePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
// ---------------------------------------------------------------------------

void __fastcall TForm2::PMOpenFolderClick(TObject *Sender){
	String sFilePath = StringGridResults->Cells[1][StringGridResults->Row];
	sysFile::ShowFileInExplorer(sFilePath.c_str());
}

// ---------------------------------------------------------------------------

void __fastcall TForm2::StringGridResultsSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect){
	String file = StringGridResults->Cells[1][ARow];
	if (file.Pos("Recycle.Bin") > 0){
		return;
	}
	String ext = ExtractFileExt(file);
	if (ext == ".bmp" || ext == ".tif" || ext == ".tiff"){
		Image1->Picture->LoadFromFile(file);
	} else if (ext == ".jpg" || ext == ".jpeg"){
		TJPEGImage *jpeg = new TJPEGImage();
		try {
			jpeg->LoadFromFile(file);
			Image1->Picture->Assign(jpeg);
		} catch (...){}

		delete jpeg;
	} else {
		Image1->Picture = NULL;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm2::Button5Click(TObject *Sender){
	TGridRect selection;
	selection.Top = 1;
	selection.Bottom = StringGridResults->RowCount;
	selection.Left = 0;
	selection.Right = 1;
	StringGridResults->Selection = selection;
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonExcludeDelClick(TObject *Sender){
	sysVCL::DeleteStringGridRow(StringGridExcludes, StringGridExcludes->Row);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonIncludeDelClick(TObject *Sender){
	sysVCL::DeleteStringGridRow(StringGridIncludes, StringGridIncludes->Row);
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::SaveResultsToLogFile(const String &file){
	if (file.IsEmpty()) {
		return;
    }

	char sBuffer[MAX_PATH];

	sysStr::WToMB(file.c_str(), sBuffer, MAX_PATH);

	std::ofstream out(sBuffer, std::ios_base::out);
	if (out.good()){
		for (int itRow = 1; itRow < StringGridResults->RowCount - 1; ++itRow){
			sysStr::WToMB(StringGridResults->Cells[0][itRow].c_str(), sBuffer, MAX_PATH);
			out.write(sBuffer, strlen(sBuffer));
			out.write("    ", 4);
			sysStr::WToMB(StringGridResults->Cells[1][itRow].c_str(), sBuffer, MAX_PATH);
			out.write(sBuffer, strlen(sBuffer));
			out.write("\n", 1);
		}
	}
	out.close();
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::ButtonSaveLogFileClick(TObject *Sender){
	String sLogFolder;
	if (SelectDirectory("Log folder", "", sLogFolder, TSelectDirExtOpts() << sdShowEdit, 0)) {
		EditLogFolder->Text = sLogFolder;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm2::MMChartClick(TObject *Sender){
	FormChart->SetDB(db);
	FormChart->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TForm2::EditFilterChange(TObject *Sender) {
	String filter = EditFilter->Text;
	if (filter.IsEmpty()) {
		if (dataSourceStringGridResult.empty()) {
			StringGridResults->RowCount = 2;
		} else {
			StringGridResults->RowCount = dataSourceStringGridResult.size() + 1;
		}

		TFile *file;
		for (int itFile = 0; itFile < dataSourceStringGridResult.size(); ++itFile) {
			file = &dataSourceStringGridResult[itFile];
			StringGridResults->Cells[0][itFile + 1] = String(file->sCreated.c_str());
			StringGridResults->Cells[1][itFile + 1] = String(file->sFilePath.c_str());
		}
	} else {
		StringGridResults->RowCount = 2;
		StringGridResults->Cells[0][1] = "";
		StringGridResults->Cells[1][1] = "";
		TFile *file;
		for (int itFile = 0; itFile < dataSourceStringGridResult.size(); ++itFile) {
			file = &dataSourceStringGridResult[itFile];
			if (file->sFilePath.find(filter.c_str(), 0) != std::string::npos) {
				StringGridResults->Cells[0][StringGridResults->RowCount - 1] = String(file->sCreated.c_str());
				StringGridResults->Cells[1][StringGridResults->RowCount - 1] = String(file->sFilePath.c_str());

				StringGridResults->RowCount += 1;
			}
		}

		if (StringGridResults->RowCount == 2) {
			StringGridResults->Cells[1][1] = "Ничего не найдено";
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm2::StartFinder(){
	TDateTime dtNow = Now();
	scanDateTime = dtNow.Val;

	SetEvent(hStartFinder);
}

//---------------------------------------------------------------------------
void __fastcall TForm2::MMSaveClick(TObject *Sender) {
	if (SaveDialogSettingsXML->Execute()) {
		String saveSettingsTo = SaveDialogSettingsXML->FileName;

		//save current xml settings to specify file
		TStringBuilder *sb = new TStringBuilder();

		std::vector<std::wstring>extensions;
		sysStr::Split(EditExtensions->Text.c_str(), L",", extensions);

		String sDate = DateTimePickerCreated->DateTime.FormatString("dd.mm.yyyy hh:nn:ss");

		sb->Append(String(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"));
		sb->Append(String(L"<settings>"));
		sb->Append(String(L"<operation type=\"find\" entity=\"file\">"));
		sb->Append(String(L"<criteria name=\"age\" op=\"gt\" value=\""))->Append(sDate)->Append(String(L"\" group=\"and\"/>"));
		for (int itExt = 0; itExt < extensions.size(); ++itExt) {
			sb->Append(String(L"<criteria name=\"ext\" op=\"eq\" value=\""))->Append(String(extensions[itExt].c_str()))->Append(String(L"\" group=\"or\"/>"));
		}

		for (int itRow = 0; itRow < StringGridIncludes->RowCount; ++itRow){
			String sIncludeFolder = StringGridIncludes->Cells[0][itRow];
			if (sIncludeFolder.IsEmpty()){
				continue;
			}
			sb->Append(String(L"<include folder=\""))->Append(sIncludeFolder)->Append(String(L"\"/>"));
		}

		for (int itRow = 0; itRow < StringGridExcludes->RowCount; ++itRow){
			String sExcludeFolder = StringGridExcludes->Cells[0][itRow];
			if (sExcludeFolder.IsEmpty()){
				continue;
			}
			sb->Append(String(L"<exclude folder=\""))->Append(sExcludeFolder)->Append(String(L"\"/>"));
		}

		sb->Append(String(L"</operation>"));
		sb->Append(String(L"</settings>"));

		String sXMLSettings = sb->ToString();

		sysFile::WriteToFileUTF8(saveSettingsTo.c_str(), sXMLSettings.c_str());

		delete sb;
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm2::MMLoadClick(TObject *Sender) {
	if (OpenDialogSettingsXML->Execute()) {
		TXMLDocument *xmlDocument = new TXMLDocument(Application);

		xmlDocument->DOMVendor = GetDOMVendor("MSXML");
		xmlDocument->Active = true;
		xmlDocument->Encoding = "UTF-8";

		xmlDocument->LoadFromFile(OpenDialogSettingsXML->FileName);

		sysFind::TXMLSettings xmlSettings(xmlDocument->DocumentElement);

		for (int itOper = 0; itOper < xmlSettings.operations.size(); ++itOper) {
			sysFind::TXMLOperation &operation = xmlSettings.operations[itOper];
			if (xmlSettings.operations[itOper].type == sysFind::TXMLOperation::Operation::_find) {
				//date time created folder
				std::vector<sysFind::TXMLCriteria> &criteriesAnd = xmlSettings.operations[itOper].criteriesAnd;
				for (int it = 0; it < criteriesAnd.size(); ++it) {
					if (criteriesAnd[it].name == sysFind::TXMLCriteria::Criteria::_created) {
						TDateTime dt;
						dt.Val = criteriesAnd[it].dayCreatedLocalTime;
						DateTimePickerCreated->DateTime = dt;
					}
				}

				//extensions
				std::vector<sysFind::TXMLCriteria> &criteriesOr = xmlSettings.operations[itOper].criteriesOr;
				std::vector<std::wstring> exts;
				for (int it = 0; it < criteriesOr.size(); ++it) {
					if (criteriesOr[it].name == sysFind::TXMLCriteria::Criteria::_ext) {
						exts.push_back(std::wstring(criteriesOr[it].ext.c_str()));
					}
				}
				wchar_t *__str = sysStr::JoinStrings(exts, L",");
				EditExtensions->Text = String(__str);
				delete []__str;

				//include folder
				std::vector<sysFind::TXMLInclude> &includes = operation.includes;
				StringGridIncludes->RowCount = includes.size();
				for (int itInclude = 0; itInclude < includes.size(); ++itInclude) {
					StringGridIncludes->Cells[0][itInclude] = includes[itInclude].folder;
				}

				//exclude folder
				std::vector<sysFind::TXMLExclude> &excludes = operation.excludes;
				StringGridExcludes->RowCount = excludes.size();
				for (int itInclude = 0; itInclude < excludes.size(); ++itInclude) {
					StringGridExcludes->Cells[0][itInclude] = excludes[itInclude].folder;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

