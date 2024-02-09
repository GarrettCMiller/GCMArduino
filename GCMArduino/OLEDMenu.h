// OLEDMenu.h

#ifndef _OLEDMENU_h
#define _OLEDMENU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "u8glib.h"
#include "Stack.h"
#include "BlunoShield.h"
#include "GFXIcons.h"
#include "RangedValue.h"

#define ArrayLength(array, type)	sizeof(array) / sizeof(type)
#define MenuOptionCount(array)		ArrayLength(array, MenuOption)

class BlunoOLED;
typedef BlunoOLED OLED;

class OLEDPage;
class OLEDMenu;

/// <summary>
/// An object describing a single "text" option in a list of options, on a particular OLEDPage,
/// that has an associated action, like setting and/or displaying a particular value, or navigating
/// to another OLEDPage.
/// </summary>
class MenuOption
{
public:
	enum Action
	{
		eAction_GoToMenu,
		eAction_PreviousMenu,
		eAction_SetValue,
		eAction_ViewTimer,
		eAction_Other,
	};

	typedef void(*GoToMenu)(OLEDPage* page);
	typedef void(*SetValue_u8)(uint8_t& var, uint8_t newValue);
	typedef void(*SetValue_u16)(uint16_t& var, uint16_t newValue);
	typedef void(*SetValue_i)(int& var, int newValue);
	typedef void(*OtherAction)();

	String Text;
	Action actionType;
	OLEDPage* pParentPage;
	
	bool valueIs16Bit;

	union
	{
		OLEDPage* goToMenu;
		SetValue_u8 setValue_u8;
		SetValue_u16 setValue_u16;
		SetValue_i setValue_i;
		/*SetValue setValue;
		RangedValue8* pValue8;
		RangedValue16* pValue16;*/
		RangedValueBase* pValue;
		ArduinoTimer* pTimer;
		OtherAction otherAction;
	};
	
	MenuOption() : pParentPage(NULL), valueIs16Bit(false)
	{
		Text = F("<{Undefined}>");
	}

	MenuOption(String _text, OLEDPage* oledPage)
		: Text(_text), goToMenu(oledPage), actionType(eAction_GoToMenu), pParentPage(NULL), valueIs16Bit(false)
	{
	}

	/*MenuOption(String _text, GoToMenu gtMenu)
		: Text(_text), goToMenu(gtMenu), actionType(eAction_GoToMenu)
	{
	}*/

	MenuOption(String _text, RangedValueBase& targetValue)
		: Text(_text), pValue(&targetValue), actionType(eAction_SetValue), pParentPage(NULL), valueIs16Bit(false)
	{		
	}

	MenuOption(String _text, RangedValueB::SetNewValueCallback newBoolCallback)
		: Text(_text), actionType(eAction_SetValue)
	{
		((RangedValueB*)pValue)->newValueCallback = newBoolCallback;
	}

	MenuOption(String _text, SetValue_i setIntCallback)
		: Text(_text), actionType(eAction_SetValue)
	{
		setValue_i = setIntCallback;
	}

	MenuOption(String _text, ArduinoTimer& _timer)
		: Text(_text), actionType(eAction_ViewTimer), pTimer(&_timer)
	{

	}

	/*MenuOption(String _text, RangedValue16& targetValue)
		: Text(_text), pValue16(&targetValue), actionType(eAction_SetValue), pParentPage(NULL), valueIs16Bit(true)
	{
	}*/

	/*MenuOption(String _text, SetValue sValue)
		: Text(_text), setValue(sValue), actionType(eAction_SetValue), pParentPage(NULL), valueIs16Bit(false)
	{
	}*/

	MenuOption(String _text, OtherAction oAction)
		: Text(_text), otherAction(oAction), actionType(eAction_Other), pParentPage(NULL), valueIs16Bit(false)
	{
	}

	bool Execute(OLEDMenu& menu);
};

/// <summary>
/// A singular collection of 'MenuOptions that represents a particular "page" of related selections;
/// a menu system is comprised of one (or, usually, more) OLEDPages that each contain a list of selectable actions,
/// some linking to other OLEDPages, some allowing variable viewing/adjustment, etc...
/// </summary>
class OLEDPage
{
	static const uint8_t MAX_OPTIONS = 10;
	static const uint8_t MAX_OPTIONS_PER_PAGE = 4;
	static const bool USE_TITLE_BAR = true;

protected:
	/// <summary>
	/// The name of the menu page
	/// </summary>
	char name[32];

	/// <summary>
	/// The pointer to the parent menu
	/// </summary>
	OLEDMenu* pOLEDMenu;

	/// <summary>
	/// The list of menu options to choose from
	/// </summary>
	MenuOption options[MAX_OPTIONS];

	/// <summary>
	/// The number of menu options to choose from
	/// </summary>
	uint8_t numOptions;

	/// <summary>
	/// The current page number (only relevant if there are more options
	/// than can be represented on one page).
	/// </summary>
	uint8_t pageNumber;

public:
	/// <summary>
	/// The X offset to draw the menu
	/// </summary>
	uint8_t offsetx;

	/// <summary>
	/// The y offset to draw the menu
	/// </summary>
	uint8_t offsety;

	/// <summary>
	/// Whether or not the menu has a "back" button
	/// </summary>
	bool hasBackButton;

	/// <summary>
	/// The currently highlighted menu option
	/// </summary>
	uint8_t currentMenuOption;

	/// <summary>
	/// The font to use
	/// </summary>
	const u8g_fntpgm_uint8_t* font;
public:
	/// <summary>
	/// Creates an OLED menu page.
	/// </summary>
	/// <param name="_name">The name of the new page</param>
	/// <param name="numOps">The number, or quantity, of menu options of the new page</param>
	/// <param name="ops">The actual list of menu options for the new page</param>
	/// <param name="hasBack">Whether or not the menu includes an extra "back" option,
	/// allowing exiting of a menu</param>
	OLEDPage(String _name, int8_t numOps, MenuOption ops[], bool hasBack = true)
		: pageNumber(0), offsetx(0), offsety(0), pOLEDMenu(NULL), currentMenuOption(0)
	{
		//Set the menu's name
		strncpy(name, _name.c_str(), 32);

		//As long as there aren't too many options (for whatever reason),
		//create and set them up
		if (numOps <= MAX_OPTIONS)
		{
			//Set the font
			font = u8g_font_helvB08;

			//Whether or not there is a "back" option
			hasBackButton = hasBack;

			//How many selectable options there are
			numOptions = numOps;

			//Add each option
			for (int o = 0; o < numOps; o++)
			{
				AddOptionText(o, ops);
			}
		}
		else
			Serial.println(F("Too many menu options!"));

		//Default to highlighting the very first option
		currentMenuOption = 0;
	}

	/// <summary>
	/// Initialize the page
	/// </summary>
	/// <param name="menu">The parent menu of the page</param>
	void Init(OLEDMenu& menu)
	{
		/*Serial.print("InitPage (");
		Serial.print(this->name);
		Serial.print("): ");
		Serial.println(int(&menu), HEX);*/
		//Set parent menu
		SetOLEDMenu(menu);
	}

	/// <summary>
	/// Adds a menu option to this page
	/// </summary>
	/// <param name="newOption">The new <see cref="MenuOption">menu option</see></param>
	/// <returns></returns>
	bool AddOption(MenuOption newOption)
	{
		//Set the new option
		options[numOptions] = newOption;
		
		//Set the option's parent to this
		options[numOptions].pParentPage = this;
		
		//We now have one more option
		numOptions++;
		
		//Success
		return true;
	}

	/// <summary>
	/// Sets the internal pointer to the parent menu
	/// </summary>
	/// <param name="menu">The parent menu</param>
	void SetOLEDMenu(OLEDMenu& menu)
	{
		/*Serial.print("SetOLEDMenu (");
		Serial.print(this->name);
		Serial.print("): ");
		Serial.println(int(&menu), HEX);*/
		pOLEDMenu = &menu;
	}

	/// <summary>
	/// Draws the menu page
	/// </summary>
	/// <param name="oled">The OLED screen to draw to/on</param>
	void Draw(OLED& oled)
	{		
		//Serial.print("Drawing menu "); Serial.println(name);
		uint8_t lineOffset = 12;
		uint8_t x = 8 + offsetx;
		uint8_t y = 12 + offsety;

		//Set the font
		oled.setFont(font);

		//Draw menu header divider line
		oled.drawLine(0, y, 113, y);
		
		//Draw the menu name just above the line we just drew
		oled.setPrintPos(0, y - 4);
		oled.print(name);

		//Move down to the next text line
		y += lineOffset;

		//(Potentially) draw each menu option
		for (int o = 0; o < MAX_OPTIONS_PER_PAGE; o++, y += lineOffset)
		{
			//If the currently drawn menu option also happens to
			//be the one "currently selected", draw a '>' symbol
			//before it
			if (currentMenuOption == o)
			{
				oled.setPrintPos(0, y);
				oled.print(F(">"));
			}

			//Figure out which option we are drawing, based on which
			//page of options we are showing
			uint8_t optionIndex = pageNumber * MAX_OPTIONS_PER_PAGE + o;

			//As long as that number isn't somehow bigger than
			//the number of options we have, draw the option
			if (optionIndex < numOptions)
			{
				//Set the cursor
				oled.setPrintPos(x, y);
				oled.print(options[optionIndex].Text);
			}
		}

		//if more than one (1) page of options, draw page counter (at the bottom)
		uint8_t numPages = GetNumPages();
		if (numPages > 1)
		{
			oled.setPrintPos(107, 34);
			oled.print(pageNumber + 1);	//The first page is technically zero
			oled.print("/");
			oled.print(numPages);

			//If we AREN'T on the last page, draw a "down" arrow
			if (pageNumber < numPages - 1)
				oled.drawXBMP(115, 43, 9, 16, downArrowIcon);
			else if (pageNumber > 0)	//otherwise, draw an "up" arrow
				oled.drawXBMP(115, 1, 9, 16, upArrowIcon);
		}
	}

	/// <summary>
	/// Handle input from the joystick
	/// </summary>
	/// <param name="blunoShield">The BlunoShield (with relevant joystick)</param>
	void HandleInput(BlunoShield& blunoShield);

	/// <summary>
	/// The number of menu options
	/// </summary>
	/// <returns>Number of menu options</returns>
	uint8_t GetNumOptions() const
	{
		return numOptions;
	}

	/// <summary>
	/// The number of pages
	/// </summary>
	/// <returns>Number of pages</returns>
	uint8_t GetNumPages() const
	{
		return (numOptions / (MAX_OPTIONS_PER_PAGE + 1)) + 1;
	}

	/// <summary>
	/// Gets the text of the indexed option
	/// </summary>
	/// <param name="index">The menu option index to read</param>
	/// <returns>The text of the indexed option</returns>
	String GetOption(uint8_t index)
	{
		return options[index].Text;
	}

	/// <summary>
	/// The current cursor position, by menu option index
	/// </summary>
	uint8_t cursorIndex = 0;

protected:
	/// <summary>
	/// Adds the selectable option to the menu
	/// </summary>
	/// <param name="o">The index of the selectable option</param>
	/// <param name="ops">The internal list of menu options</param>
	void AddOptionText(int o, MenuOption  ops[])
	{
		options[o] = ops[o];
		options[o].pParentPage = this;
		//Serial.print(F("Adding option ")); Serial.println(ops[o].Text);
	}
};

/// <summary>
/// An enum that describes whether the current input mode is upper-(/lower-)case letters, number or symbols.
/// </summary>
enum EInputSet
{
	eIS_Numbers,
	eIS_Lowercase,
	eIS_Uppercase,
	eIS_Symbols
};

/// <summary>
/// THE singleton object that conceptualizes a menu system that contains AT LEAST one OLEDPage.
/// No additional OLEDMenu objects should be declared beyond a singular instance per sketch.
/// </summary>
class OLEDMenu
{
public:
	enum EViewMode
	{
		eVM_Menu,
		eVM_SetValue,
		eVM_DrawTimer,
	};
	
public:
	Stack<OLEDPage*> menuStack;
	OLEDPage* currentPage;
	OLEDPage* lastPage;
	OLED* pOled;
	BlunoShield* pBlunoShield;	

	EInputSet inputMode = eIS_Uppercase;
	EViewMode viewMode = eVM_Menu;

	//bool setValueMode;
	bool setString;

	union
	{
		RangedValueBase* pValueToSet;
		ArduinoTimer* pTimer;
	};

 public:
	OLEDMenu() {}

	OLEDMenu(OLEDPage& firstPage, OLED& oled)
		: currentPage(&firstPage), pOled(&oled), lastPage(NULL), viewMode(eVM_Menu), setString(false)
	{
		//menuStack.Push(&firstPage);		
	}

	/// <summary>
	/// Initialize the menu.
	/// </summary>
	/// <param name="blunoShield">The reference to the BlunoShield</param>
	/// <remarks>Only sets the BlunoShield pointer currently.</remarks>
	void Init(BlunoShield& blunoShield);

	/// <summary>
	/// Handles input from the joystick
	/// </summary>
	void Update();

	/// <summary>
	/// The current menu page (if there is one).
	/// </summary>
	/// <returns>The current menu page (if there is one)</returns>
	OLEDPage* CurrentPage() const
	{
		return menuStack.Peek();
	}

	/// <summary>
	/// Draws the menu. If we are on a current page, draw it.
	/// If we are setting a value, draw that instead.
	/// </summary>
	void Draw()
	{
		switch (viewMode)
		{
			case eVM_Menu:
			{
				if (CurrentPage() != NULL)
					CurrentPage()->Draw(*pOled);
			}
				break;

			case eVM_SetValue:
			{
				DrawSetValueMode();
			}
				break;

			case eVM_DrawTimer:
			{
				DrawTimerMode();
			}
				break;
		}
	}

	/// <summary>
	/// Draws a timer meter
	/// </summary>
	void DrawTimerMode()
	{
		//Serial.print("Draw timer mode: 0x"); Serial.println((int)pTimer, HEX);

		//draw the meter based on knob value
		pBlunoShield->drawMeter(pTimer->GetTimeRemaining(), pTimer->timerLength, 0, 33, 127, 20);

		//At the top of the screen,
		pOled->setPrintPos(0, 8);
		//print the name of the value being changed
		pOled->print(/*pValueToSet->GetName()*/"Time Remaining:");
		//followed by a ':' colon character
		//pOled->print(F(": "));
		pOled->print(max(pTimer->GetTimeRemaining() / 1000, 0));
		//then, print the current value
		//pValueToSet->PrintCurrentValue(pOled);
		//and then the (current) target value (as set by the current relative knob value)
		//pOled->print(F(" -> "));
		//pValueToSet->PrintNewValue(pOled, pBlunoShield->GetKnobValue());

		//draw the arrow icon to indicate current value position on the meter
		//pOled->drawXBMP(pValueToSet->GetArrowXPos() - 4, 10, 9, 16, downArrowIcon);

		//print the minimum possible value at the bottom, on the left
		/*pOled->setPrintPos(0, 63);
		pOled->print(F("Min: "));
		pValueToSet->PrintMinValue(pOled);*/

		//print the maximum possible value on the bottom, on the right
		/*pOled->setPrintPos(80, 63);
		pOled->print(F("Max: "));
		pValueToSet->PrintMaxValue(pOled);*/
	}

	/// <summary>
	/// Do this if we are currently setting a value instead of drawing the menu
	/// </summary>
	void DrawSetValueMode()
	{
		//if this menu option sets a string value
		if (setString)
		{
			//What input sub-system are we using?
			switch (inputMode)
			{
				case eIS_Numbers:
				{
					//Print the numbers 0 - 9
					for (byte c = '0'; c <= '9'; c++)
					{
						pOled->setPrintPos(((c - '0') % 9) * 14 + 3, ((c - '0') / 9) * 12 + 10);
						pOled->print((char)c);
					}

					//Draw the frame around the currently selected digit
					byte letterN = map(pBlunoShield->GetKnobValue(), 0, 1023, 0, 9);
					pOled->drawFrame((letterN % 9) * 14, (letterN / 9) * 12, 11, 12);

					//Draw the selected digit at the bottom
					pOled->setPrintPos(0, 60);
					pOled->print((char)map(pBlunoShield->GetKnobValue(), 0, 1023, '0', '9'));
				}
				break;

				case eIS_Lowercase:
				{
					//print the lower case letters
					for (byte c = 'a'; c <= 'z'; c++)
					{
						pOled->setPrintPos(((c - 'a') % 9) * 14 + 2, ((c - 'a') / 9) * 12 + 10);
						pOled->print((char)c);
					}

					//Draw the frame around the currently selected letter
					byte letterN = map(pBlunoShield->GetKnobValue(), 0, 1023, 0, 25);
					pOled->drawFrame((letterN % 9) * 14, (letterN / 9) * 12, 11, 12);

					//Draw the selected letter at the bottom
					pOled->setPrintPos(0, 60);
					pOled->print((char)map(pBlunoShield->GetKnobValue(), 0, 1023, 'a', 'z'));
				}
				break;

				case eIS_Uppercase:
				{
					//Draw the upper case letters
					for (byte c = 'A'; c <= 'Z'; c++)
					{
						pOled->setPrintPos(((c - 'A') % 9) * 14 + 2, ((c - 'A') / 9) * 12 + 10);
						pOled->print((char)c);
					}
					
					//Draw the frame around the currently selected letter
					byte letterN = map(pBlunoShield->GetKnobValue(), 0, 1023, 0, 25);
					pOled->drawFrame((letterN % 9) * 14, (letterN / 9) * 12, 11, 12);

					//Draw the selected letter at the bottom
					pOled->setPrintPos(0, 60);
					pOled->print((char)map(pBlunoShield->GetKnobValue(), 0, 1023, 'A', 'Z'));
				}
				break;

				case eIS_Symbols:
				{
					static const char symbols[] =
					{
						'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '~',
						':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}'
					};

					//Draw the symbols in the above list
					for (byte c = 0; c < 32; c++)
					{
						pOled->setPrintPos((c % 9) * 14 + 3, (c / 9) * 12 + 10);
						pOled->print(symbols[c]);
					}

					//Draw the frame around the currently selected symbol
					byte letterN = map(pBlunoShield->GetKnobValue(), 0, 1023, 0, 31);
					pOled->drawFrame((letterN % 9) * 14, (letterN / 9) * 12, 11, 12);

					//Draw the selected symbol at the bottom
					pOled->setPrintPos(0, 60);
					pOled->print((char)map(pBlunoShield->GetKnobValue(), 0, 1023, 0, 31));
				}
				break;
			}
		}
		else // otherwise, do the usual numerical meter
		{
			//draw the meter based on knob value
			pBlunoShield->drawKnobMeter(0, 33, 127, 20);

			//At the top of the screen,
			pOled->setPrintPos(0, 8);
			//print the name of the value being changed
			pOled->print(pValueToSet->GetName());
			//followed by a ':' colon character
			pOled->print(F(": "));
			//then, print the current value
			pValueToSet->PrintCurrentValue(pOled);
			//and then the (current) target value (as set by the current relative knob value)
			pOled->print(F(" -> "));
			pValueToSet->PrintNewValue(pOled, pBlunoShield->GetKnobValue());

			//draw the arrow icon to indicate current value position on the meter
			pOled->drawXBMP(pValueToSet->GetArrowXPos() - 4, 10, 9, 16, downArrowIcon);

			//print the minimum possible value at the bottom, on the left
			pOled->setPrintPos(0, 63);
			pOled->print(F("Min: "));
			pValueToSet->PrintMinValue(pOled);

			//print the maximum possible value on the bottom, on the right
			pOled->setPrintPos(80, 63);
			pOled->print(F("Max: "));
			pValueToSet->PrintMaxValue(pOled);
		}
	}

	/// <summary>
	/// Go to the previous page
	/// </summary>
	void PreviousPage()
	{
		switch (viewMode)
		{
			case eVM_Menu:
			{
				//otherwise, go back up one menu level
				if (menuStack.Count() > 1)
					menuStack.Pop();
				else
					pBlunoShield->SetDrawMode(BlunoShield::eDM_TempAndHumidity);
			}
				break;

			case eVM_SetValue:
			{
				//If we are setting a value, just close out and go back
				viewMode = eVM_Menu;
			}
				break;

			case eVM_DrawTimer:
			{
				viewMode = eVM_Menu;
			}
				break;
		}
	}

protected:
};

#endif

