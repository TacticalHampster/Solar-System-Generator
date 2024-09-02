#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "utils.h"
#include "gen_star.h"
#include "gen_planet_props.h"
#include "gen_planet_orbit.h"
#include "gen_planet_atmos.h"
#include "gen_satellites.h"

#ifndef GEN_H
#define GEN_H

struct Planet* generate_system(struct Star *star_ptr, int *num_orbits);
void pick_feature(struct Planet *planet_ptr);

//This function generates the planets in the system.
struct Planet* generate_system(struct Star *star_ptr, int *num_orbits)
{
    //Declare variables
    int frost_index = 0,
        hab_index   = 0,
        kuip_index  = 0,
        giant_max   = false;
    double mass_max = GAS_MASS_MAX,
           dist     = 0.0;
    char logtext[100];

    //Gen star
    sprintf(logtext, "Generating star...\n");
    print_log(logtext);

    gen_star(star_ptr);

    //Create stable orbits
    double *orbits = gen_orbits(star_ptr, num_orbits);

    sprintf(logtext, "\nGenerating %i planets\n", *num_orbits);
    print_log(logtext);

    char *types = pick_types(star_ptr, num_orbits, orbits);

    //Create main planets array
    struct Planet *planets = (struct Planet*)malloc(sizeof(struct Planet)*(*num_orbits));
    if (planets == nullptr)
        crash(-100);

    //Zero planets
    for (int i = 0; i < *num_orbits; i++)
        zero_fields(&planets[i]);

    //Find the frost line
    for (int i = 0; i < *num_orbits; i++)
    {
        if (((star_ptr->zone_frost-1.2) <= orbits[i]) && (orbits[i] <= (star_ptr->zone_frost)+1.2))
        {
            if (types[i] == TYPE_DWF_PLNTSML)
            {
                frost_index = i+1;
            }
            else
                frost_index = i;

            break;
        }
    }

    sprintf(logtext, "\nLocated frost line at index %i\n", frost_index);
    print_log(logtext);

    //Generate
    struct Planet *controller;
    for (int i = 0; i < *num_orbits; i++)
    {
        sprintf(logtext, "\nCreating type %c planet for orbit %i (%f AU)\n", types[i], i+1, dist);
        print_log(logtext);
        planets[i].type = types[i];

        switch (types[i])
        {
            case TYPE_RCK_GREEN:
            case TYPE_WTR_GREEN:
                controller = nullptr;
                dist       = orbits[i];
                mass_max   = 0.0;
                hab_index  = i;
                break;
            case TYPE_BLT_KUIPR:
                controller = nullptr;
                dist       = orbits[i-1];
                mass_max   = 0.0;
                kuip_index = i;
                break;
            case TYPE_GAS_GIANT:
            case TYPE_GAS_SUPER:
            case TYPE_GAS_PUFFY:
            case TYPE_GAS_HOT:
                controller = nullptr;
                dist       = orbits[i];
                break;
            case TYPE_DWF_PLNTSML:
                controller = &planets[i-1];
                dist       = orbits[i-1];
                mass_max   = planets[i-1].mass;
                break;
            case TYPE_DWF_PLUTINO:
                controller = &planets[kuip_index];
                dist       = resonant_dist(planets[kuip_index-1].a, 3.0,2.0);
                mass_max   = planets[kuip_index].mass / (double)(*num_orbits-kuip_index);
                break;
            case TYPE_DWF_CUBEWNO:
                controller = &planets[kuip_index];
                dist       = rand_double(resonant_dist(planets[kuip_index-1].a, 3.0,2.0), resonant_dist(planets[kuip_index-1].a, 2.0,1.0));
                mass_max   = planets[kuip_index].mass / (double)(*num_orbits-kuip_index);
                break;
            case TYPE_DWF_TWOTINO:
                controller = &planets[kuip_index];
                dist       = resonant_dist(planets[kuip_index-1].a, 2.0,1.0);
                mass_max   = planets[kuip_index].mass / (double)(*num_orbits-kuip_index);
                break;
            case TYPE_DWF_SCATTER:
                controller = &planets[kuip_index];
                dist       = planets[kuip_index].a*2.0;
                mass_max   = planets[kuip_index].mass / (double)(*num_orbits-kuip_index);
            case TYPE_DWF_SEDNOID:
                controller = &planets[kuip_index];
                dist       = rand_double(planets[kuip_index].a*2.0, planets[kuip_index].a*5.0);
                mass_max   = planets[kuip_index].mass / (double)(*num_orbits-kuip_index);
                break;
            default:
                controller = nullptr;
                dist       = orbits[i];
                mass_max   = 0.0;
        }

        print_log("   Creating composition...\n");
        gen_planet_props(&planets[i], ((mass_max > 0.0) ? mass_max : 0.0));

        if (planets[i].type == TYPE_DWF_PLNTSML)
            mass_max = GAS_MASS_MAX;

        if ((planets[i].type == TYPE_GAS_GIANT ||
             planets[i].type == TYPE_GAS_SUPER ||
             planets[i].type == TYPE_GAS_PUFFY ||
             planets[i].type == TYPE_GAS_HOT ) &&
             !giant_max                         )
        {
            sprintf(logtext, "      Setting max mass for gas giants at %f M🜨\n", planets[i].mass);
            print_log(logtext);
            mass_max = planets[i].mass;
            giant_max = true;
        }

        print_log("   Creating orbit...\n");
        gen_planet_orbit(star_ptr, &planets[i], controller, dist, *num_orbits);

        print_log("   Creating atmosphere...\n");
        gen_planet_atmos(star_ptr, &planets[i]);

        print_log("   Creating satellites...\n");
        gen_satellites(star_ptr, &planets[i], orbits, dist, *num_orbits);

        print_log("   Picking interesting feature...\n");
        pick_feature(&planets[i]);
    }

    //Generate Kirkwood gaps in asteroid belts
    if (planets[frost_index-1].type == TYPE_BLT_INNER && planets[frost_index].type != '0')
    {
        print_log("\nDistributing inner belt...\n");
        double *belts = (double*)malloc(sizeof(double)*8);
        if (planets == nullptr)
            crash(-101);

        belts[0] = resonant_dist(planets[frost_index].a, 1.0,5.0);
        belts[1] = resonant_dist(planets[frost_index].a, 1.0,4.0);
        belts[2] = resonant_dist(planets[frost_index].a, 1.0,3.0);
        belts[3] = resonant_dist(planets[frost_index].a, 2.0,5.0);
        belts[4] = resonant_dist(planets[frost_index].a, 3.0,7.0);
        belts[5] = resonant_dist(planets[frost_index].a, 1.0,2.0);
        belts[6] = resonant_dist(planets[frost_index].a, 2.0,3.0);
        belts[7] = resonant_dist(planets[frost_index].a, 4.0,3.0);

        planets[frost_index-1].rings     = belts;
        planets[frost_index-1].num_rings = 8;
        planets[frost_index-1].radius    = belts[7] - belts[0];

        planets[frost_index].trojans = 1;
    }
    else if (planets[frost_index-2].type == TYPE_BLT_INNER && planets[frost_index].type != '0')
    {
        print_log("\nDistributing inner belt...\n");
        double *belts = (double*)malloc(sizeof(double)*8);
        if (planets == nullptr)
            crash(-101);

        belts[0] = resonant_dist(planets[frost_index].a, 1.0,5.0);
        belts[1] = resonant_dist(planets[frost_index].a, 1.0,4.0);
        belts[2] = resonant_dist(planets[frost_index].a, 1.0,3.0);
        belts[3] = resonant_dist(planets[frost_index].a, 2.0,5.0);
        belts[4] = resonant_dist(planets[frost_index].a, 3.0,7.0);
        belts[5] = resonant_dist(planets[frost_index].a, 1.0,2.0);
        belts[6] = resonant_dist(planets[frost_index].a, 2.0,3.0);
        belts[7] = resonant_dist(planets[frost_index].a, 4.0,3.0);

        planets[frost_index-2].rings     = belts;
        planets[frost_index-2].num_rings = 8;
        planets[frost_index-2].radius    = belts[7] - belts[0];

        planets[frost_index].trojans = 1;
    }

    //Calculate apparent magnitude of the star
    int tries = 0;
    do
    {
        sprintf(logtext, "\nCalculating relative magnitude of star (Attempt %i, hab_index %i)\n", tries, hab_index);
        print_log(logtext);

        star_ptr->rel_mag = star_ptr->abs_mag + (5.0 * (log10(planets[hab_index].a) - 6.31442));

        sprintf(logtext, "   Log of a:     %f\n"
                         "   Distance mod: %f\n"
                         "   Relative mag: %f\n",
            log10(planets[hab_index].a),
            5.0 * (log10(planets[hab_index].a) - 6.31442),
            star_ptr->rel_mag
        );
        print_log(logtext);

        tries++;

        if (tries > 1000)
            crash(-200);
    }
    while (star_ptr->rel_mag < -40 || star_ptr->rel_mag > 10);

    free((void*)orbits);
    free((void*)types );

    print_log("\nGeneration complete!\n\n\n");
    return planets;
}

//This function picks a random interesting feature for a planet.
void pick_feature(struct Planet *planet_ptr)
{
    char *(features[10]);
    int i = 0, chars = 70;

    if ((planet_ptr->type == TYPE_RCK_DESRT    &&
         planet_ptr->surf_temp <= KELVIN+200 ) ||
         planet_ptr->type == TYPE_RCK_DENSE  )
    {
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-192);
        sprintf(features[i], " Its surface is visibly cratered.");
        i++;
    }

    if ((planet_ptr->type == TYPE_RCK_DESRT ||
         planet_ptr->type == TYPE_RCK_GREEN  ) &&
         planet_ptr->surf_temp >= KELVIN+100)
    {
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-193);
        sprintf(features[i], " Tectonic and volcanic activity is evident on its surface.");
        i++;
    }

    if ((planet_ptr->type == TYPE_RCK_GREEN ||
        (planet_ptr->type == TYPE_WTR_HYCN && planet_ptr->subtype[1] != '3')) &&
         planet_ptr->surf_temp > KELVIN)
    {
        int continents = (int)round(rand_double(1.0, 6.0));
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-194);
        sprintf(features[i], " It has %i continent%s, covering about %.0f%% of its surface.",
            continents,
            (
                continents == 1
                ? ""
                : "s"
            ),
            planet_ptr->rck_cover*100.0
        );
        i++;
    }

    if ((planet_ptr->type == TYPE_WTR_HYCN || planet_ptr->type == TYPE_WTR_GREEN) && planet_ptr->subtype[1] == '3' && planet_ptr->surf_temp > KELVIN)
    {
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-195);
        sprintf(features[i], " Its surface is dominated by ocean with occasional islands.");
        i++;
    }

    if ((planet_ptr->type == TYPE_RCK_GREEN  ||
         planet_ptr->type == TYPE_WTR_HYCN   ||
         planet_ptr->type == TYPE_WTR_GREEN) &&
         planet_ptr->surf_temp <= KELVIN       )
    {

        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-196);
        sprintf(features[i], " Its surface is covered with ice.");
        i++;
    }

    if ((planet_ptr->type == TYPE_WTR_HYCN   ||
         planet_ptr->type == TYPE_WTR_GREEN   ) &&
         planet_ptr->surf_temp <= KELVIN)
    {
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-197);
        sprintf(features[i], " There are several cryovolcanoes on its surface.");
        i++;
    }

    if (planet_ptr->type == TYPE_GAS_GIANT ||
        planet_ptr->type == TYPE_GAS_SUPER  )
    {
        features[i] = (char*)malloc(sizeof(char)*chars);
        if (features[i] == nullptr)
            crash(-198);
        int sides   = (int)floor(rand_double(4.0, 9.0));
        int pole    = (int)round(rand_double(0.0, 1.0));
        if (sides == 4)
            sprintf(features[i], " There is a persistent square storm over its %s pole.",
                (
                    pole
                    ? "north"
                    : "south"
                )
            );
        else if (sides == 5)
            sprintf(features[i], " There is a persistent pentagonal storm over its %s pole.",
                (
                    pole
                    ? "north"
                    : "south"
                )
            );
        else if (sides == 6)
            sprintf(features[i], " There is a persistent hexagonal storm over its %s pole.",
                (
                    pole
                    ? "north"
                    : "south"
                )
            );
        else if (sides == 7)
            sprintf(features[i], " There is a persistent septagonal storm over its %s pole.",
                (
                    pole
                    ? "north"
                    : "south"
                )
            );
        else
            sprintf(features[i], " There is a persistent octagonal storm over its %s pole.",
                (
                    pole
                    ? "north"
                    : "south"
                )
            );
        i++;
    }

    if (planet_ptr->type == TYPE_GAS_GIANT ||
        planet_ptr->type == TYPE_GAS_SUPER ||
        planet_ptr->type == TYPE_ICE_GIANT  )
    {
        for (int j = 0; j < 3; j++)
        {
            features[i] = (char*)malloc(sizeof(char)*chars);
            if (features[i] == nullptr)
                crash(-199);
            sprintf(features[i], " A large, discolored storm rages in its %s hemisphere.",
                (
                    (int)round(rand_double(0.0, 1.0))
                    ? "northern"
                    : "southern"
                )
            );
            i++;
        }
    }

    int choice = 1;
    if (i == 0)
    {
        choice = 0;
        planet_ptr->feature = nullptr;
    }
    else if ((planet_ptr->type == TYPE_GAS_GIANT  ||
              planet_ptr->type == TYPE_GAS_SUPER  ||
              planet_ptr->type == TYPE_ICE_GIANT) &&
              rand_double(0.0, 1.0) <= FEATR_CHANCE)
    {
        choice = round(rand_double(0.0, i-1));
        planet_ptr->feature = (char*)malloc(sizeof(char)*chars);
        sprintf(planet_ptr->feature, "%s", features[choice]);
    }
    else if (planet_ptr->type == TYPE_GAS_GIANT ||
             planet_ptr->type == TYPE_GAS_SUPER ||
             planet_ptr->type == TYPE_GAS_PUFFY ||
             planet_ptr->type == TYPE_GAS_HOT   ||
             planet_ptr->type == TYPE_ICE_GIANT  )
    {
        choice = 0;
        planet_ptr->feature = nullptr;
    }
    else
    {
        choice = rand_double(0.0, i-1);
        planet_ptr->feature = (char*)malloc(sizeof(char)*chars);
        strcpy(planet_ptr->feature, features[choice]);
    }

    for (int j = 0; j < i; j++)
        free((void*)features[j]);
}


#endif // GEN_H
