#ifndef ASPACEROBJECTS_H
#define ASPACEROBJECTS_H

//#include "../kgmGame/kgmGameObject.h"

struct ASp_Vertex
{
  vec3 v;
  u32  c;
  vec2 uv;
};

struct ASp_Face
{
  u16 a, b, c;
};

class ASp_Skybox: public kgmGameObject
{
  KGM_OBJECT(ASp_Skybox);

private:

  class Stars: public kgmParticles
  {

  public:
    Stars(u32 count, float distance, float size = 1.0)
    {
      m_particles = new Particle[count];

      float scale = size / (1.0f + rand()%10);

      m_count      = count;
      m_fade       = false;
      m_fall       = false;
      m_life       = 0xffffffff;
      st_size      = size;
      en_size      = size;

      u32 colors[6] = {KGM_RGBA(200, 200, 255, 255), KGM_RGBA(0xfc, 0xff, 0xd2, 255),
                       KGM_RGBA(200, 200, 255, 255), KGM_RGBA(200, 200, 255, 255),
                       KGM_RGBA(0xff, 0xdf, 0xde, 255), KGM_RGBA(200, 200, 255, 255)};

      for(u32 i = 0; i < count; i++)
      {
        Particle* p = &m_particles[i];

        p->speed = 0.0f;
        p->scale = 0.5 + size / (1.0 + rand()%10);
        //p->scale = 1.0 ;
        p->life  = 0xffffffff;
        float    alpha = DEGTORAD(rand()%360);
        p->pos   = vec3(distance * cos(alpha),
                        distance * sin(alpha),
                        0.3f * distance * sin(DEGTORAD(rand()%360)));
        p->col.color = colors[rand() % 6];
      }
    }

    void update(u32 ms)
    {
    }
  };

  kgmIGame*  game;

public:
  ASp_Skybox(kgmIGame* g)
  {
    game = g;
    kgmMaterial*  mtl  = new kgmMaterial();

    mtl->m_type        = "simple";
    mtl->m_shader      = kgmMaterial::ShaderBlend;
    mtl->m_depth       = false;
    mtl->m_blend       = true;
    mtl->m_srcblend    = gcblend_one;
    mtl->m_dstblend    = gcblend_one;
    //mtl->m_tex_color   = g->getResources()->getTexture("point_bluee.tga");
    mtl->m_tex_color   = g->getResources()->getTexture("star.tga");

    m_visual = new kgmVisual();
    m_visual->set(mtl);
    mtl->release();

    int scount;

#ifdef ANDROID
    scount = 100;
#else
    scount = 300;
#endif

    Stars*   s = new Stars(scount, 100, 1);
    m_visual->set((kgmParticles*)s);
    s->release();
  }

  ~ASp_Skybox()
  {
  }

  virtual void update(u32 ms)
  {
    kgmGameObject::update(ms);

    if(game)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      mtx4 m, msc;

      msc.scale(80, 80, 80);
      m.translate(cam.mPos);
      m_visual->m_transform = msc * m;
    }
  }
};

class ASp_SkyboxB: public kgmGameObject
{
  KGM_OBJECT(ASp_SkyboxB);

private:
  kgmIGame*  game;

public:
  ASp_SkyboxB(kgmIGame* g)
  {
    game = g;
    kgmMaterial*  mtl  = new kgmMaterial();

    mtl->m_type        = "simple";
    mtl->m_shader      = kgmMaterial::ShaderBlend;

    m_visual = new kgmVisual();
    m_visual->set(mtl);
    mtl->release();

    u32 points;

#ifdef ANDROID
    points = 1000;
#else
    points = 3000;
#endif

    u32 colors[] = {0xfffffe9b, 0xfffff483, 0xfffffb3a, 0xfffffaba, 0xffffffff};
    kgmMesh* mesh = new kgmMesh();
    mesh->m_rtype = kgmMesh::RT_POINT;

    kgmMesh::Vertex_P_C_T* pc = (kgmMesh::Vertex_P_C_T*)mesh->vAlloc(points, kgmMesh::FVF_P_C_T);

    for(int i = 0; i < points; i++)
    {
      vec3 v(pow(-1.0, rand()%2) * rand(), pow(-1.0, rand()%2) * rand(), pow(-1.0, rand()%2) * rand());
      v.normalize();
      v = v * 1000;
      v.z *= 0.3;

      pc[i].pos = v;
      pc[i].col = colors[rand()%5];
      pc[i].uv = vec2(0, 0);
    }

    m_visual->set(mesh);
    mesh->release();
  }

  ~ASp_SkyboxB()
  {
  }

  virtual void update(u32 ms)
  {
    kgmGameObject::update(ms);

    if(game)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      mtx4 m, msc;

      m.identity();
      msc.identity();
      msc.scale(1, 1, 1);
      m.translate(cam.mPos);
      m_visual->m_transform = msc * m;
    }
  }
};

class ASp_MotorA: public kgmGameObject
{
  KGM_OBJECT(ASp_MotorA);
  kgmIGame*      game;
  kgmMaterial*   mtl;
  kgmParticles*  ptl;

  kgmVisual*     visual;
  kgmMesh*       mesh;
public:
  ASp_MotorA(kgmIGame* g)
  {
    game     = g;
    m_visual = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;
    mtl->m_blend = true;
    mtl->m_depth = false;
    mtl->m_srcblend = gcblend_srcalpha;
    mtl->m_dstblend = gcblend_one;
    mtl->m_tex_color = g->getResources()->getTexture("point_d.tga");
    mtl->m_type = "simple";
    mtl->m_shader = kgmMaterial::ShaderBlend;

    ptl = new kgmParticles();
    ptl->m_typerender  = kgmParticles::RTypePoint;
    ptl->m_count       = 25;
    ptl->m_speed       = 2.0f;
    ptl->m_life        = 200;
    ptl->div_life      = 0.95;
    ptl->div_speed     = 0.95;
    ptl->direction     = vec3(-1,0,0);
    ptl->div_direction = 0.5;
    ptl->st_size       = .08f;
    ptl->en_size       = .02f;
    ptl->build();

    visual = new kgmVisual();
    mesh = new kgmMesh();
    ASp_Vertex* v = (ASp_Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };
    v[1]   = { {0, -0.1, 0.1},  0xffffffff, {0, 0} };
    v[2]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[3]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[4]   = { {0, 0.1, -0.1},  0xffffffff, {1, 1} };
    v[5]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };

    v[6]   = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };
    v[7]   = { {0,  0.1, 0},  0xffffffff, {0.5, 0} };
    v[8]   = { {-0.5,  0.1, 0}, 0xffffffff, {1, 0} };
    v[9]   = { {-0.5, 0.1, 0},  0xffffffff, {1, 0} };
    v[10]  = { {-0.5, -0.1, 0}, 0xffffffff, {1, 1} };
    v[11]  = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };

    v[12]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };
    v[13]  = { {0, 0, 0.1},   0xffffffff, {0.5, 0} };
    v[14]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[15]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[16]  = { {-0.5, 0, -0.1}, 0xffffffff, {1, 1} };
    v[17]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };

    m_visual->set(mesh);
    m_visual->set(mtl);
    visual->set(ptl);
    visual->set(mtl);
    ((kgmGameBase*)game)->m_render->add(visual);
  }

  ~ASp_MotorA()
  {
    mesh->release();
    mtl->release();
    ptl->release();
    visual->release();
  }

  void update(u32 mls)
  {
    if(!valid())
      return;

    visual->m_transform = m_visual->m_transform;
    visual->update();
  }

  void event(kgmObject* o, kgmString arg)
  {
    if(arg == "die" && o == m_parent)
    {
      visual->disable();
      visual->remove();
      remove();
    }
  }

  void setSpeed(float speed)
  {
    ptl->m_speed = 2.0 + 25 * speed;
  }
};

class ASp_Laser: public kgmGameObject
{
  KGM_OBJECT(ASp_Laser);

  struct Vertex
  {
    vec3 v;
    u32  c;
    vec2 uv;
  };

  kgmMesh*      mesh;
  kgmMaterial*  mtl;

public:
  u32           power;

public:
  ASp_Laser(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.2)
  {
    power = 1;
    timeout(time);
    this->setId("laser");
    m_visual  = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;
    mtl->m_depth = false;
    mtl->m_blend = true;
    mtl->m_srcblend = gcblend_one;
    mtl->m_dstblend = gcblend_one;
    mtl->m_tex_color = g->getResources()->getTexture("point_redd.tga");
    mtl->m_shader = kgmMaterial::ShaderBlend;

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]  = { {0, -len, -len}, 0xffffffff, {0, 1}};
    v[1]  = { {0, -len, len},  0xffffffff, {0, 0}};
    v[2]  = { {0, len, len},   0xffffffff, {1, 0}};
    v[3]  = { {0, len, len},   0xffffffff, {1, 0}};
    v[4]  = { {0, len, -len},  0xffffffff, {1, 1}};
    v[5]  = { {0, -len, -len}, 0xffffffff, {0, 1}};

    v[6]  = { {len, -len, 0},  0xffffffff, {0, 1}};
    v[7]  = { {len,  len, 0},  0xffffffff, {0, 0}};
    v[8]  = { {-len,  len, 0}, 0xffffffff, {1, 0}};
    v[9]  = { {-len, len, 0},  0xffffffff, {1, 0}};
    v[10] = { {-len, -len, 0}, 0xffffffff, {1, 1}};
    v[11] = { {len, -len, 0},  0xffffffff, {0, 1}};

    v[12] = { {len, 0, -len},  0xffffffff, {0, 1}};
    v[13] = { {len, 0, len},   0xffffffff, {0, 0}};
    v[14] = { {-len, 0, len},  0xffffffff, {1, 0}};
    v[15] = { {-len, 0, len},  0xffffffff, {1, 0}};
    v[16] = { {-len, 0, -len}, 0xffffffff, {1, 1}};
    v[17] = { {len, 0, -len},  0xffffffff, {0, 1}};

    m_visual->set(mesh);
    m_visual->set(mtl);
    mesh->release();
    mtl->release();

    m_body = new kgmBody();

    //body->m_collision = false;
    m_body->translate(pos);
    m_body->rotate(rot);
    m_body->m_velocity  = speed;
    m_body->m_gravity   = false;
    m_body->m_udata     = this;
    m_body->m_mass      = 0.0f;
    m_body->m_bound.min = vec3(-len, -len, -len);
    m_body->m_bound.max = vec3( len,  len,  len);
  }

  ~ASp_Laser()
  {
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }
};

class ASp_LaserA: public ASp_Laser
{
  KGM_OBJECT(ASp_LaserA);

public:
  ASp_LaserA(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.2)
    :ASp_Laser(g, time, pos, rot, speed, len)
  {
  }
};

class ASp_LaserB: public ASp_Laser
{
  KGM_OBJECT(ASp_LaserB);

public:
  ASp_LaserB(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.5)
    :ASp_Laser(g, time, pos, rot, speed, len)
  {
    mtl->m_tex_color = g->getResources()->getTexture("point_yelloww.tga");
  }
};

class ASp_Asteroid: public kgmGameObject
{
  KGM_OBJECT(ASp_Asteroid);

  kgmMesh*     msh;
  kgmMaterial* mtl;

  float        angle;

public:
  ASp_Asteroid(kgmIGame* g, u32 type)
  {
    m_visual = new kgmVisual();
    msh = g->getResources()->getMesh("kasteroid_o2.kgm");

    mtl = new kgmMaterial();
    mtl->m_shader = kgmMaterial::ShaderBase;
    mtl->m_tex_color = g->getResources()->getTexture("kasteroid_o2.col.tga");
    mtl->m_tex_normal = g->getResources()->getTexture("kasteroid_o2.nor.tga");

    m_visual->set(msh);
    m_visual->set(mtl);

    m_body   = new kgmBody();
    m_body->m_udata = this;
    m_body->m_gravity = false;
    m_body->m_velocity = 0.01 + 0.02 * 1.0f / (1 + rand()%30);
    m_body->rotate(vec3(0, 0, DEGTORAD(rand()%360)));

    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);
  }

  virtual ~ASp_Asteroid()
  {
    msh->release();
    mtl->release();
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);

    angle += (0.0001f * t);

    mtx4 mrt;
    vec3 vrt(0.5f * angle, 0.7f * angle, 0);

    mrt.rotate(vrt);
    m_visual->m_transform = mrt * m_visual->m_transform;
  }
};

class ASp_AsteroidSpawner: public kgmActor
{
  KGM_OBJECT(ASp_AsteroidSpawner);

  kgmIGame*  game;
  u32        m_time_prev;
  u32        m_interval;

public:
  ASp_AsteroidSpawner(kgmIGame* g)
  {
    game        = g;
    m_time_prev = kgmTime::getTicks();
    m_interval  = 10000 + rand() % 10000;
  }

  void update(u32 ms)
  {
    kgmGameObject::update(ms);

    u32 ctick = kgmTime::getTicks();

    if(ctick - m_time_prev > m_interval)
    {
      ASp_Asteroid* as = new ASp_Asteroid(game, 0);
      as->setId("asteroid");

      as->timeout(10000 + rand() % 10000);
      as->setPosition(m_position);
      as->setParent(this);
      game->gAppend(as);
      as->release();

      m_time_prev = kgmTime::getTicks();
    }
  }
};

class ASp_SpacerSpawner: public kgmGameObject
{
  KGM_OBJECT(ASp_SpacerSpawner);

public:
  ASp_SpacerSpawner(kgmIGame* g)
  {

  }
};

class ASp_Flame: public kgmGameObject
{
  KGM_OBJECT(ASp_Flame);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  ASp_Flame(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_depth        = false;
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture("fire_a.tga");

    particles->m_count   = 10;
    particles->m_speed   = 1.1;
    particles->div_speed = 1.0;
    particles->m_life    = 2000;
    particles->div_life  = 1.0;
    particles->build();
    m_visual->set(material);
    m_visual->set(particles);
  }

  virtual ~ASp_Flame()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_FlameA: public ASp_Flame
{
  KGM_OBJECT(ASp_FlameA);
public:
  ASp_FlameA(kgmIGame* g)
    :ASp_Flame(g)
  {

  }
};

class ASp_Smoke: public kgmGameObject
{
  KGM_OBJECT(ASp_Smoke);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;
public:
  ASp_Smoke(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture("smoke_a.tga");

    particles->m_speed = .01;
    particles->m_count = 10;
    particles->m_life  = 5000;
    particles->div_speed = .5;
    particles->build();

    m_visual->set(material);
    m_visual->set(particles);
  }

  virtual ~ASp_Smoke()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_SmokeA: public ASp_Smoke
{
  KGM_OBJECT(ASp_SmokeA);
public:
  ASp_SmokeA(kgmIGame* g)
    :ASp_Smoke(g)
  {

  }
};

class ASp_Explode: public kgmGameObject
{
  KGM_OBJECT(ASp_Explode);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  ASp_Explode(kgmIGame* g, vec3 pos, vec3 vol, kgmString tid = "fire_a.tga")
  {
    timeout(3000);

    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_depth        = false;
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture(tid);

    particles->direction = vec3(1, 1, 0.4);
    particles->volume    = vol;
    particles->m_speed = .9;
    particles->m_count = 10;
    particles->m_life  = 2000;
    particles->m_loop  = false;
    particles->st_size = .1;
    particles->en_size = 2.0;
    particles->div_life = 1.0;

    particles->build();

    m_visual->set(material);
    m_visual->set(particles);

    setPosition(pos);
  }

  virtual ~ASp_Explode()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_ExplodeA: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeA);
public:
  ASp_ExplodeA(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
  {
  }
};

class ASp_ExplodeB: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeB);
public:
  ASp_ExplodeB(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
  {
    material->m_tex_color  = g->getResources()->getTexture("smoke_a.tga");
    particles->m_count     = 10;
    particles->build();
  }
};

class ASp_ExplodeC: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeC);
public:
  ASp_ExplodeC(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
  {
    particles->m_fade      = false;
    particles->m_count     = 50;
    particles->st_size     = 0.01;
    particles->en_size     = 0.05;
    particles->m_speed     = 60.0;
    particles->div_speed     = .5;

    particles->build();
  }
};

class ASp_CosmicDust: public kgmGameObject
{
  KGM_OBJECT(ASp_CosmicDust);
public:
  ASp_CosmicDust(kgmIGame* g)
  {
    kgmParticles* ptl = new kgmParticles();
    kgmMaterial*  mtl = new kgmMaterial();

    mtl->m_depth        = false;
    mtl->m_blend        = true;
    mtl->m_srcblend     = gcblend_srcalpha;
    mtl->m_dstblend     = gcblend_srcialpha;
    mtl->m_shader       = kgmMaterial::ShaderBlend;
    //mtl->m_tex_color    = g->getResources()->getTexture("smoketex.tga");
    mtl->m_tex_color    = g->getResources()->getTexture("smoketex_red.tga");

    ptl->m_loop    = true;
    ptl->m_speed   = .01;
    ptl->m_count   = 10;
    ptl->m_life    = 150000;
    ptl->div_life  = .25;
    ptl->div_speed = .25;
    ptl->st_size   = 2;
    ptl->en_size   = 3;
    ptl->volume    = vec3(5, 5, 5);
    ptl->build();

    //u32 color = KGM_RGBA(0xff, 0xff, 0xff, 155 + rand() % 100);

    for (u32 i = 0; i < ptl->m_count; i++)
    {
      ptl->m_particles[i].col.color = KGM_RGBA(0xff, 0xff, 0xff, 155 + rand() % 100);
    }

    m_visual = new kgmVisual();
    m_visual->set(ptl);
    m_visual->set(mtl);

    ptl->release();
    mtl->release();
  }
};

class ASp_Result: public kgmGameObject
{
  KGM_OBJECT(ASp_Result);

  kgmIGame* game;
  kgmString text;
  s32       result;

public:
  ASp_Result(kgmIGame* g, u32 time, s32 res, kgmString txt)
  {
    game = g;
    text = txt;
    result = res;
    timeout(time);

    int cx, cy, cw, ch, tw, tl;
    game->getWindow()->getRect(cx, cy, cw, ch);

    tl = txt.length();

    if(tl < 1)
      tl = 1;

    tw = 300 / tl;

    m_visual  = new kgmVisual();
    kgmText* text = new kgmText();
    text->m_rect  = uRect(10, ch - 2 * tw, 300, 2 * tw);
    text->m_text  = txt;
    text->m_size  = tw;

    m_visual->set(text);
    text->release();
  }

  ~ASp_Result()
  {
    int k=0;
  }

  s32 getResult()
  {
    return result;
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }

  void exit()
  {
    game->getLogic()->action(kgmILogic::ACTION_GAMEOBJECT, this, "result");    
  }
};

#endif // ASPACEROBJECTS_H
