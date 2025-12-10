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
    TMiscellaneous,
    TSpawner
}; 

enum class SecondaryTag
{
    None,

    THeal,

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
