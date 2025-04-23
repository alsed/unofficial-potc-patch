
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
			
			if (CheckQuestAttribute("smuggler_line", "fifth_mission_meet_with_barrakuda"))
			{
				dialog.text = DLG_TEXT[2];
				link.l1 = DLG_TEXT[3];
				link.l1.go = "fifth_mission_meet_with_barrakuda";
			}
			if (CheckQuestAttribute("smuggler_line", "fifth_mission_wait_for_join_barrakuda"))
			{
				dialog.text = DLG_TEXT[4];
				if (SetCompanionIndex(Pchar, -1, GetCharacterIndex("Barrakuda")) == GetCharacterIndex("Barrakuda"))
				{
					link.l1 = DLG_TEXT[5];
					link.l1.go = "exit";
					AddDialogExitQuest("fifth_mission_accept_second");
				}
				else
				{
					link.l1 = DLG_TEXT[6];
					link.l1.go = "exit";
				}
			}
			
			NextDiag.TempNode = "First time";
		break;
		
		case "fifth_mission_meet_with_barrakuda":
			dialog.text = DLG_TEXT[7];
			link.l1 = DLG_TEXT[8];
			link.l1.go = "fifth_mission_meet_with_barrakuda_2";
		break;
		
		case "fifth_mission_meet_with_barrakuda_2":
			dialog.text = DLG_TEXT[9];
			link.l1 = DLG_TEXT[10];
			link.l1.go = "fifth_mission_meet_with_barrakuda_3";
		break;
		
		case "fifth_mission_meet_with_barrakuda_3":
			dialog.text = DLG_TEXT[11];
			link.l1 = DLG_TEXT[12];
			link.l1.go = "fifth_mission_meet_with_barrakuda_4";
		break;
		
		case "fifth_mission_meet_with_barrakuda_4":
			dialog.text = DLG_TEXT[13];
			link.l1 = DLG_TEXT[14];
			link.l1.go = "fifth_mission_meet_with_barrakuda_5";
		break;
		
		case "fifth_mission_meet_with_barrakuda_5":
			SetCompanionIndex(Pchar, -1, GetCharacterIndex("Barrakuda"));
			if (IsCompanion(characterFromID("Barrakuda")))
			{
				dialog.text = DLG_TEXT[15];
				link.l1 = DLG_TEXT[16];
				link.l1.go = "exit";
				AddDialogExitQuest("fifth_mission_accept");
			}
			else
			{
				dialog.text = DLG_TEXT[17];
				link.l1 = DLG_TEXT[18];
				link.l1.go = "exit";
				AddDialogExitQuest("fifth_mission_wait_for_join_barrakuda");
			}
		break;

		case "Exit":
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
		break;
	}
}
