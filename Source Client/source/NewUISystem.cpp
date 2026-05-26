#include "stdafx.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "NewUICustomMessageBox.h"
#include "NewUIInventoryCtrl.h"
#include "wsclientinline.h"
#include "PersonalShopTitleImp.h"
#include "w_CursedTemple.h"
#include "MapManager.h"
#include "./Utilities/Log/muConsoleDebug.h"

using namespace SEASON3B;

CNewUISystem* CNewUISystem::m_pInstance = NULL;

CNewUISystem::CNewUISystem()
{
	m_pNewUIMng = NULL;
	m_pNewUI3DRenderMng = NULL;
	m_pNewUIHotKey = NULL;
	m_pNewChatInputBox = NULL;
	m_pNewChatLogWindow = NULL;
	m_pNewSlideWindow = NULL;
	m_pNewGuildMakeWindow = NULL;
	m_pNewFriendWindow = NULL;
	m_pNewMainFrameWindow = NULL;
	m_pNewSkillList = NULL;
	m_pNewItemMng = NULL;
	m_pNewMyInventory = NULL;
	m_pNewNPCShop = NULL;
	m_pNewPetInfoWindow = NULL;
	m_pNewMixInventory = NULL;
	m_pNewCastleWindow = NULL;
	m_pNewGuardWindow = NULL;
	m_pNewGatemanWindow = NULL;
	m_pNewGateSwitchWindow = NULL;
	m_pNewStorageInventory = NULL;
	m_pNewGuildInfoWindow = NULL;
	m_pNewMyShopInventory = NULL;
	m_pNewPurchaseShopInventory = NULL;
	m_pNewCharacterInfoWindow = NULL;
	m_pNewMyQuestInfoWindow = NULL;
	m_pNewPartyInfoWindow = NULL;
	m_pNewPartyListWindow = NULL;
	m_pNewNPCQuest = NULL;
	m_pNewEnterBloodCastle = NULL;
	m_pNewEnterDevilSquare = NULL;
	m_pNewBloodCastle = NULL;
	m_pNewTrade = NULL;
	m_pNewKanturu2ndEnterNpc = NULL;
	m_pNewKanturuInfoWindow = NULL;
	m_pNewCatapultWindow = NULL;
	m_pNewChaosCastleTime = NULL;
	m_pNewBattleSoccerScore = NULL;
	m_pNewCommandWindow = NULL;
	m_pNewWindowMenu = NULL;
	m_pNewOptionWindow = NULL;
	m_pNewHeroPositionInfo = NULL;
	m_pNewHelpWindow = NULL;
	m_pNewItemExplanationWindow = NULL;
	m_pNewSetItemExplanation = NULL;
	m_pNewQuickCommandWindow = NULL;
	m_pNewMoveCommandWindow = NULL;
	m_pNewDuelWindow = NULL;
	m_pNewSiegeWarfare = NULL;
	m_pNewItemEnduranceInfo = NULL;
	m_pNewBuffWindow = NULL;
	m_pNewCursedTempleEnterWindow = NULL;
	m_pNewCursedTempleWindow = NULL;
	m_pNewCursedTempleResultWindow = NULL;
	m_pNewCryWolfInterface = NULL;
	m_pNewMaster_Level_Interface = NULL;
	m_pNewGoldBowman = NULL;
	m_pNewGoldBowmanLena = NULL;
	m_pNewLuckyCoinRegistration = NULL;
	m_pNewExchangeLuckyCoinWindow = NULL;
	m_pNewDuelWatchWindow = NULL;
	m_pNewDuelWatchMainFrameWindow = NULL;
	m_pNewDuelWatchUserListWindow = NULL;
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewInGameShop = NULL;
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewDoppelGangerWindow = NULL;
	m_pNewDoppelGangerFrame = NULL;
	m_pNewQuestProgress = NULL;
	m_pNewQuestProgressByEtc = NULL;
	m_pNewEmpireGuardianNPC = NULL;
	m_pNewEmpireGuardianTimer = NULL;
	m_pNewNPCDialogue = NULL;
	m_pNewMiniMap = NULL;
	m_pNewGensRanking = NULL;
	m_pNewCustomRanking = NULL;
	m_pNewCustomEventTime = NULL;
	m_pNewCustomBuyVip = NULL;
	m_pNewCustomArena = NULL;
	m_pNewUnitedMarketPlaceWindow = NULL;
#ifdef LEM_ADD_LUCKYITEM
	m_pNewUILuckyItemWnd = NULL;
#endif // LEM_ADD_LUCKYITEM
}

CNewUISystem::~CNewUISystem()
{
	Release();
}

bool CNewUISystem::Create()
{
	m_pNewUIMng = new CNewUIManager;

	m_pNewUI3DRenderMng = new CNewUI3DRenderMng;
	if(!m_pNewUI3DRenderMng->Create(m_pNewUIMng))
		return false;

	m_pNewUIHotKey = new CNewUIHotKey;
	if(!m_pNewUIHotKey->Create(m_pNewUIMng))
		return false;

	m_pNewOptionWindow = new CNewUIOptionWindow;
	if(!m_pNewOptionWindow->Create(m_pNewUIMng, 0, 0))
		{ return false; }

	m_pNewChatLogWindow = new CNewUIChatLogWindow;
	if(!m_pNewChatLogWindow->Create(m_pNewUIMng, 0, 0, 6))
		{ return false; }

	CNewUIMessageBoxMng::GetInstance()->Create(m_pNewUIMng);

	return true;
}

void CNewUISystem::Release()
{
	CNewUIMessageBoxMng::GetInstance()->Release();

#ifdef LEM_ADD_LUCKYITEM
	SAFE_DELETE(m_pNewUILuckyItemWnd);
#endif // LEM_ADD_LUCKYITEM
	SAFE_DELETE(m_pNewUnitedMarketPlaceWindow);
	SAFE_DELETE(m_pNewCustomArena);
	SAFE_DELETE(m_pNewCustomBuyVip);
	SAFE_DELETE(m_pNewCustomEventTime);
	SAFE_DELETE(m_pNewCustomRanking);
	SAFE_DELETE(m_pNewGensRanking);
	SAFE_DELETE(m_pNewMiniMap);
	SAFE_DELETE(m_pNewNPCDialogue);
	SAFE_DELETE(m_pNewEmpireGuardianTimer);
	SAFE_DELETE(m_pNewEmpireGuardianNPC);
	SAFE_DELETE(m_pNewQuestProgressByEtc);
	SAFE_DELETE(m_pNewQuestProgress);
	SAFE_DELETE(m_pNewDoppelGangerFrame);
	SAFE_DELETE(m_pNewDoppelGangerWindow);
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	SAFE_DELETE(m_pNewInGameShop);
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	SAFE_DELETE(m_pNewDuelWatchUserListWindow);
	SAFE_DELETE(m_pNewDuelWatchMainFrameWindow);
	SAFE_DELETE(m_pNewDuelWatchWindow);
	SAFE_DELETE(m_pNewExchangeLuckyCoinWindow);
	SAFE_DELETE(m_pNewLuckyCoinRegistration);
	SAFE_DELETE(m_pNewGoldBowmanLena);
	SAFE_DELETE(m_pNewGoldBowman);
	SAFE_DELETE(m_pNewMaster_Level_Interface);
	SAFE_DELETE(m_pNewCryWolfInterface);
	SAFE_DELETE(m_pNewCursedTempleResultWindow);
	SAFE_DELETE(m_pNewCursedTempleWindow);
	SAFE_DELETE(m_pNewCursedTempleEnterWindow);
	SAFE_DELETE(m_pNewBuffWindow);
	SAFE_DELETE(m_pNewItemEnduranceInfo);
	SAFE_DELETE(m_pNewSiegeWarfare);
	SAFE_DELETE(m_pNewDuelWindow);
	SAFE_DELETE(m_pNewMoveCommandWindow);
	SAFE_DELETE(m_pNewQuickCommandWindow);
	SAFE_DELETE(m_pNewSetItemExplanation);
	SAFE_DELETE(m_pNewItemExplanationWindow);
	SAFE_DELETE(m_pNewHelpWindow);
	SAFE_DELETE(m_pNewHeroPositionInfo);
	SAFE_DELETE(m_pNewOptionWindow);
	SAFE_DELETE(m_pNewWindowMenu);
	SAFE_DELETE(m_pNewCommandWindow);
	SAFE_DELETE(m_pNewBattleSoccerScore);
	SAFE_DELETE(m_pNewChaosCastleTime);
	SAFE_DELETE(m_pNewCatapultWindow);
	SAFE_DELETE(m_pNewKanturuInfoWindow);
	SAFE_DELETE(m_pNewKanturu2ndEnterNpc);
	SAFE_DELETE(m_pNewTrade);
	SAFE_DELETE(m_pNewBloodCastle);
	SAFE_DELETE(m_pNewEnterDevilSquare);
	SAFE_DELETE(m_pNewEnterBloodCastle);
	SAFE_DELETE(m_pNewNPCQuest);
	SAFE_DELETE(m_pNewPartyListWindow);
	SAFE_DELETE(m_pNewPartyInfoWindow);
	SAFE_DELETE(m_pNewMyQuestInfoWindow);
	SAFE_DELETE(m_pNewCharacterInfoWindow);
	SAFE_DELETE(m_pNewPurchaseShopInventory);
	SAFE_DELETE(m_pNewMyShopInventory);
	SAFE_DELETE(m_pNewGuildInfoWindow);
	SAFE_DELETE(m_pNewStorageInventory);
	SAFE_DELETE(m_pNewGateSwitchWindow);
	SAFE_DELETE(m_pNewGatemanWindow);
	SAFE_DELETE(m_pNewGuardWindow);
	SAFE_DELETE(m_pNewCastleWindow);
	SAFE_DELETE(m_pNewMixInventory);
	SAFE_DELETE(m_pNewPetInfoWindow);
	SAFE_DELETE(m_pNewNPCShop);
	SAFE_DELETE(m_pNewMyInventory);
	SAFE_DELETE(m_pNewItemMng);
	SAFE_DELETE(m_pNewSkillList);
	SAFE_DELETE(m_pNewMainFrameWindow);
	SAFE_DELETE(m_pNewFriendWindow);
	SAFE_DELETE(m_pNewGuildMakeWindow);
	SAFE_DELETE(m_pNewSlideWindow);
	SAFE_DELETE(m_pNewChatLogWindow);
	SAFE_DELETE(m_pNewChatInputBox);
	SAFE_DELETE(m_pNewUIHotKey);
	SAFE_DELETE(m_pNewUI3DRenderMng);
	SAFE_DELETE(m_pNewUIMng);
}

bool CNewUISystem::LoadMainSceneInterface()
{
	int x = 0, y = 0;

	m_pNewChatInputBox = new CNewUIChatInputBox;
	if(!m_pNewChatInputBox->Create(m_pNewUIMng, m_pNewChatLogWindow, x, y))
		{ return false; }

	m_pNewChatLogWindow = new CNewUIChatLogWindow;
	if(!m_pNewChatLogWindow->Create(m_pNewUIMng, x, y, 6))
		{ return false; }

	m_pNewSlideWindow = new CNewUISlideWindow;
	if(!m_pNewSlideWindow->Create(m_pNewUIMng))
		{ return false; }

	m_pNewGuildMakeWindow = new CNewUIGuildMakeWindow;
	if(!m_pNewGuildMakeWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewFriendWindow = new CNewUIFriendWindow;
	if(!m_pNewFriendWindow->Create(m_pNewUIMng))
		{ return false; }

	m_pNewMainFrameWindow = new CNewUIMainFrameWindow;
	if(!m_pNewMainFrameWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng))
		{ return false; }

	m_pNewSkillList = new CNewUISkillList;
	if(!m_pNewSkillList->Create(m_pNewUIMng, m_pNewUI3DRenderMng))
		{ return false; }

	m_pNewItemMng = new CNewUIItemMng;
	m_pNewMyInventory = new CNewUIMyInventory;
	if(!m_pNewMyInventory->Create(m_pNewUIMng, m_pNewUI3DRenderMng, x, y))
		{ return false; }

	m_pNewNPCShop = new CNewUINPCShop;
	if(!m_pNewNPCShop->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewPetInfoWindow = new CNewUIPetInfoWindow;
	if(!m_pNewPetInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMixInventory = new CNewUIMixInventory;
	if(!m_pNewMixInventory->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCastleWindow = new CNewUICastleWindow;
	if(!m_pNewCastleWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGuardWindow = new CNewUIGuardWindow;
	if(!m_pNewGuardWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGatemanWindow = new CNewUIGatemanWindow;
	if(!m_pNewGatemanWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGateSwitchWindow = new CNewUIGateSwitchWindow;
	if(!m_pNewGateSwitchWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewStorageInventory = new CNewUIStorageInventory;
	if(!m_pNewStorageInventory->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGuildInfoWindow = new CNewUIGuildInfoWindow;
	if(!m_pNewGuildInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMyShopInventory = new CNewUIMyShopInventory;
	if(!m_pNewMyShopInventory->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewPurchaseShopInventory = new CNewUIPurchaseShopInventory;
	if(!m_pNewPurchaseShopInventory->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCharacterInfoWindow = new CNewUICharacterInfoWindow;
	if(!m_pNewCharacterInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMyQuestInfoWindow = new CNewUIMyQuestInfoWindow;
	if(!m_pNewMyQuestInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewPartyInfoWindow = new CNewUIPartyInfoWindow;
	if(!m_pNewPartyInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewPartyListWindow = new CNewUIPartyListWindow;
	if(!m_pNewPartyListWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewNPCQuest = new CNewUINPCQuest;
	if(!m_pNewNPCQuest->Create(m_pNewUIMng, m_pNewUI3DRenderMng, x, y))
		{ return false; }

	m_pNewEnterBloodCastle = new CNewUIEnterBloodCastle;
	if(!m_pNewEnterBloodCastle->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewEnterDevilSquare = new CNewUIEnterDevilSquare;
	if(!m_pNewEnterDevilSquare->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewBloodCastle = new CNewUIBloodCastle;
	if(!m_pNewBloodCastle->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewTrade = new CNewUITrade;
	if(!m_pNewTrade->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewKanturu2ndEnterNpc = new CNewUIKanturu2ndEnterNpc;
	if(!m_pNewKanturu2ndEnterNpc->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewKanturuInfoWindow = new CNewUIKanturuInfoWindow;
	if(!m_pNewKanturuInfoWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCatapultWindow = new CNewUICatapultWindow;
	if(!m_pNewCatapultWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewChaosCastleTime = new CNewUIChaosCastleTime;
	if(!m_pNewChaosCastleTime->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewBattleSoccerScore = new CNewUIBattleSoccerScore;
	if(!m_pNewBattleSoccerScore->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCommandWindow = new CNewUICommandWindow;
	if(!m_pNewCommandWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewWindowMenu = new CNewUIWindowMenu;
	if(!m_pNewWindowMenu->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewHeroPositionInfo = new CNewUIHeroPositionInfo;
	if(!m_pNewHeroPositionInfo->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewHelpWindow = new CNewUIHelpWindow;
	if(!m_pNewHelpWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewItemExplanationWindow = new CNewUIItemExplanationWindow;
	if(!m_pNewItemExplanationWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewSetItemExplanation = new CNewUISetItemExplanation;
	if(!m_pNewSetItemExplanation->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewQuickCommandWindow = new CNewUIQuickCommandWindow;
	if(!m_pNewQuickCommandWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMoveCommandWindow = new CNewUIMoveCommandWindow;
	if(!m_pNewMoveCommandWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewDuelWindow = new CNewUIDuelWindow;
	if(!m_pNewDuelWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewSiegeWarfare = new CNewUISiegeWarfare;
	if(!m_pNewSiegeWarfare->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewItemEnduranceInfo = new CNewUIItemEnduranceInfo;
	if(!m_pNewItemEnduranceInfo->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewBuffWindow = new CNewUIBuffWindow;
	if(!m_pNewBuffWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCursedTempleEnterWindow = new CNewUICursedTempleEnter;
	if(!m_pNewCursedTempleEnterWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCursedTempleWindow = new CNewUICursedTempleSystem;
	if(!m_pNewCursedTempleWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCursedTempleResultWindow = new CNewUICursedTempleResult;
	if(!m_pNewCursedTempleResultWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCryWolfInterface = new CNewUICryWolf;
	if(!m_pNewCryWolfInterface->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMaster_Level_Interface = new CNewUIMasterLevel;
	if(!m_pNewMaster_Level_Interface->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGoldBowman = new CNewUIGoldBowmanWindow;
	if(!m_pNewGoldBowman->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGoldBowmanLena = new CNewUIGoldBowmanLena;
	if(!m_pNewGoldBowmanLena->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewLuckyCoinRegistration = new CNewUIRegistrationLuckyCoin;
	if(!m_pNewLuckyCoinRegistration->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewExchangeLuckyCoinWindow = new CNewUIExchangeLuckyCoin;
	if(!m_pNewExchangeLuckyCoinWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewDuelWatchWindow = new CNewUIDuelWatchWindow;
	if(!m_pNewDuelWatchWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewDuelWatchMainFrameWindow = new CNewUIDuelWatchMainFrameWindow;
	if(!m_pNewDuelWatchMainFrameWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng))
		{ return false; }

	m_pNewDuelWatchUserListWindow = new CNewUIDuelWatchUserListWindow;
	if(!m_pNewDuelWatchUserListWindow->Create(m_pNewUIMng, x, y))
		{ return false; }

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewInGameShop = new CNewUIInGameShop;
	if(!m_pNewInGameShop->Create(m_pNewUIMng, x, y))
		{ return false; }

#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewDoppelGangerWindow = new CNewUIDoppelGangerWindow;
	if(!m_pNewDoppelGangerWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng, x, y))
		{ return false; }

	m_pNewDoppelGangerFrame = new CNewUIDoppelGangerFrame;
	if(!m_pNewDoppelGangerFrame->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewQuestProgress = new CNewUIQuestProgress;
	if(!m_pNewQuestProgress->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewQuestProgressByEtc = new CNewUIQuestProgressByEtc;
	if(!m_pNewQuestProgressByEtc->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewEmpireGuardianNPC = new CNewUIEmpireGuardianNPC;
	if(!m_pNewEmpireGuardianNPC->Create(m_pNewUIMng, m_pNewUI3DRenderMng, x, y))
		{ return false; }

	m_pNewEmpireGuardianTimer = new CNewUIEmpireGuardianTimer;
	if(!m_pNewEmpireGuardianTimer->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewNPCDialogue = new CNewUINPCDialogue;
	if(!m_pNewNPCDialogue->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewMiniMap = new CNewUIMiniMap;
	if(!m_pNewMiniMap->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewGensRanking = new CNewUIGensRanking;
	if(!m_pNewGensRanking->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCustomRanking = new CNewUICustomRanking;
	if(!m_pNewCustomRanking->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCustomEventTime = new CNewUICustomEventTime;
	if(!m_pNewCustomEventTime->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCustomBuyVip = new CNewUICustomBuyVip;
	if(!m_pNewCustomBuyVip->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewCustomArena = new CNewUICustomArena;
	if(!m_pNewCustomArena->Create(m_pNewUIMng, x, y))
		{ return false; }

	m_pNewUnitedMarketPlaceWindow = new CNewUIUnitedMarketPlaceWindow;
	if(!m_pNewUnitedMarketPlaceWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng, x, y))
		{ return false; }

#ifdef LEM_ADD_LUCKYITEM
	m_pNewUILuckyItemWnd = new CNewUILuckyItemWnd;
	if(!m_pNewUILuckyItemWnd->Create(m_pNewUIMng, x, y))
		{ return false; }

#endif // LEM_ADD_LUCKYITEM
	return true;
}

void CNewUISystem::UnloadMainSceneInterface()
{
#ifdef LEM_ADD_LUCKYITEM
	SAFE_DELETE(m_pNewUILuckyItemWnd);
#endif // LEM_ADD_LUCKYITEM
	SAFE_DELETE(m_pNewUnitedMarketPlaceWindow);
	SAFE_DELETE(m_pNewCustomArena);
	SAFE_DELETE(m_pNewCustomBuyVip);
	SAFE_DELETE(m_pNewCustomEventTime);
	SAFE_DELETE(m_pNewCustomRanking);
	SAFE_DELETE(m_pNewGensRanking);
	SAFE_DELETE(m_pNewMiniMap);
	SAFE_DELETE(m_pNewNPCDialogue);
	SAFE_DELETE(m_pNewEmpireGuardianTimer);
	SAFE_DELETE(m_pNewEmpireGuardianNPC);
	SAFE_DELETE(m_pNewQuestProgressByEtc);
	SAFE_DELETE(m_pNewQuestProgress);
	SAFE_DELETE(m_pNewDoppelGangerFrame);
	SAFE_DELETE(m_pNewDoppelGangerWindow);
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	SAFE_DELETE(m_pNewInGameShop);
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	SAFE_DELETE(m_pNewDuelWatchUserListWindow);
	SAFE_DELETE(m_pNewDuelWatchMainFrameWindow);
	SAFE_DELETE(m_pNewDuelWatchWindow);
	SAFE_DELETE(m_pNewExchangeLuckyCoinWindow);
	SAFE_DELETE(m_pNewLuckyCoinRegistration);
	SAFE_DELETE(m_pNewGoldBowmanLena);
	SAFE_DELETE(m_pNewGoldBowman);
	SAFE_DELETE(m_pNewMaster_Level_Interface);
	SAFE_DELETE(m_pNewCryWolfInterface);
	SAFE_DELETE(m_pNewCursedTempleResultWindow);
	SAFE_DELETE(m_pNewCursedTempleWindow);
	SAFE_DELETE(m_pNewCursedTempleEnterWindow);
	SAFE_DELETE(m_pNewBuffWindow);
	SAFE_DELETE(m_pNewItemEnduranceInfo);
	SAFE_DELETE(m_pNewSiegeWarfare);
	SAFE_DELETE(m_pNewDuelWindow);
	SAFE_DELETE(m_pNewMoveCommandWindow);
	SAFE_DELETE(m_pNewQuickCommandWindow);
	SAFE_DELETE(m_pNewSetItemExplanation);
	SAFE_DELETE(m_pNewItemExplanationWindow);
	SAFE_DELETE(m_pNewHelpWindow);
	SAFE_DELETE(m_pNewHeroPositionInfo);
	SAFE_DELETE(m_pNewOptionWindow);
	SAFE_DELETE(m_pNewWindowMenu);
	SAFE_DELETE(m_pNewCommandWindow);
	SAFE_DELETE(m_pNewBattleSoccerScore);
	SAFE_DELETE(m_pNewChaosCastleTime);
	SAFE_DELETE(m_pNewCatapultWindow);
	SAFE_DELETE(m_pNewKanturuInfoWindow);
	SAFE_DELETE(m_pNewKanturu2ndEnterNpc);
	SAFE_DELETE(m_pNewTrade);
	SAFE_DELETE(m_pNewBloodCastle);
	SAFE_DELETE(m_pNewEnterDevilSquare);
	SAFE_DELETE(m_pNewEnterBloodCastle);
	SAFE_DELETE(m_pNewNPCQuest);
	SAFE_DELETE(m_pNewPartyListWindow);
	SAFE_DELETE(m_pNewPartyInfoWindow);
	SAFE_DELETE(m_pNewMyQuestInfoWindow);
	SAFE_DELETE(m_pNewCharacterInfoWindow);
	SAFE_DELETE(m_pNewPurchaseShopInventory);
	SAFE_DELETE(m_pNewMyShopInventory);
	SAFE_DELETE(m_pNewGuildInfoWindow);
	SAFE_DELETE(m_pNewStorageInventory);
	SAFE_DELETE(m_pNewGateSwitchWindow);
	SAFE_DELETE(m_pNewGatemanWindow);
	SAFE_DELETE(m_pNewGuardWindow);
	SAFE_DELETE(m_pNewCastleWindow);
	SAFE_DELETE(m_pNewMixInventory);
	SAFE_DELETE(m_pNewPetInfoWindow);
	SAFE_DELETE(m_pNewNPCShop);
	SAFE_DELETE(m_pNewMyInventory);
	SAFE_DELETE(m_pNewItemMng);
	SAFE_DELETE(m_pNewSkillList);
	SAFE_DELETE(m_pNewMainFrameWindow);
	SAFE_DELETE(m_pNewFriendWindow);
	SAFE_DELETE(m_pNewGuildMakeWindow);
	SAFE_DELETE(m_pNewSlideWindow);
	SAFE_DELETE(m_pNewChatLogWindow);
	SAFE_DELETE(m_pNewChatInputBox);
}

bool CNewUISystem::IsVisible(DWORD dwKey)
{
	return m_pNewUIMng->IsInterfaceVisible(dwKey);
}

void CNewUISystem::Show(DWORD dwKey)
{
	switch(dwKey)
	{
	case INTERFACE_CHATINPUTBOX:
		m_pNewChatInputBox->OpenningProcess();
		break;
	case INTERFACE_INVENTORY:
		m_pNewMyInventory->OpenningProcess();
		break;
	case INTERFACE_NPCSHOP:
		m_pNewNPCShop->OpenningProcess();
		break;
	case INTERFACE_PET:
		m_pNewPetInfoWindow->OpenningProcess();
		break;
	case INTERFACE_GUILDINFO:
		m_pNewGuildInfoWindow->OpenningProcess();
		break;
	case INTERFACE_CHARACTER:
		m_pNewCharacterInfoWindow->OpenningProcess();
		break;
	case INTERFACE_MYQUEST:
		m_pNewMyQuestInfoWindow->OpenningProcess();
		break;
	case INTERFACE_PARTY_INFO_WINDOW:
		m_pNewPartyInfoWindow->OpenningProcess();
		break;
	case INTERFACE_PARTY:
		m_pNewPartyListWindow->OpenningProcess();
		break;
	case INTERFACE_BLOODCASTLE:
		m_pNewEnterBloodCastle->OpenningProcess();
		break;
	case INTERFACE_DEVILSQUARE:
		m_pNewEnterDevilSquare->OpenningProcess();
		break;
	case INTERFACE_BLOODCASTLE_TIME:
		m_pNewBloodCastle->OpenningProcess();
		break;
	case INTERFACE_CATAPULT:
		m_pNewCatapultWindow->OpenningProcess();
		break;
	case INTERFACE_CHAOSCASTLE_TIME:
		m_pNewChaosCastleTime->OpenningProcess();
		break;
	case INTERFACE_COMMAND:
		m_pNewCommandWindow->OpenningProcess();
		break;
	case INTERFACE_WINDOW_MENU:
		m_pNewWindowMenu->OpenningProcess();
		break;
	case INTERFACE_OPTION:
		m_pNewOptionWindow->OpenningProcess();
		break;
	case INTERFACE_HERO_POSITION_INFO:
		m_pNewHeroPositionInfo->OpenningProcess();
		break;
	case INTERFACE_HELP:
		m_pNewHelpWindow->OpenningProcess();
		break;
	case INTERFACE_ITEM_EXPLANATION:
		m_pNewItemExplanationWindow->OpenningProcess();
		break;
	case INTERFACE_SETITEM_EXPLANATION:
		m_pNewSetItemExplanation->OpenningProcess();
		break;
	case INTERFACE_QUICK_COMMAND:
		m_pNewQuickCommandWindow->OpenningProcess();
		break;
	case INTERFACE_MOVEMAP:
		m_pNewMoveCommandWindow->OpenningProcess();
		break;
	case INTERFACE_SIEGEWARFARE:
		m_pNewSiegeWarfare->OpenningProcess();
		break;
	case INTERFACE_ITEM_ENDURANCE_INFO:
		m_pNewItemEnduranceInfo->OpenningProcess();
		break;
	case INTERFACE_BUFF_WINDOW:
		m_pNewBuffWindow->OpenningProcess();
		break;
	case INTERFACE_CURSEDTEMPLE_RESULT:
		m_pNewCursedTempleResultWindow->OpenningProcess();
		break;
	case INTERFACE_CRYWOLF:
		m_pNewCryWolfInterface->OpenningProcess();
		break;
	case INTERFACE_MASTER_LEVEL:
		m_pNewMaster_Level_Interface->OpenningProcess();
		break;
	case INTERFACE_EXCHANGE_LUCKYCOIN:
		m_pNewExchangeLuckyCoinWindow->OpenningProcess();
		break;
	case INTERFACE_EMPIREGUARDIAN_NPC:
		m_pNewEmpireGuardianNPC->OpenningProcess();
		break;
	case INTERFACE_EMPIREGUARDIAN_TIMER:
		m_pNewEmpireGuardianTimer->OpenningProcess();
		break;
	case INTERFACE_MINI_MAP:
		m_pNewMiniMap->OpenningProcess();
		break;
	case INTERFACE_GENSRANKING:
		m_pNewGensRanking->OpenningProcess();
		break;
	case INTERFACE_CUSTOM_RANKING:
		m_pNewCustomRanking->OpenningProcess();
		break;
	case INTERFACE_CUSTOM_EVENTTIME:
		m_pNewCustomEventTime->OpenningProcess();
		break;
	case INTERFACE_CUSTOM_BUYVIP:
		m_pNewCustomBuyVip->OpenningProcess();
		break;
	case INTERFACE_CUSTOM_ARENA:
		m_pNewCustomArena->OpenningProcess();
		break;
	}
	m_pNewUIMng->ShowInterface(dwKey, true);
}

void CNewUISystem::Hide(DWORD dwKey)
{
	switch(dwKey)
	{
	case INTERFACE_CHATINPUTBOX:
		m_pNewChatInputBox->ClosingProcess();
		break;
	case INTERFACE_NPCGUILDMASTER:
		m_pNewGuildMakeWindow->ClosingProcess();
		break;
	case INTERFACE_INVENTORY:
		m_pNewMyInventory->ClosingProcess();
		break;
	case INTERFACE_NPCSHOP:
		m_pNewNPCShop->ClosingProcess();
		break;
	case INTERFACE_PET:
		m_pNewPetInfoWindow->ClosingProcess();
		break;
	case INTERFACE_MIXINVENTORY:
		m_pNewMixInventory->ClosingProcess();
		break;
	case INTERFACE_GUARDSMAN:
		m_pNewGuardWindow->ClosingProcess();
		break;
	case INTERFACE_GATEKEEPER:
		m_pNewGatemanWindow->ClosingProcess();
		break;
	case INTERFACE_GATESWITCH:
		m_pNewGateSwitchWindow->ClosingProcess();
		break;
	case INTERFACE_GUILDINFO:
		m_pNewGuildInfoWindow->ClosingProcess();
		break;
	case INTERFACE_MYSHOP_INVENTORY:
		m_pNewMyShopInventory->ClosingProcess();
		break;
	case INTERFACE_PURCHASESHOP_INVENTORY:
		m_pNewPurchaseShopInventory->ClosingProcess();
		break;
	case INTERFACE_MYQUEST:
		m_pNewMyQuestInfoWindow->ClosingProcess();
		break;
	case INTERFACE_PARTY_INFO_WINDOW:
		m_pNewPartyInfoWindow->ClosingProcess();
		break;
	case INTERFACE_PARTY:
		m_pNewPartyListWindow->ClosingProcess();
		break;
	case INTERFACE_BLOODCASTLE:
		m_pNewEnterBloodCastle->ClosingProcess();
		break;
	case INTERFACE_DEVILSQUARE:
		m_pNewEnterDevilSquare->ClosingProcess();
		break;
	case INTERFACE_BLOODCASTLE_TIME:
		m_pNewBloodCastle->ClosingProcess();
		break;
	case INTERFACE_CATAPULT:
		m_pNewCatapultWindow->ClosingProcess();
		break;
	case INTERFACE_CHAOSCASTLE_TIME:
		m_pNewChaosCastleTime->ClosingProcess();
		break;
	case INTERFACE_COMMAND:
		m_pNewCommandWindow->ClosingProcess();
		break;
	case INTERFACE_WINDOW_MENU:
		m_pNewWindowMenu->ClosingProcess();
		break;
	case INTERFACE_OPTION:
		m_pNewOptionWindow->ClosingProcess();
		break;
	case INTERFACE_HERO_POSITION_INFO:
		m_pNewHeroPositionInfo->ClosingProcess();
		break;
	case INTERFACE_HELP:
		m_pNewHelpWindow->ClosingProcess();
		break;
	case INTERFACE_ITEM_EXPLANATION:
		m_pNewItemExplanationWindow->ClosingProcess();
		break;
	case INTERFACE_SETITEM_EXPLANATION:
		m_pNewSetItemExplanation->ClosingProcess();
		break;
	case INTERFACE_QUICK_COMMAND:
		m_pNewQuickCommandWindow->ClosingProcess();
		break;
	case INTERFACE_MOVEMAP:
		m_pNewMoveCommandWindow->ClosingProcess();
		break;
	case INTERFACE_SIEGEWARFARE:
		m_pNewSiegeWarfare->ClosingProcess();
		break;
	case INTERFACE_ITEM_ENDURANCE_INFO:
		m_pNewItemEnduranceInfo->ClosingProcess();
		break;
	case INTERFACE_BUFF_WINDOW:
		m_pNewBuffWindow->ClosingProcess();
		break;
	case INTERFACE_CURSEDTEMPLE_RESULT:
		m_pNewCursedTempleResultWindow->ClosingProcess();
		break;
	case INTERFACE_CRYWOLF:
		m_pNewCryWolfInterface->ClosingProcess();
		break;
	case INTERFACE_MASTER_LEVEL:
		m_pNewMaster_Level_Interface->ClosingProcess();
		break;
	case INTERFACE_GOLD_BOWMAN:
		m_pNewGoldBowman->ClosingProcess();
		break;
	case INTERFACE_GOLD_BOWMAN_LENA:
		m_pNewGoldBowmanLena->ClosingProcess();
		break;
	case INTERFACE_LUCKYCOIN_REGISTRATION:
		m_pNewLuckyCoinRegistration->ClosingProcess();
		break;
	case INTERFACE_EXCHANGE_LUCKYCOIN:
		m_pNewExchangeLuckyCoinWindow->ClosingProcess();
		break;
	case INTERFACE_DUELWATCH:
		m_pNewDuelWatchWindow->ClosingProcess();
		break;
	case INTERFACE_DUELWATCH_MAINFRAME:
		m_pNewDuelWatchMainFrameWindow->ClosingProcess();
		break;
	case INTERFACE_DUELWATCH_USERLIST:
		m_pNewDuelWatchUserListWindow->ClosingProcess();
		break;
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	case INTERFACE_INGAMESHOP:
		m_pNewInGameShop->ClosingProcess();
		break;
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	case INTERFACE_DOPPELGANGER_NPC:
		m_pNewDoppelGangerWindow->ClosingProcess();
		break;
	case INTERFACE_DOPPELGANGER_FRAME:
		m_pNewDoppelGangerFrame->ClosingProcess();
		break;
	case INTERFACE_EMPIREGUARDIAN_NPC:
		m_pNewEmpireGuardianNPC->ClosingProcess();
		break;
	case INTERFACE_EMPIREGUARDIAN_TIMER:
		m_pNewEmpireGuardianTimer->ClosingProcess();
		break;
	case INTERFACE_MINI_MAP:
		m_pNewMiniMap->ClosingProcess();
		break;
	case INTERFACE_GENSRANKING:
		m_pNewGensRanking->ClosingProcess();
		break;
	case INTERFACE_CUSTOM_RANKING:
		m_pNewCustomRanking->ClosingProcess();
		break;
	case INTERFACE_CUSTOM_EVENTTIME:
		m_pNewCustomEventTime->ClosingProcess();
		break;
	case INTERFACE_CUSTOM_BUYVIP:
		m_pNewCustomBuyVip->ClosingProcess();
		break;
	case INTERFACE_CUSTOM_ARENA:
		m_pNewCustomArena->ClosingProcess();
		break;
	case INTERFACE_UNITEDMARKETPLACE_NPC_JULIA:
		m_pNewUnitedMarketPlaceWindow->ClosingProcess();
		break;
#ifdef LEM_ADD_LUCKYITEM
	case INTERFACE_LUCKYITEMWND:
		m_pNewUILuckyItemWnd->ClosingProcess();
		break;
#endif // LEM_ADD_LUCKYITEM
	}
	m_pNewUIMng->ShowInterface(dwKey, false);
}

void CNewUISystem::Toggle(DWORD dwKey)
{
	if(m_pNewUIMng->IsInterfaceVisible(dwKey))
		Hide(dwKey);
	else
		Show(dwKey);
}

void CNewUISystem::HideAll()
{
	HideAllGroupA();
	HideAllGroupB();
}

void CNewUISystem::HideAllGroupA()
{
	m_pNewChatInputBox->ClosingProcess();
	m_pNewChatInputBox->Show(false);
	m_pNewChatLogWindow->Show(false);
	m_pNewSlideWindow->Show(false);
	m_pNewGuildMakeWindow->ClosingProcess();
	m_pNewGuildMakeWindow->Show(false);
	m_pNewFriendWindow->Show(false);
	m_pNewMainFrameWindow->Show(false);
	m_pNewSkillList->Show(false);
	m_pNewMyInventory->ClosingProcess();
	m_pNewMyInventory->Show(false);
	m_pNewNPCShop->ClosingProcess();
	m_pNewNPCShop->Show(false);
	m_pNewPetInfoWindow->ClosingProcess();
	m_pNewPetInfoWindow->Show(false);
	m_pNewMixInventory->ClosingProcess();
	m_pNewMixInventory->Show(false);
	m_pNewCastleWindow->Show(false);
	m_pNewGuardWindow->ClosingProcess();
	m_pNewGuardWindow->Show(false);
	m_pNewGatemanWindow->ClosingProcess();
	m_pNewGatemanWindow->Show(false);
	m_pNewGateSwitchWindow->ClosingProcess();
	m_pNewGateSwitchWindow->Show(false);
	m_pNewStorageInventory->Show(false);
	m_pNewGuildInfoWindow->ClosingProcess();
	m_pNewGuildInfoWindow->Show(false);
	m_pNewMyShopInventory->ClosingProcess();
	m_pNewMyShopInventory->Show(false);
	m_pNewPurchaseShopInventory->ClosingProcess();
	m_pNewPurchaseShopInventory->Show(false);
	m_pNewCharacterInfoWindow->Show(false);
	m_pNewMyQuestInfoWindow->ClosingProcess();
	m_pNewMyQuestInfoWindow->Show(false);
	m_pNewPartyInfoWindow->ClosingProcess();
	m_pNewPartyInfoWindow->Show(false);
	m_pNewPartyListWindow->ClosingProcess();
	m_pNewPartyListWindow->Show(false);
	m_pNewNPCQuest->Show(false);
	m_pNewEnterBloodCastle->ClosingProcess();
	m_pNewEnterBloodCastle->Show(false);
	m_pNewEnterDevilSquare->ClosingProcess();
	m_pNewEnterDevilSquare->Show(false);
	m_pNewBloodCastle->ClosingProcess();
	m_pNewBloodCastle->Show(false);
	m_pNewTrade->Show(false);
	m_pNewKanturu2ndEnterNpc->Show(false);
	m_pNewKanturuInfoWindow->Show(false);
	m_pNewCatapultWindow->ClosingProcess();
	m_pNewCatapultWindow->Show(false);
	m_pNewChaosCastleTime->ClosingProcess();
	m_pNewChaosCastleTime->Show(false);
	m_pNewBattleSoccerScore->Show(false);
	m_pNewCommandWindow->ClosingProcess();
	m_pNewCommandWindow->Show(false);
	m_pNewWindowMenu->ClosingProcess();
	m_pNewWindowMenu->Show(false);
	m_pNewOptionWindow->ClosingProcess();
	m_pNewOptionWindow->Show(false);
	m_pNewHeroPositionInfo->ClosingProcess();
	m_pNewHeroPositionInfo->Show(false);
	m_pNewHelpWindow->ClosingProcess();
	m_pNewHelpWindow->Show(false);
	m_pNewItemExplanationWindow->ClosingProcess();
	m_pNewItemExplanationWindow->Show(false);
	m_pNewSetItemExplanation->ClosingProcess();
	m_pNewSetItemExplanation->Show(false);
	m_pNewQuickCommandWindow->ClosingProcess();
	m_pNewQuickCommandWindow->Show(false);
	m_pNewMoveCommandWindow->ClosingProcess();
	m_pNewMoveCommandWindow->Show(false);
	m_pNewDuelWindow->Show(false);
	m_pNewSiegeWarfare->ClosingProcess();
	m_pNewSiegeWarfare->Show(false);
	m_pNewItemEnduranceInfo->ClosingProcess();
	m_pNewItemEnduranceInfo->Show(false);
	m_pNewBuffWindow->ClosingProcess();
	m_pNewBuffWindow->Show(false);
	m_pNewCursedTempleEnterWindow->Show(false);
	m_pNewCursedTempleWindow->Show(false);
	m_pNewCursedTempleResultWindow->ClosingProcess();
	m_pNewCursedTempleResultWindow->Show(false);
	m_pNewCryWolfInterface->ClosingProcess();
	m_pNewCryWolfInterface->Show(false);
	m_pNewMaster_Level_Interface->ClosingProcess();
	m_pNewMaster_Level_Interface->Show(false);
	m_pNewGoldBowman->ClosingProcess();
	m_pNewGoldBowman->Show(false);
	m_pNewGoldBowmanLena->ClosingProcess();
	m_pNewGoldBowmanLena->Show(false);
	m_pNewLuckyCoinRegistration->ClosingProcess();
	m_pNewLuckyCoinRegistration->Show(false);
	m_pNewExchangeLuckyCoinWindow->ClosingProcess();
	m_pNewExchangeLuckyCoinWindow->Show(false);
	m_pNewDuelWatchWindow->ClosingProcess();
	m_pNewDuelWatchWindow->Show(false);
	m_pNewDuelWatchMainFrameWindow->ClosingProcess();
	m_pNewDuelWatchMainFrameWindow->Show(false);
	m_pNewDuelWatchUserListWindow->ClosingProcess();
	m_pNewDuelWatchUserListWindow->Show(false);
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewInGameShop->ClosingProcess();
	m_pNewInGameShop->Show(false);
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewDoppelGangerWindow->ClosingProcess();
	m_pNewDoppelGangerWindow->Show(false);
	m_pNewDoppelGangerFrame->ClosingProcess();
	m_pNewDoppelGangerFrame->Show(false);
	m_pNewQuestProgress->Show(false);
	m_pNewQuestProgressByEtc->Show(false);
	m_pNewEmpireGuardianNPC->ClosingProcess();
	m_pNewEmpireGuardianNPC->Show(false);
	m_pNewEmpireGuardianTimer->ClosingProcess();
	m_pNewEmpireGuardianTimer->Show(false);
	m_pNewNPCDialogue->Show(false);
	m_pNewMiniMap->ClosingProcess();
	m_pNewMiniMap->Show(false);
	m_pNewGensRanking->ClosingProcess();
	m_pNewGensRanking->Show(false);
	m_pNewCustomRanking->ClosingProcess();
	m_pNewCustomRanking->Show(false);
	m_pNewCustomEventTime->ClosingProcess();
	m_pNewCustomEventTime->Show(false);
	m_pNewCustomBuyVip->ClosingProcess();
	m_pNewCustomBuyVip->Show(false);
	m_pNewCustomArena->ClosingProcess();
	m_pNewCustomArena->Show(false);
	m_pNewUnitedMarketPlaceWindow->ClosingProcess();
	m_pNewUnitedMarketPlaceWindow->Show(false);
#ifdef LEM_ADD_LUCKYITEM
	m_pNewUILuckyItemWnd->ClosingProcess();
	m_pNewUILuckyItemWnd->Show(false);
#endif // LEM_ADD_LUCKYITEM
}

void CNewUISystem::HideAllGroupB()
{
	m_pNewChatInputBox->ClosingProcess();
	m_pNewChatInputBox->Show(false);
	m_pNewChatLogWindow->Show(false);
	m_pNewSlideWindow->Show(false);
	m_pNewGuildMakeWindow->ClosingProcess();
	m_pNewGuildMakeWindow->Show(false);
	m_pNewFriendWindow->Show(false);
	m_pNewMainFrameWindow->Show(false);
	m_pNewSkillList->Show(false);
	m_pNewMyInventory->ClosingProcess();
	m_pNewMyInventory->Show(false);
	m_pNewNPCShop->ClosingProcess();
	m_pNewNPCShop->Show(false);
	m_pNewPetInfoWindow->ClosingProcess();
	m_pNewPetInfoWindow->Show(false);
	m_pNewMixInventory->ClosingProcess();
	m_pNewMixInventory->Show(false);
	m_pNewCastleWindow->Show(false);
	m_pNewGuardWindow->ClosingProcess();
	m_pNewGuardWindow->Show(false);
	m_pNewGatemanWindow->ClosingProcess();
	m_pNewGatemanWindow->Show(false);
	m_pNewGateSwitchWindow->ClosingProcess();
	m_pNewGateSwitchWindow->Show(false);
	m_pNewStorageInventory->Show(false);
	m_pNewGuildInfoWindow->ClosingProcess();
	m_pNewGuildInfoWindow->Show(false);
	m_pNewMyShopInventory->ClosingProcess();
	m_pNewMyShopInventory->Show(false);
	m_pNewPurchaseShopInventory->ClosingProcess();
	m_pNewPurchaseShopInventory->Show(false);
	m_pNewCharacterInfoWindow->Show(false);
	m_pNewMyQuestInfoWindow->ClosingProcess();
	m_pNewMyQuestInfoWindow->Show(false);
	m_pNewPartyInfoWindow->ClosingProcess();
	m_pNewPartyInfoWindow->Show(false);
	m_pNewPartyListWindow->ClosingProcess();
	m_pNewPartyListWindow->Show(false);
	m_pNewNPCQuest->Show(false);
	m_pNewEnterBloodCastle->ClosingProcess();
	m_pNewEnterBloodCastle->Show(false);
	m_pNewEnterDevilSquare->ClosingProcess();
	m_pNewEnterDevilSquare->Show(false);
	m_pNewBloodCastle->ClosingProcess();
	m_pNewBloodCastle->Show(false);
	m_pNewTrade->Show(false);
	m_pNewKanturu2ndEnterNpc->Show(false);
	m_pNewKanturuInfoWindow->Show(false);
	m_pNewCatapultWindow->ClosingProcess();
	m_pNewCatapultWindow->Show(false);
	m_pNewChaosCastleTime->ClosingProcess();
	m_pNewChaosCastleTime->Show(false);
	m_pNewBattleSoccerScore->Show(false);
	m_pNewCommandWindow->ClosingProcess();
	m_pNewCommandWindow->Show(false);
	m_pNewWindowMenu->ClosingProcess();
	m_pNewWindowMenu->Show(false);
	m_pNewOptionWindow->ClosingProcess();
	m_pNewOptionWindow->Show(false);
	m_pNewHeroPositionInfo->ClosingProcess();
	m_pNewHeroPositionInfo->Show(false);
	m_pNewHelpWindow->ClosingProcess();
	m_pNewHelpWindow->Show(false);
	m_pNewItemExplanationWindow->ClosingProcess();
	m_pNewItemExplanationWindow->Show(false);
	m_pNewSetItemExplanation->ClosingProcess();
	m_pNewSetItemExplanation->Show(false);
	m_pNewQuickCommandWindow->ClosingProcess();
	m_pNewQuickCommandWindow->Show(false);
	m_pNewMoveCommandWindow->ClosingProcess();
	m_pNewMoveCommandWindow->Show(false);
	m_pNewDuelWindow->Show(false);
	m_pNewSiegeWarfare->ClosingProcess();
	m_pNewSiegeWarfare->Show(false);
	m_pNewItemEnduranceInfo->ClosingProcess();
	m_pNewItemEnduranceInfo->Show(false);
	m_pNewBuffWindow->ClosingProcess();
	m_pNewBuffWindow->Show(false);
	m_pNewCursedTempleEnterWindow->Show(false);
	m_pNewCursedTempleWindow->Show(false);
	m_pNewCursedTempleResultWindow->ClosingProcess();
	m_pNewCursedTempleResultWindow->Show(false);
	m_pNewCryWolfInterface->ClosingProcess();
	m_pNewCryWolfInterface->Show(false);
	m_pNewMaster_Level_Interface->ClosingProcess();
	m_pNewMaster_Level_Interface->Show(false);
	m_pNewGoldBowman->ClosingProcess();
	m_pNewGoldBowman->Show(false);
	m_pNewGoldBowmanLena->ClosingProcess();
	m_pNewGoldBowmanLena->Show(false);
	m_pNewLuckyCoinRegistration->ClosingProcess();
	m_pNewLuckyCoinRegistration->Show(false);
	m_pNewExchangeLuckyCoinWindow->ClosingProcess();
	m_pNewExchangeLuckyCoinWindow->Show(false);
	m_pNewDuelWatchWindow->ClosingProcess();
	m_pNewDuelWatchWindow->Show(false);
	m_pNewDuelWatchMainFrameWindow->ClosingProcess();
	m_pNewDuelWatchMainFrameWindow->Show(false);
	m_pNewDuelWatchUserListWindow->ClosingProcess();
	m_pNewDuelWatchUserListWindow->Show(false);
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewInGameShop->ClosingProcess();
	m_pNewInGameShop->Show(false);
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_pNewDoppelGangerWindow->ClosingProcess();
	m_pNewDoppelGangerWindow->Show(false);
	m_pNewDoppelGangerFrame->ClosingProcess();
	m_pNewDoppelGangerFrame->Show(false);
	m_pNewQuestProgress->Show(false);
	m_pNewQuestProgressByEtc->Show(false);
	m_pNewEmpireGuardianNPC->ClosingProcess();
	m_pNewEmpireGuardianNPC->Show(false);
	m_pNewEmpireGuardianTimer->ClosingProcess();
	m_pNewEmpireGuardianTimer->Show(false);
	m_pNewNPCDialogue->Show(false);
	m_pNewMiniMap->ClosingProcess();
	m_pNewMiniMap->Show(false);
	m_pNewGensRanking->ClosingProcess();
	m_pNewGensRanking->Show(false);
	m_pNewCustomRanking->ClosingProcess();
	m_pNewCustomRanking->Show(false);
	m_pNewCustomEventTime->ClosingProcess();
	m_pNewCustomEventTime->Show(false);
	m_pNewCustomBuyVip->ClosingProcess();
	m_pNewCustomBuyVip->Show(false);
	m_pNewCustomArena->ClosingProcess();
	m_pNewCustomArena->Show(false);
	m_pNewUnitedMarketPlaceWindow->ClosingProcess();
	m_pNewUnitedMarketPlaceWindow->Show(false);
#ifdef LEM_ADD_LUCKYITEM
	m_pNewUILuckyItemWnd->ClosingProcess();
	m_pNewUILuckyItemWnd->Show(false);
#endif // LEM_ADD_LUCKYITEM
}

void CNewUISystem::HideGroupBeforeOpenInterface()
{
	HideAllGroupA();
}

void CNewUISystem::Enable(DWORD dwKey)
{
	m_pNewUIMng->EnableInterface(dwKey);
}

void CNewUISystem::Disable(DWORD dwKey)
{
	m_pNewUIMng->EnableInterface(dwKey, false);
}

bool CNewUISystem::CheckMouseUse()
{
	return m_pNewUIMng->UpdateMouseEvent();
}

bool CNewUISystem::CheckKeyUse()
{
	return m_pNewUIMng->UpdateKeyEvent();
}

bool CNewUISystem::Update()
{
	return m_pNewUIMng->Update();
}

bool CNewUISystem::Render()
{
	return m_pNewUIMng->Render();
}

CNewUIManager* CNewUISystem::GetNewUIManager() const
{
	return m_pNewUIMng;
}

CNewUI3DRenderMng* CNewUISystem::GetNewUI3DRenderMng() const
{
	return m_pNewUI3DRenderMng;
}

CNewUIHotKey* CNewUISystem::GetNewUIHotKey() const
{
	return m_pNewUIHotKey;
}

bool CNewUISystem::IsImpossibleSendMoveInterface()
{
	return false;
}

bool CNewUISystem::IsImpossibleTradeInterface()
{
	return false;
}

bool CNewUISystem::IsImpossibleDuelInterface()
{
	return false;
}

bool CNewUISystem::IsImpossibleHideInterface(DWORD dwKey)
{
	return false;
}

void CNewUISystem::UpdateSendMoveInterface()
{
}

CNewUISystem* CNewUISystem::GetInstance()
{
	if(!m_pInstance) m_pInstance = new CNewUISystem;
	return m_pInstance;
}

CNewUIChatInputBox* CNewUISystem::GetUI_NewChatInputBox() const
{
	return m_pNewChatInputBox;
}

CNewUIChatLogWindow* CNewUISystem::GetUI_NewChatLogWindow() const
{
	return m_pNewChatLogWindow;
}

CNewUISlideWindow* CNewUISystem::GetUI_NewSlideWindow() const
{
	return m_pNewSlideWindow;
}

CNewUIGuildMakeWindow* CNewUISystem::GetUI_NewGuildMakeWindow() const
{
	return m_pNewGuildMakeWindow;
}

CNewUIFriendWindow* CNewUISystem::GetUI_NewFriendWindow() const
{
	return m_pNewFriendWindow;
}

CNewUIMainFrameWindow* CNewUISystem::GetUI_NewMainFrameWindow() const
{
	return m_pNewMainFrameWindow;
}

CNewUISkillList* CNewUISystem::GetUI_NewSkillList() const
{
	return m_pNewSkillList;
}

CNewUIItemMng* CNewUISystem::GetUI_NewItemMng() const
{
	return m_pNewItemMng;
}

CNewUIMyInventory* CNewUISystem::GetUI_NewMyInventory() const
{
	return m_pNewMyInventory;
}

CNewUINPCShop* CNewUISystem::GetUI_NewNpcShop() const
{
	return m_pNewNPCShop;
}

CNewUIPetInfoWindow* CNewUISystem::GetUI_NewPetInfoWindow() const
{
	return m_pNewPetInfoWindow;
}

CNewUIMixInventory* CNewUISystem::GetUI_NewMixInventory() const
{
	return m_pNewMixInventory;
}

CNewUICastleWindow* CNewUISystem::GetUI_NewCastleWindow() const
{
	return m_pNewCastleWindow;
}

CNewUIGuardWindow* CNewUISystem::GetUI_NewGuardWindow() const
{
	return m_pNewGuardWindow;
}

CNewUIGatemanWindow* CNewUISystem::GetUI_NewGatemanWindow() const
{
	return m_pNewGatemanWindow;
}

CNewUIGateSwitchWindow* CNewUISystem::GetUI_NewGateSwitchWindow() const
{
	return m_pNewGateSwitchWindow;
}

CNewUIStorageInventory* CNewUISystem::GetUI_NewStorageInventory() const
{
	return m_pNewStorageInventory;
}

CNewUIGuildInfoWindow* CNewUISystem::GetUI_NewGuildInfoWindow() const
{
	return m_pNewGuildInfoWindow;
}

CNewUIMyShopInventory* CNewUISystem::GetUI_NewMyShopInventory() const
{
	return m_pNewMyShopInventory;
}

CNewUIPurchaseShopInventory* CNewUISystem::GetUI_NewPurchaseShopInventory() const
{
	return m_pNewPurchaseShopInventory;
}

CNewUICharacterInfoWindow* CNewUISystem::GetUI_NewCharacterInfoWindow() const
{
	return m_pNewCharacterInfoWindow;
}

CNewUIMyQuestInfoWindow* CNewUISystem::GetUI_NewMyQuestInfoWindow() const
{
	return m_pNewMyQuestInfoWindow;
}

CNewUIPartyInfoWindow* CNewUISystem::GetUI_NewPartyInfoWindow() const
{
	return m_pNewPartyInfoWindow;
}

CNewUIPartyListWindow* CNewUISystem::GetUI_NewPartyListWindow() const
{
	return m_pNewPartyListWindow;
}

CNewUINPCQuest* CNewUISystem::GetUI_NewNPCQuest() const
{
	return m_pNewNPCQuest;
}

CNewUIEnterBloodCastle* CNewUISystem::GetUI_NewEnterBloodCastle() const
{
	return m_pNewEnterBloodCastle;
}

CNewUIEnterDevilSquare* CNewUISystem::GetUI_NewEnterDevilSquare() const
{
	return m_pNewEnterDevilSquare;
}

CNewUIBloodCastle* CNewUISystem::GetUI_NewBloodCastle() const
{
	return m_pNewBloodCastle;
}

CNewUITrade* CNewUISystem::GetUI_NewTrade() const
{
	return m_pNewTrade;
}

CNewUIKanturu2ndEnterNpc* CNewUISystem::GetUI_NewKanturu2ndEnterNpc() const
{
	return m_pNewKanturu2ndEnterNpc;
}

CNewUIKanturuInfoWindow* CNewUISystem::GetUI_NewKanturuInfoWindow() const
{
	return m_pNewKanturuInfoWindow;
}

CNewUICatapultWindow* CNewUISystem::GetUI_NewCatapultWindow() const
{
	return m_pNewCatapultWindow;
}

CNewUIChaosCastleTime* CNewUISystem::GetUI_NewChaosCastleTime() const
{
	return m_pNewChaosCastleTime;
}

CNewUIBattleSoccerScore* CNewUISystem::GetUI_NewBattleSoccerScore() const
{
	return m_pNewBattleSoccerScore;
}

CNewUICommandWindow* CNewUISystem::GetUI_NewCommandWindow() const
{
	return m_pNewCommandWindow;
}

CNewUIWindowMenu* CNewUISystem::GetUI_NewWindowMenu() const
{
	return m_pNewWindowMenu;
}

CNewUIOptionWindow* CNewUISystem::GetUI_NewOptionWindow() const
{
	return m_pNewOptionWindow;
}

CNewUIHeroPositionInfo* CNewUISystem::GetUI_NewHeroPositionInfo() const
{
	return m_pNewHeroPositionInfo;
}

CNewUIHelpWindow* CNewUISystem::GetUI_NewHelpWindow() const
{
	return m_pNewHelpWindow;
}

CNewUIItemExplanationWindow* CNewUISystem::GetUI_NewItemExplanationWindow() const
{
	return m_pNewItemExplanationWindow;
}

CNewUISetItemExplanation* CNewUISystem::GetUI_NewSetItemExplanation() const
{
	return m_pNewSetItemExplanation;
}

CNewUIQuickCommandWindow* CNewUISystem::GetUI_NewQuickCommandWindow() const
{
	return m_pNewQuickCommandWindow;
}

CNewUIMoveCommandWindow* CNewUISystem::GetUI_NewMoveCommandWindow() const
{
	return m_pNewMoveCommandWindow;
}

CNewUIDuelWindow* CNewUISystem::GetUI_NewDuelWindow() const
{
	return m_pNewDuelWindow;
}

CNewUISiegeWarfare* CNewUISystem::GetUI_NewSiegeWarfare() const
{
	return m_pNewSiegeWarfare;
}

CNewUIItemEnduranceInfo* CNewUISystem::GetUI_NewItemEnduranceInfo() const
{
	return m_pNewItemEnduranceInfo;
}

CNewUIBuffWindow* CNewUISystem::GetUI_NewBuffWindow() const
{
	return m_pNewBuffWindow;
}

CNewUICursedTempleEnter* CNewUISystem::GetUI_NewCursedTempleEnterWindow() const
{
	return m_pNewCursedTempleEnterWindow;
}

CNewUICursedTempleSystem* CNewUISystem::GetUI_NewCursedTempleWindow() const
{
	return m_pNewCursedTempleWindow;
}

CNewUICursedTempleResult* CNewUISystem::GetUI_NewCursedTempleResultWindow() const
{
	return m_pNewCursedTempleResultWindow;
}

CNewUICryWolf* CNewUISystem::GetUI_NewCryWolfInterface() const
{
	return m_pNewCryWolfInterface;
}

CNewUIMasterLevel* CNewUISystem::GetUI_NewMasterLevelInterface() const
{
	return m_pNewMaster_Level_Interface;
}

CNewUIGoldBowmanWindow* CNewUISystem::GetUI_pNewGoldBowman() const
{
	return m_pNewGoldBowman;
}

CNewUIGoldBowmanLena* CNewUISystem::GetUI_pNewGoldBowmanLena() const
{
	return m_pNewGoldBowmanLena;
}

CNewUIRegistrationLuckyCoin* CNewUISystem::GetUI_pNewLuckyCoinRegistration() const
{
	return m_pNewLuckyCoinRegistration;
}

CNewUIExchangeLuckyCoin* CNewUISystem::GetUI_pNewExchangeLuckyCoin() const
{
	return m_pNewExchangeLuckyCoinWindow;
}

CNewUIDuelWatchWindow* CNewUISystem::GetUI_pNewDuelWatch() const
{
	return m_pNewDuelWatchWindow;
}

CNewUIDuelWatchMainFrameWindow* CNewUISystem::GetUI_pNewDuelWatchMainFrame() const
{
	return m_pNewDuelWatchMainFrameWindow;
}

CNewUIDuelWatchUserListWindow* CNewUISystem::GetUI_pNewDuelWatchUserList() const
{
	return m_pNewDuelWatchUserListWindow;
}

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
CNewUIInGameShop* CNewUISystem::GetUI_pNewInGameShop() const
{
	return m_pNewInGameShop;
}
#endif // PBG_ADD_INGAMESHOP_UI_MAINFRAME

CNewUIDoppelGangerWindow* CNewUISystem::GetUI_pNewDoppelGangerWindow() const
{
	return m_pNewDoppelGangerWindow;
}

CNewUIDoppelGangerFrame* CNewUISystem::GetUI_pNewDoppelGangerFrame() const
{
	return m_pNewDoppelGangerFrame;
}

CNewUIQuestProgress* CNewUISystem::GetUI_NewQuestProgress() const
{
	return m_pNewQuestProgress;
}

CNewUIQuestProgressByEtc* CNewUISystem::GetUI_NewQuestProgressByEtc() const
{
	return m_pNewQuestProgressByEtc;
}

CNewUIEmpireGuardianNPC* CNewUISystem::GetUI_pNewEmpireGuardianNPC() const
{
	return m_pNewEmpireGuardianNPC;
}

CNewUIEmpireGuardianTimer* CNewUISystem::GetUI_pNewEmpireGuardianTimer() const
{
	return m_pNewEmpireGuardianTimer;
}

CNewUINPCDialogue* CNewUISystem::GetUI_NewNPCDialogue() const
{
	return m_pNewNPCDialogue;
}

CNewUIMiniMap* CNewUISystem::GetUI_pNewUIMiniMap() const
{
	return m_pNewMiniMap;
}

CNewUIGensRanking* CNewUISystem::GetUI_NewGensRanking() const
{
	return m_pNewGensRanking;
}

CNewUICustomRanking* CNewUISystem::GetUI_NewCustomRanking() const
{
	return m_pNewCustomRanking;
}

CNewUICustomEventTime* CNewUISystem::GetUI_NewCustomEventTime() const
{
	return m_pNewCustomEventTime;
}

CNewUICustomArena* CNewUISystem::GetUI_NewCustomArena() const
{
	return m_pNewCustomArena;
}

CNewUICustomBuyVip* CNewUISystem::GetUI_NewCustomBuyVip() const
{
	return m_pNewCustomBuyVip;
}

CNewUIUnitedMarketPlaceWindow* CNewUISystem::GetUI_pNewUnitedMarketPlaceWindow() const
{
	return m_pNewUnitedMarketPlaceWindow;
}

#ifdef LEM_ADD_LUCKYITEM
CNewUILuckyItemWnd* CNewUISystem::Get_pNewUILuckyItemWnd() const
{
	return m_pNewUILuckyItemWnd;
}
#endif // LEM_ADD_LUCKYITEM
