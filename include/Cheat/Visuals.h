#pragma once
#include "Cheat.h"
class Visuals : public Cheat
{
  public:
    Visuals();
    virtual void Run() override;
    virtual void Menu() override;

  private:
    // show team
    bool bBox = true;
    bool bAngle = true;
    bool bName = false;
    bool bLine = false;
    bool bHealthBar = false;
    bool bTeam = false;
    static constexpr float PLAYER_WIDTH = 0.5f;
};
