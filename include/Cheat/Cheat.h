#pragma once
class Cheat
{
  public:
    virtual void Run() = 0;
    virtual void Menu() = 0;
    virtual ~Cheat(){};

  protected:
    bool bEnabled = false;
};