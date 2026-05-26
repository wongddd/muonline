#include "stdafx.h"
#include "NewUICustomRanking.h"
#include "NewUISystem.h"
#include "WSclient.h"
#include "wsclientinline.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "./Utilities/Log/ErrorReport.h"

using namespace SEASON3B;

enum { IMAGE_CUSTOM_RANKING_BACK = 31990, IMAGE_CUSTOM_RANKING_CLOSE = 31991, IMAGE_CUSTOM_RANKING_MINUS = 31992 };

CNewUICustomRanking::CNewUICustomRanking()
{
    m_pNewUIMng = NULL;
    m_pBtnClose = NULL;
    m_pBtnRefresh = NULL;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_iCategoryCount = 0;
    m_iSelectedCategory = 0;
    m_iEntryCount = 0;
    memset(m_Categories, 0, sizeof(m_Categories));
    memset(m_Entries, 0, sizeof(m_Entries));
}

CNewUICustomRanking::~CNewUICustomRanking()
{
    Release();
}

bool CNewUICustomRanking::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_CUSTOM_RANKING, this);
    SetPos(x, y);
    LoadImages();
    Show(false);
    return true;
}

void CNewUICustomRanking::Release()
{
    UnloadImages();
    if (m_pBtnClose) { delete m_pBtnClose; m_pBtnClose = NULL; }
    if (m_pBtnRefresh) { delete m_pBtnRefresh; m_pBtnRefresh = NULL; }
    if (m_pNewUIMng) { m_pNewUIMng->RemoveUIObj(this); m_pNewUIMng = NULL; }
}

void CNewUICustomRanking::LoadImages()
{
    LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_CUSTOM_RANKING_BACK, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_close.tga", IMAGE_CUSTOM_RANKING_CLOSE, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_minus.tga", IMAGE_CUSTOM_RANKING_MINUS, GL_NEAREST, GL_CLAMP, 1);
}

void CNewUICustomRanking::UnloadImages()
{
    DeleteBitmap(IMAGE_CUSTOM_RANKING_BACK);
    DeleteBitmap(IMAGE_CUSTOM_RANKING_CLOSE);
    DeleteBitmap(IMAGE_CUSTOM_RANKING_MINUS);
}

void CNewUICustomRanking::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

bool CNewUICustomRanking::Render()
{
    if (!IsVisible()) return true;

    EnableAlphaTest();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    RenderFrame();
    RenderTexts();
    RenderCategoryButtons();
    RenderList();

    DisableAlphaBlend();
    return true;
}

void CNewUICustomRanking::RenderFrame()
{
    float fWidth = RANKING_WIDTH;
    float fHeight = RANKING_HEIGHT;

    RenderBitmap(IMAGE_CUSTOM_RANKING_BACK, m_Pos.x, m_Pos.y, fWidth, fHeight, 0.0f, 0.0f, 1.0f, 1.0f, true, NULL);
}

void CNewUICustomRanking::RenderTexts()
{
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 255, 200, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 10, "Custom Ranking", RANKING_WIDTH - 20, 0, RT3_SORT_CENTER);

    if (m_iEntryCount > 0 && m_iSelectedCategory < m_iCategoryCount)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(180, 180, 200, 255);
        char szTitle[64];
        sprintf(szTitle, "%s (%s / %s)",
            m_Categories[m_iSelectedCategory].szName,
            m_Categories[m_iSelectedCategory].szCol1,
            m_Categories[m_iSelectedCategory].szCol2);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 40, szTitle, RANKING_WIDTH - 20, 0, RT3_SORT_CENTER);
    }

    if (m_iCategoryCount == 0)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(200, 200, 200, 255);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 100, "Press 'Refresh' to load ranking data", RANKING_WIDTH - 20, 0, RT3_SORT_CENTER);
    }
}

void CNewUICustomRanking::RenderCategoryButtons()
{
    if (m_iCategoryCount <= 0) return;

    int btnY = m_Pos.y + 65;
    for (int i = 0; i < m_iCategoryCount && i < 10; i++)
    {
        int color = (i == m_iSelectedCategory) ? 255 : 180;
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(255, color, color, 255);
        g_pRenderText->SetBgColor(0, 0, 0, 0);
        g_pRenderText->RenderText(m_Pos.x + 10, btnY, m_Categories[i].szName, 80, 0, RT3_SORT_LEFT);
        btnY += 18;
    }
}

void CNewUICustomRanking::RenderList()
{
    if (m_iEntryCount <= 0) return;

    int listY = m_Pos.y + 80;
    int startIdx = m_iSelectedCategory * MAX_DISPLAY_ITEMS;

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(200, 200, 255, 255);
    g_pRenderText->RenderText(m_Pos.x + 110, m_Pos.y + 65, "Name", 80, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(m_Pos.x + 190, m_Pos.y + 65, "Value", 60, 0, RT3_SORT_CENTER);

    int displayCount = (m_iEntryCount > MAX_DISPLAY_ITEMS) ? MAX_DISPLAY_ITEMS : m_iEntryCount;
    for (int i = 0; i < displayCount; i++)
    {
        int y = m_Pos.y + 85 + (i * 16);
        g_pRenderText->SetTextColor(220, 220, 220, 255);
        g_pRenderText->RenderText(m_Pos.x + 110, y, m_Entries[i].szName, 80, 0, RT3_SORT_LEFT);
        char szVal[16];
        sprintf(szVal, "%d", m_Entries[i].iValue);
        g_pRenderText->RenderText(m_Pos.x + 190, y, szVal, 60, 0, RT3_SORT_CENTER);
    }
}

bool CNewUICustomRanking::Update()
{
    if (!IsVisible()) return true;
    return true;
}

bool CNewUICustomRanking::UpdateMouseEvent()
{
    if (!g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_RANKING))
        return true;

    if (BtnProcess()) return false;

    if (CheckMouseIn(m_Pos.x, m_Pos.y, RANKING_WIDTH, RANKING_HEIGHT))
    {
        if (SEASON3B::IsPress(VK_RBUTTON))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_RANKING);
            return false;
        }
        if (!SEASON3B::IsNone(VK_LBUTTON))
        {
            int mouseY = MouseY;
            if (mouseY >= m_Pos.y + 65 && mouseY < m_Pos.y + 65 + m_iCategoryCount * 18)
            {
                int catIdx = (mouseY - (m_Pos.y + 65)) / 18;
                if (catIdx >= 0 && catIdx < m_iCategoryCount && catIdx != m_iSelectedCategory)
                {
                    m_iSelectedCategory = catIdx;
                    RequestRankingData(catIdx);
                }
            }
            return false;
        }
    }

    if (CheckMouseIn(m_Pos.x, m_Pos.y, RANKING_WIDTH, RANKING_HEIGHT))
    {
        if (!SEASON3B::IsNone(VK_LBUTTON))
            return false;
    }

    return true;
}

bool CNewUICustomRanking::UpdateKeyEvent()
{
    if (g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_RANKING))
    {
        if (SEASON3B::IsPress(VK_ESCAPE))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_RANKING);
            PlayBuffer(SOUND_CLICK01);
            return false;
        }
    }
    return true;
}

bool CNewUICustomRanking::BtnProcess()
{
    return false;
}

void CNewUICustomRanking::OpenningProcess()
{
    RequestRankingCount();
}

void CNewUICustomRanking::ClosingProcess()
{
    m_iEntryCount = 0;
}

void CNewUICustomRanking::RequestRankingCount()
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xE7;
    spe.Send();
}

void CNewUICustomRanking::RequestRankingData(int type)
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xE6;
    spe << (BYTE)type;
    spe.Send();
}

void CNewUICustomRanking::ReceiveRankingCount(int count)
{
    m_iCategoryCount = (count > 10) ? 10 : count;
    if (m_iCategoryCount > 0)
    {
        m_iSelectedCategory = 0;
        RequestRankingData(0);
    }
}

bool CNewUICustomRanking::ReceiveRankingData(BYTE* pBuffer)
{
    if (pBuffer[0] != 0xC2) return false;

    int totalSize = (pBuffer[1] << 8) | pBuffer[2];
    int ofs = 5;

    int index = *(int*)(pBuffer + ofs); ofs += 4;
    char rankname[20]; memcpy(rankname, pBuffer + ofs, 20); ofs += 20;
    char col1[20]; memcpy(col1, pBuffer + ofs, 20); ofs += 20;
    char col2[20]; memcpy(col2, pBuffer + ofs, 20); ofs += 20;
    int count = *(int*)(pBuffer + ofs); ofs += 4;

    if (index >= 0 && index < 10)
    {
        memcpy(m_Categories[index].szName, rankname, 20);
        memcpy(m_Categories[index].szCol1, col1, 20);
        memcpy(m_Categories[index].szCol2, col2, 20);
    }

    m_iEntryCount = (count > 50) ? 50 : count;
    for (int n = 0; n < m_iEntryCount; n++)
    {
        if (ofs + 24 > totalSize) break;
        memcpy(m_Entries[n].szName, pBuffer + ofs, 20); ofs += 20;
        m_Entries[n].iValue = *(int*)(pBuffer + ofs); ofs += 4;
    }

    return true;
}
