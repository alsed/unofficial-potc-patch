
//nclude "DIALOGS\Isenbrandt Jurcksen_dialog.h"

void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	ref PChar;
	PChar = GetMainCharacter();

	
	switch(Dialog.CurrentNode)
	{
		// ----------------------------------- Диалог первый - первая встреча
		case "First time":
			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			Dialog.snd = "dialogs\0\009";

			NextDiag.TempNode = "first time";
			
			dialog.text = DLG_TEXT[0];
			link.l1 = DLG_TEXT[1];
			link.l1.go = "exit";

			if ( CheckQuestAttribute("story_FindRheims_IsenbrandtTask","pirate_arrest") )
			{
				AddDialogExitQuest("Herald_from_Redmond_comming_to_Isenbrandt");
				dialog.text = DLG_TEXT[2] + Characters[getCharacterIndex(DLG_TEXT[3])].lastname + DLG_TEXT[4];
				link.l1 = DLG_TEXT[5];
				link.l1.go = "AskingAboutRheims";
				PChar.quest.story_FindRheims_IsenbrandtTask = "neutral";
			}
			if ( CheckQuestAttribute("story_FindRheims_IsenbrandtTask","enemy") )
			{
				dialog.text = DLG_TEXT[6];
				link.l1 = DLG_TEXT[7];
				link.l1.go = "exit";
				AddDialogExitQuest("Herald_already_prevent_Isenbrandt");
			}
			if (pchar.quest.main_line == "letter_to_Isenbrandt")
			{
				if (makeint(pchar.reputation) > 60 || makeint(pchar.reputation) < 30)
				{
					dialog.text = pcharrepphrase(DLG_TEXT[8] + pchar.name + DLG_TEXT[9], DLG_TEXT[10] + pchar.name + DLG_TEXT[11]);
					link.l1 = pcharrepphrase(DLG_TEXT[12], DLG_TEXT[13]);
					link.l1.go = "node_2";
				}
				else
				{			
					dialog.text = DLG_TEXT[14];
					link.l1 = DLG_TEXT[15] + pchar.name + " " + pchar.lastname + DLG_TEXT[16];
					link.l1.go = "node_2";
				}
			}
			if (CheckQuestAttribute("pirate_line", "accepted"))
			{
				dialog.text = DLG_TEXT[52];
				link.l1 = DLG_TEXT[53];
				link.l1.go = "pirate_line";
			}
		break;

		case "node_2":
			dialog.text = DLG_TEXT[17];
			link.l1 = DLG_TEXT[18];
			link.l1.go = "node_3";
		break;

		case "node_3":
			dialog.text = DLG_TEXT[19];
			link.l1 = DLG_TEXT[20] + characters[GetCharacterIndex(DLG_TEXT[21])].lastname + DLG_TEXT[22];
			link.l1.go = "node_4";
		break;

		case "node_4":
			dialog.text = characters[GetCharacterIndex(DLG_TEXT[23])].lastname + DLG_TEXT[24];
			link.l1 = DLG_TEXT[25];
			link.l1.go = "exit";
			characters[GetCharacterIndex("Robert Christopher Silehard")].dialog.CurrentNode = "letter_to_Isenbrandt_complete_node";
			pchar.quest.main_line = "letter_to_Isenbrandt_complete";
			AddQuestRecord("blaze_to_Isenbrandt", "2");
		break;

		case "AskingAboutRheims":
			dialog.text = DLG_TEXT[26];
			link.l1 = DLG_TEXT[27];
			link.l1.go = "AskingAboutRheims_1";		
		break;

		case "AskingAboutRheims_1":
			dialog.text = DLG_TEXT[28];
			link.l1 = DLG_TEXT[29];
			link.l1.go = "AskingAboutRheims_2";		
		break;

		case "AskingAboutRheims_2":
			dialog.text = DLG_TEXT[30];
			link.l1 = DLG_TEXT[31];
			link.l1.go = "AskingAboutRheims_3";		
		break;

		case "AskingAboutRheims_3":
			dialog.text = DLG_TEXT[32];
			link.l1 = DLG_TEXT[33];
			link.l1.go = "exit";		
		break;
		
		case "third time":
			dialog.text = DLG_TEXT[34];
			link.l1 = DLG_TEXT[35];
			link.l1.go = "black_pearl";
			
			NextDiag.TempNode = "third time two";
		break;
		
		case "black_pearl":
			dialog.text = DLG_TEXT[36];
			link.l1 = DLG_TEXT[37];
			link.l1.go = "black_pearl_2";
		break;
		
		case "black_pearl_2":
			dialog.text = DLG_TEXT[38];
			link.l1 = DLG_TEXT[39];
			link.l1.go = "black_pearl_3";
		break;
		
		case "black_pearl_3":
			dialog.text = DLG_TEXT[40];
			link.l1 = DLG_TEXT[41];
			link.l1.go = "third time two";
		break;
		
		case "third time two":
			dialog.text = DLG_TEXT[42];
			link.l1 = DLG_TEXT[43];
			link.l1.go = "exit";
			if (CheckQuestAttribute("smuggler_line", "third_mission_speak_with_pirate_head"))
			{
				dialog.text = DLG_TEXT[44];
				link.l1 = DLG_TEXT[45];
				link.l1.go = "smuggler_line";
			}
		break;
		
		case "smuggler_line":
			dialog.text = DLG_TEXT[46];
			link.l1 = DLG_TEXT[47];
			link.l1.go = "exit";
			if (CheckQuestAttribute("main_line", "completed"))
			{
				dialog.text = DLG_TEXT[48];
				link.l1 = DLG_TEXT[49];
				link.l1.go = "possible_pirate_line";
			}
			else
			{
				dialog.text = DLG_TEXT[50];
				link.l1 = DLG_TEXT[51];
				link.l1.go = "smuggler_line_3";
			}
			if (CheckQuestAttribute("pirate_line", "sink_black_pearl"))
			{
				dialog.text = DLG_TEXT[54];
				link.l1 = DLG_TEXT[55];
				link.l1.go = "sink_black_pearl";
				link.l2 = DLG_TEXT[56];
				link.l2.go = "sink_black_pearl_fight";
			}
			if (CheckQuestAttribute("pirate_line", "capture_black_pearl"))
			{
				dialog.text = DLG_TEXT[57];
				link.l1 = DLG_TEXT[58];
				link.l1.go = "capture_black_pearl";
				link.l2 = DLG_TEXT[59];
				link.l2.go = "capture_black_pearl_fight";
			}
		break;
		
		case "possible_pirate_line":
			dialog.text = DLG_TEXT[60];
			link.l1 = DLG_TEXT[61];
			link.l1.go = "possible_pirate_line_2";
			link.l2 = DLG_TEXT[62];
			link.l2.go = "smuggler_line_2";
		break;
		
		case "smuggler_line_2":
			dialog.text = DLG_TEXT[63];
			link.l1 = DLG_TEXT[64]
			link.l1.go = DLG_TEXT[65];
		break;
		
		case "smuggler_line_3":
			dialog.text = DLG_TEXT[66];
			link.l1 = DLG_TEXT[67];
			link.l1.go = "smuggler_line_4";
		break;
		
		case "smuggler_line_4":
			dialog.text = DLG_TEXT[68];
			link.l1 = DLG_TEXT[69];
			link.l1.go = "smuggler_line_5";
		break;
		
		case "smuggler_line_5":
			dialog.text = DLG_TEXT[70];
			link.l1 = DLG_TEXT[71];
			link.l1.go = "smuggler_line_6";
		break;
		
		case "smuggler_line_6":
			dialog.text = DLG_TEXT[72];
			link.l1 = DLG_TEXT[73];
			link.l1.go = "exit";
			AddDialogExitQuest("third_mission_pirate_head_exit_from_shore");
		break;
		
		case "possible_pirate_line_2":
			dialog.text = DLG_TEXT[74];
			link.l1 = DLG_TEXT[75];
			link.l1.go = "possible_pirate_line_3";
		break;
		
		case "possible_pirate_line_3":
			dialog.text = DLG_TEXT[76];
			link.l1 = DLG_TEXT[77];
			link.l1.go = "possible_pirate_line_4";
		break;
		
		case "possible_pirate_line_4":
			dialog.text = DLG_TEXT[78];
			link.l1 = DLG_TEXT[79];
			link.l1.go = "exit";
			AddDialogExitQuest("possible_pirate_line");
		break;
		
		case "pirate_line":
			dialog.text = DLG_TEXT[80];
			link.l1 = DLG_TEXT[81];
			link.l1.go = "pirate_line_2";
		break;
		
		case "pirate_line_2":
			dialog.text = DLG_TEXT[82];
			link.l1 = DLG_TEXT[83];
			link.l1.go = "pirate_line_3";
		break;
		
		case "pirate_line_3":
			dialog.text = DLG_TEXT[84];
			link.l1 = DLG_TEXT[85];
			link.l1.go = "pirate_line_4";
		break;
		
		case "pirate_line_4":
			dialog.text = DLG_TEXT[86];
			link.l1 = DLG_TEXT[87];
			link.l1.go = "pirate_line_5";
		break;
		
		case "pirate_line_5":
			dialog.text = DLG_TEXT[88];
			link.l1 = DLG_TEXT[89];
			link.l1.go = "exit";
			AddDialogExitQuest("pirate_line_to_black_pearl");
		break;
		
		case "sink_black_pearl":
			dialog.text = DLG_TEXT[90];
			link.l1 = DLG_TEXT[91];
			link.l1.go = "exit";
			AddDialogExitQuest("sink_black_pearl_no_fight");
		break;
		
		case "sink_black_pearl_fight":
			dialog.text = DLG_TEXT[92];
			link.l1 = DLG_TEXT[93];
			link.l1.go = "exit";
			AddDialogExitQuest("sink_black_pearl_fight");
		break;
		
		case "capture_black_pearl":	
			dialog.text = DLG_TEXT[94];
			link.l1 = DLG_TEXT[95];
			link.l1.go = "exit";
			AddDialogExitQuest("capture_black_pearl_no_fight");
		break;
		
		case "capture_black_pearl_fight":
			dialog.text = DLG_TEXT[96];
			link.l1 = DLG_TEXT[97];
			link.l1.go = "exit";
			AddDialogExitQuest("sink_black_pearl_fight");
		break;

		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
		break;
	}
}
