/*
	richedi��һЩ���õĺ���
*/


void RichEdit_SetDefFont(ITextServices * pTextServices, LPCTSTR lpFontName,		// ����Ĭ������
	int nFontSize,	COLORREF clrText, BOOL bBold, 
	BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);

DWORD RichEdit_GetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

IRichEditOle* RichEdit_GetOleInterface(ITextServices * pTextServices);

void RichEdit_GetText(ITextServices * pTextServices, tString& strText);	

int RichEdit_GetWindowTextLength(ITextServices * pTextServices);

int RichEdit_GetTextRange(ITextServices * pTextServices, CHARRANGE * lpchrg, tString& strText);

int RichEdit_SetSel(ITextServices * pTextServices, LONG nStartChar, LONG nEndChar);

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText,	// �滻ѡ���ı�
	LPCTSTR lpFontName, int nFontSize,	COLORREF clrText, 
	BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink, 
	int nStartIndent, BOOL bCanUndo = FALSE);

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);

void RichEdit_GetSel(ITextServices * pTextServices, LONG& nStartChar, LONG& nEndChar);

void RichEdit_SetFont(ITextServices * pTextServices, LPCTSTR lpFontName, int nFontSize,	// ��������
	COLORREF clrText, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);

BOOL RichEdit_SetStartIndent(ITextServices * pTextServices, int nSize);			// ����������(��λ:�)

DWORD RichEdit_GetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetLinePacing(ITextServices* pTextServices, int nLinePace);