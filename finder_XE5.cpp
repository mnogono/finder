//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("FindImagexExUI.cpp", Form2);
USEFORM("ChartView.cpp", FormChart);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		TStyleManager::TrySetStyle("Amethyst Kamri");
		Application->CreateForm(__classid(TForm2), &Form2);
		Application->CreateForm(__classid(TFormChart), &FormChart);
		int arvc;
		wchar_t **argv = CommandLineToArgvW(GetCommandLine(), &arvc);
		if (arvc == 2) {
			Application->ShowMainForm = false;
		}

		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

