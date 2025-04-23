
void ProcessDialogEvent()
{
	ref NPChar;
	
	DeleteAttribute(&Dialog,"Links");
	aref Link, NextDiag;
	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	ref PChar;
	PChar = GetMainCharacter();

	switch(Dialog.CurrentNode)
	{
		// -----------------------------------Диалог первый - первая встреча
		case "First time":
			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			
			dialog.snd = "Voice\HAAU\HAAU001";
			Dialog.Text = DLG_TEXT[0] + NPChar.name + " " + NPChar.Lastname + DLG_TEXT[1];
			Link.l1 = DLG_TEXT[2] + PChar.name + " " + PChar.lastname + DLG_TEXT[3];
			link.l1.go = "Second time";
			NextDiag.TempNode = "Second time";
		break;

		case "Second time":
			dialog.snd = "Voice\HAAU\HAAU002";
			Dialog.text = DLG_TEXT[4];
			Link.l1 = DLG_TEXT[5];
			Link.l1.go = "node_3";
			if (Characters[GetCharacterIndex("Patric Cardone")].quest.teodoro == "4")
			{
				Link.l2 = DLG_TEXT[6];
				link.l2.go = "teodoro";
			}
			if (CheckQuestAttribute("smuggler_line", "fourth_mission_ring"))
			{
				link.l1 = DLG_TEXT[7];
				link.l1.go = "fourth_mission_ring";
			}
			if (CheckQuestAttribute("smuggler_line", "fourth_mission_with_officer_to_shop_wrong"))
			{
				dialog.text = DLG_TEXT[8];
				link.l1 = DLG_TEXT[9];
				link.l1.go = "fourth_mission_with_officer_to_shop_wrong";
			}
			if (CheckQuestAttribute("smuggler_line", "fourth_mission_with_officer_to_shop_right"))
			{
				dialog.text = DLG_TEXT[10];
				link.l1 = DLG_TEXT[11];
				link.l1.go = "fourth_mission_with_officer_to_shop_right";
			}
			NextDiag.TempNode = "Second time";
		break;

		case "node_3":
			dialog.snd = "Voice\HAAU\HAAU003";
			Dialog.text = DLG_TEXT[12] + Address_Form.Fra + DLG_TEXT[13];
			link.l1 = DLG_TEXT[14];
			link.l1.go = "exit";
		break;

		case "teodoro":
			dialog.snd = "Voice\HAAU\HAAU004";
			Dialog.text = DLG_TEXT[15];
			link.l1 = DLG_TEXT[16] + Characters[GetCharacterIndex(DLG_TEXT[17])].name + " " + Characters[GetCharacterIndex(DLG_TEXT[18])].lastname + DLG_TEXT[19];
			link.l1.go = "teodoro_1";
		break;

		case "teodoro_1":
			dialog.snd = "Voice\HAAU\HAAU005";
			Dialog.text = DLG_TEXT[20];
			link.l1 = DLG_TEXT[21];
			link.l1.go ="teodoro_2";
		break;

		case "teodoro_2":
			dialog.snd = "Voice\HAAU\HAAU006";
			Dialog.text = DLG_TEXT[22];
			Link.l1 = DLG_TEXT[23] + Characters[GetCharacterIndex(DLG_TEXT[24])].name + " " + Characters[GetCharacterIndex(DLG_TEXT[25])].lastname + DLG_TEXT[26];
			link.l1.go = "teodoro_3";
		break;

		case "teodoro_3":
			dialog.snd = "Voice\HAAU\HAAU007";
			Dialog.text = DLG_TEXT[27];
			link.l1 = DLG_TEXT[28];
			Link.l1.go = "teodoro_4";
		break;

		case "teodoro_4":
			dialog.snd = "Voice\HAAU\HAAU008";
			Dialog.text = DLG_TEXT[29];
			Link.l1 = DLG_TEXT[30];
			link.l1.go = "teodoro_5";
		break;

		case "teodoro_5":
			dialog.snd = "Voice\HAAU\HAAU009";
			Dialog.text = DLG_TEXT[31];
			Link.l1 = DLG_TEXT[32];
			link.l1.go = "teodoro_6";
		break;

		case "teodoro_6":
			dialog.snd = "Voice\HAAU\HAAU010";
			Dialog.text = DLG_TEXT[33];
			Link.l1 = DLG_TEXT[34];
			link.l1.go = "exit";
			Characters[GetCharacterIndex("Patric Cardone")].dialog.currentnode = "Teodoro_completed";
			characters[GetCharacterIndex("Lucien Bescanceny")].location	= "none";
		break;
		
		case "fourth_mission_ring":
			if (GetNationRelation2MainCharacter(FRANCE) == RELATION_ENEMY)
			{
				dialog.text = DLG_TEXT[35];
				link.l1 = DLG_TEXT[36];
				link.l1.go = "exit";
				AddDialogExitQuest("fourth_mission_ring_france_was_enemy");
			}
			else
			{
				dialog.text = DLG_TEXT[37];
				link.l1 = DLG_TEXT[38];
				link.l1.go = "fourth_mission_ring_2";
			}
		break;
		
		case "fourth_mission_ring_2":
			dialog.text = DLG_TEXT[39];
			link.l1 = DLG_TEXT[40];
			link.l1.go = "fourth_mission_ring_3";
		break;
		
		case "fourth_mission_ring_3":
			dialog.text = DLG_TEXT[41];
			link.l1 = DLG_TEXT[42];
			link.l1.go = "fourth_mission_ring_4";
			link.l2 = DLG_TEXT[43];
			link.l2.go = "fourth_mission_ring_6";
		break;
		
		case "fourth_mission_ring_4":
			dialog.text = DLG_TEXT[44];
			link.l1 = DLG_TEXT[45];
			link.l1.go = "fourth_mission_ring_5";
		break;
		
		case "fourth_mission_ring_5":
			dialog.text = DLG_TEXT[46];
			link.l1 = DLG_TEXT[47];
			link.l1.go = "exit";
			AddDialogExitQuest("fourth_mission_with_officer_to_shop_wrong");
		break;
		
		case "fourth_mission_ring_6":
			dialog.text = DLG_TEXT[48];
			link.l1 = DLG_TEXT[49];
			link.l1.go = "fourth_mission_ring_7";
		break;
		
		case "fourth_mission_ring_7":
			dialog.text = DLG_TEXT[50];
			link.l1 = DLG_TEXT[51];
			link.l1.go = "exit";
			AddDialogExitQuest("fourth_mission_with_officer_to_shop_right");
		break;
		
		case "fourth_mission_with_officer_to_shop_wrong":
			dialog.text = DLG_TEXT[52];
			link.l1 = DLG_TEXT[53];
			link.l1.go = "fourth_mission_with_officer_to_shop_wrong_2";
		break;
		
		case "fourth_mission_with_officer_to_shop_wrong_2":
			dialog.text = DLG_TEXT[54];
			link.l1 = DLG_TEXT[55];
			link.l1.go = "fourth_mission_with_officer_to_shop_wrong_3";
		break;
		
		case "fourth_mission_with_officer_to_shop_wrong_3":
			dialog.text = DLG_TEXT[56];
			link.l1 = DLG_TEXT[57];
			link.l1.go = "fourth_mission_with_officer_to_shop_wrong_4";
		break;
		
		case "fourth_mission_with_officer_to_shop_wrong_4":
			dialog.text = DLG_TEXT[58];
			link.l1 = DLG_TEXT[59];
			link.l1.go = "exit";
			AddDialogExitQuest("fourth_mission_after_shop_wrong");
		break;
		
		case "fourth_mission_with_officer_to_shop_right":
			dialog.text = DLG_TEXT[60];
			link.l1 = DLG_TEXT[61];
			link.l1.go = "fourth_mission_with_officer_to_shop_right_2";
		break;
		
		case "fourth_mission_with_officer_to_shop_right_2":
			dialog.text = DLG_TEXT[62];
			link.l1  = DLG_TEXT[63];
			link.l1.go = "fourth_mission_with_officer_to_shop_right_3";
		break;
		
		case "fourth_mission_with_officer_to_shop_right_3":
			dialog.text = DLG_TEXT[64];
			link.l1 = DLG_TEXT[65];
			link.l1.go = "exit";
			AddDialogExitQuest("fourth_mission_officer_exit_from_shop");
		break;

		case "Exit":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
		break;
	}
}
