// STXDlg.h: archivo de encabezado
//

#pragma once


// Cuadro de di�logo de CSTXDlg
class CSTXDlg : public CDialog
{
// Construcci�n
public:
	CSTXDlg(CWnd* pParent = NULL);	// Constructor est�ndar

// Datos del cuadro de di�logo
	enum { IDD = IDD_STX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementaci�n
	LRESULT OnDropFiles(WPARAM wParam,LPARAM lParam);
	bool BrowseFolder( bool bKSF = true );
	void LoadSTX( CString sPath, bool bSameDir, bool bLoadToDlg = true );
	int ParseSTX( bool bFromToDlg = true );
	bool AjustarBPMyTick( float& fBPM, unsigned& uTick, unsigned& uSpeed );
protected:
	HICON m_hIcon;
	bool m_bChecked;
	LPITEMIDLIST m_pidl;

	// Funciones de asignaci�n de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDefaultFilenames();
	afx_msg void OnBnClickedButtonKIU();
	afx_msg void OnBnClickedButtonDM();
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonCrazy();
	afx_msg void OnBnClickedBrowseSTX();
	afx_msg void OnBnClickedBrowseKSF();
	afx_msg void OnBnClickedConvertToKSF();
	afx_msg void OnBnClickedBatchmode();
};
