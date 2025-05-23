
//nclude "DIALOGS\Amerigo Vieira_dialog.h"
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
		// -----------------------------------������ ������ - ������ �������
		case "First time":

			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			
			dialog.snd = "Voice\AMVI\AMVI001";
			dialog.text = DLG_TEXT[0];
			link.l1 = DLG_TEXT[1];
			link.l1.go = "Exit";

			if(PChar.quest.Hitman == "mark_amerigo")
			{
				dialog.snd = "Voice\AMVI\AMVI002";
				dialog.text = DLG_TEXT[2]+ Characters[GetCharacterIndex(DLG_TEXT[3])].lastname +DLG_TEXT[4];
				link.l1 = DLG_TEXT[5];
				link.l1.go = "1";

			}
			

		break;

		case "1":
			dialog.snd = "Voice\AMVI\AMVI003";
			dialog.text = DLG_TEXT[6]+ Characters[GetCharacterIndex(DLG_TEXT[7])].lastname +DLG_TEXT[8]+ Characters[GetCharacterIndex(DLG_TEXT[9])].lastname +DLG_TEXT[10];
			link.l1 = DLG_TEXT[11];
			link.l1.go = "2";

		break;

		case "2":
			dialog.snd = "Voice\AMVI\AMVI004";
			dialog.text = DLG_TEXT[12];
			link.l1 = DLG_TEXT[13];
			link.l1.go = "3";

		break;

		case "3":
			dialog.snd = "Voice\AMVI\AMVI005";
			dialog.text = DLG_TEXT[14];
			link.l1 = DLG_TEXT[15];
			link.l1.go = "Exit_kill";
			link.l2 = DLG_TEXT[16];
			link.l2.go = "spare";

		break;

		case "spare":
			dialog.snd = "Voice\AMVI\AMVI006";
			dialog.text = DLG_TEXT[17]+ Characters[GetCharacterIndex(DLG_TEXT[18])].lastname +DLG_TEXT[19];
			link.l1 = DLG_TEXT[20];
			link.l1.go = "Exit_spare";
			link.l2 = DLG_TEXT[21];
			link.l2.go = "Exit_kill";

			if(makeint(PChar.skill.Leadership) > 5)
			{
				link.l3 = DLG_TEXT[22];
				link.l3.go = "join";
			}
		break;

		case "join":
			dialog.snd = "Voice\AMVI\AMVI007";
			dialog.text = DLG_TEXT[23];
			link.l1 = DLG_TEXT[24];
			link.l1.go = "Exit_join";
		break;

		case "Exit":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

		break;

		case "Exit_kill":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			ChangeCharacterReputation(pchar, -10);

			AddDialogExitQuest("Hit_kill_Amerigo");

		break;

		case "Exit_spare":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			ChangeCharacterReputation(pchar, 5);

			AddDialogExitQuest("Hit_spare_Amerigo");

		break;

		case "Exit_join":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			ChangeCharacterReputation(pchar, 5);

			AddDialogExitQuest("Hit_join_Amerigo");

		break;
		
		case "hired":
			dialog.snd = "Voice\AMVI\AMVI001";
			dialog.text = DLG_TEXT[25];
			link.l1 = DLG_TEXT[26];
			link.l1.go = "Exit";
			Link.l2 = DLG_TEXT[27];
			Link.l2.go = "abordage";
			
			NextDiag.TempNode = "hired";
		break;
		
		case "abordage":
			dialog.text = DLG_TEXT[28];
			if (npchar.abordage_enable == "true")
			{
				link.l10 = DLG_TEXT[29];
			}
			else
			{
				link.l10 = DLG_TEXT[30];
			}
			link.l10.go = "abordage_switcher";
		break;
		
		case "abordage_switcher":
			dialog.text = DLG_TEXT[31];
			link.l1 = DLG_TEXT[32];
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

	}
}
