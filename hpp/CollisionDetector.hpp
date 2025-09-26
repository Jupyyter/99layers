#pragma once
class Object;
class CollisionDetector {
public:
friend class Sprite;
    bool isGrounded,solid;
    CollisionDetector();
    // Enum to represent the side of collision
    enum class CollisionInfo {
        None,
        Top,
        Bottom,
        Left,
        Right
    };
    // Check for collision between an object and obstacles
    CollisionInfo CollisionSide(const sf::FloatRect& bounds1, const sf::FloatRect& bounds2);
    virtual void onCollision(Sprite *other){};
    virtual void collide(Sprite *other){
         if (solid&&other->getName()=="Terrain")
       {
        Sprite *en1 = dynamic_cast<Sprite *>(this);
              switch (CollisionDetector::CollisionSide(en1->getBounds(), other->getBounds()))
              {
              case CollisionInfo::Left:
                     if (!(other->getBounds().top > en1->getBounds().top && other->getBounds().top - en1->getBounds().top > 27 && en1->velocity.y >= 0)) // in case of stairs
                     {
                            en1->position.x = other->getBounds().left + other->getBounds().width;
                            en1->velocity.x = 0;
                     }
                     break;
              case CollisionInfo::Right:
                     if (!(other->getBounds().top > en1->getBounds().top && other->getBounds().top - en1->getBounds().top > 27 && en1->velocity.y >= 0))
                     {
                            en1->position.x = other->getBounds().left - en1->getBounds().width;
                            en1->velocity.x = 0;
                     }
                     break;
              case CollisionInfo::Bottom:
                     if (en1->velocity.y >= 0)
                     {
                            en1->position.y = other->getBounds().top - en1->getBounds().height;
                            en1->velocity.y = 0;
                            isGrounded = true;
                     }
                     break;
              case CollisionInfo::Top:
              {
                     en1->position.y = other->getBounds().top + other->getBounds().height;
                     en1->velocity.y = 0;

                     break;
              }
              default:
                     break;
              }
       }
    };
};