#pragma once

enum class PrimaryTag
{
    None,
    TPlayer,
    TWeapon,
    TThrowableWeapon,
    TCollectible,
    TAmmo,
    TGround,
    TEnemy,
    TProjectile,

    TMiscellaneous
};

enum class SecondaryTag
{
    None,

    //Weapon
    TBlunderBuss,
    TMusket,

    //Collectibles
    TBomb,
    TDynamite,
    TNoodles,
    TNem,
    TRice,

    //Ammos
    THeavyAmmo,
    TNormalAmmo,
    TLightAmmo,

    //Mobs
    TMogwai,
    TJiangshi,
    TGuHuoNiao
};
