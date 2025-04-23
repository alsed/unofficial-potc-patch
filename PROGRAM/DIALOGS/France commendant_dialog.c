
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
			Link.l1 = DLG_TEXT[1];
			link.l1.go = "exit";
			
			NextDiag.TempNode = "First time";
		break;

		case "Exit":
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
		break;
	}
}
