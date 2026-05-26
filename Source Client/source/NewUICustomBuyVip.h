#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"

#pragma pack(push, 1)
struct PMSG_CUSTOM_BUYVIP_INFO_RECV
{
    BYTE byProtocol;
    BYTE bySize;
    BYTE byHeadCode;
    BYTE bySubCode;
    int count;
};
#pragma pack(pop)

namespace SEASON3B
{
    class CNewUICustomBuyVip : public CNewUIObj
    {
    public:
        CNewUICustomBuyVip();
        virtual ~CNewUICustomBuyVip();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();

        bool Render();
        bool Update();
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();

        float GetLayerDepth() { return 4.3f; }
        float GetKeyEventOrder() { return 3.0f; }

        void OpenningProcess();
        void ClosingProcess();

        void SetPos(int x, int y);
        void RequestVipInfo();
        void RequestBuyVip(int vipType);
        void ReceiveVipInfo(BYTE* pBuffer);

    private:
        enum
        {
            VIP_WND_WIDTH = 300,
            VIP_WND_HEIGHT = 350,
            MAX_VIP_DISPLAY = 3,
            VIP_ENTRY_HEIGHT = 80,
        };

        struct VIP_ENTRY
        {
            int Index;
            int Exp;
            int Drop;
            int Days;
            int Coin1;
            int Coin2;
            int Coin3;
            char VipName[32];
        };

        void LoadImages();
        void UnloadImages();

        void RenderFrame();
        void RenderTexts();
        void RenderVipEntry(int idx, const VIP_ENTRY& entry);
        void RenderBuyButton(int idx);

        bool BtnProcess();

        CNewUIManager* m_pNewUIMng;
        CNewUIButton* m_pBtnClose;

        POINT m_Pos;

        VIP_ENTRY m_VipEntries[MAX_VIP_DISPLAY];
        int m_iVipCount;
    };
}
