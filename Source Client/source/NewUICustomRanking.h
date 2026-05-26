#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"

#pragma pack(push, 1)
struct PMSG_CUSTOM_RANKING_COUNT_SEND
{
    BYTE byProtocol;    // 0xC1
    BYTE bySize;
    BYTE byHeadCode;    // 0xF3
    BYTE bySubCode;     // 0xE7
    int count;
};

struct PMSG_CUSTOM_RANKING_DATA_SEND
{
    BYTE byProtocol;    // 0xC2
    BYTE bySizeH;
    BYTE bySizeL;
    BYTE byHeadCode;    // 0xF3
    BYTE bySubCode;     // 0xE6
    int index;
    char rankname[20];
    char col1[20];
    char col2[20];
    int count;
    // entries follow
};
#pragma pack(pop)

namespace SEASON3B
{
    class CNewUICustomRanking : public CNewUIObj
    {
    public:
        CNewUICustomRanking();
        virtual ~CNewUICustomRanking();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();

        bool Render();
        bool Update();
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();

        float GetLayerDepth() { return 4.2f; }
        float GetKeyEventOrder() { return 3.0f; }

        void OpenningProcess();
        void ClosingProcess();

        void SetPos(int x, int y);
        void RequestRankingCount();
        void RequestRankingData(int type);

        void ReceiveRankingCount(int count);
        bool ReceiveRankingData(BYTE* pBuffer);

    private:
        enum
        {
            RANKING_WIDTH = 280,
            RANKING_HEIGHT = 350,
            MAX_DISPLAY_ITEMS = 15,
        };

        struct RANKING_CATEGORY
        {
            char szName[20];
            char szCol1[20];
            char szCol2[20];
        };

        struct RANKING_ENTRY
        {
            char szName[20];
            int iValue;
        };

        void LoadImages();
        void UnloadImages();

        void RenderFrame();
        void RenderTexts();
        void RenderCategoryButtons();
        void RenderList();

        bool BtnProcess();

        CNewUIManager* m_pNewUIMng;
        CNewUIButton* m_pBtnClose;
        CNewUIButton* m_pBtnRefresh;

        POINT m_Pos;

        int m_iCategoryCount;
        int m_iSelectedCategory;
        RANKING_CATEGORY m_Categories[10];
        RANKING_ENTRY m_Entries[50];
        int m_iEntryCount;
    };
}
