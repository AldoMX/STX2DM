// STX.cpp : define los comportamientos de las clases para la aplicación.
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

// Construcción de CSTXApp

CSTXApp::CSTXApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}

// El único objeto CSTXApp
CSTXApp theApp;

// Inicialización de CSTXApp

BOOL CSTXApp::InitInstance()
{
	// Windows XP requiere InitCommonControls() si un manifiesto de 
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CSTXDlg dlg;
	m_pMainWnd = &dlg;
	DragAcceptFiles(dlg, true);
	INT_PTR nResponse = dlg.DoModal();

	// Dado que el cuadro de diálogo se ha cerrado, devolver FALSE para salir
	//  de la aplicación en vez de iniciar el suministro de mensajes de dicha aplicación.
	return FALSE;
}
