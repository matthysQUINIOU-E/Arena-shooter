#pragma once

enum class Tag // Game Objects Tags
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
    TBullet,
    TMiscellaneous,
    TSpawner,
    TMapObject,

    THeal,

    //Weapon
    TMelee,
    TBlunderBuss,
    TMusket,
    TStarwheel,

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
