#ifndef KGUI_H
#define KGUI_H

#include "kgmBase/kgmObject.h"
#include "kgmGame/kgmGameBase.h"

class kGui : public kgmEvent
{
public:
    kgmGameBase* m_game;
    kgmGui*      m_guiMain;
    kgmGui*      m_guiGame;
    kgmGui*      m_guiSettings;
    kgmGui*      m_guiPause;
    kgmGuiList*  m_guiLevels;

public:
    kGui(kgmGameBase*);
    virtual ~kGui();
    void onAction(kgmEvent*, int);

    void viewAgain();
    void updateMaps(char* maps[], u32 cnt);
};

#endif // KGUI_H
