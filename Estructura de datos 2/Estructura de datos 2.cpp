#include <Windows.h>
#include <CommCtrl.h>
#include <shellapi.h>
#include <string>
#include <fstream>
#include "Estructura de datos 2.h"
#define AGREGAR 1
#define ACTUALIZAR 2
using namespace std;
struct registro {
	wchar_t imagen[200];
	SYSTEMTIME fecha;
	wchar_t Nombre[256];
	wchar_t Telefono[256];
	int turno;
	registro* siguiente;
	registro* anterior;

};
registro* inicio = NULL;
wchar_t direccion[200];

int modo = 0;
void agregar(registro* nuevo) {
	if (inicio == NULL) {
		inicio = nuevo;//el siguiente del ultimo debe ser del el primero y el anterior del primero debe ser el ultimo
		nuevo->siguiente = nuevo;
		nuevo->anterior = nuevo;

	}
	else {
		inicio->anterior->siguiente = nuevo;
		nuevo->anterior = inicio->anterior;
		nuevo->siguiente = inicio;
		inicio->anterior = nuevo;

	}

}	

BOOL CALLBACK CallbackClienteconsulta(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{

	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}

	default:
		break;
	}
	return false;
}
BOOL CALLBACK CallbackClienteregistro(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{

	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}

	default:
		break;
	}
	return false;
}
void actualizarListbox(HWND handler) {

	if (inicio == NULL) return;
	SendDlgItemMessage(handler, IDC_LIST1, LB_RESETCONTENT, NULL, NULL);
	registro* aux = inicio;

	while (aux->siguiente != inicio) {
		SendDlgItemMessage(handler, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)aux->Nombre);
		aux = aux->siguiente;
	}
	SendDlgItemMessage(handler, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)aux->Nombre);
}
registro* buscar(int i) {
	registro* aux = inicio;
	for (int j = 0; j < i; j++) {
		aux = aux->siguiente;

	}
	return aux;
}

BOOL CALLBACK CallbackMecanicoconsulta(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{
	case WM_INITDIALOG: {
		//SendDlgItemMessage(handler, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)"Opcion de list");

		break;
	}
	case WM_COMMAND: {
	
	case ID_MODIFICAR_MODIFICARREGISTRO: {
		int seleccionado = SendDlgItemMessage(handler, IDC_LIST1, LB_GETCURSEL, NULL, NULL);
		if (seleccionado == -1) {
			MessageBox(handler, L"No se selecciona nada para actualizar.", L"Error", MB_OK | MB_ICONERROR);
			
		}
		modo = ACTUALIZAR;
		DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), handler, (DLGPROC)CallbackMecanicoconsulta);
		break;
	}
	case IDC_LIST1: {

		if (HIWORD(wParam) == LBN_SELCHANGE) {

			int seleccionado = SendDlgItemMessage(handler, IDC_LIST1, LB_GETCURSEL, NULL, NULL);
			registro* aMostrar = buscar(seleccionado);
			SendDlgItemMessage(handler, nombrecompleto, WM_SETTEXT, NULL, (LPARAM)aMostrar->Nombre);
			SendDlgItemMessage(handler, telefono, WM_SETTEXT, NULL, (LPARAM)aMostrar->Telefono);

			HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, aMostrar->imagen, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(handler, IMAGEN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}
		actualizarListbox(handler);
		break;
	}
	
	

				  break;
	}
	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}

	default:
		break;
	}
	return false;
}
BOOL CALLBACK CallbackMecanico(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			registro* Nuevo = new registro;
			SendDlgItemMessage(handler, NOMBREMECANICO, WM_GETTEXT, sizeof(Nuevo->Nombre) / sizeof(Nuevo->Nombre[0]), (LPARAM)Nuevo->Nombre);
			SendDlgItemMessage(handler, NUMEROMECANICO, WM_GETTEXT, sizeof(Nuevo->Telefono) / sizeof(Nuevo->Telefono[0]), (LPARAM)Nuevo->Telefono);

			Nuevo->turno = SendDlgItemMessage(handler, TURNO, CB_GETCURSEL, 0, 0);
			wcscpy_s(Nuevo->imagen, direccion);

			agregar(Nuevo);


			EndDialog(handler, 0);

			break;
		}
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}

	default:
		break;
	}
	return false;
}


BOOL CALLBACK Callbackprincipal(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_NUEVOREGISTRO_MECANICO: {
			modo = AGREGAR;
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), handler, (DLGPROC)CallbackMecanico);
			break;
		}
		
		case ID_CONSULTA_MECANICO: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), handler, (DLGPROC)CallbackMecanicoconsulta);
			
			break;
		}
		case ID_NUEVOREGISTRO_CLIENTE: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), handler, (DLGPROC)CallbackClienteregistro);
			break;
		}
		case ID_CONSULTA_CLIENTE: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), handler, (DLGPROC)CallbackClienteconsulta);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}
	
	default:
		break;
	}
	return false;
}


BOOL CALLBACK sesionCallback(HWND handler, UINT mensaje, WPARAM  wParam, LPARAM lParam) {
	switch (mensaje)
	{
	case WM_INITDIALOG: {
		//SendDlgItemMessage(handler, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)"Opcion de list");

		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		
		case INICIOSESION: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), handler, (DLGPROC)Callbackprincipal);
			
			break;

		}

			default:
			break;
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handler, 0);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return false;
}

int WINAPI WinMain(HINSTANCE instancia, HINSTANCE legado, LPSTR comando, int modo) {

	DialogBox(instancia, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)sesionCallback);

	return 0;
}