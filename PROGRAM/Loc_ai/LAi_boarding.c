

#define BRDLT_SHIP	0
#define BRDLT_FORT	1

#define BRDLT_MAXCREW	32

#define LOCINITBRD_DEFAULTLOCATION "ship default deck"
#define LOCINITBRD_DEFAULTLOCATION_FORT "fort default deck"

#define MAX_GROUP_SIZE	6

int boarding_location = -1;
int boarding_location_type = -1;
object boarding_fader;
int boarding_enemy_crew = 0;
int boarding_player_crew = 0;
int boarding_enemy_base_crew = 0;
int boarding_officers = 0;
float boarding_player_crew_per_chr = 1.0;
ref boarding_enemy;
object boarding_adr[256];
float boarding_exp = 0;
float boarding_player_hp = 50;
float boarding_enemy_hp = 50;
int boarding_echr_index = -1;
int boarding_erank = 10;

bool LAi_boarding_process = false;

bool Surrendered = false;
float boarding_enemy_original_morale = 0;
float boarding_enemy_crew_per_chr = 1.0;
int SurrenderModifier = 0;
int defeat = 0;
int boarding_enemy_crew_start = 0;
int boarding_player_crew_start = 0;

//Процес абордажа
bool LAi_IsBoardingProcess()
{
	return LAi_boarding_process;
}

//Получить картинку для перехода в абордаж
string LAi_GetBoardingImage(ref echr, bool isMCAttack)
{
	ref mchr = GetMainCharacter();
	string deckID = "";
	if(isMCAttack)
	{
		deckID = GetShipLocationID(echr);
	}else{
		deckID = GetShipLocationID(mchr);
	}
	int locID = -1;
	if(deckID != "")
	{
		locID = FindLocation(deckID);
		if(locID < 0)
		{
			Trace("Boarding: ship location not found <" + deckID + ">, set default");
			deckID = "";
		}
	}
	if(locID >= 0)
	{
		if(CheckAttribute(&Locations[locID], "image"))
		{
			if(Locations[locID].image != "")
			{
				return Locations[locID].image;
			}
		}
	}
	return "loading\battle.tga";
}

//Начать абордаж с главным персонажем
void LAi_StartBoarding(int locType, ref echr, bool isMCAttack)
{
	ResetSoundScheme();
	PauseAllSounds();
	if(locType == BRDLT_FORT) isMCAttack = true;
	//Установим обработчик на убийство группы
	SetEventHandler("LAi_event_GroupKill", "LAi_BoardingGroupKill", 1);
	//Настроим интерфейс
	DeleteBattleInterface();
	StartBattleLandInterface();
	//Сохраним индекс врага
	boarding_echr_index = sti(echr.index);
	//Параметры сражающихся сторон	
	ref mchr = GetMainCharacter();
	int mclass = GetCharacterShipClass(mchr);
	int mcrew = GetCrewQuantity(mchr);
	int eclass = GetCharacterShipClass(echr);
	int ecrew = GetCrewQuantity(echr) + 1;
	
	if(ecrew < 1) ecrew = 1;
	
	boarding_erank = sti(echr.rank);
	
	boarding_enemy_base_crew = ecrew;
	
	//Количество хитпойнтов
	boarding_player_hp = LAi_GetCharacterMaxHP(mchr);
	float moral;
	if(CheckOfficersPerk(mchr,"MusketsShoot"))
	{
		float musketminus = ecrew * (0.25 - (stf(echr.skill.defence) * 0.025));
		ecrew -= makeint(musketminus);
		Log_SetStringToLog("Musket volley kills " + makeint(musketminus) + " of enemy's crew");
	}
	if(CheckAttribute(mchr, "ship.crew.moral"))
	{
		moral = (stf(mchr.ship.crew.moral) - MORALE_NORMAL)/MORALE_MAX;
		if(moral < -0.5) moral = -0.5;
		if(moral > 0.5) moral = 0.5;
		boarding_player_hp = boarding_player_hp * (moral * 0.3 + 1.2);
	}
	boarding_enemy_hp = LAi_GetCharacterMaxHP(echr);
	if(CheckAttribute(echr, "ship.crew.moral"))
	{
		moral = (stf(echr.ship.crew.moral) - MORALE_NORMAL)/MORALE_MAX;
		if(moral < -0.5) moral = -0.5;
		if(moral > 0.5) moral = 0.5;
		boarding_enemy_hp = boarding_enemy_hp * (moral * 0.3 + 1.0);
	}
	//Расчитаем получаемый опыт в случае победы
	boarding_exp = 0.0;
	if(CheckAttribute(echr, "rank"))
	{
		boarding_exp = stf(echr.rank)*100.0;
		if(boarding_exp < 0.0) boarding_exp = 0.0;
	}
	boarding_exp = 100.0 + boarding_exp + 10.0*ecrew*(LAi_GetCharacterFightLevel(echr)*9.0 + 1.0)/(LAi_GetCharacterFightLevel(mchr)*9.0 + 1.0);

	//Определяем цепь локаций для абордажа
	boarding_location = -1;
	boarding_location_type = locType;
	string deckID = "";
	if(isMCAttack)
	{
		deckID = GetShipLocationID(echr);
	}else{
		deckID = GetShipLocationID(mchr);
	}
	int locID = -1;
	if(deckID != "")
	{
		locID = FindLocation(deckID);
		if(locID < 0)
		{
			Trace("Boarding: ship location not found <" + deckID + ">, set default");
			deckID = "";
		}
	}
	if(deckID == "")
	{
		if(locType == BRDLT_FORT)
		{
			deckID = LOCINITBRD_DEFAULTLOCATION_FORT;
		}else{
			deckID = LOCINITBRD_DEFAULTLOCATION;
		}
		locID = FindLocation(deckID);
	}
	if(locID < 0)
	{
		Trace("Boarding: ship location not found <" + deckID + ">, no start boarding");
		if(boarding_location_type == BRDLT_SHIP)
		{
			LaunchRansackMain(GetMainCharacter(), echr, "");
			LAi_boarding_process = false;
		}else{
			if(boarding_location_type == BRDLT_FORT)
			{
				LaunchFortCapture(echr);
				LAi_boarding_process = false;
			}else{
				Trace("Boarding: unknow boarding location type");
			}
		}
		return;
	}

	SurrenderModifier = 14 - eclass;
	boarding_enemy_original_morale = stf(echr.ship.crew.morale);
	int firstparam  = rand(makeint(sti(mchr.skill.Grappling)+sti(mchr.skill.Sneak)));
	int secondparam = sti(mchr.skill.Leadership/2)+sti(mchr.skill.Fencing)/2;
	defeat = firstparam*secondparam;
	if (CheckForSurrender(mchr, echr))
	{
		echr.ship.crew.morale=GetAdjustedMorale(GetMainCharacter(), echr, true);

		if(mclass<eclass) { mchr.ship.crew.morale=makeint(stf(mchr.ship.crew.morale) + (rand((eclass-mclass)) * rand((eclass-mclass)))); } 
		if(sti(mchr.ship.crew.morale) > MORALE_MAX) mchr.ship.crew.morale = MORALE_MAX;

		LaunchRansackJoin(GetMainCharacter(), echr);
		LAi_boarding_process = false;
		return;
	}

	//Определяем размеры команд
	boarding_enemy = echr;
	
	//Максимальное количество человек на корабле
	int maxcrew = MAX_GROUP_SIZE;
	int addcrew = MAX_GROUP_SIZE;
	while(maxcrew < BRDLT_MAXCREW)
	{
		if(!CheckAttribute(&Locations[locID], "boarding.nextdeck")) break;
		if(Locations[locID].boarding.nextdeck == "") break;
		locID = FindLocation(Locations[locID].boarding.nextdeck);
		if(locID < 0) break;
		maxcrew += addcrew;
	}
	//Отношение сторон
	float curplayercrew = mcrew;
	if(mcrew < 1) mcrew = 1;
	if(ecrew < 1) ecrew = 1;
	//Максимальное количество человек
	float rel;
	if(mcrew > ecrew)
	{
		if(mcrew > maxcrew)
		{
			rel = mcrew/maxcrew;
			mcrew = maxcrew;
			ecrew = MakeInt(ecrew/rel + 0.5);
		}
	}else{
		if(ecrew > maxcrew)
		{
			rel = ecrew/maxcrew;
			ecrew = maxcrew;
			mcrew = MakeInt(mcrew/rel + 0.5);
		}
	}
	if(mcrew < 1) mcrew = 1;
	if(ecrew < 1) ecrew = 1;
	boarding_enemy_crew = ecrew;
	
	boarding_player_crew = mcrew;
	//Количество офицеров
	boarding_officers = 0;
	int passq;
	for(int i = 0; i < GetPassengersQuantity(mchr); i++)
	{
		passq = GetNotCaptivePassenger(mchr, i) ;
		if(passq < 0) continue;
		if(CheckAttribute(&characters[passq], "abordage_enable"))
		{
			if (characters[passq].abordage_enable == "true")
			{
				boarding_officers = boarding_officers + 1;
				LAi_SetOfficerType(&characters[passq]);
			}
		}
	}
	boarding_player_crew_per_chr = (curplayercrew + boarding_officers + 1)/(mcrew + boarding_officers + 1);
	boarding_enemy_crew_per_chr = boarding_enemy_base_crew/ecrew;
	characters[0].boarding_player_crew_per_chr = boarding_player_crew_per_chr;
	characters[0].boarding_officers = boarding_officers;
	//Выставим игроку и офицерам максимальную жизнь и запомним адреса
	LAi_SetCurHPMax(mchr);
	boarding_adr[0].location = mchr.location;
	boarding_adr[0].group = mchr.location.group;
	boarding_adr[0].locator = mchr.location.locator;
	for(i = 0; i < GetPassengersQuantity(mchr); i++)
	{
		int idx = GetNotCaptivePassenger(mchr,  i);
		if(idx < 0) continue;
		if(CheckAttribute(&characters[idx], "abordage_enable"))
		{
			if (characters[idx].abordage_enable == "true")
			{
				if(idx < 0)
				{
					boarding_adr[i].location = "";
					boarding_adr[i].group = "";
					boarding_adr[i].locator = "";
					continue;
				}
				LAi_SetCurHPMax(&Characters[idx]);
				boarding_adr[i].location = Characters[idx].location;
				boarding_adr[i].group = Characters[idx].location.group;
				boarding_adr[i].locator = Characters[idx].location.locator;
			}
		}
	}
	//Стартуем
	LAi_boarding_process = true;
	LAi_LoadLocation(deckID);
	CreateEntity(&boarding_fader, "fader");
	SendMessage(&boarding_fader, "lfl", FADER_IN, RELOAD_TIME_FADE_IN, true);
	//!!!
	//LAi_SetImmortal(mchr, true);
}

//Загрузить локацию абордажа
void LAi_LoadLocation(string locationID)
{
	ReloadProgressStart();
	//Ищем локацию
	int locIndex = FindLocation(locationID);
	Log_SetActiveAction("Nothing");
	if(locIndex >= 0)
	{
		//Устанавливаем главного персонажа
		ref mchr = GetMainCharacter();
		mchr.location = locationID;
		mchr.location.group = "rld";
		mchr.location.locator = "loc0";
		//Устанавливаем офицеров, если такие есть
		int logined = 1;
		for(int i = 0; i < GetPassengersQuantity(mchr); i++)
		{
			int idx = GetNotCaptivePassenger(mchr, i);
			if(idx < 0) continue;
			//проверка флага абордажа.
			if(CheckAttribute(&characters[idx], "abordage_enable"))
			{
				if (characters[idx].abordage_enable == "true")
				{
					if(idx < 0) continue;
					DeleteAttribute(&Characters[idx], "location");
					Characters[idx].location = locationID;
					Characters[idx].location.group = "rld";
					Characters[idx].location.locator = "loc" + logined;
					logined = logined + 1;
				}
			}
		}
		//Перегружаемся в локацию
		boarding_location = locIndex;
		if(LoadLocation(&Locations[boarding_location]))
		{
			//Вытащим саблю
			SendMessage(&mchr, "lsl", MSG_CHARACTER_EX_MSG, "SetFightMode", 1);
			//Расставляем персонажей
			LAi_SetBoardingActors(locationID);
			//Запретим диалог
			dialogDisable = true;
		}else{
			Trace("Boarding: Boarding location not loaded, current loc <" + locationID + ">");
		}
	}else{
		Trace("Boarding: Boarding location not found, current loc <" + locationID + ">");
	}
	ReloadProgressEnd();
	PostEvent("LoadSceneSound", 500);
}

//Перегрузиться в следующую локации
void LAi_ReloadBoarding()
{
	//Разрешим диалоги
	dialogDisable = false;
	//Проверим предыдущую локацию
	if(boarding_location < 0)
	{
		Trace("Boarding: No loaded current boarding location");
		LAi_boarding_process = false;
		return;
	}
	//Установить хендлеры для обработки
	SetEventHandler("FaderEvent_StartFade", "LAi_ReloadStartFade", 0);
	SetEventHandler("FaderEvent_EndFade", "LAi_ReloadEndFade", 0);
	//Создаём фейдер и запускаем
	CreateEntity(&boarding_fader, "fader");
	
	string nextDeck = Locations[boarding_location].boarding.nextdeck;
	if (nextDeck != "")
	{
		if(CheckAttribute(&Locations[FindLocation(nextDeck)], "image"))
		{
			SendMessage(&boarding_fader, "ls", FADER_PICTURE, Locations[FindLocation(nextDeck)].image);
		}
	}
	float fadeOutTime = RELOAD_TIME_FADE_OUT;
	SendMessage(&boarding_fader, "lfl", FADER_OUT, fadeOutTime, false);
	SendMessage(&boarding_fader, "l", FADER_STARTFRAME);
}

void LAi_ReloadStartFade()
{
	//Выгружаем локацию
	ResetSoundScheme();
	PauseAllSounds();
	DelEventHandler("FaderEvent_StartFade", "LAi_ReloadStartFade");
	if(boarding_location >= 0) UnloadLocation(&Locations[boarding_location]);
}

void LAi_ReloadEndFade()
{
	//Загружаем следующую локацию
	DelEventHandler("FaderEvent_EndFade", "LAi_ReloadEndFade");
	SendMessage(&boarding_fader, "lfl", FADER_IN, RELOAD_TIME_FADE_IN, true);
	//Определим возможность продолжения перегрузок
	bool canReload = true;
	if(boarding_enemy_crew <= 0) canReload = false;
	if(!CheckAttribute(&Locations[boarding_location], "boarding.nextdeck")) canReload = false;
	if(Locations[boarding_location].boarding.nextdeck == "") canReload = false;
	if(canReload && !Surrendered)
	{
		//Продолжаем абордаж
		LAi_LoadLocation(Locations[boarding_location].boarding.nextdeck);
	}else{
		//Следующей локации нет
		DelEventHandler("LAi_event_GroupKill", "LAi_BoardingGroupKill");
		boarding_location = -1;
		//Пересчитываем команду игрока
		SetEndingStats();
		
		//Начислим опыт
		AddCharacterExp(GetMainCharacter(), MakeInt(boarding_exp));
		//Настроим интерфейс
		Log_SetActiveAction("Nothing");
		EndBattleLandInterface();
		//Востановим адреса
		ref mchr = GetMainCharacter();
		mchr.location = boarding_adr[0].location;
		mchr.location.group = boarding_adr[0].group;
		mchr.location.locator = boarding_adr[0].locator;
		for(int i = 0; i < GetPassengersQuantity(mchr); i++)
		{
			int idx = GetNotCaptivePassenger(mchr, i);
			if(idx < 0) continue;
			if(CheckAttribute(&characters[idx], "abordage_enable"))
			{
				if (characters[idx].abordage_enable == "true")
				{
					Characters[idx].location = boarding_adr[i].location;
					Characters[idx].location.group = boarding_adr[i].group;
					Characters[idx].location.locator = boarding_adr[i].locator;
				}
			}
		}
		//Выгружаемся в интерфейс		
		LAi_boarding_process = false;
		
		if(Surrendered)
		{
			LaunchRansackCaptain(GetMainCharacter(), boarding_enemy);
			LAi_boarding_process = false;
			return;
		}
		
		if(boarding_location_type == BRDLT_SHIP)
		{
			LaunchRansackMain(GetMainCharacter(), boarding_enemy, "");
			LAi_boarding_process = false;
		}else{
			if(boarding_location_type == BRDLT_FORT)
			{
				LaunchFortCapture(boarding_enemy);
				LAi_boarding_process = false;
			}else{
				Trace("Boarding: unknow boarding location type");
			}
		}
	}
}


//Разрешить перегрузку на следующую палубу
#event_handler("LAi_event_boarding_EnableReload", "LAi_EnableReload");
void LAi_EnableReload()
{
	SurrenderModifier--;
	defeat = defeat + (10-SurrenderModifier);
	boarding_enemy.ship.crew.morale = makeint((stf(boarding_enemy.ship.crew.morale)-rand(20))+ rand(sti(boarding_enemy.skill.Leadership))); // Rally or fall back?
	Surrendered = CheckForSurrender(GetMainCharacter(), boarding_enemy);

	SetEventHandler("Control Activation","LAi_ActivateReload",1);
	Log_SetActiveAction("Reload");
	//Уберём саблю
	ref mchr = GetMainCharacter();
	SendMessage(&mchr, "lsl", MSG_CHARACTER_EX_MSG, "ChangeFightMode", 0);
	//Вернём выживших обратно в кучу
	for(int i = 0; i < LAi_numloginedcharacters; i++)
	{
		int index = LAi_loginedcharacters[i];
		if(index >= 0)
		{
			if(index != GetMainCharacterIndex())
			{
				ref chr = &Characters[index];
				if(!LAi_IsDead(chr) && !IsOfficer(chr) && !IsCompanion(chr))
				{
					if(chr.chr_ai.group == LAI_GROUP_PLAYER) boarding_player_crew++;
				}
			}
		}
	}
}

//Активация перегрузки на следующую палубу
void LAi_ActivateReload()
{
	string controlName = GetEventData();
	if(controlName != "ChrAction") return;
	DelEventHandler("Control Activation", "LAi_ActivateReload");
	Log_SetActiveAction("Nothing");
	LAi_ReloadBoarding();
}

//Расставить персонажей для боя
void LAi_SetBoardingActors(string locID)
{
	int i, limit;
	ref chr;
	string model;
	string ani;
	string bString = "blade";
	chr = GetMainCharacter();
	int mchr_rank = sti(chr.rank);
	int xhp;
	int locIndex = FindLocation(locID);
	int mclass = GetCharacterShipClass(GetMainCharacter());
	int eclass = GetCharacterShipClass(boarding_enemy);
	
	limit = MAX_GROUP_SIZE;
	if(Locations[locIndex].models.always.l1 == "cap") limit = 4;
	if(Locations[locIndex].models.always.l1 == "hold") limit = 4;
	if(Locations[locIndex].models.always.l1 == "fort1") limit = 11;
	if(Locations[locIndex].models.always.l1 == "fort2") limit = 5;
	if(Locations[locIndex].models.always.l1 == "fort3") limit = 5;
	if(Locations[locIndex].models.always.l1 == "deck1") limit = 7;
	if(Locations[locIndex].models.always.l1 == "deck2") limit = 7;
	if(Locations[locIndex].models.always.l1 == "udeck") limit = 5;
	
	//Установим союзников из команды
	for (i = LAi_numloginedcharacters; i < limit; i++)
	{
		if(boarding_player_crew <= 0) break;
		model = LAi_GetBoardingModel(GetMainCharacter(), &ani);
		chr = LAi_CreateFantomCharacterEx(model, ani, "rld", "loc" + i);
		chr.blade = "saber";
		bString=bString + makeint(6+rand(5)-mclass);
		chr.blade.itemID = bString;
		chr.blade.time = 0.1;
		chr.blade.colorstart = argb(128, 22, 46, 190);
		chr.blade.colorend = argb(0, 20, 60, 100);
		LAi_group_MoveCharacter(chr, LAI_GROUP_PLAYER);
		LAi_SetAdjustFencingSkill(chr, 2.0, 5.0);
		LAi_AdjustFencingSkill(chr);
		xhp = boarding_player_hp + rand(10) + 5;
		LAi_SetHP(chr, xhp, xhp);
		LAi_NPC_Equip(chr, mchr_rank, true, true);
		boarding_player_crew--;
	}
	//Установим врагов
	for (i = 0; i < limit; i++)
	{
		if(boarding_enemy_crew <= 0) break;
		model = LAi_GetBoardingModel(boarding_enemy, &ani);
		chr = LAi_CreateFantomCharacterEx(model, ani, "rld", "aloc" + i);		
		chr.blade = "saber";
		bString=bString + makeint(6+rand(5)-eclass);
		chr.blade.itemID = bString;
		chr.blade.time = 0.1;
		chr.blade.colorstart = argb(128, 22, 46, 190);
		chr.blade.colorend = argb(0, 20, 60, 100);
		LAi_group_MoveCharacter(chr, LAI_GROUP_BRDENEMY);
		LAi_SetAdjustFencingSkill(chr, 2.0, 5.0);
		LAi_AdjustFencingSkill(chr);
		xhp = boarding_enemy_hp + rand(20) - 10;
		LAi_SetHP(chr, xhp, xhp);
		LAi_NPC_Equip(chr, boarding_erank, true, true);
		boarding_enemy_crew--;
	}
	//Заставим драться эти 2 группы
	LAi_group_FightGroupsEx(LAI_GROUP_PLAYER, LAI_GROUP_BRDENEMY, true, GetMainCharacterIndex(), -1, false, false);
	LAi_group_SetCheckEvent(LAI_GROUP_BRDENEMY);
}

//Убийство группы
void LAi_BoardingGroupKill()
{
	string group = GetEventData();
	if(group != LAI_GROUP_BRDENEMY) return;
	PostEvent("LAi_event_boarding_EnableReload", 5000);
}

//Моделька для абордажного персонажа
string LAi_GetBoardingModel(ref rCharacter, ref ani)
{
	ani = "man";
	int iNation = sti(rCharacter.nation);
	if (iNation < 0) iNation = PIRATE;
	Nations[iNation].boardingModel.player = "";
	Nations[iNation].boardingModel.enemy = "";
	string atr;
	if(sti(rCharacter.index) == GetMainCharacterIndex())
	{
		atr = "boardingModel.player";
	}else{
		atr = "boardingModel.enemy";
	}
	aref models;
	makearef(models, Nations[iNation].(atr));
	int num = GetAttributesNum(models);
	if(num <= 0) return "";
	int i = rand(num - 1);
	atr = GetAttributeName(GetAttributeN(models, i));
	string model = models.(atr);
	atr = atr + ".ani";
	if(CheckAttribute(models, atr))
	{
		if(models.(atr) != "") ani = models.(atr);
	}
	return model;
}

bool CheckForSurrender(ref mchr, ref echr)
{
	aref ms, es;

	makearef(ms, mchr.skill);
	makearef(es, echr.skill);

	int morale = GetAdjustedMorale(mchr, echr, false);
	int mskill = makeint((sti(ms.Leadership) + sti(ms.Fencing) + sti(ms.Grappling) + sti(ms.Sneak))/4);
	int eskill = makeint((sti(es.Leadership) + sti(es.Fencing) + sti(es.Defence) + sti(es.Sneak))/4);
	int losing = makeint(defeat/SurrenderModifier); 

	int defend = makeint(eskill+morale+sti(es.Defence));
	int attack = makeint(mskill+losing+sti(ms.Grappling));

	int mclass = GetCharacterShipClass(mchr);
	int eclass = GetCharacterShipClass(echr);

	if(mclass < eclass) { attack = attack + (eclass-mclass); }

	if(losing<sti(ms.Sneak)) losing=sti(ms.Sneak); // Defence against wierd negative value

	if(boarding_location_type == BRDLT_FORT) return false; // Forts don't surrender.

	if ((rand(20)+mskill+sti(mchr.skill.Leadership)+sti(mchr.skill.Sneak))>(eskill+sti(echr.skill.Leadership)) && (attack>defend))
	{
		return true;
	}
	return false;
}

int GetAdjustedMorale(ref mchr, ref echr, bool FinalAdjust)
{

	aref ms, es;

	makearef(ms, mchr.skill);
	makearef(es, echr.skill);

	int morale = makeint(stf(echr.ship.crew.morale));
	int mreput = sti(echr.reputation);
	int losing = makeint(defeat/SurrenderModifier); // See note #1 at function end
	int mCrewQ = sti(mchr.ship.crew.quantity);
	int eCrewQ = sti(echr.ship.crew.quantity);
	int mclass = GetCharacterShipClass(mchr);
	int eclass = GetCharacterShipClass(echr);
	int mfight = boarding_player_crew + boarding_officers + 1; // # of representative fighters
	int efight = boarding_player_crew; // # of representative fighters
	ref mstype = GetShipByType(sti(mchr.ship.Type));

	if(losing<sti(ms.Sneak)) losing=sti(ms.Sneak); // Defence against wierd negative value

	if(mclass > eclass) morale -= (mclass-eclass)*2; // Slightly nervous but not overmuch
	else if(mclass < eclass) morale += (mclass-eclass)*10; // Ohhh, you are soooo fucking DEAD!

	if(((mCrewQ + eCrewQ) > sti(mstype.MaxCrew)) && mreput < 31) morale += makeint(rand(10) + 10); // See note #2 at function end
	morale -= makeint(100 - GetHullPercent(echr));
	if(GetHullPercent(echr) < 25.0) morale -= 5; // See note #3 at function end
	if(GetHullPercent(echr) < 10.0) morale -= 10; // "Hey Jimbo, see that play at the Discovery theatre about sharks last month?"
	if((eCrewQ*2) < mCrewQ) morale -= 10; // See note #4 at function end
	if(mfight > efight) morale -= 5;
	if(mfight < efight) morale += 5;
	if(FinalAdjust) morale -= losing; // See note #5 at function end
	if(morale < 1) morale = 1; // Safety against division by zero for the future

	return morale;
}

/*
Note #1
'SurrenderModifier' is an indicator of your progress through a ship's
infrastructure and is subtracted by one each time you clear a deck.
Defeat is used and defined in both functions at the moment.  Not very
efficient, but WTF. Used to be Defeat - Defeat moved to the top, then
added Losing to maintain consistency.

Note #2
This essentially states what the enemy's crew thinks of their chances
should they surrender. If your crew plus their surviving crew exceeds
your ship's capacity AND you're reputed to be a bloody bastard  - ie,
you sink ships coz it looks kinda neat - they will want to take their
chances and attempt  to take YOUR ship instead of surrendering.  What
do they have to lose? Note that THEY don't know how many officers you
have... basically, whether or not you can take the ship if you wanted
to.  Being somewhat uniform-less, the wankers waving sharpened pieces
of metal at them could just be regular crew for all they know.

Note #3
Their ship is leaking badly and is unlikely to make it to port.  They
won't exactly be singing and dancing, now will they? Yo ho bloody ho,
fuck your damned rum and help me plug this fuckin' leak!

Note #4
There's waaay more of you than them. NOT good. Change Multiplier to 3
if you haven't fixed the enemy ship's starting crew numbers.

Note #5
No more sailing happily into the sunset for you, matey!  You've gotta
cheer up the crew on the captured ship now.  :)
Code by Stone-D : 27/07/2003
*/

void SetEndingStats()
{
	int i;
	int PBMM; // Post-Battle Morale Modifier
	float crew;
	ref PlayerChar = GetMainCharacter();
	int mclass = GetCharacterShipClass(PlayerChar);
	int eclass = GetCharacterShipClass(boarding_enemy);
	int fixbug;

	crew = (boarding_player_crew + boarding_officers + 1)*boarding_player_crew_per_chr;

	if(boarding_player_crew_per_chr>boarding_enemy_crew_per_chr)
	{
		while(crew < (GetCrewQuantity(PlayerChar)*0.8))
		{
			fixbug = (((boarding_player_crew_start - boarding_player_crew) + boarding_officers + 1)*(boarding_player_crew_per_chr-boarding_enemy_crew_per_chr))
			if (fixbug <=0) fixbug = 1.0;
			crew += fixbug;
		}
	}

	if(crew < (GetCrewQuantity(PlayerChar)-20))
	{
		crew = crew + (((GetCrewQuantity(PlayerChar)-crew)/2) + rand(makeint((GetCrewQuantity(PlayerChar)-crew)/2)));
		PBMM = makeint(GetCrewQuantity(PlayerChar)-makeint(crew + 0.5));
		if(PBMM < 1) { PBMM=0; }
		else { PBMM = makeint(((PBMM/sti(PlayerChar.skill.leadership))*7)/SurrenderModifier); } 
		if(PBMM < 1) { PBMM=0; }
	}

	if(eclass>mclass) { PBMM = PBMM - makeint(rand(makeint((eclass-mclass)*10))); }
	SetCrewQuantity(PlayerChar, makeint(crew + 0.5));

	PlayerChar.ship.crew.morale=makeint(sti(PlayerChar.ship.crew.morale)-PBMM/2); 
	if(PlayerChar.ship.crew.morale < 1) PlayerChar.ship.crew.morale = 1;
	if(PlayerChar.ship.crew.morale > MORALE_MAX) PlayerChar.ship.crew.morale = MORALE_MAX;
	if(boarding_enemy_crew<1) { boarding_enemy_crew=1; }
	crew = boarding_enemy_base_crew*(0.1 + rand(20)*0.01); 
	crew = crew + (rand(makeint(boarding_enemy_base_crew*0.4))*1.5); 

	if(boarding_echr_index >= 0)
	{
		SetCrewQuantity(&Characters[boarding_echr_index], MakeInt(crew + 0.5));
		boarding_enemy.ship.crew.morale=GetAdjustedMorale(PlayerChar, boarding_enemy, true);
		
		boarding_echr_index = -1;
	}
}
