// STX.cpp : define los comportamientos de las clases para la aplicaci�n.
//
#include "stdafx.h"
#include "STX.h"
#include "STXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSTXApp

BEGIN_MESSAGE_MAP(CSTXApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// Construcci�n de CSTXApp

CSTXApp::CSTXApp()
{
	// TODO: agregar aqu� el c�digo de construcci�n,
	// Colocar toda la inicializaci�n importante en InitInstance
}

// El �nico objeto CSTXApp
CSTXApp theApp;

// Inicializaci�n de CSTXApp

BOOL CSTXApp::InitInstance()
{
	// Windows XP requiere InitCommonControls() si un manifiesto de 
	// aplicaci�n especifica el uso de ComCtl32.dll versi�n 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generar� un error al crear ventanas.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CSTXDlg dlg;
	m_pMainWnd = &dlg;
	DragAcceptFiles(dlg, true);
	INT_PTR nResponse = dlg.DoModal();

	// Dado que el cuadro de di�logo se ha cerrado, devolver FALSE para salir
	//  de la aplicaci�n en vez de iniciar el suministro de mensajes de dicha aplicaci�n.
	return FALSE;
}
