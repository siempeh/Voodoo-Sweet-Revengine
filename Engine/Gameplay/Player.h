#pragma once

#include "Framework\States.h"
#include "Assets\Assets.h"
#include "Graphsicks\Renderer.h"
#include "Keyboard.h"
#include "Utility\DrawUtils.h"

// ****************************************************************************
class Player
{
public:
  enum Sprite
  {
    Idle,
    Move,
    Run,
    Jump,
  };

public:
  Player(Renderer& renderer);

  void Update()
  {
    movement.Update();
  }
  void Input(Keyboard& input)
  {
    movement.Input(input);
  }
  void Draw()
  {
	b2Vec2 pos = b2Vec2(x - 0.5f * width, y - 0.5f * height);
	b2Vec2 size = b2Vec2(width, height);
	RenderQuad<ForegroundShader>(renderer, pos, size,
		flip_sprite, false, Colors::Cyan, &sprites[curr_sprite]);
  }

  void SetSprite(Sprite sprite)
  {
    curr_sprite = sprite;
  }
  void SetFlipped(bool flipped)
  {
    flip_sprite = flipped;
  }
  
public:
  float x, y;
  float width, height;

private:
  Renderer& renderer;
  
  StateMachine<Player> movement;
  Texture sprites[4];

  Sprite curr_sprite;
  bool flip_sprite;

};

// ****************************************************************************
class IdleState : public IState<Player>
{
public:
  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

};
class MoveState : public IState<Player>
{
public:
  MoveState(float direction)
    : direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

private:
  float direction;

};
class RunState : public IState<Player>
{
public:
  RunState(float direction)
    :
    direction(direction)
  {}

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

private:
  float direction;

};
class JumpState : public IState<Player>
{
public:
  JumpState()
    :
    direction(0.f),
    velocity(500.f),
    jump_y(0.f),
    fart("fart0.wav")
  {
  }

  void OnEnter(Player& player) override;
  void OnExit() override;

  State Update() override;
  State Input(Keyboard& input) override;

private:
  float base_y;
  float jump_y;
  float velocity;
  float direction;
  Audio fart;

};
