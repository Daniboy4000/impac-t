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


#include "stdafx.h"

namespace Impact {


  Block::Block(int index, Game *game)
    : Body(Body::BodyType::Block, game)
    , mGravityScale(2.f)
  {
    setZIndex(Body::ZIndex::Intermediate + 0);
    setEnergy(100);
    mName = std::string("Block");

    const sf::Texture &texture = mGame->level()->tile(index).texture;
    sf::Image img;
    img.create(texture.getSize().x + 2 * TextureBorderWidth, texture.getSize().y + 2 * TextureBorderWidth, sf::Color(255, 255, 255, 0));
    img.copy(texture.copyToImage(), TextureBorderWidth, TextureBorderWidth, sf::IntRect(0, 0, 0, 0), true);
    mTexture.loadFromImage(img);

    mShader.loadFromFile(gShadersDir + "/fallingblock.frag", sf::Shader::Fragment);
    mShader.setParameter("uAge", .0f);
    mShader.setParameter("uBlur", .0f);
    mShader.setParameter("uWeight", 0.17f);
    mShader.setParameter("uColor", sf::Color(255, 255, 255, 255));

    mSprite.setTexture(mTexture);
    mSprite.setOrigin(.5f * mTexture.getSize().x, .5f * mTexture.getSize().y);

    const int W = texture.getSize().x;
    const int H = texture.getSize().y;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.angle = .0f;
    bd.linearDamping = .5f;
    bd.angularDamping = .5f;
    bd.gravityScale = .0f;
    bd.allowSleep = true;
    bd.awake = false;
    bd.fixedRotation = false;
    bd.bullet = false;
    bd.active = true;
    bd.userData = this;
    mBody = game->world()->CreateBody(&bd);

    b2PolygonShape polygon;
    const float32 hs = .5f * Game::InvScale;
    const float32 hh = hs * H;
    const float32 xoff = hs * (W - H);
    polygon.SetAsBox(xoff, hh);

    b2FixtureDef fdBox;
    fdBox.shape = &polygon;
    fdBox.density = 800.f;
    fdBox.friction = mGame->world()->GetGravity().y;
    fdBox.restitution = .04f;
    fdBox.userData = this;
    mBody->CreateFixture(&fdBox);

    b2CircleShape circleL;
    circleL.m_p.Set(-xoff, 0.f);
    circleL.m_radius = hh;

    b2FixtureDef fdCircleL;
    fdCircleL.shape = &circleL;
    fdCircleL.density = 800.f;
    fdCircleL.friction = mGame->world()->GetGravity().y;
    fdCircleL.restitution = 0.04f;
    fdCircleL.userData = this;
    mBody->CreateFixture(&fdCircleL);

    b2CircleShape circleR;
    circleR.m_p.Set(xoff, 0.f);
    circleR.m_radius = hh;

    b2FixtureDef fdCircleR;
    fdCircleR.shape = &circleR;
    fdCircleR.density = 800.f;
    fdCircleR.friction = mGame->world()->GetGravity().y;
    fdCircleR.restitution = 0.04f;
    fdCircleR.userData = this;
    mBody->CreateFixture(&fdCircleR);
  }


  void Block::onUpdate(float elapsedSeconds)
  {
    UNUSED(elapsedSeconds);
    const b2Transform &tx = mBody->GetTransform();
    mSprite.setPosition(Game::Scale * tx.p.x, Game::Scale * tx.p.y);
    mSprite.setRotation(rad2deg(tx.q.GetAngle()));
    mShader.setParameter("uAge", age().asSeconds());
  }


  void Block::onDraw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    states.shader = &mShader;
    target.draw(mSprite, states);
  }


  bool Block::hit(float impulse)
  {
    bool destroyed = Body::hit(int(impulse));
    if (!destroyed) {
      mBody->SetGravityScale(mGravityScale);
      mShader.setParameter("uColor", sf::Color(sf::Color(255, 255, 255, 0xbf)));
      mShader.setParameter("uBlur", 0.02f);
    }
    return destroyed;
  }


  void Block::setGravityScale(float32 gravityScale)
  {
    mGravityScale = gravityScale;
  }

}