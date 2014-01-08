#include "kgmSystem/kgmSystem.h"
#include "kgmGame/kgmGameApp.h"
#include "kgmGame/kgmGameBase.h"

#include "kgmBase/kgmXml.h"
#include "kgmBase/kgmLog.h"
#include "kgmGraphics/kgmGuiButton.h"

#include "kGlobals.h"
#include "kGui.h"
#include "Actors/ASp_Gui.h"
#include "Actors/ASpacerObjects.h"
#include "Actors/ASpacer.h"
#include "Actors/ASp_Gun.h"
#include "Actors/ASp_Spacer.h"
#include "Actors/ASp_Spaceship.h"

KGMOBJECT_IMPLEMENT(ASpacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_Gui, kgmObject);
KGMOBJECT_IMPLEMENT(ASp_Gun, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_GunA, ASp_Gun);
KGMOBJECT_IMPLEMENT(ASp_GunFA, ASp_Gun);
KGMOBJECT_IMPLEMENT(ASp_Spacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpacerA, ASp_Spacer);
KGMOBJECT_IMPLEMENT(ASp_SpacerB, ASp_SpacerA);
KGMOBJECT_IMPLEMENT(ASp_Result, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Skybox, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_MotorA, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Laser, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_LaserA, ASp_Laser);
KGMOBJECT_IMPLEMENT(ASp_LaserB, ASp_Laser);
KGMOBJECT_IMPLEMENT(ASp_Flame, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_FlameA, ASp_Flame);
KGMOBJECT_IMPLEMENT(ASp_Smoke, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_SmokeA, ASp_Smoke);
KGMOBJECT_IMPLEMENT(ASp_Explode, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_ExplodeA, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_ExplodeB, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_ExplodeC, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_Asteroid, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_AsteroidSpawner, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpacerSpawner, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Spaceship, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpaceshipA, ASp_Spaceship);
KGMOBJECT_IMPLEMENT(ASp_SpaceshipB, ASp_SpaceshipA);

kgmString g_loc_dir;
bool g_ms_camera = false;
s8   g_map_current = 0;
s8   g_maps_count  = 5;
s8   g_maps_unlock = 1;

const char* g_maps[] =
{
#ifdef TEST
  "map test0", "map000.kgm",
#endif
  "Mission 1", "map001.kgm",
  "Mission 2", "map002.kgm",
  "Mission 3", "map003.kgm",
  "Mission 4", "map004.kgm",
  "Mission 5", "map004.kgm",
};

kgmString kgame_get_map_by_id(s32 i)
{
  if(i > 4)
    return "";

  return g_maps[2 * i + 1];
}

void kgame_set_current_map(s8 i)
{
  g_map_current = i;
}

class kGame;

class ASp_Logic: public kgmGameLogic
{
  kgmIGame*  game;
  kgmVisual* vtext;
  kgmVisual* vresult;

  u32        enemies;

public:
  ASp_Logic(kgmIGame* g)
  :kgmGameLogic()
  {
    game    = g;
    vtext   = null;
    vresult = null;

    enemies = 1;
  }

  void prepare()
  {
    enemies = 1;
    kgmList<kgmGameObject*> objs;
    kgmGameLogic::prepare();
    this->getObjectsByType(ASp_Spaceship::Class, objs);

    enemies = objs.size();
    objs.clear();
  }


  void action(kgmILogic::ACTION type, kgmObject* src, kgmString arg)
  {
    if(type == kgmILogic::ACTION_GAMEOBJECT)
    {
      if(arg == "die")
      {
        if(src->isType(ASp_Spaceship::Class))
        {
          enemies--;

          if(enemies == 0)
          {
              ASp_Result* res = new ASp_Result(game, 3000, 1, "Success");
              game->gAppend(res);

              if(g_map_current == g_maps_unlock && g_maps_unlock < g_maps_count)
              {
                g_maps_unlock ++;
                game->gCommand("gui_update");
              }
          }
        }
        else if(src->isType(ASpacer::Class))
        {
            ASp_Result* res = new ASp_Result(game, 3000, 0, "Failed");
            game->gAppend(res);
        }
      }
      else if(arg == "result")
      {
        if(src->isClass(ASp_Result::Class))
        {
          if(((ASp_Result*)src)->getResult())
            game->gCommand("gameover_success");
          else
            game->gCommand("gameover_fail");

          src->release();
          game->gSwitch(kgmIGame::State_Uload);
        }
      }
    }
  }

  void collide(kgmGameObject *os, kgmGameObject *od)
  {
    if(!vtext)
    {
      vtext = new kgmVisual();

      kgmText* text = new kgmText();
      text->m_rect  = uRect(10, 250, 500, 200);

      vtext->set(text);
      ((kgmGameBase*)game)->m_render->add(vtext);
      text->release();
      vtext->release();
    }
    else
    {
    }

    if(os->isType(ASp_Laser::Class) && !od->isType(ASp_Laser::Class))
    {
      if(os->getGroup() != od->getGroup())
      {
        ASp_Laser* laser = (ASp_Laser*)os;

        if(od->isType(kgmActor::Class))
        {
          kgmActor* actor = (kgmActor*)od;

          actor->m_health -= laser->power;
        }

        os->remove();
      }
    }
    else if(od->isType(ASp_Spacer::Class) && os->isType(ASpacer::Class))
    {
      ((kgmActor*)od)->m_health = 0;
      ((kgmActor*)os)->m_health = 0;
    }
    else if(od->isType(ASp_GunFA::Class) && os->isType(ASpacer::Class))
    {
      ((kgmActor*)od)->m_health = 0;
      ((kgmActor*)os)->m_health = 0;
    }
    else if(od->isType(ASp_Spaceship::Class) && os->isType(ASpacer::Class))
    {
      ((kgmActor*)os)->m_health = 0;
    }
  }
};

class kGame: public kgmGameBase{
  struct GameData
  {
    u16 sig;
    u8  umaps;
    u8  cmap;
  };

  kGui*      gui;
  GameData   data;

public:
  kGame()
  {
    gui = new kGui(this);
    m_gamemode = true;

    setMsAbsolute(true);

    if(m_physics)
      m_physics->m_gravity = 1.0f;

    if(m_logic)
    {
      m_logic->release();
    }
    m_logic = new ASp_Logic(this);

    data.sig  = 0xffff;
    data.cmap = 1;
    data.umaps = 1;

    readData();
    saveData();

    g_maps_unlock = data.umaps;
    gui->updateMaps((char**)g_maps, (u32)data.umaps);
  }

  ~kGame()
  {
  }

//protected:
  void  initLogic();

public:
  void onIdle()
  {
    kgmGameBase::onIdle();

    if(m_state == State_Play)
    {

    }
  }

  void onClose()
  {
    if(gui)
      gui->release();

    saveData();

    kgmGameBase::onClose();
  }

  void onKeyUp(int k){
    kgmGameBase::onKeyUp(k);

    if(k == KEY_ESCAPE)
    {
      if(gState() == State_Play)
      {
        gPause(true);
        gui->m_guiPause->show();
        setMsAbsolute(true);
      }
      else if(gState() == State_Pause)
      {
        gPause(false);
        gui->m_guiPause->hide();
        setMsAbsolute(false);
      }
    }
  }

  void onKeyDown(int k){
    kgmGameBase::onKeyDown(k);
  }

  void onMsLeftUp(int k, int x, int y)
  {
    kgmGameBase::onMsLeftUp(k, x, y);
  }

  void onMsLeftDown(int k, int x, int y)
  {
    kgmGameBase::onMsLeftDown(k, x, y);
  }

  void onMsMove(int k, int x, int y)
  {
    kgmGameBase::onMsMove(k, x, y);
  }

  void onResize(int w, int h)
  {
    kgmGameBase::onResize(w, h);

    if(m_state == kgmIGame::State_Play || m_state == kgmIGame::State_Pause)
    {
      ASpacer* gp = (ASpacer*)m_logic->m_gameplayer;

      if(gp)
      {
        gp->guiResize();
      }
    }
  }

  int gCommand(kgmString s)
  {
    if(s == "gameover_fail")
    {
      m_state = kgmIGame::State_Stop;
      gui->viewAgain();
    }
    else if(s == "gameover_success")
    {
      m_state = kgmIGame::State_Stop;
      gui->viewAgain();
    }
    else if(s == "gui_update")
    {
      kgm_log() << "update gui";
      gui->updateMaps((char**)g_maps, (u32)g_maps_unlock);
      data.umaps = g_maps_unlock;
      saveData();
      kgm_log() << "updated gui";
    }
  }

  int gLoad(kgmString s)
  {
    int res = kgmGameBase::gLoad(s);

    return res;
  }

  kgmGameObject* gObject(kgmString t)
  {
    if(t == "KSpacer")
    {
      return new ASpacer(this);
    }
    else if(t == "MotorA")
    {
      return new ASp_MotorA(this);
    }
    else if(t == "KAsteroidSpawner")
    {
      return new ASp_AsteroidSpawner(this);
    }
    else if(t == "ASp_AsteroidSpawner")
    {
      return new ASp_AsteroidSpawner(this);
    }
    else if(t == "GunFA")
    {
      return new ASp_GunFA(this);
    }
    else if(t == "KSpacerA")
    {
      return new ASp_SpacerA(this);
    }
    else if(t == "KSpacerB")
    {
      return new ASp_SpacerB(this);
    }
    else if(t == "KSpaceshipA")
    {
      return new ASp_SpaceshipA(this);
    }
    else if(t == "KSpaceshipB")
    {
      return new ASp_SpaceshipB(this);
    }
    else if(t == "KSpacerSpawner")
    {
      return new ASp_SpacerSpawner(this);
    }
    else if(t == "KFlameA")
    {
      return new ASp_FlameA(this);
    }

    return kgmGameBase::gObject(t);
  }

private:
  void readData()
  {
    kgmString path;
    kgmString delim;

    kgmSystem::getPathDelim(delim);

#ifdef ANDROID
    path = g_loc_dir + "/cdata";
#else
    kgmSystem::getHomeDirectory(path);

    path += delim;
    path += ".kSpacer";

    if(!kgmSystem::isDirectory(path))
      return;

    path += delim;
    path += "data";
#endif;

    kgmFile f;

    f.open(path, kgmFile::Read);

    if(f.m_file)
      f.read(&data, sizeof(data));

    f.close();
  }

  void saveData()
  {
    kgmString path;
    kgmString delim;

    kgmSystem::getPathDelim(delim);

#ifdef ANDROID
    path = g_loc_dir + "/cdata";
#else
    kgmSystem::getHomeDirectory(path);

    path += delim;
    path += ".kSpacer";

    if(!kgmSystem::isDirectory(path))
      kgmFile::make_directory(path);

    path += delim;
    path += "data";
#endif;

    kgmFile f;

    f.open(path, kgmFile::Write | kgmFile::Create);

    if(f.m_file)
      f.write(&data, sizeof(data));

    f.close();
  }
};

void  kGame::initLogic()
{
  m_logic = new ASp_Logic(this);
}


class kSpacer: public kgmGameApp{
#ifdef ANDROID
public:
#endif
  kGame* game;
public:
  kSpacer()
  {
  }

  ~kSpacer(){
  }

  void main()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = game = new kGame();
    game->setRect(0, 0, w, h);
    game->loop();
    game->release();
  }

#ifdef ANDROID
  kgmIGame* android_init_game()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = game = new kGame();
    game->setRect(0, 0, w, h);

    return game;
  }
#endif
};

//main object
//#ifndef ANDROID
kSpacer theApp;
//#endif
//////////////

//FOR ANDROID
#ifdef ANDROID

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

kSpacer*          m_app  = null;

bool kgm_android_init_app()
{
  m_app = new kSpacer();

  return true;
}

const char*  kgm_android_classname()
{
  return "com/example/kSpacer/kSpacer";
}

extern "C"
{
JNIEXPORT void  JNICALL Java_com_example_kSpacer_kSpacerLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
                                                           jobject am, jobject surface);
JNIEXPORT void  JNICALL Java_com_example_kSpacer_kSpacerLib_quit(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_kSpacer_kSpacerLib_idle(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_kSpacer_kSpacerLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key);
JNIEXPORT void  JNICALL Java_com_example_kSpacer_kSpacerLib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y);
};

JNIEXPORT void JNICALL Java_com_example_kSpacer_kSpacerLib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am,
                                                          jobject surface)
{
#ifdef TEST
  kgm_log() << "Java_com_example_kSpacer_kSpacerLib_init\n";
#endif

  g_loc_dir = "/data/data/com.example.kSpacer";

  if(kgmGameApp::gameApplication()->game())
  {
    kgmGameApp::gameApplication()->game()->getWindow()->setRect(0, 0, width, height);
  }
  else
  {
    kgm_android_init_app();
    kgmGameApp::gameApplication()->android_init(env, obj, width, height, am, surface);
  }
}

JNIEXPORT void JNICALL Java_com_example_kSpacer_kSpacerLib_quit(JNIEnv * env, jobject obj)
{
#ifdef TEST
  kgm_log() << "Java_com_example_kSpacer_kSpacerLib_quit \n";
#endif

  kgmGameApp::gameApplication()->android_quit(env, obj);
}

JNIEXPORT void JNICALL Java_com_example_kSpacer_kSpacerLib_idle(JNIEnv * env, jobject obj)
{
#ifdef TEST
  kgm_log() << "Java_com_example_kSpacer_kSpacerLib_idle \n";
#endif

  kgmGameApp::gameApplication()->android_idle(env, obj);
}

JNIEXPORT void JNICALL Java_com_example_kSpacer_kSpacerLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
#ifdef TEST
  kgm_log() << "Java_com_example_kSpacer_kSpacerLib_onKeyboard \n";
#endif

  kgmGameApp::gameApplication()->android_onKeyboard(env, obj, a, key);
}

JNIEXPORT void JNICALL Java_com_example_kSpacer_kSpacerLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
#ifdef TEST
  kgm_log() << "Java_com_example_kSpacer_kSpacerLib_onTouch \n";
#endif

  kgmGameApp::gameApplication()->android_onTouch(env, obj,  a, x, y);
}
#endif
