/*  

    Copyright (c) 2015 Oliver Lau <ola@ct.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef __PARTICLESYSTEM_H_
#define __PARTICLESYSTEM_H_

#include <Box2D/Box2D.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Body.h"
#include "Impact.h"

namespace Impact {
  
  struct SimpleParticle 
  {
    b2Body *body;
    sf::Time lifeTime;
    bool dead;
    sf::Sprite sprite;
  };


  struct ParticleSystemDef
  {
    ParticleSystemDef(Game *game, const b2Vec2 &pos, bool ballCollisionEnabled = false, int count = 50)
      : game(game)
      , pos(pos)
      , ballCollisionEnabled(ballCollisionEnabled)
      , count(count)
    {}
    Game *game;
    b2Vec2 pos;
    bool ballCollisionEnabled;
    int count;
  };

  class ParticleSystem : public Body
  {
  public:
    ParticleSystem(const ParticleSystemDef &);
    virtual ~ParticleSystem();

    // Body implementation
    virtual void onUpdate(float elapsedSeconds);
    virtual void onDraw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual BodyType type(void) const { return Body::BodyType::Particle; }

  protected:
    static const float32 DefaultDensity;
    static const float32 DefaultFriction;
    static const float32 DefaultRestitution;

    static const sf::Time sMaxAge;
    static const sf::Color sColor;

    std::vector<SimpleParticle> mParticles;
    
  };

}

#endif // __PARTICLESYSTEM_H_
