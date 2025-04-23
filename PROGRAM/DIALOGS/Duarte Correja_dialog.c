
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
	int iTest, NPC_meeting; 
	
	string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;

	
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

			dialog.snd = "Voice\DUCO\DUCO001";
			dialog.text = DLG_TEXT[0];
			link.l1 = pcharrepphrase(DLG_TEXT[1] + pchar.name + " " + pchar.lastname + DLG_TEXT[2], DLG_TEXT[3]);
			link.l1.go = "node_2";
			NextDiag.TempNode = "second time";
		break;

		case "node_2":
			dialog.snd = "Voice\DUCO\DUCO002";
			dialog.text = DLG_TEXT[4] + npchar.name + " " + npchar.lastname + DLG_TEXT[5];
			link.l1 = pcharrepphrase(DLG_TEXT[6] + npchar.name + DLG_TEXT[7], DLG_TEXT[8] + pchar.name + " " + pchar.lastname + DLG_TEXT[9]);
			link.l1.go = "second time";
		break;

		case "second time":
			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			
			dialog.snd = "Voice\DUCO\DUCO003";
			dialog.text = DLG_TEXT[10];
			link.l1 = pcharrepphrase(DLG_TEXT[11], DLG_TEXT[12]);
			link.l1.go = "crew";
			link.l2 = DLG_TEXT[13];
			link.l2.go = "rumours";
			link.l3 = pcharrepphrase(DLG_TEXT[14], DLG_TEXT[15]);
			link.l3.go = "room";
			link.l4 = DLG_TEXT[16];
			link.l4.go = "exit";
		break;

		case "rumours":
			iTest = 0;
			dialog.snd = "Voice\DUCO\DUCO004";
			dialog.text = DLG_TEXT[17] + address_form.por + DLG_TEXT[18];
			//вопрос блейза о Реймсе
			if (pchar.quest.main_line == "blaze_talk_with_innes_diaz_complete")
			{
				link.l1 = DLG_TEXT[19] + characters[GetCharacterIndex(DLG_TEXT[20])].lastname + DLG_TEXT[21];
				link.l1.go = "blaze_search_rheims";
			}
			if (pchar.quest.main_line ==  "blaze_talk_with_innes_diaz_complete_21" && iTest < QUEST_COUNTER)
			{
				link.l1 = DLG_TEXT[22] + characters[GetCharacterIndex(DLG_TEXT[23])].name + " " + characters[GetCharacterIndex(DLG_TEXT[24])].lastname + DLG_TEXT[25];
				link.l1.go = "search_rheims";
				iTest = iTest + 1;
			}
			if (pchar.quest.main_line == "search_for_Ferro_Cerezo"  && iTest < QUEST_COUNTER)
			{
				link.l2 = DLG_TEXT[26] + characters[GetCharacterIndex(DLG_TEXT[27])].name + " " + characters[GetCharacterIndex(DLG_TEXT[28])].lastname + DLG_TEXT[29];
				link.l2.go = "search_for_Ferro_Cerezo";
			}
//Hitman
			if(PChar.quest.Hitman == "mark_vito")
			{
				link.l3 = DLG_TEXT[30];
				link.l3.go = "vito";
			}
			if (CheckQuestAttribute("Voysey_lost", "1"))
			{
				link.l4 = DLG_TEXT[31];
				link.l4.go = "artois";
			}
			if (!CheckQuestAttribute("generate_convoy_quest_progress", "begin") && npchar.work != lastspeak_date)
			{
				link.l10 = DLG_TEXT[32];
				link.l10.go = "convoy";
			}
			if (checkquestattribute("smuggler_line", "fifth_mission_accepted"))
			{
				link.l11 = DLG_TEXT[33];
				link.l11.go = "fifth_mission_accepted";
			}
			link.l99 = DLG_TEXT[34];
			link.l99.go = "second time";
		break;
		
		case "artois":
			dialog.snd = "Voice\DUCO\DUCO005";
			dialog.text = DLG_TEXT[35];
			link.l1 = DLG_TEXT[36];
			link.l1.go = "exit";
			AddDialogExitQuest("artois_before_room_on_second_floor");
		break;

//Hitman
		case "vito":
			dialog.snd = "Voice\DUCO\DUCO006";
			dialog.text = DLG_TEXT[37]+ Characters[GetCharacterIndex(DLG_TEXT[38])].lastname +DLG_TEXT[39];
			link.l1 = DLG_TEXT[40];
			link.l1.go = "Exit";
		break;

		case "search_for_Ferro_Cerezo":
			dialog.snd = "Voice\DUCO\DUCO007";
			dialog.text = DLG_TEXT[41];
			link.l1 = DLG_TEXT[42];
			link.l1.go = "exit";
			pchar.quest.main_line = "search_for_Ferro_Cerezo_find";
		break;

		case "search_rheims":
			dialog.snd = "Voice\DUCO\DUCO008";
			dialog.text = DLG_TEXT[43];
			link.l1 = DLG_TEXT[44];
			link.l1.go = "search_rheims_2";
		break;

		case "search_rheims_2":
			dialog.snd = "Voice\DUCO\DUCO009";
			dialog.text = DLG_TEXT[45] + characters[GetCharacterIndex(DLG_TEXT[46])].name + " " + characters[GetCharacterIndex(DLG_TEXT[47])] + DLG_TEXT[48];
			link.l1 = DLG_TEXT[49] + characters[GetCharacterIndex(DLG_TEXT[50])].lastname + DLG_TEXT[51] + characters[GetCharacterIndex(DLG_TEXT[52])].lastname + DLG_TEXT[53];
			link.l1.go = "blaze_search_rheims_2";
		break;

		case "blaze_search_rheims":
			dialog.snd = "Voice\DUCO\DUCO010";
			dialog.text = DLG_TEXT[54];
			link.l1 = DLG_TEXT[55] + characters[GetCharacterIndex(DLG_TEXT[56])].name + " " + characters[GetCharacterIndex(DLG_TEXT[57])].lastname + DLG_TEXT[58];
			link.l1.go = "blaze_search_rheims_2";
		break;

		case "blaze_search_rheims_2":
			dialog.snd = "Voice\DUCO\DUCO011";
			dialog.text = DLG_TEXT[59];
			link.l1 = pcharrepphrase(DLG_TEXT[60], DLG_TEXT[61]);
			link.l1.go = "exit";
			pchar.quest.main_line = "blaze_talk_with_duarte";
			pchar.quest.exit_from_duarte.win_condition.l1 = "location";
			pchar.quest.exit_from_duarte.win_condition.l1.location = "Conceicao_town";
			pchar.quest.exit_from_duarte.win_condition = "exit_from_duarte_complete";
			LAi_SetHP(characterFromID("quest_smuggler_01"), 80.0, 80.0);
			ChangeCharacterAddress(&characters[GetCharacterIndex("quest_smuggler_01")], "Conceicao_town", "goto4");
			AddQuestRecord("Blaze_search_Rheims", "6");
			Locations[FindLocation("Conceicao_town")].reload.l3.disable = 1;
		break;

		case "crew":
			if (makeint(environment.time) > 22.0 || makeint(environment.time) < 10.0)
			{
				dialog.snd = "Voice\DUCO\DUCO012";
				Dialog.text = DLG_TEXT[62];
				link.l1 = DLG_TEXT[63];
				link.l1.go = "room";
				link.l2 = DLG_TEXT[64];
				link.l2.go = "rumours";
			}
			else
			{
				Dialog.CurrentNode = Dialog.TempNode;
				DialogExit();
				LaunchHireCrew();
			}
		break;

		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			if (pchar.location.group == "sit")
			{
				AddDialogExitQuest("exit_sit");
			}
		break;

		case "room":
			dialog.snd = "Voice\DUCO\DUCO013";
			dialog.text = DLG_TEXT[65];
			if (makeint(environment.time) >= 22.0 || makeint(environment.time) < 10.0)
			{

				link.l1 = DLG_TEXT[66];
				link.l1.go = "room_day";
			}
			else
			{
				link.l1 = DLG_TEXT[67];
				link.l1.go = "room_night";
			}
		break;

		case "room_day":
			dialog.snd = "Voice\DUCO\DUCO014";
			Dialog.text = DLG_TEXT[68];
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = DLG_TEXT[69];
				link.l1.go = "room_day_wait";
			}
			link.l2 = pcharrepphrase(DLG_TEXT[70], DLG_TEXT[71]);
			link.l2.go = "hall_day_wait";
			link.l3 = DLG_TEXT[72];
			link.l3.go = "second time";
		break;

		case "room_night":
			dialog.snd = "Voice\DUCO\DUCO014";
			Dialog.text = DLG_TEXT[73];
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = DLG_TEXT[74];
				link.l1.go = "room_night_wait";
			}
			link.l2 = pcharrepphrase(DLG_TEXT[75], DLG_TEXT[76]);
			link.l2.go = "hall_night_wait";
			link.l3 = DLG_TEXT[77];
			link.l3.go = "second time";
		break;

		case "room_night_wait":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate2("wait_night");
		break;

		case "hall_night_wait":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
			TavernWaitDate("wait_night");
		break;

		case "room_day_wait":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate2("wait_day");
		break;

		case "hall_day_wait":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
			TavernWaitDate("wait_day");
		break;

		case "tavern_keeper":
			NextDiag.TempNode = "second time";
			
			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			Dialog.snd = "dialogs\0\009";

			dialog.snd = "Voice\DUCO\DUCO015";
			dialog.text = DLG_TEXT[78];
			link.l1 = DLG_TEXT[79];
			link.l1.go = "exit_sit";
		break;

		case "exit_sit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			NPChar.quest.meeting = NPC_Meeting;
			DialogExit();
			AddDialogExitQuest("exit_sit");
		break;
		
		case "convoy":
			npchar.work = lastspeak_date;
			if (GetCompanionIndex(pchar,1) != -1 && GetCompanionIndex(pchar,2) != -1 && GetCompanionIndex(pchar,3) != -1)
			{
				dialog.text = DLG_TEXT[80];
				link.l1 = DLG_TEXT[81];
				link.l1.go = "exit";
			}
			else
			{
				if (GetNationRelation2MainCharacter(PORTUGAL) == RELATION_ENEMY)
				{
					dialog.text = DLG_TEXT[82];
					link.l1 = DLG_TEXT[83];
					link.l1.go = "exit";
				}
				else
				{
					if (makeint(pchar.reputation) <25)
					{
						dialog.text = DLG_TEXT[84];
						link.l1 = DLG_TEXT[85];
						link.l1.go = "exit";
					}
					else
					{
						int randomGod = rand(1);
						if (randomGod == 1)
						{
							dialog.text = DLG_TEXT[86];
							link.l1 = DLG_TEXT[87];
							link.l1.go = "exit";
							pchar.quest.destination = "Conceicao_port";
							AddDialogExitQuest("prepare_for_convoy_quest");
							
						}
						else
						{
							dialog.text = DLG_TEXT[88];
							link.l1 = DLG_TEXT[89];
							link.l1.go = "exit";
						}
					}
				}
			}
		break;
		
		case "fifth_mission_accepted":
			dialog.text = DLG_TEXT[90];
			link.l1 = DLG_TEXT[91];
			link.l1.go = "fifth_mission_accepted_2";
		break;
		
		case "fifth_mission_accepted_2":
			dialog.text = DLG_TEXT[92];
			link.l1 = DLG_TEXT[93];
			link.l1.go = "fifth_mission_accepted_3";
		break;
		
		case "fifth_mission_accepted_3":
			dialog.text = DLG_TEXT[94];
			link.l1 = DLG_TEXT[95];
			link.l1.go = "fifth_mission_accepted_4";
		break;
		
		case "fifth_mission_accepted_4":
			dialog.text = DLG_TEXT[96];
			link.l1 = DLG_TEXT[97];
			link.l1.go = "fifth_mission_accepted_5";
		break;
		
		case "fifth_mission_accepted_5":
			dialog.text = DLG_TEXT[98];
			link.l1 = DLG_TEXT[40];
			link.l1.go = "exit";
			AddDialogExitQuest("fifth_mission_know_about_betrayer");
		break;
	}
}

	
