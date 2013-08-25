#include "kGui.h"
#include "kGlobals.h"
#include "kgmBase/kgmLog.h"
#include "kgmGame/kgmGameApp.h"

extern kgmString  kgame_get_map_by_id(s32 i);
extern void       kgame_set_current_map(s8 i);


kGui::kGui(kgmGameBase* game)
{
  m_guiMain = null;
  kgmString text;

  if(!game)
    return;
  else
    m_game = game;

  int x, y, w, h;

  game->getRect(x, y, w, h);
  kgm_log() << "GameRect: " << w << " " << h << ".";

  m_guiMain = new kgmGui(null, 0, 0, w, h);
  m_guiMain->addListener(this);

  kgmGui* gui = new kgmGui(m_guiMain, 0, 0, w, 50);

  kgmGuiButton* btn = new kgmGuiButton(gui, 0, 0, w/3, 50);
  text = "Game";
  btn->setSid("game");
  btn->setText(text);
  btn = new kgmGuiButton(gui, w/3, 0, w/3, 50);
  text = "Credits";
  btn->setSid("credits");
  btn->setText(text);
  btn = new kgmGuiButton(gui, 2 * w/3, 0, w/3, 50);
  text = "Quit";
  btn->setText(text);
  btn->setSid("quit");

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiCredits");
  gui->hide();
  m_guiCredits = gui;

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiGame");
  m_guiGame = gui;
  /*
  btn = new kgmGuiButton(gui, 1, 10, 100, 30);
  text = "Start";
  btn->setText(text);
  btn->setSid("start");
  btn = new kgmGuiButton(gui, 1, 50, 100, 30);
  text = "Continue";
  btn->setText(text);
  btn->setSid("continue");
  */

  kgmGuiList* lst = m_guiLevels = new kgmGuiList(gui, w / 2 - 150, 10, 300, h - 100);
  lst->m_itemHeight = 40;
  lst->setSid("levels");
  gui->show();

  game->guiAdd(m_guiMain);


  m_guiPause = new kgmGui(null, w / 2 - 50, h / 2 - 50, 100, 100);
  m_guiPause->addListener(this);
  m_guiPause->hide();

  btn = new kgmGuiButton(m_guiPause, 1, 1, 98, 48);
  btn->setText("Resume");
  btn->setSid("gameResume");

  btn = new kgmGuiButton(m_guiPause, 1, 51, 98, 48);
  btn->setText("Exit");
  btn->setSid("gameExit");

  game->guiAdd(m_guiPause);
}

kGui::~kGui()
{
  m_guiMain->release();
}

void kGui::onAction(kgmEvent* e, int a)
{
  kgmGui* gui = (kgmGui*)e;

  kgmString sid = gui->getSid();

  if(sid == "quit")
  {
    m_game->gQuit();
#ifdef ANDROID
    kgmGameApp::gameApplication()->android_exit();
#endif
  }
  else if(sid == "game")
  {
    m_guiGame->show();
    m_guiCredits->hide();
  }
  else if(sid == "credits")
  {
    m_guiGame->hide();
    m_guiCredits->show();
  }
  else if(sid == "start")
  {
    m_guiGame->show();
    m_guiCredits->hide();
  }
  else if(sid == "continue")
  {
    m_guiGame->show();
    m_guiCredits->hide();
  }
  else if(sid == "levels")
  {
    kgmGuiList* lst = (kgmGuiList*)gui;
    int i = lst->getSel();

    kgmString map = lst->getItem(i);
    int res = m_game->gLoad(kgame_get_map_by_id(i)); //map

    if(res == kgmIGame::State_Play)
    {
      m_guiMain->hide();

      if(g_ms_camera)
        m_game->m_msAbs = true;

      kgame_set_current_map(i + 1);
    }
  }
  else if(sid == "gameExit")
  {
    m_game->gSwitch(kgmIGame::State_Uload);
    m_guiPause->hide();
    m_guiMain->show();

    if(g_ms_camera)
      m_game->m_msAbs = false;
  }
  else if(sid == "gameResume")
  {
    m_guiPause->hide();
    m_game->gPause(false);

    if(g_ms_camera)
      m_game->m_msAbs = true;
  }
}

void kGui::viewAgain()
{
  m_guiMain->show();

  if(g_ms_camera)
    m_game->m_msAbs = false;
}

void kGui::updateMaps(char *maps[], u32 cnt)
{
  if(!m_guiLevels)
    return;

  m_guiLevels->clear();

  for(u32 i = 0; i < cnt; i++)
  {
    if(maps[2 * i] == null)
      return;

    m_guiLevels->addItem(maps[2 * i]);
  }
}
