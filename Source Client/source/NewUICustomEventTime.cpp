#include "stdafx.h"
#include "NewUICustomEventTime.h"
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

enum { IMAGE_CUSTOM_EVENTTIME_BACK = 31993 };

static const char* g_szEventNames[42] =
{
    "Blood Castle", "Devil Square", "Chaos Castle", "Illusion Temple",
    "Doppelganger", "Empire Guardian", "Crywolf", "White Wizard",
    "Golden Dragon", "Golden Goer", "Golden Devil", "Golden Sorcerer",
    "Red Dragon", "Blue Dragon", "Black Dragon", "Red Goer",
    "Blue Goer", "Black Goer", "Hell Main", "Hell Mission",
    "Medusa", "Kundun", "Selupan", "Nightmare",
    "Acheron Guard", "Acheron Berserker", "Acheron Mage", "Acheron Archer",
    "Bokung", "Maya", "Alice", "Great Dragon",
    "Arena 1", "Arena 2", "Arena 3", "Arena 4",
    "Arena 5", "Arena 6", "Arena 7", "Arena 8",
    "Arena 9", "Arena 10"
};

CNewUICustomEventTime::CNewUICustomEventTime()
{
    m_pNewUIMng = NULL;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_iEventCount = 0;
    m_bHasData = false;
    m_iScrollOffset = 0;
    memset(m_Events, 0, sizeof(m_Events));
}

CNewUICustomEventTime::~CNewUICustomEventTime()
{
    Release();
}

bool CNewUICustomEventTime::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_CUSTOM_EVENTTIME, this);
    SetPos(x, y);
    LoadImages();
    Show(false);
    return true;
}

void CNewUICustomEventTime::Release()
{
    UnloadImages();
    if (m_pNewUIMng) { m_pNewUIMng->RemoveUIObj(this); m_pNewUIMng = NULL; }
}

void CNewUICustomEventTime::LoadImages()
{
    LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_CUSTOM_EVENTTIME_BACK, GL_NEAREST, GL_CLAMP, 1);
}

void CNewUICustomEventTime::UnloadImages()
{
    DeleteBitmap(IMAGE_CUSTOM_EVENTTIME_BACK);
}

void CNewUICustomEventTime::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

bool CNewUICustomEventTime::Render()
{
    if (!IsVisible()) return true;

    EnableAlphaTest();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    RenderFrame();
    RenderTexts();
    RenderEventList();

    DisableAlphaBlend();
    return true;
}

void CNewUICustomEventTime::RenderFrame()
{
    RenderBitmap(IMAGE_CUSTOM_EVENTTIME_BACK,
        m_Pos.x, m_Pos.y, EVENT_WND_WIDTH, EVENT_WND_HEIGHT,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);
}

void CNewUICustomEventTime::RenderTexts()
{
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 255, 200, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 10, "Event Schedule", EVENT_WND_WIDTH - 20, 0, RT3_SORT_CENTER);

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(180, 180, 200, 255);
    g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 40, "Event", 150, 0, RT3_SORT_LEFT);
    g_pRenderText->RenderText(m_Pos.x + 175, m_Pos.y + 40, "Status", 100, 0, RT3_SORT_LEFT);

    if (!m_bHasData && m_iEventCount <= 0)
    {
        g_pRenderText->SetTextColor(200, 200, 200, 255);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 150, "Loading event data...", EVENT_WND_WIDTH - 20, 0, RT3_SORT_CENTER);
    }
}

void CNewUICustomEventTime::RenderEventList()
{
    if (!m_bHasData || m_iEventCount <= 0) return;

    int startIdx = m_iScrollOffset;
    int maxDisplay = 18;
    int displayCount = (m_iEventCount - startIdx > maxDisplay) ? maxDisplay : (m_iEventCount - startIdx);

    for (int i = 0; i < displayCount; i++)
    {
        int idx = startIdx + i;
        if (idx >= m_iEventCount) break;

        int y = m_Pos.y + 60 + (i * 17);
        int time = m_Events[idx].iTime;

        g_pRenderText->SetFont(g_hFont);

        if (time <= 0)
        {
            g_pRenderText->SetTextColor(120, 120, 120, 255);
            g_pRenderText->RenderText(m_Pos.x + 15, y, g_szEventNames[idx % MAX_EVENT_DISPLAY], 150, 0, RT3_SORT_LEFT);
            g_pRenderText->SetTextColor(120, 120, 120, 255);
            g_pRenderText->RenderText(m_Pos.x + 175, y, "Offline / Closed", 120, 0, RT3_SORT_LEFT);
        }
        else
        {
            g_pRenderText->SetTextColor(220, 220, 220, 255);
            g_pRenderText->RenderText(m_Pos.x + 15, y, g_szEventNames[idx % MAX_EVENT_DISPLAY], 150, 0, RT3_SORT_LEFT);

            int minutes = time / 60;
            int seconds = time % 60;
            char szTime[16];
            if (minutes >= 60)
            {
                int hours = minutes / 60;
                minutes %= 60;
                sprintf(szTime, "%dh %02dm", hours, minutes);
            }
            else
            {
                sprintf(szTime, "%02d:%02d", minutes, seconds);
            }

            g_pRenderText->SetTextColor(100, 255, 100, 255);
            g_pRenderText->RenderText(m_Pos.x + 175, y, szTime, 100, 0, RT3_SORT_LEFT);
        }
    }

    if (m_iEventCount > maxDisplay)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(150, 150, 150, 255);
        char szScroll[32];
        sprintf(szScroll, "Scroll: %d/%d", m_iScrollOffset / maxDisplay + 1, (m_iEventCount + maxDisplay - 1) / maxDisplay);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + EVENT_WND_HEIGHT - 25, szScroll, EVENT_WND_WIDTH - 20, 0, RT3_SORT_CENTER);
    }
}

bool CNewUICustomEventTime::Update()
{
    if (!IsVisible()) return true;
    return true;
}

bool CNewUICustomEventTime::UpdateMouseEvent()
{
    if (!g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_EVENTTIME))
        return true;

    if (CheckMouseIn(m_Pos.x, m_Pos.y, EVENT_WND_WIDTH, EVENT_WND_HEIGHT))
    {
        if (SEASON3B::IsPress(VK_RBUTTON))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_EVENTTIME);
            return false;
        }

        if (SEASON3B::IsPress(VK_MBUTTON))
        {
            return false;
        }

        if (!SEASON3B::IsNone(VK_LBUTTON))
            return false;
    }

    return true;
}

bool CNewUICustomEventTime::UpdateKeyEvent()
{
    if (g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_EVENTTIME))
    {
        if (SEASON3B::IsPress(VK_ESCAPE))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_EVENTTIME);
            PlayBuffer(SOUND_CLICK01);
            return false;
        }

        if (SEASON3B::IsPress(VK_UP))
        {
            if (m_iScrollOffset > 0)
            {
                m_iScrollOffset -= 18;
                if (m_iScrollOffset < 0) m_iScrollOffset = 0;
            }
            return false;
        }

        if (SEASON3B::IsPress(VK_DOWN))
        {
            int maxOffset = m_iEventCount - 18;
            if (maxOffset < 0) maxOffset = 0;
            if (m_iScrollOffset < maxOffset)
            {
                m_iScrollOffset += 18;
                if (m_iScrollOffset > maxOffset) m_iScrollOffset = maxOffset;
            }
            return false;
        }
    }
    return true;
}

void CNewUICustomEventTime::OpenningProcess()
{
    RequestEventTime();
}

void CNewUICustomEventTime::ClosingProcess()
{
    m_iEventCount = 0;
    m_bHasData = false;
    m_iScrollOffset = 0;
}

void CNewUICustomEventTime::RequestEventTime()
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xE8;
    spe.Send();
}

void CNewUICustomEventTime::ReceiveEventTime(BYTE* pBuffer)
{
    if (pBuffer[0] != 0xC2) return;

    int ofs = 5;
    int count = *(int*)(pBuffer + ofs); ofs += 4;

    m_iEventCount = (count > MAX_EVENT_DISPLAY) ? MAX_EVENT_DISPLAY : count;
    for (int i = 0; i < m_iEventCount; i++)
    {
        if (ofs + 8 > (int)((pBuffer[1] << 8) | pBuffer[2]) + 2) break;
        m_Events[i].iIndex = *(int*)(pBuffer + ofs); ofs += 4;
        m_Events[i].iTime = *(int*)(pBuffer + ofs); ofs += 4;
    }

    m_bHasData = true;
}
