
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
			
			if (CheckQuestAttribute("smuggler_line", "smuggler_line_first_mission_meet_soldiers"))
			{
				dialog.text = DLG_TEXT[2];
				link.l1 = DLG_TEXT[3];
				link.l1.go = "exit";
				AddDialogExitQuest("smuggler_line_first_mission_arnaud_first_talk_with_soldiers");
			}
			if (CheckQuestAttribute("smuggler_line", "second_mission_money_prepare_for_troubles"))
			{
				dialog.text = DLG_TEXT[4];
				link.l1 = DLG_TEXT[5];
				link.l1.go = "second_mission_money_prepare_for_troubles";
			}
			if (CheckQuestAttribute("smuggler_line", "second_mission_to_tavern_room_wait_for_killers"))
			{
				dialog.text = DLG_TEXT[6] + PChar.name + " " + PChar.lastname + "?";
				link.l1 = DLG_TEXT[7];
				link.l1.go = "second_mission_to_tavern_room_wait_for_killers";
			}
			NextDiag.TempNode = "First time";
		break;
		
		case "second_mission_money_prepare_for_troubles":
			dialog.text = DLG_TEXT[8];
			link.l1 = DLG_TEXT[9];
			link.l1.go = "second_mission_money_prepare_for_troubles_search";
			link.l2 = DLG_TEXT[10];
			link.l2.go = "second_mission_money_prepare_for_troubles_not_search";
		break;
		
		case "second_mission_money_prepare_for_troubles_search":
			dialog.text = DLG_TEXT[11];
			link.l1 = DLG_TEXT[12];
			link.l1.go = "second_mission_money_prepare_for_troubles_search_2";
		break;
		
		case "second_mission_money_prepare_for_troubles_search_2":
			dialog.text = DLG_TEXT[13];
			link.l1 = DLG_TEXT[14];
			link.l1.go = "second_mission_money_prepare_for_troubles_search_3";
			link.l2 = DLG_TEXT[15];
			link.l2.go = "second_mission_money_prepare_for_troubles_not_search";
		break;
		
		case "second_mission_money_prepare_for_troubles_search_3":
			dialog.text = DLG_TEXT[16];
			link.l1 = DLG_TEXT[17];
			link.l1.go = "second_mission_money_prepare_for_troubles_search_4";
			link.l2 = DLG_TEXT[18];
			link.l2.go = "second_mission_money_prepare_for_troubles_not_search";
		break;
		
		case "second_mission_money_prepare_for_troubles_search_4":
			dialog.text = DLG_TEXT[19];
			link.l1 = DLG_TEXT[20];
			link.l1.go = "exit";
			AddDialogExitQuest("second_mission_money_to_france_governor");
		break;
		
		case "second_mission_money_prepare_for_troubles_not_search":
			dialog.text = DLG_TEXT[21];
			link.l1 = DLG_TEXT[22];
			link.l1.go = "exit";
			AddDialogExitQuest("second_mission_money_troubles_fight");
		break;
		
		case "second_mission_to_tavern_room_wait_for_killers":
			dialog.text = DLG_TEXT[23];
			link.l1 = DLG_TEXT[24];
			link.l1.go = "exit";
			AddDialogExitQuest("second_mission_fight_with_killers");
		break;

		case "Exit":
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
		break;
	}
}
