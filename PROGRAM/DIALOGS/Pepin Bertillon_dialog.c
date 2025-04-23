//nclude "DIALOGS\Pepin Bertillon_dialog.h"
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
		// -----------------------------------Диалог первый - первая встреча
		case "First time":
			dialog.text = DLG_TEXT[0];
			link.l1 = DLG_TEXT[1]+ Characters[GetCharacterIndex(DLG_TEXT[2])].name + " " + Characters[GetCharacterIndex(DLG_TEXT[3])].lastname +DLG_TEXT[4];
			link.l1.go = "1";
		break;

		case "1":
			Dialog.snd = "voice\PEBE\PEBE002";
			dialog.text = DLG_TEXT[5];
			link.l1 = DLG_TEXT[6];
			link.l1.go = "2";

		break;

		case "2":
			Dialog.snd = "voice\PEBE\PEBE003";
			dialog.text = DLG_TEXT[7];
			link.l1 = DLG_TEXT[8];
			link.l1.go = "Exit_kill";
			link.l2 = DLG_TEXT[9];
			link.l2.go = "Exit_spare";

		break;

		case "Exit_kill":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			ChangeCharacterReputation(pchar, -10);

			AddDialogExitQuest("Hit_kill_Pepin");

		break;

		case "Exit_spare":

			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			ChangeCharacterReputation(pchar, -5);

			AddDialogExitQuest("Hit_spare_Pepin");

		break;

	}
}
