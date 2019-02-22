// STXDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "STX.h"
#include "STXDlg.h"
#include "zlib.h"
#include <sys/stat.h>
#include ".\stxdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	STX_MEMORY			1048576
#define	STX_UNCOMPRESSED	16777216
unsigned char m_cFile[STX_MEMORY];
unsigned char m_zData[STX_UNCOMPRESSED];

#define STX_VERSION		"0.98"


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	// Cargando variables localizadas
	CString sTitle;

	sTitle.LoadString(IDS_ABOUT_DIALOG);
	SetWindowText(sTitle);

	sTitle.LoadString(IDS_LABEL_STX2DM);
	sTitle += STX_VERSION;
	SetDlgItemText(IDC_LABEL_STX2DM,sTitle);

	sTitle.LoadString(IDS_LABEL_ALDONIO);
	SetDlgItemText(IDC_LABEL_ALDONIO,sTitle);

	sTitle.LoadString(IDS_LABEL_THANKS);
	SetDlgItemText(IDC_LABEL_THANKS,sTitle);

	sTitle.LoadString(IDS_OK);
	SetDlgItemText(IDOK,sTitle);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cuadro de diálogo de CSTXDlg



CSTXDlg::CSTXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSTXDlg::IDD, pParent)
{
	m_bChecked = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSTXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSTXDlg, CDialog)
	ON_MESSAGE(WM_DROPFILES, OnDropFiles)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnBnClickedOK)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_FILENAMES, OnBnClickedButtonDefaultFilenames)
	ON_BN_CLICKED(IDC_BUTTON_KIU, OnBnClickedButtonKIU)
	ON_BN_CLICKED(IDC_BUTTON_DM, OnBnClickedButtonDM)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_CRAZY, OnBnClickedButtonCrazy)
	ON_BN_CLICKED(IDC_BROWSE_STX, OnBnClickedBrowseSTX)
	ON_BN_CLICKED(IDC_BROWSE_KSF, OnBnClickedBrowseKSF)
	ON_BN_CLICKED(IDC_CONVERTFILE, OnBnClickedConvertToKSF)
	ON_BN_CLICKED(IDC_BATCHMODE, OnBnClickedBatchmode)
END_MESSAGE_MAP()


// Controladores de mensaje de CSTXDlg

BOOL CSTXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ZeroMemory(&m_cFile, STX_MEMORY);
	ZeroMemory(&m_zData, STX_UNCOMPRESSED);

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUT_DIALOG debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUT_DIALOG & 0xFFF0) == IDM_ABOUT_DIALOG);
	ASSERT(IDM_ABOUT_DIALOG < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->InsertMenu(0, MF_BYPOSITION, MF_SEPARATOR);
			pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_ABOUT_DIALOG, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// Cargando variables localizadas
	CString sTitle;

	sTitle.LoadString(IDS_STX_DIALOG);
	sTitle += STX_VERSION;
	SetWindowText(sTitle);

	sTitle.LoadString(IDS_LABEL_STX);
	SetDlgItemText(IDC_LABEL_STX,sTitle);

	//sTitle.LoadString(IDS_STX_FILE);
	//SetDlgItemText(IDC_STX_FILE,sTitle);

	sTitle.LoadString(IDS_BROWSE_STX);
	SetDlgItemText(IDC_BROWSE_STX,sTitle);

	sTitle.LoadString(IDS_LABEL_KSFFOLDER);
	SetDlgItemText(IDC_LABEL_KSFFOLDER,sTitle);

	//sTitle.LoadString(IDS_KSF_FOLDER);
	//SetDlgItemText(IDC_KSF_FOLDER,sTitle);

	sTitle.LoadString(IDS_BROWSE_KSF);
	SetDlgItemText(IDC_BROWSE_KSF,sTitle);

	sTitle.LoadString(IDS_TITLE_MODES);
	SetDlgItemText(IDC_TITLE_MODES,sTitle);

	sTitle.LoadString(IDS_BUTTON_DEFAULT_FILENAMES);
	SetDlgItemText(IDC_BUTTON_DEFAULT_FILENAMES,sTitle);

	sTitle.LoadString(IDS_BUTTON_KIU);
	SetDlgItemText(IDC_BUTTON_KIU,sTitle);

	sTitle.LoadString(IDS_BUTTON_DM);
	SetDlgItemText(IDC_BUTTON_DM,sTitle);

	sTitle.LoadString(IDS_BUTTON_SELECTALL);
	SetDlgItemText(IDC_BUTTON_SELECTALL,sTitle);

	sTitle.LoadString(IDS_BUTTON_CRAZY);
	SetDlgItemText(IDC_BUTTON_CRAZY,sTitle);

	sTitle.LoadString(IDS_CHECK_PRACTICE);
	SetDlgItemText(IDC_CHECK_PRACTICE,sTitle);

	sTitle.LoadString(IDS_CHECK_NORMAL);
	SetDlgItemText(IDC_CHECK_NORMAL,sTitle);

	sTitle.LoadString(IDS_CHECK_HARD);
	SetDlgItemText(IDC_CHECK_HARD,sTitle);

	sTitle.LoadString(IDS_CHECK_NIGHTMARE);
	SetDlgItemText(IDC_CHECK_NIGHTMARE,sTitle);

	sTitle.LoadString(IDS_CHECK_CRAZY);
	SetDlgItemText(IDC_CHECK_CRAZY,sTitle);

	sTitle.LoadString(IDS_CHECK_FULLDOUBLE);
	SetDlgItemText(IDC_CHECK_FULLDOUBLE,sTitle);

	sTitle.LoadString(IDS_CHECK_HALFDOUBLE);
	SetDlgItemText(IDC_CHECK_HALFDOUBLE,sTitle);

	sTitle.LoadString(IDS_CHECK_DIVISION);
	SetDlgItemText(IDC_CHECK_DIVISION,sTitle);

	sTitle.LoadString(IDS_CHECK_LIGHTMAP);
	SetDlgItemText(IDC_CHECK_LIGHTMAP,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_PRACTICE);
	SetDlgItemText(IDC_DIFFICULTY_PRACTICE,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_NORMAL);
	SetDlgItemText(IDC_DIFFICULTY_NORMAL,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_HARD);
	SetDlgItemText(IDC_DIFFICULTY_HARD,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_NIGHTMARE);
	SetDlgItemText(IDC_DIFFICULTY_NIGHTMARE,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_CRAZY);
	SetDlgItemText(IDC_DIFFICULTY_CRAZY,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_FULLDOUBLE);
	SetDlgItemText(IDC_DIFFICULTY_FULLDOUBLE,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_HALFDOUBLE);
	SetDlgItemText(IDC_DIFFICULTY_HALFDOUBLE,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_DIVISION);
	SetDlgItemText(IDC_DIFFICULTY_DIVISION,sTitle);

	sTitle.LoadString(IDS_DIFFICULTY_LIGHTMAP);
	SetDlgItemText(IDC_DIFFICULTY_LIGHTMAP,sTitle);

	sTitle.LoadString(IDS_FILENAME_PRACTICE);
	SetDlgItemText(IDC_FILENAME_PRACTICE,sTitle);

	sTitle.LoadString(IDS_FILENAME_NORMAL);
	SetDlgItemText(IDC_FILENAME_NORMAL,sTitle);

	sTitle.LoadString(IDS_FILENAME_HARD);
	SetDlgItemText(IDC_FILENAME_HARD,sTitle);

	sTitle.LoadString(IDS_FILENAME_NIGHTMARE);
	SetDlgItemText(IDC_FILENAME_NIGHTMARE,sTitle);

	sTitle.LoadString(IDS_FILENAME_CRAZY);
	SetDlgItemText(IDC_FILENAME_CRAZY,sTitle);

	sTitle.LoadString(IDS_FILENAME_FULLDOUBLE);
	SetDlgItemText(IDC_FILENAME_FULLDOUBLE,sTitle);

	sTitle.LoadString(IDS_FILENAME_HALFDOUBLE);
	SetDlgItemText(IDC_FILENAME_HALFDOUBLE,sTitle);

	sTitle.LoadString(IDS_FILENAME_DIVISION);
	SetDlgItemText(IDC_FILENAME_DIVISION,sTitle);

	sTitle.LoadString(IDS_FILENAME_LIGHTMAP);
	SetDlgItemText(IDC_FILENAME_LIGHTMAP,sTitle);

	sTitle.LoadString(IDS_TITLE_STXINFO);
	SetDlgItemText(IDC_TITLE_STXINFO,sTitle);

	sTitle.LoadString(IDS_LABEL_ARTIST);
	SetDlgItemText(IDC_LABEL_ARTIST,sTitle);

	sTitle.LoadString(IDS_TEXT_ARTIST);
	SetDlgItemText(IDC_TEXT_ARTIST,sTitle);

	sTitle.LoadString(IDS_LABEL_SONG);
	SetDlgItemText(IDC_LABEL_SONG,sTitle);

	sTitle.LoadString(IDS_TEXT_SONG);
	SetDlgItemText(IDC_TEXT_SONG,sTitle);

	sTitle.LoadString(IDS_LABEL_AUTHOR);
	SetDlgItemText(IDC_LABEL_AUTHOR,sTitle);

	sTitle.LoadString(IDS_TEXT_AUTHOR);
	SetDlgItemText(IDC_TEXT_AUTHOR,sTitle);

	sTitle.LoadString(IDS_TITLE_OPTIONS);
	SetDlgItemText(IDC_TITLE_OPTIONS,sTitle);

	sTitle.LoadString(IDS_CHECK_OVERWRITE);
	SetDlgItemText(IDC_CHECK_OVERWRITE,sTitle);

	sTitle.LoadString(IDS_CHECK_SYNCRO);
	SetDlgItemText(IDC_CHECK_SYNCRO,sTitle);

	sTitle.LoadString(IDS_BUTTON_ADV_OPTIONS);
	SetDlgItemText(IDC_BUTTON_ADV_OPTIONS,sTitle);

	sTitle.LoadString(IDS_CONVERTFILE);
	SetDlgItemText(IDC_CONVERTFILE,sTitle);

	sTitle.LoadString(IDS_BATCHMODE);
	SetDlgItemText(IDC_BATCHMODE,sTitle);

	CheckDlgButton(IDC_CHECK_PRACTICE,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_NORMAL,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_HARD,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_CRAZY,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_DIVISION,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_LIGHTMAP,BST_UNCHECKED);

	CheckDlgButton(IDC_CHECK_OVERWRITE,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_SYNCRO,BST_UNCHECKED);

	DragAcceptFiles();
	
	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

LRESULT CSTXDlg::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	char szDroppedFile[MAX_PATH];
	HDROP hDrop;
	int nFiles;
	hDrop = (HDROP)wParam;
	nFiles = DragQueryFile( hDrop, 0, szDroppedFile, MAX_PATH );
	LoadSTX( szDroppedFile, true );
	return 0;
}

void CSTXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUT_DIALOG)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono. Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CSTXDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CSTXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSTXDlg::OnBnClickedOK()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CSTXDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSTXDlg::OnBnClickedButtonDefaultFilenames()
{
	CString sTitle;

	// Cargar nombres de archivo por default.
	sTitle.LoadString(IDS_FILENAME_PRACTICE);
	SetDlgItemText(IDC_FILENAME_PRACTICE,sTitle);
	sTitle.LoadString(IDS_FILENAME_NORMAL);
	SetDlgItemText(IDC_FILENAME_NORMAL,sTitle);
	sTitle.LoadString(IDS_FILENAME_HARD);
	SetDlgItemText(IDC_FILENAME_HARD,sTitle);
	sTitle.LoadString(IDS_FILENAME_NIGHTMARE);
	SetDlgItemText(IDC_FILENAME_NIGHTMARE,sTitle);
	sTitle.LoadString(IDS_FILENAME_CRAZY);
	SetDlgItemText(IDC_FILENAME_CRAZY,sTitle);
	sTitle.LoadString(IDS_FILENAME_FULLDOUBLE);
	SetDlgItemText(IDC_FILENAME_FULLDOUBLE,sTitle);
	sTitle.LoadString(IDS_FILENAME_HALFDOUBLE);
	SetDlgItemText(IDC_FILENAME_HALFDOUBLE,sTitle);
	sTitle.LoadString(IDS_FILENAME_DIVISION);
	SetDlgItemText(IDC_FILENAME_DIVISION,sTitle);
	sTitle.LoadString(IDS_FILENAME_LIGHTMAP);
	SetDlgItemText(IDC_FILENAME_LIGHTMAP,sTitle);

	// Actualizar checkboxes

	bool bInvalid = true;

	for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
	{
		CString sDiff;
		GetDlgItemText(IDC_DIFFICULTY_PRACTICE+i, sDiff);

		if( atoi(sDiff) == 0 )
		{
			CheckDlgButton(IDC_CHECK_PRACTICE+i,BST_UNCHECKED);
			continue;
		}

		bInvalid = false;
		CheckDlgButton(IDC_CHECK_PRACTICE+i,BST_CHECKED);
	}

	if( bInvalid )
	{
		CheckDlgButton(IDC_CHECK_PRACTICE,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_NORMAL,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_HARD,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_CRAZY,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_DIVISION,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_LIGHTMAP,BST_UNCHECKED);
	}
}

void CSTXDlg::OnBnClickedButtonKIU()
{
	CString sTitle;

	// Cargar nombres de archivo por default.
	sTitle.LoadString(IDS_KIU_FILENAME_NORMAL);
	SetDlgItemText(IDC_FILENAME_NORMAL,sTitle);
	sTitle.LoadString(IDS_KIU_FILENAME_HARD);
	SetDlgItemText(IDC_FILENAME_HARD,sTitle);
	sTitle.LoadString(IDS_KIU_FILENAME_NIGHTMARE);
	SetDlgItemText(IDC_FILENAME_NIGHTMARE,sTitle);
	sTitle.LoadString(IDS_KIU_FILENAME_CRAZY);
	SetDlgItemText(IDC_FILENAME_CRAZY,sTitle);
	sTitle.LoadString(IDS_KIU_FILENAME_FULLDOUBLE);
	SetDlgItemText(IDC_FILENAME_FULLDOUBLE,sTitle);
	sTitle.LoadString(IDS_KIU_FILENAME_HALFDOUBLE);
	SetDlgItemText(IDC_FILENAME_HALFDOUBLE,sTitle);
}

void CSTXDlg::OnBnClickedButtonDM()
{
	CString sTitle;

	sTitle.LoadString(IDS_DM_FILENAME_PRACTICE);
	SetDlgItemText(IDC_FILENAME_PRACTICE,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_NORMAL);
	SetDlgItemText(IDC_FILENAME_NORMAL,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_HARD);
	SetDlgItemText(IDC_FILENAME_HARD,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_NIGHTMARE);
	SetDlgItemText(IDC_FILENAME_NIGHTMARE,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_CRAZY);
	SetDlgItemText(IDC_FILENAME_CRAZY,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_FULLDOUBLE);
	SetDlgItemText(IDC_FILENAME_FULLDOUBLE,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_HALFDOUBLE);
	SetDlgItemText(IDC_FILENAME_HALFDOUBLE,sTitle);
	sTitle.LoadString(IDS_DM_FILENAME_DIVISION);
	SetDlgItemText(IDC_FILENAME_DIVISION,sTitle);
}

void CSTXDlg::OnBnClickedButtonSelectAll()
{
	bool bChecked = m_bChecked;

	for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
	{
		if( IsDlgButtonChecked(IDC_CHECK_PRACTICE+i) && bChecked )
		{
			m_bChecked = true;
			continue;
		}
		else if( !IsDlgButtonChecked(IDC_CHECK_PRACTICE+i) && !bChecked )
		{
			m_bChecked = false;
			continue;
		}

		if( bChecked )
			m_bChecked = false;

		break;
	}

	if( m_bChecked )
	{
		CheckDlgButton(IDC_CHECK_PRACTICE,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_NORMAL,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_HARD,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_CRAZY,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_DIVISION,BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_LIGHTMAP,BST_UNCHECKED);
		m_bChecked = false;
	}
	else
	{
		CheckDlgButton(IDC_CHECK_PRACTICE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_NORMAL,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_HARD,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_CRAZY,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_DIVISION,BST_CHECKED);
		CheckDlgButton(IDC_CHECK_LIGHTMAP,BST_CHECKED);
		m_bChecked = true;
	}
}

void CSTXDlg::OnBnClickedButtonCrazy()
{
	CheckDlgButton(IDC_CHECK_PRACTICE,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_NORMAL,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_HARD,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_CRAZY,BST_CHECKED);
	CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_DIVISION,BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_LIGHTMAP,BST_UNCHECKED);
}

void CSTXDlg::OnBnClickedBrowseSTX()
{
	CString sTitle, sFilter;
	sTitle.LoadString(IDS_FILE_STX);
	sFilter = sTitle + "|*.stx|";
	// TODO: Habilitar Todos los archivos
	//sTitle.LoadString(IDS_FILE_ALL);
	//sFilter += sTitle + "|*.*|";

	const char* cStepFilter = (LPCTSTR)sFilter;

	GetDlgItemText(IDC_STX_FILE,sTitle);
	CFileDialog FileDlg(TRUE, "*.stx", sTitle, OFN_FILEMUSTEXIST|OFN_EXPLORER, cStepFilter);
	if( FileDlg.DoModal() == IDOK )
		LoadSTX( FileDlg.GetPathName(), true );
}

void CSTXDlg::LoadSTX( CString sPath, bool bSameDir, bool bLoadToDlg )
{
	CString sTitle;
	sTitle = sPath;
	CFile fSTX;
	{
		bool bCantOpen = fSTX.Open(sTitle, CFile::modeRead) == FALSE,
			bNotSTX = sTitle.Right(4).MakeLower() != ".stx";

		if( bCantOpen || bNotSTX )
		{
			if( bLoadToDlg )
			{
				SetDlgItemText(IDC_STX_FILE,"");
				SetDlgItemText(IDC_KSF_FOLDER,"");

				sTitle.LoadString(IDS_TEXT_ARTIST);
				SetDlgItemText(IDC_TEXT_ARTIST,sTitle);
				sTitle.LoadString(IDS_TEXT_SONG);
				SetDlgItemText(IDC_TEXT_SONG,sTitle);
				sTitle.LoadString(IDS_TEXT_AUTHOR);
				SetDlgItemText(IDC_TEXT_AUTHOR,sTitle);

				sTitle.LoadString(IDS_DIFFICULTY_PRACTICE);
				SetDlgItemText(IDC_DIFFICULTY_PRACTICE,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_NORMAL);
				SetDlgItemText(IDC_DIFFICULTY_NORMAL,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_HARD);
				SetDlgItemText(IDC_DIFFICULTY_HARD,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_NIGHTMARE);
				SetDlgItemText(IDC_DIFFICULTY_NIGHTMARE,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_CRAZY);
				SetDlgItemText(IDC_DIFFICULTY_CRAZY,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_FULLDOUBLE);
				SetDlgItemText(IDC_DIFFICULTY_FULLDOUBLE,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_HALFDOUBLE);
				SetDlgItemText(IDC_DIFFICULTY_HALFDOUBLE,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_DIVISION);
				SetDlgItemText(IDC_DIFFICULTY_DIVISION,sTitle);
				sTitle.LoadString(IDS_DIFFICULTY_LIGHTMAP);
				SetDlgItemText(IDC_DIFFICULTY_LIGHTMAP,sTitle);

				if( bCantOpen )
				{
					sTitle.LoadString(IDS_ERROR_CANTOPEN);
					AfxMessageBox(sTitle, MB_ICONERROR);
				}
				else if( bNotSTX )
				{
					sTitle.LoadString(IDS_ERROR_INVALIDSTX);
					AfxMessageBox(sTitle, MB_ICONERROR);
				}
			}
			return;
		}
	}

	sTitle = sPath;
	SetDlgItemText(IDC_STX_FILE,sTitle);

	if( bSameDir )
		sTitle = sTitle.Left(sTitle.GetLength() - fSTX.GetFileName().GetLength());
	else
		sTitle = sTitle.Left(sTitle.GetLength()-4) + "\\";

	SetDlgItemText(IDC_KSF_FOLDER,sTitle);

	{
		ZeroMemory(&m_cFile, STX_MEMORY);
		UINT nBytesRead = fSTX.Read(m_cFile,STX_MEMORY);
		stx::header_t* stxHeader = reinterpret_cast<stx::header_t*>(m_cFile);

		if( stricmp(stxHeader->mArtist, "") == 0 )
		{
			sTitle.LoadString(IDS_TEXT_ARTIST);
			SetDlgItemText(IDC_TEXT_ARTIST,sTitle);
		}
		else
			SetDlgItemText(IDC_TEXT_ARTIST,stxHeader->mArtist);

		if( stricmp(stxHeader->mTitle, "") == 0 )
		{
			sTitle.LoadString(IDS_TEXT_SONG);
			SetDlgItemText(IDC_TEXT_SONG,sTitle);
		}
		else
			SetDlgItemText(IDC_TEXT_SONG,stxHeader->mTitle);

		if( stricmp(stxHeader->mStepAuthor, "") == 0 )
		{
			sTitle.LoadString(IDS_TEXT_AUTHOR);
			SetDlgItemText(IDC_TEXT_AUTHOR,sTitle);
		}
		else
			SetDlgItemText(IDC_TEXT_AUTHOR,stxHeader->mStepAuthor);

		for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
		{
			stx::track_t* stxTrack = (stx::track_t*)(m_cFile + stxHeader->mOffsets[i]);
			itoa((int)stxTrack->mDifficulty, sTitle.GetBuffer(), 10);
			SetDlgItemText(IDC_DIFFICULTY_PRACTICE+i, sTitle);

			//if( bLoadToDlg )
			//{
			//	bool bValid = stxTrack->mDifficulty != 0;
			//	CheckDlgButton(IDC_CHECK_PRACTICE+i, bValid ? BST_CHECKED : BST_UNCHECKED);
			//}
		}

		//if( bLoadToDlg )
		//{
		//	bool bInvalid = true;

		//	for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
		//	{
		//		if( !IsDlgButtonChecked(IDC_CHECK_PRACTICE+i) )
		//			continue;
		//		bInvalid = false;
		//		break;
		//	}

		//	if( bInvalid )
		//	{
		//		CheckDlgButton(IDC_CHECK_NORMAL,BST_CHECKED);
		//		CheckDlgButton(IDC_CHECK_HARD,BST_CHECKED);
		//		CheckDlgButton(IDC_CHECK_NIGHTMARE,BST_CHECKED);
		//		CheckDlgButton(IDC_CHECK_CRAZY,BST_CHECKED);
		//		CheckDlgButton(IDC_CHECK_FULLDOUBLE,BST_CHECKED);
		//		CheckDlgButton(IDC_CHECK_HALFDOUBLE,BST_CHECKED);
		//	}
		//}
	}

	fSTX.Close();
}

INT CALLBACK BrowseFolderCallback(HWND hwnd, 
                               UINT uMsg,
                               LPARAM lp, 
                               LPARAM pData) 
{
	TCHAR szDir[MAX_PATH];

   if(uMsg == BFFM_INITIALIZED)
      if (GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR), szDir))
         SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
   return 0;
}

void CSTXDlg::OnBnClickedBrowseKSF()
{
	BrowseFolder();
}

bool CSTXDlg::BrowseFolder( bool bKSF )
{
	CString sTitle;
	GetDlgItemText(IDC_KSF_FOLDER,sTitle);
	m_pidl = NULL;
	PCWSTR pSTR;

	if( sTitle != "" )
	{
try_again:
#ifdef _UNICODE 
		pSTR = LPCTSTR(sTitle);
#else
		USES_CONVERSION;
		pSTR = A2CW(LPCTSTR(sTitle));
#endif
		SHParseDisplayName(pSTR, NULL, &m_pidl, 0, 0);
		if( m_pidl == NULL )
		{
			while( sTitle.Right(1) == "\\" )
				sTitle = sTitle.Left( sTitle.GetLength()-1 );

			if( sTitle.FindOneOf("\\") != -1 )
			{
				sTitle.MakeReverse();
				sTitle = sTitle.Right(sTitle.GetLength()-sTitle.FindOneOf("\\"));
				sTitle.MakeReverse();
				goto try_again;
			}
		}
	}

	BROWSEINFO bi;
    ZeroMemory(&bi,	sizeof(bi));
    TCHAR szPath[MAX_PATH];
    ZeroMemory(&szPath,	sizeof(szPath));

    bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	if( bKSF )
	{
		sTitle.LoadString(IDS_BROWSE);
		bi.lpszTitle = _T(sTitle);
	}
	else
	{
		sTitle.LoadString(IDS_BROWSEFORSTX);
		bi.lpszTitle = _T(sTitle);
	}
    bi.ulFlags = BIF_USENEWUI|BIF_VALIDATE;
    bi.lpfn = BrowseFolderCallback;

    m_pidl = SHBrowseForFolder(&bi);

    if (NULL != m_pidl)
    {
         BOOL bRet = SHGetPathFromIDList(m_pidl,szPath);
         if(FALSE == bRet)
              return false;
		sTitle = szPath;
		sTitle += "\\";
		SetDlgItemText(IDC_KSF_FOLDER,sTitle);
		return true;
    }
	return false;
}

int CSTXDlg::ParseSTX( bool bFromToDlg )
{
	CString sFile, sDlg;
	stx::header_t* stxHeader = reinterpret_cast<stx::header_t*>(m_cFile);
	bool bConverted = true, bNoConverted = true, bRemoved = false;
	for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
	{
		if( IsDlgButtonChecked(IDC_CHECK_PRACTICE+i) )
		{
			CString sDir;
			GetDlgItemText(IDC_KSF_FOLDER,sDir);
			sFile = sDir;
			GetDlgItemText(IDC_FILENAME_PRACTICE+i,sDir);
			sFile += sDir;

			{
				struct stat St;
				if( stat( sFile, &St ) == 0 && !IsDlgButtonChecked(IDC_CHECK_OVERWRITE) )
				{
					bConverted = false;
					continue;
				}
			}

			FILE* ksfOUT = fopen(sFile, "w");

			if( ksfOUT == NULL )
			{
				if( bFromToDlg )
				{
					sDlg.LoadString(IDS_ERROR_CANTSAVE);
					sDlg += sDir;
					AfxMessageBox(sDlg, MB_ICONERROR);
				}
				bConverted = false;
				continue;
			}

			if( !bConverted )
				bNoConverted = false;

			bool bHasSteps = false;
			bool bIsLightMap = i == stx::TR_LIGHTS;

			stx::track_t* stxTrack = (stx::track_t*) (m_cFile + stxHeader->mOffsets[i]);
			stx::packed_block_t* stxBlocks = reinterpret_cast<stx::packed_block_t*>(stxTrack+1);
			{
				{
					ZeroMemory(&m_zData,STX_UNCOMPRESSED);
					unsigned long lUnzipstreamSize = STX_UNCOMPRESSED;
					int result = uncompress(m_zData,&lUnzipstreamSize,reinterpret_cast<unsigned char*>(stxBlocks+1), stxBlocks->mSize);
					if (result != 0)
					{
						bConverted = false;
						continue;
					}
					stx::block_t* stxBlock = reinterpret_cast<stx::block_t*>(m_zData);

					float fBPM = stxBlock->mBpm;
					unsigned uTick = stxBlock->mBeatSplit;
					unsigned uSpeed = stxBlock->mSpeed;
					AjustarBPMyTick( fBPM, uTick, uSpeed );

					// #TITLE:;
					GetDlgItemText(IDC_TEXT_SONG,sDlg);
					fprintf( ksfOUT, "#TITLE:%s;\n", sDlg );

					// #ARTIST:;
					GetDlgItemText(IDC_TEXT_ARTIST,sDlg);
					fprintf( ksfOUT, "#ARTIST:%s;\n", sDlg );

					// #STEPMAKER:;
					GetDlgItemText(IDC_TEXT_AUTHOR,sDlg);
					fprintf( ksfOUT, "#STEPMAKER:Andamiro Co., Ltd.;\n#MUSICINTRO:%s;\n", sDlg );

					// #BPM:;
					fprintf( ksfOUT, "#BPM:%.3f;\n", fBPM );

					// #DIFFICULTY:;
					if( !bIsLightMap )
					{
						GetDlgItemText(IDC_DIFFICULTY_PRACTICE+i,sDlg);

						if( atoi(sDlg) > 0 )
							fprintf( ksfOUT, "#DIFFICULTY:%d;\n", atoi(sDlg) );
					}

					// #TICKCOUNT:;
					fprintf( ksfOUT, "#TICKCOUNT:%u;\n", uTick );

					// #MEASURE:;
					if( stxBlock->mBeatPerMeasure != 4 )
						fprintf( ksfOUT, "#MEASURE:%d;\n", stxBlock->mBeatPerMeasure );

					// #STARTTIME:;
					if( bIsLightMap || stxBlock->mDelay < 0 ) // HACK: StepMania (LightMap) AND DirectMove (Delay < 0) are ugly =/
						fprintf( ksfOUT, "#STARTTIME:0;\n" );
					else
						fprintf( ksfOUT, "#STARTTIME:%d;\n", stxBlock->mDelay );

					// #SPEED:;
					if( !bIsLightMap )
						if( uSpeed != 1000 )
							fprintf( ksfOUT, "#SPEED:%.3f;\n", float(uSpeed)/1000.f );

					// #PLAYER:DOUBLE;
					switch(i)
					{
					case stx::TR_PRACTICE:
					case stx::TR_NORMAL:
					case stx::TR_HARD:
					case stx::TR_CRAZY:
					case stx::TR_DIVISION:
						fprintf( ksfOUT, "#PLAYER:SINGLE;\n" );
						break;
					case stx::TR_NIGHTMARE:
					case stx::TR_FULLDOUBLE:
					case stx::TR_HALFDOUBLE:
						fprintf( ksfOUT, "#PLAYER:DOUBLE;\n" );
						break;
					case stx::TR_LIGHTS:
						fprintf( ksfOUT, "#PLAYER:LIGHTS;\n" );
						break;
					}

					// #BGAFILE:;
					if( bFromToDlg )
					{
						CString sDir, sBGAFile;
						GetDlgItemText(IDC_KSF_FOLDER,sDir);

						bool bFinishLoop = false;
						CFile fBGA;
						int iTry = 0;
next_try:
						switch( iTry )
						{
							case 0:		goto try_1;			break;
							case 1:		goto try_2;			break;
							case 2:		goto try_3;			break;
							default:	goto finish_loop;	break;
						}
try_1:
						sBGAFile = sDir;
						sDlg = "SONG.avi";
						goto try_bga;
try_2:
						sBGAFile = sDir;
						sDlg = "SONG.mpg";
						goto try_bga;
try_3:
						sBGAFile = sDir;
						sDlg = "SONG.mpeg";
						goto try_bga;
finish_loop:
						bFinishLoop = true;
try_bga:
						if( !bFinishLoop )
						{
							sBGAFile += sDlg;

							if( fBGA.Open(sBGAFile, CFile::modeRead) != FALSE )
							{
								fprintf( ksfOUT, "#BGAFILE:%s;\n", sDlg );
								fBGA.Close();
							}
							else
							{
								iTry++;
								goto next_try;
							}
						}
					}

					// #STEP:;
					fprintf( ksfOUT, "#STEP:\n" );
				}

				float fBPM;
				unsigned uMeasure, uTick, uSpeed;

				//unsigned uCMult, uNS[3];
				//int iLMult, iSMult;

				bHasSteps = false;

				for(int iBlock = 0; iBlock<50 && stxTrack->mBlocks[iBlock]; ++iBlock)
				{
					ZeroMemory(&m_zData,STX_UNCOMPRESSED);
					unsigned long lUnzipstreamSize = STX_UNCOMPRESSED;
					int result = uncompress(m_zData,&lUnzipstreamSize,reinterpret_cast<unsigned char*>(stxBlocks+1), stxBlocks->mSize);

					if (result != 0)
						continue;

					stx::block_t* stxBlock = reinterpret_cast<stx::block_t*>(m_zData);

					if( iBlock > 0 )
					{
						float fBPMA = stxBlock->mBpm;
						unsigned uTickA = stxBlock->mBeatSplit,
							uSpeedA = stxBlock->mSpeed;

						bool bAjustado = AjustarBPMyTick( fBPMA, uTickA, uSpeedA );

						if( fBPM != fBPMA )
						{
							fprintf( ksfOUT, "|B%.3f|\n", fBPMA );
							fBPM = fBPMA;
						}

						if( uMeasure != stxBlock->mBeatPerMeasure )
						{
							fprintf( ksfOUT, "|U%d|\n", stxBlock->mBeatPerMeasure );
							uMeasure = stxBlock->mBeatPerMeasure;
						}

						if( uTick != uTickA )
						{
							fprintf( ksfOUT, "|T%u|\n", uTickA );
							uTick = uTickA;
						}

						if( stxBlock->mDelay != 0 )
							fprintf( ksfOUT, "|D%d|\n", stxBlock->mDelay * 10 );

						if( uSpeed != uSpeedA )
						{
							fprintf( ksfOUT, "|S%.3f%s|\n", float(uSpeedA)/1000.f, bAjustado ? ",0" : "" );
							uSpeed = uSpeedA;
						}

						//if( stxBlock->mSplitDelay != 0 )
						//	fprintf( ksfOUT, "|E%d|\n", stxBlock->mSplitDelay * 10 );

						//if( uCMult != stxBlock->mComboMultiplier || iLMult != stxBlock->mLifeMultiplier || iSMult != stxBlock->mScoreMultiplier )
						//{
						//	if( stxBlock->mComboMultiplier == (int)abs( stxBlock->mLifeMultiplier/1000.f ) &&
						//		stxBlock->mComboMultiplier == (int)abs( stxBlock->mScoreMultiplier/1000.f ) )
						//			fprintf( ksfOUT, "|M%d|\n", stxBlock->mComboMultiplier );
						//	else
						//	{
						//		fprintf( ksfOUT, "|M" );

						//		bool bComma = false;
						//		if( uCMult != stxBlock->mComboMultiplier )
						//		{
						//			fprintf( ksfOUT, "%dC", stxBlock->mComboMultiplier );
						//			uCMult = stxBlock->mComboMultiplier;
						//			bComma = true;
						//		}

						//		if( iLMult != stxBlock->mLifeMultiplier )
						//		{
						//			if( bComma )
						//				fprintf( ksfOUT, "," );

						//			fprintf( ksfOUT, "%.3fL", float(stxBlock->mLifeMultiplier)/1000.f );
						//			iLMult = stxBlock->mLifeMultiplier;
						//			bComma = true;
						//		}

						//		if( iSMult != stxBlock->mScoreMultiplier )
						//		{
						//			if( bComma )
						//				fprintf( ksfOUT, "," );

						//			fprintf( ksfOUT, "%.3fS", float(stxBlock->mScoreMultiplier)/1000.f );
						//			iSMult = stxBlock->mScoreMultiplier;
						//		}

						//		fprintf( ksfOUT, "|\n" );
						//	}
						//}

						//{
						//	int iNS = 0;
						//	for( int n=2; n>=0; n-- )
						//	{
						//		if( uNS[n] != stxBlock->mNoteSkin[n] )
						//		{
						//			iNS = n + 1;
						//			break;
						//		}
						//	}

						//	if( iNS > 0 )
						//	{
						//		fprintf( ksfOUT, "|N" );

						//		switch( iNS )
						//		{
						//		case 3:
						//			fprintf( ksfOUT, "%d,", stxBlock->mNoteSkin[iNS-3] );
						//		case 2:
						//			fprintf( ksfOUT, "%d,", stxBlock->mNoteSkin[iNS-2] );
						//		case 1:
						//			fprintf( ksfOUT, "%d", stxBlock->mNoteSkin[iNS-1] );
						//		}

						//		fprintf( ksfOUT, "|\n" );

						//		for( int n=0; n<3; n++ )
						//			uNS[n] = stxBlock->mNoteSkin[n];
						//	}
						//}
					}
					else
					{
						// HACK: DirectMove is ugly =/
						if( stxBlock->mDelay != 0 && bIsLightMap || stxBlock->mDelay < 0 )
							fprintf( ksfOUT, "|D%d|\n", stxBlock->mDelay * 10 );

						fBPM = stxBlock->mBpm;
						uMeasure = stxBlock->mBeatPerMeasure;
						uTick = stxBlock->mBeatSplit;
						uSpeed = stxBlock->mSpeed;

						AjustarBPMyTick( fBPM, uTick, uSpeed );

						//uCMult = stxBlock->mComboMultiplier;
						//iLMult = stxBlock->mLifeMultiplier;
						//iSMult = stxBlock->mScoreMultiplier;
						//for( int n=0; n<3; n++ )
						//	uNS[n] = stxBlock->mNoteSkin[n];
					}

					stx::row_t* stxRows = reinterpret_cast<stx::row_t*>(stxBlock+1);

					for (unsigned iRow = 0; iRow < stxBlock->mBlockLength; ++iRow)
					{
						sDlg.Empty();
						for(int iCol = 0; iCol<13; ++iCol)
						{
							int iSyncro = iCol;

							switch(i)
							{
							case stx::TR_LIGHTS:
							case stx::TR_HALFDOUBLE:
							case stx::TR_FULLDOUBLE:
							case stx::TR_NIGHTMARE:
								break;
							default:
								if( IsDlgButtonChecked(IDC_CHECK_SYNCRO) && ( iCol > 4 && iCol < 10 ) )
									iSyncro -= 5;
							}

							switch(i)
							{
							case stx::TR_PRACTICE:
							case stx::TR_NORMAL:
							case stx::TR_HARD:
							case stx::TR_CRAZY:
							case stx::TR_DIVISION:
								if( iSyncro >= 5 )
								{
									sDlg += "0";
									continue;
								}
								break;
							case stx::TR_NIGHTMARE:
							case stx::TR_FULLDOUBLE:
								if( iSyncro >= 10 )
								{
									sDlg += "0";
									continue;
								}
								break;
							case stx::TR_HALFDOUBLE:
								if( iSyncro < 2 || iSyncro > 7 )
								{
									sDlg += "0";
									continue;
								}
								break;
							case stx::TR_LIGHTS:
								if( iSyncro < 10 )
								{
									sDlg += "0";
									continue;
								}
								break;
							}

							if( bIsLightMap )
							{
								switch( stxRows[iRow].mRow[iSyncro] )
								{
								case 0:		sDlg += "0";						break;
								default:	sDlg += "1";	bHasSteps = true;	break;
								}
							}
							else
							{
								switch( stxRows[iRow].mRow[iSyncro] )
								{
								case 3:		sDlg += "W";	bHasSteps = true;	break;
								case 2:		sDlg += "G";	bHasSteps = true;	break;
								case 1:		sDlg += "1";	bHasSteps = true;	break;
								case 10:	sDlg += "2";	bHasSteps = true;	break;
								case 12:	sDlg += "3";	bHasSteps = true;	break;
								case 11:	sDlg += "4";	bHasSteps = true;	break;
								case 6:		sDlg += "C";	bHasSteps = true;	break;
								case 5:		sDlg += "B";	bHasSteps = true;	break;
								case 4:		sDlg += "A";	bHasSteps = true;	break;
								case 0:		sDlg += "0";						break;
								default:	sDlg += "1";	bHasSteps = true;	break;
								}
							}
						}
						fprintf( ksfOUT, "%s\n", sDlg );
					}

					for(unsigned iSubBlock = 0; iSubBlock < stxTrack->mBlocks[iBlock]; ++iSubBlock)
						stxBlocks = stxBlocks->Next();
				}
			}
			fprintf( ksfOUT, ";\n" );
			fclose( ksfOUT );

			if( !bHasSteps )
			{
				ksfOUT = fopen(sFile, "wD");
				fclose( ksfOUT );
				bRemoved = true;
			}
		}
	}

	if( bConverted )
	{
		if( bRemoved )
			return 3;
		else
			return 1;
	}
	else if( !bNoConverted )
	{
		if( bRemoved )
			return 2;
		else
			return 0;
	}
	else
		return -1;
}

void CSTXDlg::OnBnClickedConvertToKSF()
{
	CString sTitle, sFile;

	GetDlgItemText(IDC_STX_FILE,sFile);
	if( sFile == "" )
	{
		sTitle.LoadString(IDS_ERROR_NOTSTX);
		AfxMessageBox(sTitle, MB_ICONERROR);
		return;
	}

	GetDlgItemText(IDC_KSF_FOLDER,sFile);
	if( sFile == "" )
	{
		sTitle.LoadString(IDS_ERROR_NOFOLDER);
		AfxMessageBox(sTitle, MB_ICONERROR);
		return;
	}

	{
		struct stat St;
		if( stat( sFile, &St ) != 0 )
			CreateDirectory( sFile, NULL );
	}

	bool bNoDiff = true;
	for( int i=stx::TR_PRACTICE; i<stx::TR_COUNT; i++ )
	{
		if( IsDlgButtonChecked(IDC_CHECK_PRACTICE+i) )
		{
			GetDlgItemText(IDC_FILENAME_PRACTICE+i,sFile);
			if( sFile == "" || sFile.Right(4).MakeLower() != ".ksf" )
			{
				sTitle.LoadString(IDS_ERROR_EMPTYFILENAME);
				AfxMessageBox(sTitle, MB_ICONERROR);
				return;
			}
			bNoDiff = false;
		}
	}

	if( bNoDiff )
	{
		sTitle.LoadString(IDS_ERROR_NOMODES);
		AfxMessageBox(sTitle, MB_ICONERROR);
		return;
	}

	GetDlgItemText(IDC_TEXT_ARTIST,sFile);
	if( sFile == "" )
	{
		sTitle.LoadString(IDS_ERROR_NOARTIST);
		AfxMessageBox(sTitle, MB_ICONERROR);
		return;
	}

	GetDlgItemText(IDC_TEXT_SONG,sFile);
	if( sFile == "" )
	{
		sTitle.LoadString(IDS_ERROR_NOSONG);
		AfxMessageBox(sTitle, MB_ICONERROR);
		return;
	}

	bool bRemoved = false;
	switch( ParseSTX() )
	{
	case 3:
		bRemoved = true;
	case 1:
		sTitle.LoadString(IDS_SUCCESS);
		if( bRemoved ) sTitle += "\nEmpty steps were automatically removed.";
		AfxMessageBox(sTitle, MB_ICONINFORMATION);
		break;
	case 2:
		bRemoved = true;
	case 0:
		sTitle.LoadString(IDS_HALF_SUCCESS);
		if( bRemoved ) sTitle += "\nEmpty steps were automatically removed.";
		AfxMessageBox(sTitle, MB_ICONWARNING);
		break;
	case -1:
		sTitle.LoadString(IDS_NO_SUCCESS);
		AfxMessageBox(sTitle, MB_ICONERROR);
		break;
	}
}

void CSTXDlg::OnBnClickedBatchmode()
{
	if( BrowseFolder( false ) )
	{
		CString sFolder;
		GetDlgItemText(IDC_KSF_FOLDER,sFolder);
		sFolder += "*.stx";

		TCHAR scPath[MAX_PATH];
		_tcscpy(scPath,sFolder);
		GetDlgItemText(IDC_KSF_FOLDER,sFolder);

		WIN32_FIND_DATA fData;
		HANDLE hFind = FindFirstFile(scPath,&fData);

		if( hFind == INVALID_HANDLE_VALUE )
		{
			sFolder.LoadString(IDS_ERROR_NOFOLDERSTX);
			AfxMessageBox(sFolder, MB_ICONERROR);
		}
		else
		{
			do {
				CString sFile = sFolder, sSaveTo;
				sFile += fData.cFileName;

				LoadSTX( sFile, false, false );

				GetDlgItemText(IDC_KSF_FOLDER,sSaveTo);
				struct stat St;
				if( stat( sSaveTo, &St ) != 0 )
					CreateDirectory( sSaveTo, NULL );

				ParseSTX( false );

				theApp.PumpMessage();
			} while(FindNextFile(hFind,&fData));
			FindClose(hFind);

			sFolder.LoadString(IDS_SUCCESS);
			AfxMessageBox(sFolder, MB_ICONINFORMATION);
		}
	}
}

bool CSTXDlg::AjustarBPMyTick( float& fBPM, unsigned& uTick, unsigned& uSpeed )
{
	if( uSpeed == 0 )
		uSpeed = 1000;

	float fBPMxTick = fBPM * uTick;

	// StepMania max. Tickcount
	unsigned uRPB = 192;

	if( uRPB % uTick == 0 )
		return false;

	unsigned uNearUp = uTick, uNearDown = uTick, uNearDown2;

	while( uRPB % uNearUp != 0 )
		uNearUp = (unsigned)ceil((float)uRPB/floor((float)uRPB/uNearUp));

	while( uRPB % uNearDown != 0 )
		uNearDown = (unsigned)floor((float)uRPB/ceil((float)uRPB/uNearDown));

	if( uNearDown % 3 == 0 )
		uNearDown2 = uNearDown * 2/3;
	else
		uNearDown2 = uNearDown;

	unsigned uUpDiff, uDownDiff, uDownDiff2;
	uDownDiff = uTick - uNearDown;
	uUpDiff = uNearUp - uTick;
	uDownDiff2 = uTick - uNearDown2;

	if( uUpDiff <= uDownDiff && uUpDiff <= uDownDiff2 )
		uTick = uNearUp;
	else if( uDownDiff2 < uDownDiff )
		uTick = uNearDown2;
	else
		uTick = uNearDown;

	float fOriginalBPM = fBPM;
	fBPM = fBPMxTick / uTick;
	uSpeed = unsigned(uSpeed * (fBPM/fOriginalBPM));

	return true;
}