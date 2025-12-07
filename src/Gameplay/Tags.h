#pragma once

enum class Tag1 : uint32_t
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

enum class Tag2 : uint32_t
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
    TShotgunAmmo,
    TBowAmmo,
    TRiffleAmmo,

    //Mobs
    TMogwai,
    TJiangshi,
    TGuHuoNiao
};
