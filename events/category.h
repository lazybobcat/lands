#ifndef CATEGORY_H
#define CATEGORY_H

namespace Category
{
    enum Type
    {
        None            = 0,
        SoundEffect     = 1 << 0,
        ParticleSystem  = 1 << 1,
        Player          = 1 << 2,
        Ghost           = 1 << 3,
        EntityLayer     = 1 << 4,
        AlliedBullet    = 1 << 5,
        EnemyBullet     = 1 << 6,
        Light           = 1 << 7,
        LightLayer      = 1 << 8,
        Wall            = 1 << 9,
        Exit            = 1 << 10,
        Decoration      = 1 << 11,
        Key             = 1 << 12,
        Door            = 1 << 13,
        UILayer         = 1 << 14,
        // ...
        //MixedCategory        = Category1 | Category2,
        All             = ~0
    };
}

#endif // CATEGORY_H
