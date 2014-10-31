//---------------------------------------------------------------------------

#ifndef ChartH
#define ChartH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormChart : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TListView *ListView1;
private:	// User declarations
public:		// User declarations
	__fastcall TFormChart(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormChart *FormChart;
//---------------------------------------------------------------------------
#endif

