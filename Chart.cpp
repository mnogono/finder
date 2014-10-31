//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TFormChart *FormChart;
//---------------------------------------------------------------------------
__fastcall TFormChart::TFormChart(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

