//                                                                -*- C++ -*-
// ==========================================================================
//!
//! \file LedButton.h
//!
//! \brief CLedButton and CLedStateCondition Classes.
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
//!    \li \b Last-Updated  22/Dec/2004
//!    \li \b Compiler      Visual C++
//!    \li \b Requirements  Win98/Win2k or later, MFC.
//!    \li \b Tested        with Visual C++ 6.0 and 7.1(.NET 2003)
//!
//!
// ==========================================================================
#ifndef _LEDBUTTON__H_ // _LEDBUTTON__H_
#define _LEDBUTTON__H_ // _LEDBUTTON__H_

// //////////////////////////////////////////////////////////////////////////
// Include Files
// //////////////////////////////////////////////////////////////////////////
#include <afxtempl.h>


// //////////////////////////////////////////////////////////////////////////
// Type Definitions
// //////////////////////////////////////////////////////////////////////////
typedef unsigned int LedState;

// //////////////////////////////////////////////////////////////////////////
// Name Spaces
// //////////////////////////////////////////////////////////////////////////

namespace
{
    /** Predefined initial Led State as "Off" state. */
    const LedState LED_BUTTON_STATE_OFF = 0; 

    /** Predefined "On" Led State. */
    const LedState LED_BUTTON_STATE_ON = 1;  

    /** Predefined activity duration in msec. */
    const int LED_BUTTON_DEFAULT_ACTIVITY_DURATION = 150;  

    /** Predefined Led States Number. */
    const int LED_BUTTON_PREDEFINED_STATES_NUMBER = 2;
                                                          
};

//
// To avoid a warning for 'C4100: unreferenced formal parameter' on Level4 Warnings,
// on the CLedStateCondition::ChangeState method. I'm adding the following
// preprocessor warning disable command. The warning is generated due
// that 'oldLedState' and 'isForcedChange' are parameters that are not used in
// the inline implementation of this method. Derived classes from CLedStateCondition
// may use this parameters for its proper behavior.
//
#pragma warning( push )
#pragma warning( disable : 4100 )

/**
 * \ingroup LedButton
 *
 * \brief CLedStateCondition Class
 *
 * This is the base class for setting a Led State condition. If the user want control
 * when the Led changes a state, he/she should derive a class from this base class and
 * put its code there, and give the CLedButton class a pointer to an object from the 
 * derived class. (If this pointer is not NULL, the CLedButton object will used to
 * check the validity of the new state and change it if required.
 *
 * \par requirements
 * win2k or later\n
 * MFC\n
 *
 * \author Ricky Marek <A HREF="mailto:ricky.marek@gmail.com">ricky.marek@gmail.com</A>
 *
 */

class CLedStateCondition
{
public:
    /*! Default constructor: Do nothing */
    CLedStateCondition(void) {};

    /*! Default Destructor: Do nothing */
    virtual ~CLedStateCondition(void) {};

    /*!
     * Decide which \a newLedState to change according to the \a oldLedState.
     * 
     * Derived classes should overwrite this method in order to provide more
     * control over the LedState transitions. (e.g. to change the \a newLedState
     * according to a private finite state machine(FSM), that is implementation 
     * dependent.
     *
     * This virtual function is called by the CLedButton::SetLedState method
     * when the user requests a LedState change.
     *
     * \param newLedState is the requested new LedState.
     * \param oldLedState the current LedState before the transition.
     * \param isForcedChange a boolean flag that may be used to force a transition
     *        to the required \a newLedState. The derived class should fix its
     *        private FSM to reflect this forced change.
     *
     * \return the nee LedState the LedButton will show.
     */
    virtual LedState ChangeState(LedState newLedState, 
                                 LedState oldLedState, 
                                 bool isForcedChange=false) { return(newLedState); };
};

//
// Undo the above warning disabling.
//
#pragma warning( pop )

/**
 * \ingroup LedButton
 * \brief CLedButton Class
 *
 * \par requirements
 * win2k or later\n
 * MFC\n
 *
 * \author Ricky Marek <A HREF="mailto:ricky.marek@gmail.com">ricky.marek@gmail.com</A>
 *
 */
class CLedButton : public CButton
{
	DECLARE_DYNAMIC(CLedButton)

public:
	CLedButton();

    /*! Default Destructor: Do nothing */
	virtual ~CLedButton();

    /*! \name Led State Attributes */
    //@{
    /*!
     * Set the number of Led States this CLedButton will manage.
     * 
     * The LedStates are stored in a "Zero-Based" list, accessing to a
     * LedState above \a maxLedStatesNumber will cause an ASSERT.
     *
     * By default, the number of Led States is two. Calling this method
     * the user can change it to any positive non-zero number.
     * \param maxLedStatesNumber The number of Led States to manage.
     * \param isInvalidate Is Invalidate() required?
     */
    void SetLedStatesNumber(int maxLedStatesNumber, bool isInvalidate=true);

    /**
     * Set a new \a ledState to the CLedButton control. 
     *
     * According to the control setup, the led may change it state to the
     * new state directly (when no CLedStateCondition is setup) or turn off
     * after an activity period when this Led button is an activity LED.
     *
     * \param ledState to set.
     * \param isForcedChange force a change to this state. 
     */
    void SetLedState(LedState ledState, bool isForcedChange=false);

    /**
     * Get the current Led State.
     * \return the current Led State.
     */
    LedState GetLedState(void) { return (m_ledState); };

    /**
     * Set the Activity property of the Led Control.
     *
     * If a valid (non-zero) \a timerId is provided, the Led control becomes 
     * an activity led, controlled by an internal timer related to the \a timerId,
     * having a \a duration in milliseconds. When the timer ages, the led will
     * change automatically to the provided \a offState.
     * 
     * If the \a timerId is zero, the control stops from being an activity led.
     *
     * \note Timers are a limited global resource; therefore this method provides
     * an interface for setting the \a timerId used by the Led Control.
     *
     * \param timerId the timer identifier. use Zero to remove any activity property.
     * \param duration the timer activity duration before it goes to its 
     *                 \a offState in Milliseconds (defaults:150)
     * \param offState the "off" state to use. (defaults:0).
     */
    void SetLedActivityTimer(UINT timerId, 
                             int duration=LED_BUTTON_DEFAULT_ACTIVITY_DURATION, 
                             LedState offState=LED_BUTTON_STATE_OFF);

    /**
     * Set the Condition property of the Led Control.
     *
     * If the \a pCondition pointer is not NULL, the Led Control will call the 
     * derived CLedStateCondition::ChangeState() method. (this method is virtual) to
     * get the new Led State to change. 
     *
     * If the \a pCondition parameter is NULL, the Led returns to be a simple Led.
     *
     * If Activity and Condition properties are provided (in any order), the method
     * CLedButton::SetState() will first check for the Condition and then for the Activity
     * properties.
     *
     * \param *pCondition pointer to a derived class object from CLedStateCondition or NULL.
     */
    void SetLedStateCondition(CLedStateCondition *pCondition); 
    //@}

    /*! \name Icon operations */
    //@{

    /**
     * Set the Icon(from a resourceID) associated to the \a ledState.
     *
     * \param ledState the LedState to associate.
     * \param iconId the resourceID to use
     * \param width Optional width (otherwise use the first-resource-icon width, default:0
     * \param height Optional height (otherwise use the first-resource-icon height, default:0)
     * \return success or failure.
     */
    bool SetIcon(LedState ledState, UINT iconId, int width=0, int height=0);

    /**
     * Set the Icon(from an HICON handle) associated to the \a ledState.
     * \param ledState the LedState to associate.
     * \param hIcon the Icon Handle.
     * \return success or failure.
     */
    bool SetIcon(LedState ledState, HICON hIcon);

    /**
     * Set the 'off' and 'on' icons based on resourceIDs.
     * 
     * This method is a shortcut and its effect is the same as:
     * <pre>
     *      if (false == SetIcon(LED_BUTTON_STATE_OFF, offIconId))
     *      {
     *          return(false);
     *      }
     *      return(SetIcon(LED_BUTTON_STATE_ON, onIconId));
     * </pre>
     * \param offIconId the "off" icon resource id.
     * \param onIconId the "on" icon resource id.
     * \return success or failure.
     */
    bool SetIcons(UINT offIconId, UINT onIconId);

    /**
     * Remove the icon from a Led State.
     * \param ledState the victim Led State to remove it's icon.
     */
    void RemoveIcon(LedState ledState);
    //@}

    /*! \name Text CButton operations */
    //@{
    /**
     * Restore the Led Button Control default system colors.
     * \param isInvalidate is Invalidate() required? (default:true)
     */
    void RestoreDefaultColors(bool isInvalidate=true);

	/**
	 * Set the text foreground color associated with the \a ledState.
	 * \param ledState Led State to use.
	 * \param colorRef foreground color.
	 * \param isInvalidate is Invalidate() required? (default:true)
	 */
	void SetTextForeground(LedState ledState, COLORREF colorRef, bool isInvalidate=true);

    /**
	 * Set the text background color associated with the \a ledState. 
     *
     * That is, the button background color.
     *
	 * \param ledState Led State to use.
	 * \param colorRef background color.
	 * \param isInvalidate is Invalidate() required? (default:true)
	 */
    void SetTextBackground(LedState ledState, COLORREF colorRef, bool isInvalidate=true);
    
    /**
     * Set the 'foreground' and 'background' text/button colors to a Led State.
     * 
     * This method is a shortcut and its effect is the same as:
     * <pre>
     *      SetTextForeground(ledState, fgColorRef, false);
     *      SetTextBackground(ledState, bfColorRef, isInvalidate);
     * </pre>
     *
     * \param ledState Led State to use.
     * \param fgColorRef text foreground color
     * \param bgColorRef button background color
     * \param isInvalidate is Invalidate() required? (default:true)
     */
    void SetTextColors(LedState ledState, COLORREF fgColorRef, COLORREF bgColorRef, bool isInvalidate=true);

	/**
	 * Get the current text foreground color for the \a ledState.
	 * \param ledState Led State to select
	 * \return text foreground color for the selected state.
	 */
	COLORREF GetTextForegroundColor(LedState ledState);

    /**
	 * Get the current button background color for the \a ledState.
	 * \param ledState Led State to select
	 * \return button background color for the selected state.
	 */
    COLORREF GetTextBackgroundColor(LedState ledState);
    //@}

    /*! \name Tooltip Stuff */
    //@{
    
    /**
     * Set the Tooltip Text (from a resource string).
     * \param id the resource string identifier to use.
     * \param isActivate is this tooltip active?
     */
    void SetTooltipText(UINT id, bool isActivate=true);

	/**
	 * Set the Tooltip Text (directly from a string).
	 * \param text the string buffer
	 * \param isActivate is this tooltip active?
	 */
	void SetTooltipText(LPCTSTR text, bool isActivate=true);
	
    /**
	 * Activate this tool tip.
     * Call it with false if you want to de-activate the tooltip.
	 * \param isActivate is this tooltip active?
	 */
	void ActivateTooltip(bool isActivate=true);
    //@}

    /*! \name Message Handlers */
    //@{
    afx_msg void OnDestroy();
    afx_msg void OnSysColorChange();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    //@}

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
    virtual void PreSubclassWindow();
   	virtual void OnDrawBackground(CDC* pDC, CRect* pRect);

    DECLARE_MESSAGE_MAP()

private:

    /** 
     * Structure that defines the internal data kept for each defined
     * Led State.
     */
    typedef struct LedData
	{
        COLORREF    textForegroundColor;   /*!< The text foreground to use */
        COLORREF    textBackgroundColor;   /*!< The text background to use */

		HICON		hIcon;                 /*!< The Icon handler. */
		DWORD		width;                 /*!< The Icon width */
		DWORD		height;                /*!< The Icon height */
	
    } LedData;

    /** Initialize the CToolTipCtrl control associated with this Led Button Control. */ 
   	void ToolTipInit(void);

    LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);

    CArray<LedData , LedData&> m_ledDataList;  ///< List of Led States this control.
	CToolTipCtrl m_toolTip;                    ///< Tooltip control associated with the control.

    LedState m_ledState;                       ///< Current LedState the control is in it.
    UINT     m_listCount;                      ///< Number of Led States in this control.
    UINT     m_align;                          ///< Text/Icon align attribute.
    bool     m_isDisabled;                     ///< is this control disabled?
    LONG     m_buttonStyle;                    ///< the button style.

    UINT_PTR  m_activityTimer;                 ///< The internal activation timer if relevant.
    int       m_activityDuration;              ///< The activation timer duration if relevant.
    UINT      m_activityId;                    ///< The activation timer identifier if relevant.
    LedState  m_activityState;                 ///< The activation "off" state to return.

    CLedStateCondition *m_pCondition;          ///< the condition led to use if relevant.

};



#endif //_LEDBUTTON__H_

//
// --- No-code-allowed-beyond-this-line--------------------------------------
//

