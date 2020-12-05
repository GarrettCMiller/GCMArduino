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
/// A singular collection of 'MenuOption's that represents a particular "page" of related selections;
/// a menu system is comprised of one (or, usually, more) OLEDPage's that each contain a list of selectable actions,
/// some linking to other OLEDPage's, some alowing variable viewing/adjustment, etc...
/// </summary>
class OLEDPage
{
	static const uint8_t MAX_OPTIONS = 10;
	static const uint8_t MAX_OPTIONS_PER_PAGE = 4;
	static const bool USE_TITLE_BAR = true;

protected:
	char name[32];
	OLEDMenu* pOLEDMenu;
	MenuOption options[MAX_OPTIONS];
	uint8_t numOptions;
	uint8_t pageNumber;

public:
	uint8_t offsetx;
	uint8_t offsety;
	bool hasBackButton;

	uint8_t currentMenuOption;

	const u8g_fntpgm_uint8_t* font;
public:
	OLEDPage(String _name, int8_t numOps, MenuOption ops[], bool hasBack = true)
		: pageNumber(0), offsetx(0), offsety(0), pOLEDMenu(NULL), currentMenuOption(0)
	{
		strncpy(name, _name.c_str(), 32);
		if (numOps <= MAX_OPTIONS)
		{
			font = u8g_font_helvB08;
			hasBackButton = hasBack;
			numOptions = numOps;

			for (int o = 0; o < numOps; o++)
			{
				options[o] = ops[o];
				options[o].pParentPage = this;
				//Serial.print(F("Adding option ")); Serial.println(ops[o].Text);
			}
		}
		/*else
			Serial.println(F("Too many menu options!"));*/

		currentMenuOption = 0;
	}

	void Init(OLEDMenu& menu)
	{
		SetOLEDMenu(menu);
	}

	void SetOLEDMenu(OLEDMenu& menu)
	{
		pOLEDMenu = &menu;
	}

	void Draw(OLED& oled)
	{		
		uint8_t lineOffset = 12;
		uint8_t x = 8 + offsetx;
		uint8_t y = 12 + offsety;

		oled.setFont(font);

		//Draw menu header divider line
		oled.drawLine(0, y, 113, y);
		
		oled.setPrintPos(0, y - 4);
		oled.print(name);
		y += lineOffset;

		for (int o = 0; o < MAX_OPTIONS_PER_PAGE; o++, y += lineOffset)
		{
			if (currentMenuOption == o)
			{
				oled.setPrintPos(0, y);
				oled.print(F(">"));
			}

			uint8_t optionIndex = pageNumber * MAX_OPTIONS_PER_PAGE + o;
			if (optionIndex < numOptions)
			{
				oled.setPrintPos(x, y);
				oled.print(options[optionIndex].Text);
			}
		}

		//if more than one (1) page of options, draw page counter
		uint8_t numPages = GetNumPages();
		if (numPages > 1)
		{
			oled.setPrintPos(107, 34);
			oled.print(pageNumber + 1);
			oled.print("/");
			oled.print(numPages);

			if (pageNumber < numPages - 1)
				oled.drawXBMP(115, 43, 9, 16, downArrowIcon);
			else if (pageNumber > 0)
				oled.drawXBMP(115, 1, 9, 16, upArrowIcon);;
		}
	}

	void HandleInput(BlunoShield& blunoShield);

	uint8_t GetNumOptions() const
	{
		return numOptions;
	}

	uint8_t GetNumPages() const
	{
		return (numOptions / (MAX_OPTIONS_PER_PAGE + 1)) + 1;
	}

	String GetOption(uint8_t index)
	{
		return options[index].Text;
	}

	uint8_t cursorIndex = 0;
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
 protected:
	
public:
	Stack<OLEDPage*> menuStack;
	//OLEDPage* currentPage;
	OLEDPage* lastPage;
	OLED* pOled;
	BlunoShield* pBlunoShield;	

	EInputSet mode = eIS_Uppercase;

	bool setValueMode;
	bool setString;

	RangedValueBase* pValueToSet;

 public:
	 OLEDMenu() {}
	OLEDMenu(OLEDPage& firstPage, OLED& oled)
		: /*currentPage(firstPage),*/ pOled(&oled), lastPage(NULL), setValueMode(false), setString(false)
	{
		menuStack.Push(&firstPage);		
	}

	void Init(BlunoShield& blunoShield);

	void Update();

	OLEDPage* CurrentPage() const
	{
		return menuStack.Peek();
	}

	void Draw()
	{
		if (setValueMode)
			DrawSetValueMode();
		else if (CurrentPage() != NULL)
			CurrentPage()->Draw(*pOled);
	}

	void DrawSetValueMode()
	{
		//if this menu option sets a string value
		if (setString)
		{
			//for (byte c = 33; c < 126; c++)
			switch (mode)
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

			//print the name of the value being changed, followed by a ':' colon character
			pOled->setPrintPos(0, 8);
			pOled->print(pValueToSet->GetName());
			pOled->print(F(": "));

			//print the current value
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

	void PreviousPage()
	{
		if (setValueMode)
			setValueMode = false;
		else
		{
			if (menuStack.Count() > 1)
			{
				menuStack.Pop();
			}
			else
				pBlunoShield->SetDrawMode(BlunoShield::eDM_TempAndHumidity);
		}
	}

protected:
};

#endif

