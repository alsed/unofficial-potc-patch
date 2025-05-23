//nclude "DIALOGS\Humberto de Paiva_dialog.h"
void ProcessDialogEvent()
{
	ref NPChar, PChar, d;
	PChar = GetMainCharacter();
	aref Link, Diag;
	string NPC_Meeting;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);

	string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;
	
	switch(Dialog.CurrentNode)
	{
	// -----------------------------------������ ������ - ������ �������
		case "First time":
			Dialog.defAni = "Gov_Dialog_1";
			Dialog.ani = "Gov_Dialog_2";
			Dialog.defCam = "1";
			Dialog.defSnd = "dialogs\0\017";
			Dialog.defLinkAni = "dialog_1";
			Dialog.defLinkCam = "1";
			Dialog.defLinkSnd = "dialogs\woman\024";
			Dialog.cam = "1";
			

			if(NPChar.quest.meeting == "0")
			{
				dialog.snd = "Voice\HUDP\HUDP001";
				d.Text = TimeGreeting() + DLG_TEXT[0] + NPChar.name + " " + NPChar.lastname + DLG_TEXT[1];
				Link.l1 = DLG_TEXT[2] + Pchar.name + " " + Pchar.lastname + DLG_TEXT[3] + PChar.Ship.name + DLG_TEXT[4];
				Link.l1.go = "meeting";
				NPChar.quest.meeting = "1"; 
			}
			else
			{
				dialog.snd = "Voice\HUDP\HUDP002";
				d.Text = TimeGreeting() + DLG_TEXT[5] + PChar.name + DLG_TEXT[6] + address_form.eng + DLG_TEXT[7];
				Link.l1 = DLG_TEXT[8];
				Link.l1.go = "Shipyard";
				Link.l2 = DLG_TEXT[9];
				Link.l2.go = "quest lines";
				Link.l3 = DLG_TEXT[10];
				Link.l3.go = "exit";
			}
		break;

		case "Meeting":
			dialog.snd = "Voice\HUDP\HUDP003";
				d.Text = DLG_TEXT[11];
				Link.l1 = DLG_TEXT[12] + NPChar.name +DLG_TEXT[13];				
				Link.l1.go = "Shipyard";		
				Link.l2 = DLG_TEXT[14];
				Link.l2.go = "quest lines";
				Link.l3 = DLG_TEXT[15];
				Link.l3.go = "exit";
		break;

		case "quest lines":
			dialog.snd = "Voice\HUDP\HUDP004";
				d.Text = DLG_TEXT[16] + PChar.name + DLG_TEXT[17];
				Link.l1 = DLG_TEXT[18];
				Link.l1.go = "no quest";
		break;

		case "No quest":
			dialog.snd = "Voice\HUDP\HUDP005";
				d.Text = DLG_TEXT[19] + PChar.name + DLG_TEXT[20];
				Link.l1 = DLG_TEXT[21];
				Link.l1.go = "Shipyard";
				Link.l2 = DLG_TEXT[22];
				Link.l2.go = "exit";
		break;

		case "shipyard":
			dialog.snd = "Voice\HUDP\HUDP006";
				d.Text = DLG_TEXT[23];
				Link.l1 = DLG_TEXT[24];
				Link.l1.go = "SHIPYARD_interface";
				Link.l2 = DLG_TEXT[25];
				Link.l2.go = "Cannons_interface";
				Link.l3 = DLG_TEXT[26];
				Link.l3.go = "no quest";
		break;

		case "SHIPYARD_interface":
			Diag.CurrentNode = Diag.TempNode;
			
			if (npchar.quest.Meeting != lastspeak_date)
			{
				int iTest_ship;

				DeleteAttribute(npchar, "shipyard");
			
				npchar.shipyard.ship1 = SHIP_TARTANE;

				iTest_ship = rand(2);
				if (iTest_ship == 1)
				{
					npchar.shipyard.ship2 = SHIP_BARQUE_PORTUGAL;
				}
				if (iTest_ship == 2) 
				{
					npchar.shipyard.ship2 = SHIP_BARQUE2_PORTUGAL;
				}

				iTest_ship = rand(2);
				if (iTest_ship == 1) npchar.shipyard.ship3 = SHIP_YACHT_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship3 = SHIP_YACHT2_PORTUGAL;
				
				iTest_ship = rand(2);
				if (iTest_ship == 1) npchar.shipyard.ship4 = SHIP_LUGGER_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship4 = SHIP_LUGGER2_PORTUGAL;

				iTest_ship = rand(3);
				if (iTest_ship == 1) npchar.shipyard.ship5 = SHIP_SLOOP_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship5 = SHIP_SLOOP2_PORTUGAL;
				
				iTest_ship = rand(3);
				if (iTest_ship == 1) npchar.shipyard.ship6 = SHIP_GALEOTH_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship6 = SHIP_GALEOTH2_PORTUGAL;
		
				iTest_ship = rand(3);
				if (iTest_ship == 1) npchar.shipyard.ship7 = SHIP_XEBEC_PORTUGAL;
				if (iTest_ship == 2)npchar.shipyard.ship7 = SHIP_XEBEC2_PORTUGAL;
				
				iTest_ship = rand(4);
				if (iTest_ship == 1) npchar.shipyard.ship8 = SHIP_SCHOONER_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship8 = SHIP_SCHOONER2_PORTUGAL;
				
				iTest_ship = rand(4);
				if (iTest_ship == 1) npchar.shipyard.ship9 = SHIP_CARAVEL_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship9 = SHIP_CARAVEL2_PORTUGAL;
				
				iTest_ship = rand(4);
				if (iTest_ship == 1) npchar.shipyard.ship10 = SHIP_FLEUT_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship10 = SHIP_FLEUT2_PORTUGAL;
				
				iTest_ship = rand(5);
				if (iTest_ship == 1) npchar.shipyard.ship11 = SHIP_PINNACE_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship11 = SHIP_PINNACE2_PORTUGAL;
				
				iTest_ship = rand(5);
				if (iTest_ship == 1) npchar.shipyard.ship12 = SHIP_BRIG_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship12 = SHIP_BRIG2_PORTUGAL;
				
				iTest_ship = rand(5);
				if (iTest_ship == 1) npchar.shipyard.ship13 = SHIP_GALEON_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship13 = SHIP_GALEON2_PORTUGAL;

				iTest_ship = rand(6);
				if (iTest_ship == 1) npchar.shipyard.ship14 = SHIP_CORVETTE_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship14 = SHIP_CORVETTE2_PORTUGAL;

				iTest_ship = rand(6);
				if (iTest_ship == 1) npchar.shipyard.ship15 = SHIP_FRIGATE_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship15 = SHIP_FRIGATE2_PORTUGAL;

				iTest_ship = rand(7);
				if (iTest_ship == 1) npchar.shipyard.ship16 = SHIP_BATTLESHIP_PORTUGAL;
				if (iTest_ship == 2) npchar.shipyard.ship16 = SHIP_BATTLESHIP2_PORTUGAL;

				npchar.quest.Meeting = lastspeak_date;
			}

			DialogExit();
			LaunchShipyard(npchar);
		break;

		case "CANNONS_interface":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			LaunchCannons();
		break;

		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
		break;
		
	}
}