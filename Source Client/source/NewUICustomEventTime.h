#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"

namespace SEASON3B
{
    class CNewUICustomEventTime : public CNewUIObj
    {
    public:
        CNewUICustomEventTime();
        virtual ~CNewUICustomEventTime();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();

        bool Render();
        bool Update();
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();

        float GetLayerDepth() { return 4.1f; }
        float GetKeyEventOrder() { return 3.0f; }

        void OpenningProcess();
        void ClosingProcess();

        void SetPos(int x, int y);
        void RequestEventTime();
        void ReceiveEventTime(BYTE* pBuffer);
        bool HasData() { return m_bHasData; }

    private:
        enum
        {
            EVENT_WND_WIDTH = 320,
            EVENT_WND_HEIGHT = 400,
            MAX_EVENT_DISPLAY = 42,
        };

        struct EVENT_ENTRY
        {
            int iIndex;
            int iTime;
        };

        void LoadImages();
        void UnloadImages();

        void RenderFrame();
        void RenderTexts();
        void RenderEventList();

        CNewUIManager* m_pNewUIMng;
        POINT m_Pos;

        EVENT_ENTRY m_Events[MAX_EVENT_DISPLAY];
        int m_iEventCount;
        bool m_bHasData;
        int m_iScrollOffset;
    };
}
