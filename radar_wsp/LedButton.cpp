//                                                                -*- C++ -*-
// ==========================================================================
//!
//! \file LedButton.cpp
//!
//! \brief Implementation of the CLedButton Class.
//!
//! \author 
//!    Ricky Marek <A HREF="mailto:ricky.marek@gmail.com">ricky.marek@gmail.com</A>
//!
//!	\par Disclaimer
//!    This code and the accompanying files are provided <b>"as is"</b> with
//!    no expressed  or  implied warranty.  No responsibilities  for possible
//!    damages, or side effects in its functionality.  The  user must assume 
//!    the entire risk of using this code.  The author accepts no  liability
//!    if it causes any damage to your computer, causes your pet to fall ill, 
//!    increases baldness or makes your car  start  emitting  strange noises 
//!    when you start it up.  <i>This code  has no bugs,  just  undocumented 
//!    features!.</i>
//!
//! \par Terms of use
//!    This code is <b>free</b> for personal use, or freeware applications as
//!    long as this comment-header  header remains like this.  If you plan to 
//!    use  this  code in  a commercial  or shareware  application,   you are 
//!    politely  asked  to  contact the author for his permission via e-mail. 
//!    From: <A HREF="mailto:ricky.marek@gmail.com">ricky.marek@gmail.com</A>
//!
//! \par Attributes
//!    \li \b Created       16/Aug/2002
//!    \li \b Last-Updated  16/Dec/2004
//!    \li \b Compiler      Visual C++
//!    \li \b Requirements  Win98/Win2k or later, MFC.
//!    \li \b Tested        with Visual C++ 6.0 and 7.1(.NET 2003)
//!
//!
// ==========================================================================


// ///////////////////////////////////////////////////////////////////////////
// Header Files
// ///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h" // should be the 1st include ...
#include "LedButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef BS_TYPEMASK
# define BS_TYPEMASK  0x0000000F
#endif // BS_TYPEMASK

// ###########################################################################
// ##                                                                       ##
// ##                   C L e d B u t t o n   C l a s s                     ##
// ##                                                                       ##
// ###########################################################################

// ///////////////////////////////////////////////////////////////////////////
// Default Constructor (Public)
// ///////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CLedButton, CButton)
CLedButton::CLedButton()
{
    m_ledDataList.RemoveAll();

    m_listCount        = 0;
    m_ledState         = LED_BUTTON_STATE_OFF;
    m_isDisabled       = FALSE;
    m_buttonStyle      = BS_TYPEMASK;
    m_activityTimer    = NULL;
    m_activityDuration = LED_BUTTON_DEFAULT_ACTIVITY_DURATION;
    m_activityId       = 0;
    m_activityState    = LED_BUTTON_STATE_OFF;
    m_pCondition       = NULL;

    SetLedStatesNumber(LED_BUTTON_PREDEFINED_STATES_NUMBER, false);
}


// ///////////////////////////////////////////////////////////////////////////
// Default Destructor (Public)
// ///////////////////////////////////////////////////////////////////////////
CLedButton::~CLedButton()
{

    for(LedState ledState=0; ledState < m_listCount; ledState++)
    {
        RemoveIcon(ledState);
    }
    
    if (NULL != m_activityTimer)
    {
        KillTimer(m_activityTimer);
        m_activityTimer = NULL;
    }

    m_ledDataList.RemoveAll();
    m_listCount = 0;

}


// ///////////////////////////////////////////////////////////////////////////
// Message Map
// ///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLedButton, CButton)
    ON_WM_DESTROY()
    ON_WM_SYSCOLORCHANGE()
    ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// ###########################################################################
// ##                                                                       ##
// ##               L E D   S T A T E   A T T R I B U T E S                 ##
// ##                                                                       ##
// ###########################################################################

// ///////////////////////////////////////////////////////////////////////////
// SetLedStatesNumber (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetLedStatesNumber(int maxLedStatesNumber, bool isInvalidate /*=true*/)
{
    ASSERT(maxLedStatesNumber > 0);
	LedState ledState;

    for(ledState=0; ledState < m_listCount; ledState++)
    {
        RemoveIcon(ledState);
    }

    m_ledDataList.RemoveAll();
    m_listCount = maxLedStatesNumber;
    m_ledDataList.SetSize(maxLedStatesNumber);

    LedData *pData = m_ledDataList.GetData();
    
    for(ledState=0; ledState < m_listCount; ledState++)
    {
        ::ZeroMemory(&(pData[ledState]), sizeof(LedData));

    }

    RestoreDefaultColors(isInvalidate);

}



// ///////////////////////////////////////////////////////////////////////////
// SetLedState (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetLedState(LedState ledState, bool isForcedChange /*=false*/)
{
    if (NULL != m_pCondition)
    {
        ledState = m_pCondition->ChangeState(ledState, m_ledState, isForcedChange);
    }

    //
    // the following is added to remove flickering when the led is called
    // with the same led state as it is displaying.
    //
    if (ledState == m_ledState)
    {
        return;
    }

    if (ledState < m_listCount)
    {
        m_ledState = ledState;
    }

    if (0 != m_activityId)
    {
        //
        // Kill Timer if running.
        //
        if (NULL != m_activityTimer)
        {
            KillTimer(m_activityTimer);
            m_activityTimer = NULL;

        }
     
        if (m_ledState != m_activityState)
        {
            m_activityTimer = SetTimer(m_activityId, m_activityDuration, NULL);
        }
    }

    Invalidate();

}


// ///////////////////////////////////////////////////////////////////////////
// SetLedActivityTimer (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetLedActivityTimer(UINT timerId, 
                                     int duration /* =LED_BUTTON_DEFAULT_ACTIVITY_DURATION */, 
                                     LedState offState /*=LED_BUTTON_STATE_OFF*/)
{
    if (NULL != m_activityTimer)
    {
        KillTimer(m_activityTimer);
        m_activityTimer = NULL;
    }

    m_activityId       = timerId;
    m_activityDuration = duration;
    m_activityState    = offState;

}



// ///////////////////////////////////////////////////////////////////////////
// SetLedStateCondition (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetLedStateCondition(CLedStateCondition *pCondition)
{
    m_pCondition = pCondition;
    SetLedState(m_ledState, true);
}


// ###########################################################################
// ##                                                                       ##
// ##                    I C O N   O P E R A T I O N S                      ##
// ##                                                                       ##
// ###########################################################################

// ///////////////////////////////////////////////////////////////////////////
// SetIcon(1) (Public)
// ///////////////////////////////////////////////////////////////////////////
bool CLedButton::SetIcon(LedState ledState, UINT iconId, int width/*=0*/, int height/*=0*/)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?.

    if (ledState < m_listCount)
    {
	    HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(iconId), RT_GROUP_ICON);
        HICON     hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(iconId), IMAGE_ICON, width,height,0);

        return(SetIcon(ledState, hIcon));

    }

    return(false);

}

// ///////////////////////////////////////////////////////////////////////////
// SetIcon(2) (Public)
// ///////////////////////////////////////////////////////////////////////////
bool CLedButton::SetIcon(LedState ledState, HICON hIcon)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        RemoveIcon(ledState);

        if (hIcon)
        {
      	    ICONINFO	iconInfo;

            m_ledDataList[ledState].hIcon = hIcon;

		    ::ZeroMemory(&iconInfo, sizeof(ICONINFO));

		    if (FALSE == ::GetIconInfo(hIcon, &iconInfo))
            {
                RemoveIcon(ledState);
                return(false);
            }

		    m_ledDataList[ledState].width  = iconInfo.xHotspot * 2;
		    m_ledDataList[ledState].height = iconInfo.yHotspot * 2;

		    ::DeleteObject(iconInfo.hbmMask);
		    ::DeleteObject(iconInfo.hbmColor);

        }
    }

    return(ledState < m_listCount);


}

// ///////////////////////////////////////////////////////////////////////////
// SetIcons (Public)
// ///////////////////////////////////////////////////////////////////////////
bool CLedButton::SetIcons(UINT offIconId, UINT onIconId)
{
    bool retVal = true;  // Optimistic..

    if ((offIconId > 0) && (m_listCount > 0))
    {
        retVal = SetIcon(LED_BUTTON_STATE_OFF, offIconId);
    }

    if ((retVal) && (onIconId > 0) && (m_listCount > 0))
    {
        retVal = SetIcon(LED_BUTTON_STATE_ON, onIconId);
    }

    return(retVal);

}

// ///////////////////////////////////////////////////////////////////////////
// RemoveIcon (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::RemoveIcon(LedState ledState)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        if (NULL != m_ledDataList[ledState].hIcon)
        {
            ::DestroyIcon(m_ledDataList[ledState].hIcon);
            m_ledDataList[ledState].hIcon = NULL;
        }

        m_ledDataList[ledState].height = 0;
        m_ledDataList[ledState].width = 0;
    
    }

}


// ###########################################################################
// ##                                                                       ##
// ##     T E X T   A T T R I B U T E S   A N D   O P E R A T I O N S       ##
// ##                                                                       ##
// ###########################################################################


// ///////////////////////////////////////////////////////////////////////////
// RestoreDefaultColors (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::RestoreDefaultColors(bool isInvalidate /* =true */)
{

    for(LedState ledState = 0; ledState < m_listCount; ledState++)
    {
        m_ledDataList[ledState].textForegroundColor = ::GetSysColor(COLOR_BTNTEXT);
        m_ledDataList[ledState].textBackgroundColor = ::GetSysColor(COLOR_BTNFACE);
    }

	if (isInvalidate)
    {
        Invalidate();
    }
}


// ///////////////////////////////////////////////////////////////////////////
// SetTextForeground (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetTextForeground(LedState ledState, COLORREF colorRef, bool isInvalidate /*=true*/)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        m_ledDataList[ledState].textForegroundColor = colorRef;
    }

    if (isInvalidate)
    {
        Invalidate();
    }

}


// ///////////////////////////////////////////////////////////////////////////
// SetTextBackground (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetTextBackground(LedState ledState, COLORREF colorRef, bool isInvalidate /*=true*/)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        m_ledDataList[ledState].textBackgroundColor = colorRef;
    }

    if (isInvalidate)
    {
        Invalidate();
    }

 
}


// ///////////////////////////////////////////////////////////////////////////
// SetTextColors (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetTextColors(LedState ledState, COLORREF fgColorRef, COLORREF bgColorRef, bool isInvalidate /*=true*/)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    CLedButton::SetTextBackground(ledState, bgColorRef, false);
    CLedButton::SetTextForeground(ledState, fgColorRef, isInvalidate);
}


// ///////////////////////////////////////////////////////////////////////////
// GetTextForegroundColor (Public)
// ///////////////////////////////////////////////////////////////////////////
COLORREF CLedButton::GetTextForegroundColor(LedState ledState)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        return(m_ledDataList[ledState].textForegroundColor);
    }

    return(::GetSysColor(COLOR_BTNTEXT));
}


// ///////////////////////////////////////////////////////////////////////////
// GetTextBackgroundColor (Public)
// ///////////////////////////////////////////////////////////////////////////
COLORREF CLedButton::GetTextBackgroundColor(LedState ledState)
{
    ASSERT(ledState < m_listCount);  // No room for this Led?

    if (ledState < m_listCount)
    {
        return(m_ledDataList[ledState].textBackgroundColor);
    }

    return(::GetSysColor(COLOR_BTNFACE));
}


// ###########################################################################
// ##                                                                       ##
// ##                      T O O L T I P   S T U F F                        ##
// ##                                                                       ##
// ###########################################################################


// ///////////////////////////////////////////////////////////////////////////
// ToolTipInit (Private)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::ToolTipInit(void)
{
	if (NULL == m_toolTip.m_hWnd)
	{
		m_toolTip.Create(this);
		m_toolTip.Activate(FALSE);
		m_toolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}

}


// ///////////////////////////////////////////////////////////////////////////
// SetTooltipText(1) (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetTooltipText(UINT id, bool isActivate /*=true*/)
{
   	CString text;
	
    if (text.LoadString(id))
    {
        SetTooltipText(text, isActivate);
    }

}
// ///////////////////////////////////////////////////////////////////////////
// SetTooltipText(2) (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::SetTooltipText(LPCTSTR text, bool isActivate /*=true*/)
{
	if (NULL == text)
    {
        return;
    }

	ToolTipInit();

	if (0 == m_toolTip.GetToolCount())
	{
		CRect rect; 
		GetClientRect(rect);
		m_toolTip.AddTool(this, text, rect, 1);
	} 

	m_toolTip.UpdateTipText(text, this, 1);
	m_toolTip.Activate(isActivate);

}

// ///////////////////////////////////////////////////////////////////////////
// ActivateTooltip (Public)
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::ActivateTooltip(bool isActivate /*=true*/)
{
	if (0 != m_toolTip.GetToolCount())
    {
    	m_toolTip.Activate(isActivate);
    }

    return;

}


// ###########################################################################
// ##                                                                       ##
// ##                   M E S S A G E   H A N D L E R S                     ##
// ##                                                                       ##
// ###########################################################################

// ///////////////////////////////////////////////////////////////////////////
// OnDestroy
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::OnDestroy()
{
    CButton::OnDestroy();

    //
    // Kill timer
    //
    if (NULL != m_activityTimer)
    {
        KillTimer(m_activityTimer);
        m_activityTimer = NULL;
    }

}



// ///////////////////////////////////////////////////////////////////////////
// DrawItem
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
   	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	m_isDisabled = ((lpDIS->itemState & ODS_DISABLED) != 0);

	CRect itemRect = lpDIS->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	OnDrawBackground(pDC, &itemRect);

    CRect captionRect = lpDIS->rcItem;
    CRect ledRect;
  
    //
    // Now the cases are:
    // - BS_LEFTTEXT: The text appears on the left side of the LED icon.( true/false)
    //
    // - BS_LEFT:   Text will be left aligned.
    // - BS_RIGHT:  Text will be right aligned.
    // - BS_CENTER: Text will be center
    //
    LedData ledData;
    ::ZeroMemory(&ledData, sizeof(LedData));
    ledData.textForegroundColor = ::GetSysColor(COLOR_BTNTEXT);
    ledData.textBackgroundColor = ::GetSysColor(COLOR_BTNFACE);

    if (m_ledState < m_listCount)
    {
        ledData = m_ledDataList[m_ledState];
    }

    if (m_buttonStyle & BS_LEFTTEXT)
    {
        ledRect.left      = captionRect.right - ledData.width;
        ledRect.right     = ledRect.left + ledData.width;
        captionRect.right = ledRect.left - 3;
    }
    else
    {
        ledRect.left     = captionRect.left;
        ledRect.right    = ledRect.left + ledData.width;
        captionRect.left = ledRect.right + 3;
    }

    ledRect.top    = (captionRect.Height() - ledData.height)/2;
    ledRect.bottom = ledRect.top + ledData.height;

    if (ledData.hIcon)
    {
        pDC->DrawState(ledRect.TopLeft(), 
                       ledRect.Size(),  
                       ledData.hIcon, 
                       (m_isDisabled ? DSS_DISABLED : DSS_NORMAL), 
                       (CBrush*)NULL);

    }

    //
    // Now deal with the text.(if any)
    //
	CString title;
	GetWindowText(title);

    if (FALSE == title.IsEmpty())
    {
        CRect centerRect = captionRect;
        UINT textFormat = DT_WORDBREAK | DT_VCENTER;

        if (BS_CENTER == (m_buttonStyle &  BS_CENTER))
        {
            textFormat |= DT_CENTER;
            pDC->DrawText(title, -1, &captionRect, (textFormat | DT_CALCRECT));
            captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, 
                                   ((centerRect.Height() - captionRect.Height())/2));

        }
        else if (BS_RIGHT == (m_buttonStyle & BS_RIGHT))
        {
            textFormat |= DT_RIGHT;
            pDC->DrawText(title, -1, &captionRect, (textFormat | DT_CALCRECT));
            captionRect.OffsetRect((centerRect.Width() - captionRect.Width()), 
                                   ((centerRect.Height() - captionRect.Height())/2));
        }
        else
        {
            textFormat |= DT_LEFT;
            pDC->DrawText(title, -1, &captionRect, (textFormat | DT_CALCRECT));
            captionRect.OffsetRect(0, ((centerRect.Height() - captionRect.Height())/2));

        }

	    pDC->SetBkMode(TRANSPARENT);

        if (m_isDisabled)
        {
            captionRect.OffsetRect(1, 1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(title, -1, &captionRect, textFormat);
            captionRect.OffsetRect(-1, -1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(title, -1, &captionRect, textFormat);
        }
        else
        {
            pDC->SetTextColor(ledData.textForegroundColor);
            pDC->SetBkColor(ledData.textForegroundColor);
            pDC->DrawText(title, -1, &captionRect, textFormat);
        }

    }

}



// ///////////////////////////////////////////////////////////////////////////
// OnTimer
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::OnTimer(UINT nIDEvent)
{
    if ((nIDEvent == m_activityId) && (0 != m_activityId))
    {
        SetLedState(m_activityState);
        return;
    }

    CButton::OnTimer(nIDEvent);
}

// ///////////////////////////////////////////////////////////////////////////
// OnSysColorChange
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::OnSysColorChange()
{
    CWnd::OnSysColorChange();
    
    RestoreDefaultColors();
}


// ///////////////////////////////////////////////////////////////////////////
// OnDrawBackground
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::OnDrawBackground(CDC* pDC, CRect* pRect)
{
    ASSERT(pDC);
    ASSERT(pRect);

    COLORREF color = GetTextBackgroundColor(m_ledState);
	CBrush brBackground(color);
	pDC->FillRect(pRect, &brBackground);
}


// ///////////////////////////////////////////////////////////////////////////
// OnSetStyle
// ///////////////////////////////////////////////////////////////////////////
LRESULT CLedButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	return(DefWindowProc(BM_SETSTYLE, (wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam));
}


// ///////////////////////////////////////////////////////////////////////////
// PreTranslateMessage
// ///////////////////////////////////////////////////////////////////////////
BOOL CLedButton::PreTranslateMessage(MSG* pMsg)
{
	ToolTipInit();
	m_toolTip.RelayEvent(pMsg);
	
    return(CButton::PreTranslateMessage(pMsg));
}

//
// To avoid a  warning C4189: 'style' : local variable is initialized but not referenced
// in the PreSubclassWindow method on Level4 Warnings/Release configuration.
// Reason: The ASSERT lines do not exist under Release. therefore the 'style' variable
// is never used.
#pragma warning( push )
#pragma warning( disable : 4189 )


// ///////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
// ///////////////////////////////////////////////////////////////////////////
void CLedButton::PreSubclassWindow()
{
	UINT style = BS_TYPEMASK  & GetButtonStyle();
    
    ASSERT(style & BS_CHECKBOX);
	ASSERT(style != BS_OWNERDRAW);

	// Switch to owner-draw
	ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

    m_buttonStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

    CButton::PreSubclassWindow();
}
//
// Undo the above warning disabling.
//
#pragma warning( pop )


// ///////////////////////////////////////////////////////////////////////////
// CtlColor
// ///////////////////////////////////////////////////////////////////////////
HBRUSH CLedButton::CtlColor(CDC* /*pDC*/, UINT /*CtlColor*/)
{
	return ((HBRUSH)::GetStockObject(NULL_BRUSH)); 
}


//
// --- No-code-allowed-beyond-this-line--------------------------------------
//

