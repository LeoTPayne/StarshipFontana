#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : score(0), fire(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);

  const int number_of_wall = 5;
  for(int i=0; i<number_of_wall; i++) {
    // place an wall at width/number_of_walls * i
    auto walls = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2((canvas_w/number_of_wall) * i+40, 200.0f);
    walls->SetPosition(pos);
    wall.push_back(walls);
  }
	

   const int number_of_coins = 5;
   for(int i=0; i<number_of_coins; i++) {
    // place an wall at width/number_of_coins * i
    auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
    auto pos  = Point2((canvas_w/number_of_coins)*i+50, 400.0f);
    coin->SetPosition(pos);
     coins.push_back(coin);
  }
}


SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */

void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    {
      for(auto w : wall) {
      if(player->CollidesWith(w)) {
	player->HandleCollision();
	player->GoEast();
       }
      }
    }
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
	{
      for(auto w : wall) {
      if(player->CollidesWith(w)) {
	player->HandleCollision();
	player->GoWest();
       }
      }
    }
    break;
  case SFEVENT_PLAYER_UP:
    player->GoNorth();
	{
      for(auto w : wall) {
      if(player->CollidesWith(w)) {
	player->HandleCollision();
	player->GoSouth();
       }
    }
}
    break;
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();
	{
      for(auto w : wall) {
      if(player->CollidesWith(w)) {
	player->HandleCollision();
	player->GoNorth();
       }
      }
    }
    break;
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  for(auto c: coins) {
  }

  // Update enemy positions
  for(auto w : wall) {
    // do something here
  }
  // Detect collisions
  for(auto p : projectiles) {
    for(auto w : wall) {
      if(p->CollidesWith(w)) {
        p->HandleCollision();
      }
    }
}
    for(auto p : projectiles)
 {
   	for(auto c : coins)
{
      if(p->CollidesWith(c)) 
{
        c->HandleCollision();
	p->HandleCollision();
	score=score+10;
	std::wcout<<score<<std::endl;
}
	if (score>=50){
	is_running=false;}
  	}
 }

  // remove dead wall (the long way)
  list<shared_ptr<SFAsset>> tmp;

 for(auto c : coins) {
    if(c->IsAlive()) {
      tmp.push_back(c);
    }
  }
  coins.clear();
  coins = list<shared_ptr<SFAsset>>(tmp);
}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player
  player->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto w: wall) {
    if(w->IsAlive()) {w->OnRender();}
  }

  for(auto c: coins) {
      if(c->IsAlive()) {c->OnRender();}

}

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
