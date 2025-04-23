//nclude "DIALOGS\Sidonio Ogarrio_dialog.h"
void ProcessDialogEvent()
{
	ref NPChar, PChar, d; 
	PChar = GetMainCharacter();
	aref Link, Diag;
	string NPC_Meeting, TempInfoCharacterID_1, TempInfoCharacterID_2;
	int DonationSize;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);
	
	switch(Dialog.CurrentNode)
	{

		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = NPC_meeting;
			DialogExit();
		break;

		case "First time":
			Dialog.defAni = "dialog_stay1";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.ani = "dialog_stay2";
			Dialog.cam = "1";
			Dialog.snd = "voice\SIOG\SIOG001";
			Diag.TempNode = "First time";
			d.Text = DLG_TEXT[0];
			link.l1 = DLG_TEXT[1];
			link.l1.go = "Exit";

			if (characters[GetCharacterIndex("Wilfred Bythesea")].quest.kill_Ogarrio == "wait")
			{
				link.l2 = DLG_TEXT[2];
				link.l2.go = "kill_Ogario_1";
			}
		break;

		case "kill_Ogario_1":
			d.Text = DLG_TEXT[3];
			link.l1 = DLG_TEXT[4];
			link.l1.go = "feint_offer";
		break;

		case "feint_offer":
			d.Text = DLG_TEXT[5];
			link.l1 = DLG_TEXT[6];
			link.l1.go = "kill_Ogario_2";
			link.l2 = DLG_TEXT[9];
			link.l2.go = "listen_fake_offer";
		break;

		case "kill_Ogario_2":
			d.Text = DLG_TEXT[7];
			link.l1 = DLG_TEXT[8];
			AddDialogExitQuest("kill_Ogario_fight");
			link.l1.go = "exit";
		break;

		case "listen_fake_offer":
			d.Text = DLG_TEXT[10];
			link.l1 = DLG_TEXT[11];
			link.l1.go = "fools_deal";
		break;

		case "fools_deal":
			d.Text = DLG_TEXT[12];
			link.l1 = DLG_TEXT[13];
			AddDialogExitQuest("kill_Ogario_fight");
			link.l1.go = "exit";
		break;

	}

}
