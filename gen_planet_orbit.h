#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "utils.h"
#include "gen_planet_atmos.h"
#include "gen_satellites.h"

#ifndef ORBITS_H
#define ORBITS_H

void gen_planet_orbit(struct Star *star_ptr, struct Planet *planet_ptr, struct Planet *belt_ptr, double orbit, int num);

void rocky_orbit(struct Planet *planet_ptr);
void giant_orbit(struct Planet *planet_ptr);

//This is the top level function. It should be the only function called outside this file.
void gen_planet_orbit(struct Star *star_ptr, struct Planet *planet_ptr, struct Planet *belt_ptr, double orbit, int num)
{
    planet_ptr->a = orbit;
    planet_ptr->e = rand_double(0.001, 0.01);

    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
        case TYPE_RCK_DESRT:
        case TYPE_WTR_HYCN:
            planet_ptr->e = expo_double(0.0, 0.2, 0.584*pow(num,-1.2))                              ; //Eccentricity
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / star_ptr->mass)                           ; //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827                            ; //Orbital velocity
            planet_ptr->i = rand_double(0.0,   2.0)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0); //Inclination
            planet_ptr->l = rand_double(0.0, 360.0)                                                 ; //LoAN
            planet_ptr->p = rand_double(0.0, 360.0)                                                 ; //AoP
            rocky_orbit(planet_ptr);
            break;

        case TYPE_RCK_GREEN:
        case TYPE_WTR_GREEN:
            planet_ptr->e = expo_double(0.0, 0.2, 0.584*pow(num,-1.2))   ; //Eccentricity
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / star_ptr->mass); //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827 ; //Orbital velocity
            planet_ptr->i =  0.0                                         ; //Inclination
            planet_ptr->l = 90.0                                         ; //LoAN
            planet_ptr->p =  0.0                                         ; //AoP
            rocky_orbit(planet_ptr);
            break;

        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            planet_ptr->e = expo_double(0.001, 0.1, 0.584*pow(num,-1.2))                            ; //Eccentricity
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / star_ptr->mass)                           ; //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827                            ; //Orbital velocity
            planet_ptr->i = rand_double(0.0,   2.0)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0); //Inclination
            planet_ptr->l = rand_double(0.0, 360.0)                                                 ; //LoAN
            planet_ptr->p = rand_double(0.0, 360.0)                                                 ; //AoP
            giant_orbit(planet_ptr);
            break;

        case TYPE_BLT_KUIPR:
            planet_ptr->a = (resonant_dist(orbit, 3.0,2.0) + orbit*1.3) / 2.0;
            /* FALL THROUGH */

        case TYPE_BLT_INNER:
            planet_ptr->e = expo_double(0.001, 0.1, 0.584*pow(num,-1.2)) ; //Eccentricity
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / star_ptr->mass); //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827 ; //Orbital velocity
            planet_ptr->i = rand_double(0.0,   10.0)                     ; //Inclination
            planet_ptr->l = rand_double(0.0, 360.0)                      ; //LoAN
            planet_ptr->p = rand_double(0.0, 360.0)                      ; //AoP
            break;

        case TYPE_DWF_PLNTSML:
            planet_ptr->a = fudge_double(belt_ptr->a)                                                          ; //Semimajor axis
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / (star_ptr->mass + planet_ptr->mass))                 ; //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827                                       ; //Orbital velocity
            planet_ptr->i = rand_double(0.0, belt_ptr->height)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0); //Inclination
            planet_ptr->l = fudge_double(belt_ptr->l)                                                          ; //LoAN
            planet_ptr->p = fudge_double(belt_ptr->p)                                                          ; //AoP
            belt_ptr->subtype[1] = '1';
            break;

        case TYPE_DWF_SEDNOID:
            planet_ptr->e = rand_double(0.75, 1.0);
            /* FALL THROUGH */

        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
            planet_ptr->a = fudge_double(orbit)                                                                ; //Semimajor axis
            planet_ptr->y = sqrt( pow(planet_ptr->a, 3) / (star_ptr->mass + planet_ptr->mass))                 ; //Year length
            planet_ptr->v = sqrt(star_ptr->mass / planet_ptr->a)*29.7827                                       ; //Orbital velocity
            planet_ptr->i = rand_double(0.0, belt_ptr->height)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0); //Inclination
            planet_ptr->l = fudge_double(belt_ptr->l)                                                          ; //LoAN
            planet_ptr->p = fudge_double(belt_ptr->p)                                                          ; //AoP
            rocky_orbit(planet_ptr);
            break;

        default:
            crash(-230);
    }
    print_log("      Generated orbital parameters\n");
}

//Assigns orientation and day length of terrestrial planets.
void rocky_orbit(struct Planet *planet_ptr)
{
    //Assign tilt
    planet_ptr->axial_tilt  = rand_double(0.0, 90.0); //Axial tilt

    //The day length will have 1d3+1 digits
    int die = (int)floor(rand_double(0,3));

    if (die == 0)
        planet_ptr->day_length = rand_double(DAY_MIN, 100);
    else if (die == 1)
        planet_ptr->day_length = rand_double(100, 1000);
    else
        planet_ptr->day_length = rand_double(1000, DAY_MAX);

    //Retrograde spin is caused by impacts, so only slow spins can be retrograde
    if (planet_ptr->day_length >= 1500.0)
        planet_ptr->axial_tilt += (rand_double(0.0,1.0) < RET_PROB ? 180.0 : 0.0);
}

//Assigns orientation and day length of giant planets.
void giant_orbit(struct Planet *planet_ptr)
{
    //Assign tilt
    planet_ptr->axial_tilt = rand_double(0.0,  90.0); //Axial tilt

    //Giant planets have fast spins since they're not rigid
    planet_ptr->day_length = rand_double(DAY_MIN, 24.0);
}
#endif // ORBITS_H
