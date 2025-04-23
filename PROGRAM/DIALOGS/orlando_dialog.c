
void ProcessDialogEvent()
{
	ref NPChar;
	
	DeleteAttribute(&Dialog,"Links");
	aref Link, NextDiag;
	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	int iTest;

	string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;

	ref PChar;
	PChar = GetMainCharacter();


	switch(Dialog.CurrentNode)
	{
		// -----------------------------------Диалог первый - первая встреча
		case "First time":
			Dialog.Text = DLG_TEXT[0];
			link.l1 = DLG_TEXT[1];
			link.l1.go = "exit";
			link.l2 = DLG_TEXT[2];
			link.l2.go = "abordage";
			
			
			
			NextDiag.TempNode = "First time";
		break;
		
		case "abordage":
			dialog.text = DLG_TEXT[3];
			if (npchar.abordage_enable == "true")
			{
				link.l10 = DLG_TEXT[4];
			}
			else
			{
				link.l10 = DLG_TEXT[5];
			}
			link.l10.go = "abordage_switcher";
		break;
		
		case "abordage_switcher":
			dialog.text = DLG_TEXT[6];
			link.l1 = DLG_TEXT[7];
			link.l1.go = "exit";
			if (npchar.abordage_enable == "true")
			{
				npchar.abordage_enable = "false";
			}
			else
			{
				npchar.abordage_enable = "true";
			}
		break;

		case "Exit":
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
		break;
	}
}
