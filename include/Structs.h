#pragma once
#include "Vec.h"

class Entity
{
  public:
    Vec3 getHeadPos()
    {
        return pos;
    }
    Vec3 getPos()
    {
        return { pos.x, pos.y, pos.z - 14.f };
    }

  public:
    Vec3 pos;            // 0x0000
    Vec3 vel;            // 0x000C
    char pad_0018[24];   // 0x0018
    Vec3 pos2;           // 0x0030
    Vec3 angle;          // 0x003C
    char pad_0048[304];  // 0x0048
    int32_t health;      // 0x0178
    int32_t maxHealth;   // 0x017C
    int32_t armor;       // 0x0180
    char pad_0184[8];    // 0x0184
    int32_t weaponSlot;  // 0x018C
    int32_t shotDelay;   // 0x0190
    char pad_0194[224];  // 0x0194
    char name[260];      // 0x0274
    char team[260];      // 0x0378
    char pad_047C[116];  // 0x047C
};

template <typename T>
struct Vector
{
    T* data;
    int32_t max;
    int32_t size;

    T* begin()
    {
        return data;
    }

    T* end()
    {
        return data + size;
    }

    const T* begin() const
    {
        return data;
    }

    const T* end() const
    {
        return data + size;
    }
};

struct physent
{
    Vec3 o;
    Vec3 vel;
    Vec3 falling;
    Vec3 deltapos;
    Vec3 newpos;
    float yaw;
    float pitch;
    float roll;
    float maxspeed;
    float radius;
    float eyeheight;
    float aboveeye;
    float xradius;
    float yradius;
    float zmargin;
    Vec3 floor;
    unsigned __int16 timeinair;
    unsigned __int8 inwater;
    bool jumping;
    char move;
    char strafe;
    unsigned __int8 physstate;
    unsigned __int8 state;
    unsigned __int8 editstate;
    unsigned __int8 type;
    unsigned __int8 collidetype;
    bool blocked;
};
struct entitylight
{
    Vec3 color;
    Vec3 dir;
    int millis;
};
/* 932 */
struct animinfo
{
    int anim;
    int frame;
    int range;
    int basetime;
    float speed;
    unsigned int varseed;
};

/* 933 */
struct animinterpinfo
{
    animinfo prev;
    animinfo cur;
    int lastswitch;
    void* lastmodel;
};
struct dynent : physent
{
    bool k_left;
    bool k_right;
    bool k_up;
    bool k_down;
    entitylight light;
    animinterpinfo animinterp[3];
    void* ragdoll;
    void* query;
    int lastrendered;
    unsigned __int8 occluded;
};
/* 2219 */
struct fpsstate
{
    int health;
    int maxhealth;
    int armour;
    int armourtype;
    int quadmillis;
    int gunselect;
    int gunwait;
    int ammo[12];
    int aitype;
    int skill;
};

struct fpsent : dynent, fpsstate
{
    int weight;
    int clientnum;
    int privilege;
    int lastupdate;
    int plag;
    int ping;
    int lifesequence;
    int respawned;
    int suicided;
    int lastpain;
    int lastaction;
    int lastattackgun;
    bool attacking;
    int attacksound;
    int attackchan;
    int idlesound;
    int idlechan;
    int lasttaunt;
    int lastpickup;
    int lastpickupmillis;
    int lastbase;
    int lastrepammo;
    int flagpickup;
    int tokens;
    Vec3 lastcollect;
    int frags;
    int flags;
    int deaths;
    int totaldamage;
    int totalshots;
    void* edit;
    float deltayaw;
    float deltapitch;
    float deltaroll;
    float newyaw;
    float newpitch;
    float newroll;
    int smoothmillis;
    char name[260];
    char team[260];
    char info[260];
    int playermodel;
    void* ai;
    int ownernum;
    int lastnode;
    Vec3 muzzle;
};