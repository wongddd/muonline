#include "stdafx.h"
#include "NewUICustomArena.h"
#include "NewUISystem.h"
#include "WSclient.h"
#include "wsclientinline.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "./Utilities/Log/ErrorReport.h"
#include "Input.h"

using namespace SEASON3B;

CNewUICustomArena::CNewUICustomArena()
{
    m_pNewUIMng = NULL;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_iArenaCount = 0;
    m_iScoreCount = 0;
    m_iScoreArenaIndex = -1;
    memset(m_ArenaEntries, 0, sizeof(m_ArenaEntries));
    memset(m_ScoreEntries, 0, sizeof(m_ScoreEntries));
    m_dwLastRequestTick = 0;
    m_iScrollOffset = 0;
}

CNewUICustomArena::~CNewUICustomArena()
{
    Release();
}

bool CNewUICustomArena::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_CUSTOM_ARENA, this);
    SetPos(x, y);
    LoadImages();
    Show(false);
    return true;
}

void CNewUICustomArena::Release()
{
    UnloadImages();
    if (m_pNewUIMng) { m_pNewUIMng->RemoveUIObj(this); m_pNewUIMng = NULL; }
}

void CNewUICustomArena::LoadImages()
{
    LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_BACK, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_close.tga", IMAGE_CLOSE, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_empty_very_small.tga", IMAGE_BTN, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_empty_very_small.tga", IMAGE_DOT, GL_NEAREST, GL_CLAMP, 1);
}

void CNewUICustomArena::UnloadImages()
{
    DeleteBitmap(IMAGE_BACK);
    DeleteBitmap(IMAGE_CLOSE);
    DeleteBitmap(IMAGE_BTN);
    DeleteBitmap(IMAGE_DOT);
}

void CNewUICustomArena::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
    m_iScrollOffset = 0;
}

bool CNewUICustomArena::Render()
{
    if (!IsVisible()) return true;

    EnableAlphaTest();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    RenderFrame();
    RenderTexts();

    int startIdx = m_iScrollOffset;
    int endIdx = min(m_iArenaCount, startIdx + MAX_VISIBLE_ENTRIES);

    for (int i = startIdx; i < endIdx; i++)
    {
        RenderArenaEntry(i - startIdx, m_ArenaEntries[i]);
    }

    // Render scroll indicators if needed
    if (m_iScrollOffset > 0)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(150, 150, 150, 255);
        g_pRenderText->SetBgColor(0, 0, 0, 0);
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 42, "^^ More ^^", WND_WIDTH, 0, RT3_SORT_CENTER);
    }
    if (m_iArenaCount > m_iScrollOffset + MAX_VISIBLE_ENTRIES)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(150, 150, 150, 255);
        g_pRenderText->SetBgColor(0, 0, 0, 0);
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + WND_HEIGHT - 20, "vv More vv", WND_WIDTH, 0, RT3_SORT_CENTER);
    }

    // Render scoreboard overlay if we have score data
    if (m_iScoreCount > 0 && m_iScoreArenaIndex >= 0)
    {
        RenderScoreboard();
    }

    DisableAlphaBlend();
    return true;
}

void CNewUICustomArena::RenderFrame()
{
    RenderBitmap(IMAGE_BACK, m_Pos.x, m_Pos.y, WND_WIDTH, WND_HEIGHT,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);
}

void CNewUICustomArena::RenderTexts()
{
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 200, 100, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 10, "Custom Arena", WND_WIDTH - 20, 0, RT3_SORT_CENTER);

    // Column headers
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(180, 180, 180, 255);
    g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 44, "State", 60, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(m_Pos.x + 70, m_Pos.y + 44, "Name", 120, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(m_Pos.x + 190, m_Pos.y + 44, "Time", 60, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(m_Pos.x + 250, m_Pos.y + 44, "Players", 60, 0, RT3_SORT_LEFT);

    if (m_iArenaCount <= 0)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(200, 200, 200, 255);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 120, "No active arenas...", WND_WIDTH - 20, 0, RT3_SORT_CENTER);
    }
}

void CNewUICustomArena::RenderArenaEntry(int idx, const ARENA_ENTRY& entry)
{
    int y = m_Pos.y + 62 + (idx * ENTRY_HEIGHT);
    int baseY = y;

    // Background stripe
    g_pRenderText->SetBgColor(0, 0, 0, 80);
    RenderBitmap(IMAGE_DOT, m_Pos.x + 10, y, WND_WIDTH - 20, ENTRY_HEIGHT - 2,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);

    // State text color
    BYTE r = 180, g = 180, b = 180; // gray for default
    switch (entry.State)
    {
    case 1: r = 180; g = 180; b = 180; break; // EMPTY - gray
    case 2: r = 255; g = 220; b = 80; break;  // STAND - yellow
    case 3: r = 80; g = 255; b = 80; break;   // START - green
    case 4: r = 255; g = 80; b = 80; break;   // CLEAN - red
    }

    // State
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(r, g, b, 255);
    g_pRenderText->RenderText(m_Pos.x + 15, baseY + 2, GetStateText(entry.State), 55, 0, RT3_SORT_LEFT);

    // Name
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 200, 100, 255);
    g_pRenderText->RenderText(m_Pos.x + 70, baseY + 2, entry.Name, 115, 0, RT3_SORT_LEFT);

    // Time
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(180, 220, 255, 255);
    char szTime[32];
    if (entry.RemainTime <= 0 && entry.State == 1)
        sprintf(szTime, "--:--");
    else
        sprintf(szTime, "%d:%02d", entry.RemainTime / 60, entry.RemainTime % 60);
    g_pRenderText->RenderText(m_Pos.x + 190, baseY + 2, szTime, 60, 0, RT3_SORT_LEFT);

    // Players
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(220, 220, 180, 255);
    char szPlayers[32];
    sprintf(szPlayers, "%d / %d", entry.UserCount, entry.MaxUsers);
    g_pRenderText->RenderText(m_Pos.x + 250, baseY + 2, szPlayers, 55, 0, RT3_SORT_LEFT);

    // Join button
    RenderJoinButton(idx);
}

void CNewUICustomArena::RenderJoinButton(int idx)
{
    int y = m_Pos.y + 62 + (idx * ENTRY_HEIGHT);
    RenderBitmap(IMAGE_BTN, m_Pos.x + WND_WIDTH - 80, y + 15, 60, 20,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(255, 255, 255, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + WND_WIDTH - 80, y + 16, "Join", 60, 0, RT3_SORT_CENTER);
}

void CNewUICustomArena::RenderScoreboard()
{
    CInput& rInput = CInput::Instance();
    int sx = (rInput.GetScreenWidth() - SCORE_WND_WIDTH) / 2;
    int sy = (rInput.GetScreenHeight() - SCORE_WND_HEIGHT) / 2;

    RenderBitmap(IMAGE_BACK, sx, sy, SCORE_WND_WIDTH, SCORE_WND_HEIGHT,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);

    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 200, 100, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(sx, sy + 8, "Arena Scoreboard", SCORE_WND_WIDTH, 0, RT3_SORT_CENTER);

    // Column headers
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(180, 180, 180, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(sx + 8, sy + 32, "Rank", 35, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(sx + 45, sy + 32, "Name", 70, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(sx + 125, sy + 32, "Kill", 35, 0, RT3_SORT_CENTER);
    g_pRenderText->RenderText(sx + 160, sy + 32, "Death", 40, 0, RT3_SORT_CENTER);
    g_pRenderText->RenderText(sx + 200, sy + 32, "Score", 45, 0, RT3_SORT_CENTER);

    int visCount = min(m_iScoreCount, MAX_VISIBLE_SCORE);
    for (int i = 0; i < visCount; i++)
    {
        RenderScoreEntry(i, m_ScoreEntries[i]);
    }

    if (m_iScoreCount > MAX_VISIBLE_SCORE)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(150, 150, 150, 255);
        g_pRenderText->RenderText(sx, sy + SCORE_WND_HEIGHT - 20, "... and more", SCORE_WND_WIDTH, 0, RT3_SORT_CENTER);
    }
}

void CNewUICustomArena::RenderScoreEntry(int idx, const SCORE_ENTRY& entry)
{
    CInput& rInput = CInput::Instance();
    int sx = (rInput.GetScreenWidth() - SCORE_WND_WIDTH) / 2;
    int sy = (rInput.GetScreenHeight() - SCORE_WND_HEIGHT) / 2;
    int y = sy + 52 + (idx * SCORE_ENTRY_HEIGHT);

    // Highlight own entry
    if (strncmp(entry.Name, Hero->ID, 10) == 0)
    {
        g_pRenderText->SetBgColor(0, 60, 120, 160);
    }
    else
    {
        g_pRenderText->SetBgColor(0, 0, 0, 60);
    }

    // Background bar
    RenderBitmap(IMAGE_DOT, sx + 5, y, SCORE_WND_WIDTH - 10, SCORE_ENTRY_HEIGHT - 1,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);

    g_pRenderText->SetFont(g_hFont);
    BYTE rankColor = (entry.Rank == 1) ? 255 : 220;
    BYTE rankG = (entry.Rank == 1) ? 220 : 220;
    BYTE rankB = (entry.Rank == 1) ? 80 : 220;
    g_pRenderText->SetTextColor(rankColor, rankG, rankB, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);

    char szRank[8];
    sprintf(szRank, "#%d", entry.Rank);
    g_pRenderText->RenderText(sx + 8, y, szRank, 35, 0, RT3_SORT_LEFT);

    g_pRenderText->SetTextColor(200, 200, 200, 255);
    g_pRenderText->RenderText(sx + 45, y, entry.Name, 70, 0, RT3_SORT_LEFT);

    g_pRenderText->SetTextColor(255, 100, 100, 255);
    char szVal[16];
    sprintf(szVal, "%d", entry.KillCount);
    g_pRenderText->RenderText(sx + 125, y, szVal, 35, 0, RT3_SORT_CENTER);

    g_pRenderText->SetTextColor(150, 150, 150, 255);
    sprintf(szVal, "%d", entry.DeathCount);
    g_pRenderText->RenderText(sx + 160, y, szVal, 40, 0, RT3_SORT_CENTER);

    g_pRenderText->SetTextColor(100, 200, 255, 255);
    sprintf(szVal, "%d", entry.Score);
    g_pRenderText->RenderText(sx + 200, y, szVal, 45, 0, RT3_SORT_CENTER);
}

const char* CNewUICustomArena::GetStateText(int state)
{
    switch (state)
    {
    case 1: return "Waiting";
    case 2: return "Standby";
    case 3: return "Active";
    case 4: return "Ending";
    default: return "Off";
    }
}

bool CNewUICustomArena::Update()
{
    if (!IsVisible()) return true;

    // Auto-refresh every REFRESH_INTERVAL ms
    DWORD tick = GetTickCount();
    if (tick - m_dwLastRequestTick >= REFRESH_INTERVAL)
    {
        m_dwLastRequestTick = tick;
        RequestArenaList();
    }

    return true;
}

bool CNewUICustomArena::UpdateMouseEvent()
{
    if (!IsVisible()) return true;

    if (CheckMouseIn(m_Pos.x, m_Pos.y, WND_WIDTH, WND_HEIGHT))
    {
        if (IsRelease(VK_RBUTTON))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_ARENA);
            return false;
        }

        // Scroll up/down keys
        if (SEASON3B::IsPress(VK_UP))
        {
            if (m_iScrollOffset > 0) m_iScrollOffset--;
            return false;
        }
        if (SEASON3B::IsPress(VK_DOWN))
        {
            if (m_iScrollOffset + MAX_VISIBLE_ENTRIES < m_iArenaCount) m_iScrollOffset++;
            return false;
        }

        if (IsPress(VK_LBUTTON))
        {
            int startIdx = m_iScrollOffset;
            int endIdx = min(m_iArenaCount, startIdx + MAX_VISIBLE_ENTRIES);

            for (int i = startIdx; i < endIdx; i++)
            {
                int y = m_Pos.y + 62 + ((i - startIdx) * ENTRY_HEIGHT);
                if (CheckMouseIn(m_Pos.x + WND_WIDTH - 80, y + 15, 60, 20))
                {
                    RequestJoinArena(m_ArenaEntries[i].Index);
                    return false;
                }
            }
            return false;
        }
    }

    return true;
}

bool CNewUICustomArena::UpdateKeyEvent()
{
    if (IsVisible())
    {
        if (IsPress(VK_ESCAPE))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_ARENA);
            PlayBuffer(SOUND_CLICK01);
            return false;
        }
    }
    return true;
}

void CNewUICustomArena::OpenningProcess()
{
    m_iScrollOffset = 0;
    m_iScoreCount = 0;
    m_iScoreArenaIndex = -1;
    m_dwLastRequestTick = 0;
    RequestArenaList();
}

void CNewUICustomArena::ClosingProcess()
{
    m_iArenaCount = 0;
    m_iScoreCount = 0;
    m_iScoreArenaIndex = -1;
}

void CNewUICustomArena::RequestArenaList()
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xE9;
    spe.Send();
}

void CNewUICustomArena::RequestJoinArena(int arenaIndex)
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xEC;
    spe << (int)arenaIndex;
    spe.Send();
}

void CNewUICustomArena::ReceiveArenaList(BYTE* pBuffer)
{
    if (pBuffer[0] != 0xC1) return;
    if (pBuffer[2] != 0xEE) return;

    int count = *(int*)(pBuffer + 3);
    m_iArenaCount = (count > MAX_ARENA_DISPLAY) ? MAX_ARENA_DISPLAY : count;

    int ofs = 7;
    for (int i = 0; i < m_iArenaCount; i++)
    {
        if (ofs + (int)sizeof(PMSG_CUSTOM_ARENA_ENTRY) > (int)pBuffer[1]) break;
        PMSG_CUSTOM_ARENA_ENTRY* pEntry = (PMSG_CUSTOM_ARENA_ENTRY*)(pBuffer + ofs);
        m_ArenaEntries[i].Index = pEntry->Index;
        m_ArenaEntries[i].State = pEntry->State;
        m_ArenaEntries[i].RemainTime = pEntry->RemainTime;
        m_ArenaEntries[i].UserCount = pEntry->UserCount;
        m_ArenaEntries[i].MaxUsers = pEntry->MaxUsers;
        m_ArenaEntries[i].EnterEnabled = pEntry->EnterEnabled;
        memcpy(m_ArenaEntries[i].Name, pEntry->Name, 32);
        ofs += sizeof(PMSG_CUSTOM_ARENA_ENTRY);
    }
}

void CNewUICustomArena::ReceiveScoreboard(BYTE* pBuffer)
{
    if (pBuffer[0] != 0xC1) return;
    if (pBuffer[2] != 0xEF) return;

    m_iScoreArenaIndex = *(int*)(pBuffer + 3);
    int count = *(int*)(pBuffer + 7);
    m_iScoreCount = (count > MAX_CUSTOM_ARENA_USER) ? MAX_CUSTOM_ARENA_USER : count;

    int ofs = 11;
    for (int i = 0; i < m_iScoreCount; i++)
    {
        if (ofs + (int)sizeof(PMSG_CUSTOM_ARENA_SCORE_ENTRY) > (int)pBuffer[1]) break;
        PMSG_CUSTOM_ARENA_SCORE_ENTRY* pEntry = (PMSG_CUSTOM_ARENA_SCORE_ENTRY*)(pBuffer + ofs);
        m_ScoreEntries[i].Rank = pEntry->Rank;
        memcpy(m_ScoreEntries[i].Name, pEntry->Name, 10);
        m_ScoreEntries[i].KillCount = pEntry->KillCount;
        m_ScoreEntries[i].DeathCount = pEntry->DeathCount;
        m_ScoreEntries[i].Score = pEntry->Score;
        ofs += sizeof(PMSG_CUSTOM_ARENA_SCORE_ENTRY);
    }
}
