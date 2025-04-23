#define LEFT_CHARACTER	0
#define RIGHT_CHARACTER	1

void InitInterface(string iniName)
{
	GameInterface.title = "titleNationRelation";
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);

	CreateImage("England","relations",GetRelationName(GetNationRelation2MainCharacter(ENGLAND)),171,69,285,183);
	CreateImage("France","relations",GetRelationName(GetNationRelation2MainCharacter(FRANCE)),171,203,292,317);
	CreateImage("Spain","relations",GetRelationName(GetNationRelation2MainCharacter(SPAIN)),171,337,292,451);
	CreateImage("Pirate","relations",GetRelationName(GetNationRelation2MainCharacter(PIRATE)),489,69,603,183);
	CreateImage("Holland","relations",GetRelationName(GetNationRelation2MainCharacter(HOLLAND)),489,203,603,317);
	CreateImage("Portugal","relations",GetRelationName(GetNationRelation2MainCharacter(PORTUGAL)),489,337,603,451);

	SetEventHandler("InterfaceBreak","ProcessCancelExit",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
}

void ProcessCancelExit()
{
	IDoExit(RC_INTERFACE_NATIONRELATION_EXIT);
}

void IDoExit(int exitCode)
{
	DelEventHandler("InterfaceBreak","ProcessCancelExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	interfaceResultCommand = exitCode;
	if( CheckAttribute(&InterfaceStates,"InstantExit") && sti(InterfaceStates.InstantExit)==true )
	{
		EndCancelInterface(true);
	}
	else
	{
		PostEvent("LaunchIAfterFrame",1,"sl", "I_SELECTMENU", 1);
		EndCancelInterface(false);
		PlaySound("interface\ok.wav");
	}
}

string GetRelationName(int relationCode)
{
	switch(relationCode)
	{
	case RELATION_FRIEND:	return "Friend"; break;
	case RELATION_ENEMY:	return "Enemy"; break;
	case RELATION_NEUTRAL:	return "Neutral"; break;
	}
	return "Unknown";
}
