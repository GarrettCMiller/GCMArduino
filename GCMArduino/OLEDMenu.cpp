// 
// 
// 

#include "OLEDMenu.h"


void OLEDMenu::Init(BlunoShield& blunoShield)
{
	pBlunoShield = &blunoShield;
	menuStack.Push(currentPage);
}

void OLEDMenu::Update()
{
	if (CurrentPage() != NULL)
		CurrentPage()->HandleInput(*pBlunoShield);
}

bool MenuOption::Execute(OLEDMenu& menu)
{
	switch (actionType)
	{
		case eAction_GoToMenu:
		{
			Serial.print(F("Opening menu "));
			Serial.println(Text);
			menu.lastPage = pParentPage;
			menu.menuStack.Push(goToMenu);
		}
			break;

		case eAction_SetValue:
		{
			Serial.print(F("Setting value "));
			Serial.println(Text);
			menu.viewMode = OLEDMenu::eVM_SetValue;
			menu.pValueToSet = pValue;
			if (menu.pValueToSet->HasCallback())
				menu.pValueToSet->SetNewValue(pValue);
			menu.setString = pValue->IsString();
		}
			break;

		case eAction_ViewTimer:
		{
			Serial.print(F("Viewing timer "));
			Serial.println(Text);
			menu.viewMode = OLEDMenu::eVM_DrawTimer;
			menu.pTimer = pTimer;
		}
			break;

		case eAction_Other:
		{
			Serial.println("Other action...");
			if (otherAction != NULL)
				otherAction();
		}
			break;
	}

	return true;
}

void OLEDPage::HandleInput(BlunoShield& blunoShield)
{
	switch (blunoShield.GetJoystickValue())
	{
		case eJoy_Up:
		{
			///*Serial.print("up:");*/ Serial.println((*pOLEDMenu).setValueMode);
			/*Serial.print(this->name);
			Serial.print(", Addr: ");
			Serial.println(int(pOLEDMenu), HEX);*/
			switch (pOLEDMenu->viewMode)
			{
				case OLEDMenu::eVM_SetValue:
				{
					switch (pOLEDMenu->inputMode)
					{
						case eIS_Numbers:
							pOLEDMenu->inputMode = eIS_Symbols;
							break;

						case eIS_Lowercase:
							pOLEDMenu->inputMode = eIS_Uppercase;
							break;

						case eIS_Uppercase:
							pOLEDMenu->inputMode = eIS_Numbers;
							break;

						case eIS_Symbols:
							pOLEDMenu->inputMode = eIS_Lowercase;
							break;
					}
				}
					break;

				case OLEDMenu::eVM_Menu:
				{
					if (currentMenuOption > 0 || pageNumber > 0)
					{
						if (currentMenuOption == 0)
						{
							pageNumber--;
							currentMenuOption = MAX_OPTIONS_PER_PAGE - 1;
						}
						else
							currentMenuOption--;
					}
				}
					break;
			}

			delay(500);
		}
			break;

		case eJoy_Down:
		{
			//Serial.print("down:"); Serial.println(pOLEDMenu->setValueMode);
			//uint8_t index = pageNumber * MAX_OPTIONS_PER_PAGE + currentMenuOption;
			//if (index < numOptions - 1)
			switch (pOLEDMenu->viewMode)
			{
				case OLEDMenu::eVM_SetValue:
				{
					switch (pOLEDMenu->inputMode)
					{
						case eIS_Numbers:
							pOLEDMenu->inputMode = eIS_Uppercase;
							break;

						case eIS_Lowercase:
							pOLEDMenu->inputMode = eIS_Symbols;
							break;

						case eIS_Uppercase:
							pOLEDMenu->inputMode = eIS_Lowercase;
							break;

						case eIS_Symbols:
							pOLEDMenu->inputMode = eIS_Numbers;
							break;
					}
				}
					break;

				case OLEDMenu::eVM_Menu:
				{
					if (currentMenuOption < numOptions - 1)
					{
						if (currentMenuOption == MAX_OPTIONS_PER_PAGE - 1)
						{
							pageNumber++;
							currentMenuOption = 0;
						}
						else
							currentMenuOption++;
					}
				}
					break;
			}

			delay(500);
		}
			break;

		case eJoy_Push:
		{
			//Serial.println(/*"Push"*/pOLEDMenu->viewMode);
			switch (pOLEDMenu->viewMode)
			{
				case OLEDMenu::eVM_Menu:
				{
					uint8_t index = pageNumber * MAX_OPTIONS_PER_PAGE + currentMenuOption;
					//Serial.println("Index = " + String(index));
					options[index].Execute(*pOLEDMenu);
					//Serial.println("Done executing push!");
				}
				break;

				case OLEDMenu::eVM_SetValue:
				{
					Serial.println("Setting new value...");
					long value = pOLEDMenu->pValueToSet->MapKnobValue(blunoShield.GetKnobValue());
					Serial.println(value);
					pOLEDMenu->pValueToSet->SetNewValue(value);
				}
				break;

				default:
					Serial.println("Other push mode");
			}

			delay(500);
		}
			break;

		case eJoy_Left:
		{
			pageNumber = 0;
			pOLEDMenu->PreviousPage();
			delay(500);
		}
			break;

		case eJoy_Right:
		{

		}
			break;

		default:
		{

		}
			break;
	}
}

