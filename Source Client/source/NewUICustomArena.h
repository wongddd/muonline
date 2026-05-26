#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"

#define MAX_CUSTOM_ARENA_USER 100

#pragma pack(push, 1)
struct PMSG_CUSTOM_ARENA_ENTRY
{
    int Index;
    int State;
    int RemainTime;
    int UserCount;
    int MaxUsers;
    int EnterEnabled;
    char Name[32];
};

struct PMSG_CUSTOM_ARENA_SCORE_ENTRY
{
    int Rank;
    char Name[10];
    int KillCount;
    int DeathCount;
    int Score;
};
#pragma pack(pop)

namespace SEASON3B
{
    class CNewUICustomArena : public CNewUIObj
    {
    public:
        CNewUICustomArena();
        virtual ~CNewUICustomArena();

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
        void RequestArenaList();
        void RequestJoinArena(int arenaIndex);
        void ReceiveArenaList(BYTE* pBuffer);
        void ReceiveScoreboard(BYTE* pBuffer);

    private:
        enum
        {
            WND_WIDTH = 400,
            WND_HEIGHT = 450,
            MAX_ARENA_DISPLAY = 30,
            ENTRY_HEIGHT = 52,
            MAX_VISIBLE_ENTRIES = 7,
            SCORE_WND_WIDTH = 250,
            SCORE_WND_HEIGHT = 380,
            SCORE_ENTRY_HEIGHT = 22,
            MAX_VISIBLE_SCORE = 14,
            REFRESH_INTERVAL = 5000,
        };

        enum { IMAGE_BACK = 31994, IMAGE_CLOSE = 31995, IMAGE_BTN = 31996, IMAGE_DOT = 31997 };

        struct ARENA_ENTRY
        {
            int Index;
            int State;
            int RemainTime;
            int UserCount;
            int MaxUsers;
            int EnterEnabled;
            char Name[32];
        };

        struct SCORE_ENTRY
        {
            int Rank;
            char Name[10];
            int KillCount;
            int DeathCount;
            int Score;
        };

        void LoadImages();
        void UnloadImages();

        void RenderFrame();
        void RenderTexts();
        void RenderArenaEntry(int idx, const ARENA_ENTRY& entry);
        void RenderJoinButton(int idx);

        void RenderScoreboard();
        void RenderScoreEntry(int idx, const SCORE_ENTRY& entry);

        const char* GetStateText(int state);

        CNewUIManager* m_pNewUIMng;
        POINT m_Pos;

        ARENA_ENTRY m_ArenaEntries[MAX_ARENA_DISPLAY];
        int m_iArenaCount;

        SCORE_ENTRY m_ScoreEntries[MAX_CUSTOM_ARENA_USER];
        int m_iScoreCount;
        int m_iScoreArenaIndex;

        DWORD m_dwLastRequestTick;
        int m_iScrollOffset;
    };
}
