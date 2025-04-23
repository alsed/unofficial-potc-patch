string totalInfo;
void InitInterface(string iniName)
{
	EngineLayersOffOn(true);
	SetTimeScale(0.0);
	if(IsPerkIntoList("TimeSpeed")) { DelPerkFromActiveList("TimeSpeed"); }
    GameInterface.title = "titleBoal_map";

    SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
    

	CalculateInfoData();

	//int tmpLangFileID = LanguageOpenFile("interface_strings.txt");
	SetFormatedText("INFO_TEXT",totalInfo);//"Капитан, кто-то явно плывет.");
	//LanguageCloseFile(tmpLangFileID);
	SendMessage(&GameInterface,"lsl",MSG_INTERFACE_MSG_TO_NODE,"INFO_TEXT",5);

	SetEventHandler("InterfaceBreak","ProcessBreakExit",0); // Выход на море
	SetEventHandler("exitCancel","ProcessCancelExit",0); // Выход на море по крестику или Esc
	SetEventHandler("ievnt_command","ProcCommand",0); // выход на карту только тут (по НЕТ)
	SetEventHandler("evntDoPostExit","DoPostExit",0); // выход из интерфейса
}

void ProcessBreakExit()
{
	IDoExit( RC_INTERFACE_BOAL_MAP_EXIT );
	wdmReloadToSea();
	SetTimeScale(1.0);
}

void ProcessCancelExit()
{
	IDoExit( RC_INTERFACE_BOAL_MAP_EXIT );
	wdmReloadToSea();
	SetTimeScale(1.0);
}

void IDoExit(int exitCode)
{
	DelEventHandler("InterfaceBreak","ProcessBreakExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("ievnt_command","ProcCommand");
	DelEventHandler("evntDoPostExit","DoPostExit");

	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
	SetTimeScale(1.0);
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();

	switch(nodName)
	{
	case "B_OK":
		if(comName=="activate" || comName=="click")
		{
			// напасть
			PostEvent("evntDoPostExit",1,"l",RC_INTERFACE_BOAL_MAP_EXIT);
			wdmReloadToSea();
		}
	break;

	case "B_CANCEL":
		if(comName=="activate" || comName=="click")
		{
			//пропустить
			PostEvent("evntDoPostExit",1,"l",RC_INTERFACE_BOAL_MAP_EXIT);
			SetTimeScale(1.0);
		}
		if(comName=="upstep")
		{
			if(GetSelectable("B_OK"))	{SetCurrentNode("B_OK");}
		}
	break;
	}
}

void wdmRecalcReloadToSea()
{

	worldMap.encounter.type = "";
	totalInfo = "";
	//Encounters
	int numEncounters = wdmGetNumberShipEncounters();
	int isShipEncounterType = 0;
	ref rEncounter;
	for(int i = 0; i < numEncounters; i++)
	{
		if(wdmSetCurrentShipData(i))
		{
		    if(MakeInt(worldMap.encounter.select) == 0) continue;			
			isShipEncounterType++;
			if (isShipEncounterType>1)
			{
			    totalInfo = totalInfo + XI_ConvertString("against");
			}
			rEncounter = &MapEncounters[sti(worldMap.encounter.type)];
			
			switch(sti(rEncounter.RealEncounterType))
			{
				  case 0:		
				     totalInfo = totalInfo + XI_ConvertString("case0");
				  break;
				  case 1:		
				     totalInfo = totalInfo + XI_ConvertString("case1");
				  break;
				  case 2:		
				     totalInfo = totalInfo + XI_ConvertString("case2");
				  break;
				  case 3:		
				     totalInfo = totalInfo + XI_ConvertString("case3");
				  break;
				  case 4:		
				     totalInfo = totalInfo + XI_ConvertString("case4");
				  break;
				  case 5:		
				     totalInfo = totalInfo + XI_ConvertString("case5");
				  break;
				  case 6:		
				     totalInfo = totalInfo + XI_ConvertString("case6");
				  break;
				  case 7:		
				     totalInfo = totalInfo + XI_ConvertString("case7");
				  break;
			}
	        //totalInfo = totalInfo + ", sailing ";
	        switch(sti(rEncounter.Nation))
	        {
		        case 0:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under English colours");
		        break;
		        case 1:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under French colours");
		        break;
		        case 2:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under Spanish colours");
		        break;
		        case 3:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under the Jolly Roger");
		        break;
		        case 4:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under Dutch colours");
		        break;
		        case 5:		
			        totalInfo = totalInfo + XI_ConvertString("sailing under Portuguese colours");
		        break;
		    }
		}
	}
	if (isShipEncounterType>1)
	{
	   totalInfo = XI_ConvertString("Battle up ahead Captain") + totalInfo + ".";
	}
	else
	{
	   totalInfo = XI_ConvertString("Ship ahoy Captain") + totalInfo + ".";
	}
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void CalculateInfoData()
{
	
    wdmRecalcReloadToSea();
    ref mainCh = GetMainCharacter();
    
	if(!CheckAttribute(mainCh, "perks.list.SailingProfessional"))
	{
		SetSelectable("B_CANCEL",false);
		SetCurrentNode("B_OK");
	}
	else
	{
		SetSelectable("B_CANCEL",true);
		SetCurrentNode("B_OK");
	}
}
