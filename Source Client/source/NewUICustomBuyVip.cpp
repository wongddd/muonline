#include "stdafx.h"
#include "NewUICustomBuyVip.h"
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

enum { IMAGE_CUSTOM_BUYVIP_BACK = 31994, IMAGE_CUSTOM_BUYVIP_CLOSE = 31995, IMAGE_CUSTOM_BUYVIP_BTN = 31996 };

CNewUICustomBuyVip::CNewUICustomBuyVip()
{
    m_pNewUIMng = NULL;
    m_pBtnClose = NULL;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_iVipCount = 0;
    memset(m_VipEntries, 0, sizeof(m_VipEntries));
}

CNewUICustomBuyVip::~CNewUICustomBuyVip()
{
    Release();
}

bool CNewUICustomBuyVip::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_CUSTOM_BUYVIP, this);
    SetPos(x, y);
    LoadImages();
    Show(false);
    return true;
}

void CNewUICustomBuyVip::Release()
{
    UnloadImages();
    if (m_pBtnClose) { delete m_pBtnClose; m_pBtnClose = NULL; }
    if (m_pNewUIMng) { m_pNewUIMng->RemoveUIObj(this); m_pNewUIMng = NULL; }
}

void CNewUICustomBuyVip::LoadImages()
{
    LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_CUSTOM_BUYVIP_BACK, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_close.tga", IMAGE_CUSTOM_BUYVIP_CLOSE, GL_NEAREST, GL_CLAMP, 1);
    LoadBitmap("Interface\\newui_btn_empty_very_small.tga", IMAGE_CUSTOM_BUYVIP_BTN, GL_NEAREST, GL_CLAMP, 1);
}

void CNewUICustomBuyVip::UnloadImages()
{
    DeleteBitmap(IMAGE_CUSTOM_BUYVIP_BACK);
    DeleteBitmap(IMAGE_CUSTOM_BUYVIP_CLOSE);
    DeleteBitmap(IMAGE_CUSTOM_BUYVIP_BTN);
}

void CNewUICustomBuyVip::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

bool CNewUICustomBuyVip::Render()
{
    if (!IsVisible()) return true;

    EnableAlphaTest();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    RenderFrame();
    RenderTexts();

    for (int i = 0; i < m_iVipCount; i++)
    {
        RenderVipEntry(i, m_VipEntries[i]);
    }

    DisableAlphaBlend();
    return true;
}

void CNewUICustomBuyVip::RenderFrame()
{
    RenderBitmap(IMAGE_CUSTOM_BUYVIP_BACK, m_Pos.x, m_Pos.y, VIP_WND_WIDTH, VIP_WND_HEIGHT,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);
}

void CNewUICustomBuyVip::RenderTexts()
{
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 255, 200, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 10, "VIP Package Purchase", VIP_WND_WIDTH - 20, 0, RT3_SORT_CENTER);

    if (m_iVipCount <= 0)
    {
        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetTextColor(200, 200, 200, 255);
        g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 100, "Loading VIP info...", VIP_WND_WIDTH - 20, 0, RT3_SORT_CENTER);
    }
}

void CNewUICustomBuyVip::RenderVipEntry(int idx, const VIP_ENTRY& entry)
{
    int y = m_Pos.y + 45 + (idx * VIP_ENTRY_HEIGHT);

    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetTextColor(255, 200, 100, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 15, y, entry.VipName, VIP_WND_WIDTH - 30, 0, RT3_SORT_LEFT);

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(180, 220, 255, 255);
    char szDays[64];
    sprintf(szDays, "Duration: %d days", entry.Days);
    g_pRenderText->RenderText(m_Pos.x + 15, y + 18, szDays, VIP_WND_WIDTH - 30, 0, RT3_SORT_LEFT);

    g_pRenderText->SetTextColor(220, 220, 180, 255);
    char szCoin[128];
    sprintf(szCoin, "Cost: Coin1=%d  Coin2=%d  Coin3=%d", entry.Coin1, entry.Coin2, entry.Coin3);
    g_pRenderText->RenderText(m_Pos.x + 15, y + 36, szCoin, VIP_WND_WIDTH - 30, 0, RT3_SORT_LEFT);

    RenderBuyButton(idx);
}

void CNewUICustomBuyVip::RenderBuyButton(int idx)
{
    int y = m_Pos.y + 45 + (idx * VIP_ENTRY_HEIGHT);
    RenderBitmap(IMAGE_CUSTOM_BUYVIP_BTN, m_Pos.x + 200, y + 50, 70, 20,
        0.0f, 0.0f, 1.0f, 1.0f, true, NULL);

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(255, 255, 255, 255);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->RenderText(m_Pos.x + 200, y + 50, "Buy", 70, 0, RT3_SORT_CENTER);
}

bool CNewUICustomBuyVip::Update()
{
    if (!IsVisible()) return true;
    return true;
}

bool CNewUICustomBuyVip::UpdateMouseEvent()
{
    if (!g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_BUYVIP))
        return true;

    if (BtnProcess()) return false;

    if (CheckMouseIn(m_Pos.x, m_Pos.y, VIP_WND_WIDTH, VIP_WND_HEIGHT))
    {
        if (SEASON3B::IsPress(VK_RBUTTON))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_BUYVIP);
            return false;
        }
        if (!SEASON3B::IsNone(VK_LBUTTON))
        {
            for (int i = 0; i < m_iVipCount; i++)
            {
                if (CheckMouseIn(m_Pos.x + 200, m_Pos.y + 45 + (i * VIP_ENTRY_HEIGHT) + 50, 70, 20))
                {
                    RequestBuyVip(m_VipEntries[i].Index + 1);
                    return false;
                }
            }
            return false;
        }
    }

    return true;
}

bool CNewUICustomBuyVip::UpdateKeyEvent()
{
    if (g_pNewUISystem->IsVisible(INTERFACE_CUSTOM_BUYVIP))
    {
        if (SEASON3B::IsPress(VK_ESCAPE))
        {
            g_pNewUISystem->Hide(INTERFACE_CUSTOM_BUYVIP);
            PlayBuffer(SOUND_CLICK01);
            return false;
        }
    }
    return true;
}

bool CNewUICustomBuyVip::BtnProcess()
{
    return false;
}

void CNewUICustomBuyVip::OpenningProcess()
{
    RequestVipInfo();
}

void CNewUICustomBuyVip::ClosingProcess()
{
    m_iVipCount = 0;
}

void CNewUICustomBuyVip::RequestVipInfo()
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xEA;
    spe.Send();
}

void CNewUICustomBuyVip::RequestBuyVip(int vipType)
{
    CStreamPacketEngine spe;
    spe.Init(0xC1, 0xF3);
    spe << (BYTE)0xF0;
    spe << (int)vipType;
    spe.Send();
}

void CNewUICustomBuyVip::ReceiveVipInfo(BYTE* pBuffer)
{
    if (pBuffer[0] != 0xC1) return;

    int count = *(int*)(pBuffer + 4);
    m_iVipCount = (count > MAX_VIP_DISPLAY) ? MAX_VIP_DISPLAY : count;

    int ofs = 8;
    for (int i = 0; i < m_iVipCount; i++)
    {
        if (ofs + 60 > (int)pBuffer[1]) break;
        m_VipEntries[i].Index = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Exp = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Drop = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Days = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Coin1 = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Coin2 = *(int*)(pBuffer + ofs); ofs += 4;
        m_VipEntries[i].Coin3 = *(int*)(pBuffer + ofs); ofs += 4;
        memcpy(m_VipEntries[i].VipName, pBuffer + ofs, 32); ofs += 32;
    }
}
