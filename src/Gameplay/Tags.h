#pragma once

enum class Tag1 : uint32_t
{
    None,
    TPlayer,
    TWeapon,
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

    //Ammos
    TShotgunAmmo,
    TBowAmmo,
    TRiffleAmmo,

    //Mobs
    TMogwai,
    TJiangshi,
    THarpie
};
