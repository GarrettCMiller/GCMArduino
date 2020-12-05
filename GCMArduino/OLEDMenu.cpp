// 
// 
// 

#include "OLEDMenu.h"


void OLEDMenu::Init(BlunoShield& blunoShield)
{
	pBlunoShield = &blunoShield;
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
			Serial.print(F("Opening menu "));
			Serial.println(Text);
			menu.lastPage = pParentPage;
			menu.menuStack.Push(goToMenu);
			break;

		case eAction_SetValue:
			Serial.print(F("Setting value "));
			Serial.println(Text);
			menu.setValueMode = true;
			menu.pValueToSet = pValue;
			menu.setString = pValue->IsString();
			break;

		case eAction_Other:
			Serial.println("Other action...");
			if (otherAction != NULL)
				otherAction();
			break;
	}

	return true;
}

void OLEDPage::HandleInput(BlunoShield& blunoShield)
{
	switch (blunoShield.GetJoystickValue())
	{
		case eJoy_Up:
			if (pOLEDMenu->setValueMode)
			{
				switch (pOLEDMenu->mode)
				{
					case eIS_Numbers:
						pOLEDMenu->mode = eIS_Symbols;
						break;

					case eIS_Lowercase:
						pOLEDMenu->mode = eIS_Uppercase;
						break;

					case eIS_Uppercase:
						pOLEDMenu->mode = eIS_Numbers;
						break;

					case eIS_Symbols:
						pOLEDMenu->mode = eIS_Lowercase;
						break;
				}
			}
			else if (currentMenuOption > 0 || pageNumber > 0)
			{
				if (currentMenuOption == 0)
				{
					pageNumber--;
					currentMenuOption = MAX_OPTIONS_PER_PAGE - 1;
				}
				else
					currentMenuOption--;

				delay(500);
			}
			break;

		case eJoy_Down:
			//uint8_t index = pageNumber * MAX_OPTIONS_PER_PAGE + currentMenuOption;
			//if (index < numOptions - 1)
			if (pOLEDMenu->setValueMode)
			{
				switch (pOLEDMenu->mode)
				{
					case eIS_Numbers:
						pOLEDMenu->mode = eIS_Uppercase;
						break;

					case eIS_Lowercase:
						pOLEDMenu->mode = eIS_Symbols;
						break;

					case eIS_Uppercase:
						pOLEDMenu->mode = eIS_Lowercase;
						break;

					case eIS_Symbols:
						pOLEDMenu->mode = eIS_Numbers;
						break;
				}
			}
			else if (currentMenuOption < numOptions - 1)
			{
				if (currentMenuOption == MAX_OPTIONS_PER_PAGE - 1)
				{
					pageNumber++;
					currentMenuOption = 0;
				}
				else
					currentMenuOption++;

				delay(500);
			}
			break;

		case eJoy_Push:
			if (!pOLEDMenu->setValueMode)
			{
				uint8_t index = pageNumber * MAX_OPTIONS_PER_PAGE + currentMenuOption;
				options[index].Execute(*pOLEDMenu);
			}
			else
				pOLEDMenu->pValueToSet->SetNewValue(pOLEDMenu->pValueToSet->MapKnobValue(blunoShield.GetKnobValue()));
			delay(500);
			break;

		case eJoy_Left:
			pageNumber = 0;
			pOLEDMenu->PreviousPage();
			delay(500);
			break;

		case eJoy_Right:
			break;

		default:
			break;
	}
}

