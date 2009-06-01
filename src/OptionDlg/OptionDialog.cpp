#include "OptionDialog.h"
#include "PluginInterface.h"
#include <Commctrl.h>
#include <shlobj.h>
#include <uxtheme.h>

typedef HRESULT (WINAPI * ETDTProc) (HWND, DWORD);

UINT OptionDialog::doDialog(struct sColorSettings * Settings)
{
    _ColorSettings = Settings;
    return (UINT)::DialogBoxParam(_hInst, MAKEINTRESOURCE(IDD_OPTION_DIALOG), _hParent,  (DLGPROC)dlgProc, (LPARAM)this);
	//goToCenter();
}

BOOL CALLBACK OptionDialog::run_dlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) 
	{
        case WM_INITDIALOG :
		{
			goToCenter();

			ETDTProc EnableDlgTheme = (ETDTProc)::SendMessage(_nppData._nppHandle, NPPM_GETENABLETHEMETEXTUREFUNC, 0, 0);
			
            if (EnableDlgTheme != NULL)
            {
                EnableDlgTheme(_hSelf, ETDT_ENABLETAB);
            }

            _ColorComboAdded.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_ADDED_COLOR));
            _ColorComboChanged.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_CHANGED_COLOR));
            _ColorComboBlank.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_BLANK_COLOR));
            _ColorComboMoved.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_MOVED_COLOR));
            _ColorComboRemoved.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_REMOVED_COLOR));
            _ColorComboHighlight.init(_hInst, _hParent, ::GetDlgItem(_hSelf, IDC_COMBO_HIGHLIGHT_COLOR));

            HWND hUpDown = GetDlgItem(_hSelf, IDC_SPIN_CTL);
            SendMessage(hUpDown, UDM_SETRANGE, 0L, MAKELONG(100, 0));

            SetParams();

            break;
		}
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case IDOK:
					if (GetParams() == FALSE) return FALSE;
					::EndDialog(_hSelf, IDOK);
					return TRUE;
				case IDCANCEL:
					::EndDialog(_hSelf, IDCANCEL);
					return TRUE;
                case IDDEFAULT:
                    _ColorSettings->added     = DEFAULT_ADDED_COLOR;
                    _ColorSettings->blank     = DEFAULT_BLANK_COLOR;
                    _ColorSettings->changed   = DEFAULT_CHANGED_COLOR;
                    _ColorSettings->deleted   = DEFAULT_DELETED_COLOR;
                    _ColorSettings->moved     = DEFAULT_MOVED_COLOR;
                    _ColorSettings->highlight = DEFAULT_HIGHLIGHT_COLOR;
                    _ColorSettings->alpha     = DEFAULT_HIGHLIGHT_ALPHA;
                    SetParams();
                    break;
				case IDC_COMBO_ADDED_COLOR :
					_ColorComboAdded.onSelect();
                    break;
                case IDC_COMBO_CHANGED_COLOR :
                    _ColorComboChanged.onSelect();
                    break;
                case IDC_COMBO_BLANK_COLOR :
                    _ColorComboBlank.onSelect();
                    break;
                case IDC_COMBO_MOVED_COLOR :
                    _ColorComboMoved.onSelect();
                    break;
                case IDC_COMBO_REMOVED_COLOR :
                    _ColorComboRemoved.onSelect();
					break;
                case IDC_COMBO_HIGHLIGHT_COLOR:
                    _ColorComboHighlight.onSelect();
                    break;
				default :
					return FALSE;
			}
			break;
		}
	}
	return FALSE;
}

void OptionDialog::SetParams(void)
{
    _ColorComboAdded.setColor(_ColorSettings->added);
    _ColorComboMoved.setColor(_ColorSettings->moved);
    _ColorComboRemoved.setColor(_ColorSettings->deleted);
	_ColorComboChanged.setColor(_ColorSettings->changed);
	_ColorComboBlank.setColor(_ColorSettings->blank);
    _ColorComboHighlight.setColor(_ColorSettings->highlight);

    SetDlgItemInt(_hSelf, IDC_SPIN_BOX, _ColorSettings->alpha, FALSE);
}

BOOL OptionDialog::GetParams(void)
{
	_ColorComboAdded.getColor((LPCOLORREF)&_ColorSettings->added);
    _ColorComboMoved.getColor((LPCOLORREF)&_ColorSettings->moved);
	_ColorComboRemoved.getColor((LPCOLORREF)&_ColorSettings->deleted);
	_ColorComboChanged.getColor((LPCOLORREF)&_ColorSettings->changed);
	_ColorComboBlank.getColor((LPCOLORREF)&_ColorSettings->blank);
    _ColorComboHighlight.getColor((LPCOLORREF)&_ColorSettings->highlight);

    _ColorSettings->alpha = GetDlgItemInt(_hSelf, IDC_SPIN_BOX, NULL, FALSE);

    return TRUE;
}