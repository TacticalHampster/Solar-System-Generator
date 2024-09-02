#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "utils.h"


#ifndef SATELLITES_H
#define SATELLITES_H

void gen_satellites(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num);

void moon_props(struct Moon *moon_ptr);
void gen_ice_major(struct Planet *planet_ptr, struct Moon *moon_ptr, double mass_max);
void gen_ice_minor(struct Planet *planet_ptr, struct Moon *moon_ptr);
void gen_rck_major(struct Planet *planet_ptr, struct Moon *moon_ptr, double mass_max);
void gen_rck_minor(struct Planet *planet_ptr, struct Moon *moon_ptr);

void rckmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num);
void gasmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num);
void dwfmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num);

void gen_ter_moon_orbit(struct Star *star_ptr, struct Moon *moon_ptr);
void gen_gas_moon_orbit(struct Star *star_ptr, struct Moon *moon_ptr, double a);

//This is the top level function. It should be the only function called outside this file.
void gen_satellites(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num)
{
    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
        case TYPE_RCK_DESRT:
        case TYPE_WTR_HYCN:
            rckmn_orbit(star_ptr, planet_ptr, orbits, orbit, num);
            break;
        case TYPE_RCK_GREEN:
        case TYPE_WTR_GREEN:
            planet_ptr->num_moons = 1;

            //Create moon array
            planet_ptr->moons = (struct Moon*)malloc(sizeof(struct Moon)*planet_ptr->num_moons);
            if (planet_ptr->moons == nullptr)
                crash(-141);

            //Create moon
            gen_rck_major(planet_ptr, &(planet_ptr->moons[0]), 0.0);
            gen_ter_moon_orbit(star_ptr, &(planet_ptr->moons[0]));
            break;
        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            gasmn_orbit(star_ptr, planet_ptr, orbits, orbit, num);
            break;
        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
            dwfmn_orbit(star_ptr, planet_ptr, orbits, orbit, num);
            break;
        case TYPE_BLT_INNER:
        case TYPE_BLT_KUIPR:
        case TYPE_DWF_PLNTSML:
        case TYPE_DWF_SEDNOID:
            break;
        default:
            crash(-250);
    }
}

//Calculates the properties and composition of a moon given its mass.
void moon_props(struct Moon *moon_ptr)
{
    double avg_rad = (moon_ptr->rad_a + moon_ptr->rad_b + moon_ptr->rad_c)/3.0;

    moon_ptr->bulk_dens = (moon_ptr->bulk_metal*7.87) + (moon_ptr->bulk_rock*3.25) + (moon_ptr->bulk_ices*0.93);
    moon_ptr->mass      = moon_ptr->rad_a * moon_ptr->rad_b * moon_ptr->rad_c * (moon_ptr->bulk_dens*5.513) * pow((1.0/6378.0), 3.0);
    moon_ptr->surf_grav = (moon_ptr->mass / pow(avg_rad/6378.0, 2.0));
    moon_ptr->esc_v     = sqrt(moon_ptr->mass / avg_rad/6378.0)*1118.6;

    moon_ptr->subtype = '\0';
}

//This function generates an icy major moon.
void gen_ice_major(struct Planet *planet_ptr, struct Moon *moon_ptr, double mass_max)
{
    moon_ptr->type  = TYPE_SAT_MAJOR;

    if (mass_max > 0.0)
        moon_ptr->rad_a = rand_double(300.0, mass_max*2660.16);
    else
        moon_ptr->rad_a = rand_double(300.0, planet_ptr->radius*EARTH_RADIUS);

    moon_ptr->rad_b = moon_ptr->rad_a;
    moon_ptr->rad_c = moon_ptr->rad_a;

    moon_ptr->bulk_ices  = rand_double(0.6, 0.9);
    moon_ptr->bulk_rock  = (1.0-moon_ptr->bulk_ices)*rand_double(0.6, 0.9);
    moon_ptr->bulk_metal =  1.0-moon_ptr->bulk_ices-moon_ptr->bulk_rock;

    moon_ptr->host = planet_ptr;

    moon_props(moon_ptr);
}

//This function generates an icy minor moon.
void gen_ice_minor(struct Planet *planet_ptr, struct Moon *moon_ptr)
{
    moon_ptr->type  = TYPE_SAT_MINOR;
    moon_ptr->rad_a = rand_double(0.0, 300.0);
    moon_ptr->rad_b = rand_double(0.0, 300.0);
    moon_ptr->rad_c = rand_double(0.0, 300.0);

    moon_ptr->bulk_ices  = rand_double(0.6, 0.9);
    moon_ptr->bulk_rock  = (1.0-moon_ptr->bulk_ices)*rand_double(0.6, 0.9);
    moon_ptr->bulk_metal =  1.0-moon_ptr->bulk_ices-moon_ptr->bulk_rock;

    moon_ptr->host = planet_ptr;

    moon_props(moon_ptr);
}

//This function generates a rocky major moon.
void gen_rck_major(struct Planet *planet_ptr, struct Moon *moon_ptr, double mass_max)
{
    moon_ptr->type  = TYPE_SAT_MAJOR;

    if (mass_max > 0.0)
        moon_ptr->rad_a = rand_double(300.0, mass_max*2660.16);
    else
        moon_ptr->rad_a = rand_double(300.0, planet_ptr->radius*EARTH_RADIUS);

    moon_ptr->rad_b = moon_ptr->rad_a;
    moon_ptr->rad_c = moon_ptr->rad_a;

    moon_ptr->bulk_rock  = rand_double(0.6, 0.9);
    moon_ptr->bulk_metal = (1.0-moon_ptr->bulk_rock)*rand_double(0.6, 0.9);
    moon_ptr->bulk_ices  =  1.0-moon_ptr->bulk_metal-moon_ptr->bulk_rock;
    moon_ptr->host = planet_ptr;

    moon_props(moon_ptr);

    if ((moon_ptr->mass > planet_ptr->mass) || (moon_ptr->mass < 0))
        gen_rck_major(planet_ptr, moon_ptr, mass_max);
}

//This function generates a rocky minor moon.
void gen_rck_minor(struct Planet *planet_ptr, struct Moon *moon_ptr)
{
    moon_ptr->type  = TYPE_SAT_MINOR;
    moon_ptr->rad_a = rand_double(0.0, 300.0);
    moon_ptr->rad_b = rand_double(0.0, 300.0);
    moon_ptr->rad_c = rand_double(0.0, 300.0);

    moon_ptr->bulk_rock  = rand_double(0.6, 0.9);
    moon_ptr->bulk_metal = (1.0-moon_ptr->bulk_rock)*rand_double(0.6, 0.9);
    moon_ptr->bulk_ices  =  1.0-moon_ptr->bulk_metal-moon_ptr->bulk_rock;

    moon_ptr->host = planet_ptr;

    moon_props(moon_ptr);
}




//This function generates a minor moon and places it in orbit of a terrestrial planet.
void rckmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num)
{
    int i, frost_index;

    if (orbits[0] == orbit)
    {
        planet_ptr->num_moons = 0;
        return;
    }

    //First find the frost line
    for (i = 0; i < num; i++)
        if (((star_ptr->zone_frost-1.2) <= orbits[i]) && (orbits[i] <= (star_ptr->zone_frost)+1.2))
            break;

    frost_index = i;

    //Second find the current orbit
    for (i = 0; i < num; i++)
        if (orbits[i] == orbit)
            break;

    //Generate satellites
    double chance = MOONS_CHANCE;

    if (MOONS_DECAY == 'L')
        chance = (MOONS_CHANCE/(double)(frost_index-2))*i;
    else if (MOONS_DECAY == 'E')
        chance = exp(i - frost_index - log(1.0/MOONS_CHANCE));

    if (rand_double(0.0,1.0) <= chance)
        planet_ptr->num_moons = (int)floor(expo_double(0.0,4.0, chance));
    else
        planet_ptr->num_moons = 0;

    //If the planet has a moon
    if (planet_ptr->num_moons > 0)
    {
        //Create the moon array
        planet_ptr->moons = (struct Moon*)malloc(sizeof(struct Moon)*planet_ptr->num_moons);
        if (planet_ptr->moons == nullptr)
            crash(-140);
    }

    for (int j = 0; j < planet_ptr->num_moons; j++)
    {
        chance = ICEMN_CHANCE;
        if (ICEMN_DECAY == 'L')
            chance = (ICEMN_CHANCE/(double)(frost_index-1))*i;
        else if (ICEMN_DECAY == 'E')
            chance = exp(i - frost_index - log(1.0/ICEMN_CHANCE));

        if (rand_double(0.0,1.0) <= chance)
            gen_ice_minor(planet_ptr, &(planet_ptr->moons[j]));
        else
            gen_rck_minor(planet_ptr, &(planet_ptr->moons[j]));

        gen_ter_moon_orbit(star_ptr, &(planet_ptr->moons[j]));
    }
}

//This function generates a gas giant satellite system.
void gasmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num)
{
    int i, cur_index, frost_index, num_moons = 0, num_rings = 0;
    double a, chance;
    char logtext[100];

    //First find the frost line
    for (i = 0; i < num; i++)
        if (((star_ptr->zone_frost-1.2) <= orbits[i]) && (orbits[i] <= (star_ptr->zone_frost)+1.2))
            break;

    frost_index = i;

    print_log("      Found the frost line\n");

    //Second find the current orbit
    for (i = 0; i < num; i++)
        if (orbits[i] == orbit)
            break;

    cur_index = i;

    print_log("      Found the current orbit\n");

    //Create the moon array
    struct Moon *moons = (struct Moon*)malloc(sizeof(struct Moon)*100);
    if (moons == nullptr)
        crash(-142);

    //Create the ring gaps array
    double gaps[100][2];

    //Zero the gaps
    for (int j = 0; j < 100; j++)
    {
        gaps[j][0] = 0;
        gaps[j][1] = 0;
    }

    print_log("      Allocated the moon array\n");

    //Set the limits of the ring system
    double rings_min = fudge_double(1.34)*planet_ptr->radius;
    double rings_max = fudge_double(2.44)*planet_ptr->radius;

    planet_ptr->rings_vis = rand_double(0.0, 1.0) < RINGS_CHANCE;

    sprintf(logtext, "      Set the edges of the rings from %f to %f\n", rings_min, rings_max);
    print_log(logtext);

    //Create the type A moons
    print_log("      Generating type-A moons\n");

    double inn_lim = (rings_max + rings_min)/2.0;
    double out_lim =  rings_max + fudge_double(1.0);

    print_log("         Calculated generation limits\n");

    a = inn_lim;
    for (i = 0; a < out_lim; i++)
    {
        chance = ICEMN_CHANCE;
        if (ICEMN_DECAY == 'L')
            chance = (ICEMN_CHANCE/(double)(frost_index-1))*cur_index;
        else if (ICEMN_DECAY == 'E')
            chance = exp(cur_index - frost_index - log(1.0/ICEMN_CHANCE));

        chance = 1.0-chance;

        if (rand_double(0.0,1.0) <= chance)
            gen_ice_minor(planet_ptr, &moons[i]);
        else
            gen_rck_minor(planet_ptr, &moons[i]);

        print_log("         Generated moon\n");

        gen_gas_moon_orbit(star_ptr, &moons[i], a);

        //Add ring gap (if within the rings)
        if (a < rings_max)
        {
            gaps[i][0] = a - (fudge_double(MAX(moons[i].rad_a, MAX(moons[i].rad_b, moons[i].rad_c)))/EARTH_RADIUS);
            gaps[i][1] = a + (fudge_double(MAX(moons[i].rad_a, MAX(moons[i].rad_b, moons[i].rad_c)))/EARTH_RADIUS);
            num_rings++;
            sprintf(logtext, "            Generated ring gap: %f to %f\n", gaps[i][0], gaps[i][1]);
            print_log(logtext);
        }

        moons[i].type = TYPE_SAT_MOONA;

        num_moons++;
        a += rand_double(0.25, 1.5);
    }

    //Create the type-B major moons
    print_log("      Generating type-B moons\n");

    inn_lim = fudge_double(3.0)*planet_ptr->radius;
    out_lim = fudge_double(15.0)*planet_ptr->radius;

    a = inn_lim;
    for (; a < out_lim; i++)
    {
        chance = ICEMN_CHANCE;
        if (ICEMN_DECAY == 'L')
            chance = (ICEMN_CHANCE/(double)(frost_index-1))*cur_index;
        else if (ICEMN_DECAY == 'E')
            chance = exp(cur_index - frost_index - log(1.0/ICEMN_CHANCE));

        chance = 1.0-chance;

        do
        {
            if (rand_double(0.0,1.0) <= chance)
                gen_ice_major(planet_ptr, &moons[i], planet_ptr->mass*0.0001);
            else
                gen_rck_major(planet_ptr, &moons[i], planet_ptr->mass*0.0001);
        }
        while (moons[i].mass > planet_ptr->mass*0.0001);

        print_log("         Generated moon\n");

        gen_gas_moon_orbit(star_ptr, &moons[i], a);

        //Add ring gap (if one exists)
        int j = 0;
        double resonance = a / pow(2, 2.0/3.0);

        if (rings_min < resonance && resonance < rings_max)
        {
            gaps[i][0] = resonance - fudge_double(0.25);
            gaps[i][1] = resonance + fudge_double(0.25);

            //Check for overlap
            int overlap = false;
            for (j = 0; j < num_moons; j++)
            {
                if ((gaps[j][0] >= gaps[i][0] && gaps[j][1] <= gaps[i][1]) || //Existing gap is entirely within this gap
                    (gaps[i][0] >= gaps[j][0] && gaps[i][1] <= gaps[j][1]) || //This gap is entirely within existing gap
                    (gaps[j][0] <  gaps[i][0] && gaps[j][1] >= gaps[i][0] && gaps[j][1] < gaps[i][1]) || //Existing gap overlaps on inner edge
                    (gaps[j][0] >  gaps[i][0] && gaps[j][0] <  gaps[i][1] && gaps[j][1] > gaps[i][1])  ) //Existing gap overlaps on outer edge
                {
                    overlap = true;
                    break;
                }
            }

            if (!overlap)
            {
                num_rings++;
                sprintf(logtext, "            Generated ring gap: %f to %f\n", gaps[i][0], gaps[i][1]);
                print_log(logtext);
            }
            else
            {
                //Overwrite overlapped gap
                sprintf(logtext, "            Overwrote gap %i | From: %f to %f | To: %f to %f\n", j, gaps[j][0], gaps[j][1], gaps[i][0], gaps[i][1]);
                print_log(logtext);
                gaps[j][0] = gaps[i][0];
                gaps[j][1] = gaps[i][1];
                gaps[i][0] = 0;
                gaps[i][1] = 0;

                //Move minor moon into resonance
                sprintf(logtext, "            Moved moon %i (a = %f) into resonance with moon %i (a = %f, 1:2 = %f)\n",
                    j,
                    moons[j].a,
                    i,
                    moons[i].a,
                    resonance
                );
                print_log(logtext);
                moons[j].a = resonance;
            }
        }

        moons[i].type = TYPE_SAT_MOONB;

        num_moons++;

        if (rand_double(0.0,1.0) < LAGRA_CHANCE)
        {
            print_log("            Generating forward lagrangian companion\n");

            i++;

            chance = ICEMN_CHANCE;

            if (ICEMN_DECAY == 'L')
                chance = (ICEMN_CHANCE/(double)(frost_index-1))*cur_index;
            else if (ICEMN_DECAY == 'E')
                chance = exp(cur_index - frost_index - log(1.0/ICEMN_CHANCE));

            chance = 1.0-chance;

            if (rand_double(0.0,1.0) <= chance)
                gen_ice_minor(planet_ptr, &moons[i]);
            else
                gen_rck_minor(planet_ptr, &moons[i]);

            print_log("         Generated moon\n");

            //Copy the orbital parameters from the major partner
            moons[i].a = moons[i-1].a; //Semimajor axis
            moons[i].e = moons[i-1].e; //Eccentricity
            moons[i].y = moons[i-1].y; //Year length
            moons[i].v = moons[i-1].v; //Orbital velocity
            moons[i].i = moons[i-1].i; //Inclination
            moons[i].l = moons[i-1].l; //LoAN
            moons[i].p = moons[i-1].p; //AoP

            moons[i].axial_tilt = rand_double(0.0, 180.0); //Axial tilt
            moons[i].day_length = moons[i].y;              //Close moons are tidally locked

            moons[i].type = TYPE_SAT_FOR_B;

            num_moons++;
        }

        if (rand_double(0.0,1.0) < LAGRA_CHANCE)
        {
            print_log("            Generating reverse lagrangian companion\n");

            i++;

            chance = ICEMN_CHANCE;

            if (ICEMN_DECAY == 'L')
                chance = (ICEMN_CHANCE/(double)(frost_index-1))*cur_index;
            else if (ICEMN_DECAY == 'E')
                chance = exp(cur_index - frost_index - log(1.0/ICEMN_CHANCE));

            chance = 1.0-chance;

            if (rand_double(0.0,1.0) <= chance)
                gen_ice_minor(planet_ptr, &moons[i]);
            else
                gen_rck_minor(planet_ptr, &moons[i]);

            print_log("         Generated moon\n");

            //Copy the orbital parameters from the major partner
            if (moons[i-1].type == TYPE_SAT_FOR_B)
            {
                moons[i].a = moons[i-2].a; //Semimajor axis
                moons[i].e = moons[i-2].e; //Eccentricity
                moons[i].y = moons[i-2].y; //Year length
                moons[i].v = moons[i-2].v; //Orbital velocity
                moons[i].i = moons[i-2].i; //Inclination
                moons[i].l = moons[i-2].l; //LoAN
                moons[i].p = moons[i-2].p; //AoP
            }
            else
            {
                moons[i].a = moons[i-1].a; //Semimajor axis
                moons[i].e = moons[i-1].e; //Eccentricity
                moons[i].y = moons[i-1].y; //Year length
                moons[i].v = moons[i-1].v; //Orbital velocity
                moons[i].i = moons[i-1].i; //Inclination
                moons[i].l = moons[i-1].l; //LoAN
                moons[i].p = moons[i-1].p; //AoP
            }

            moons[i].axial_tilt = rand_double(0.0, 180.0); //Axial tilt
            moons[i].day_length = moons[i].y;             //Close moons are tidally locked

            moons[i].type = TYPE_SAT_REV_B;

            num_moons++;
        }

        a += rand_double(planet_ptr->radius, planet_ptr->radius*MOON_SPACE_MAX);
    }

    //Create the type-C irregular minor moons
    print_log("      Generating type-C moons\n");

    inn_lim = out_lim;
    out_lim = planet_ptr->a * pow(planet_ptr->mass/star_ptr->mass, (1.0/3.0)) * 235.0;


    if (abs(orbits[cur_index]-orbits[cur_index-1]) < out_lim)
        out_lim = abs(orbits[cur_index]-orbits[cur_index-1]);

    if (abs(orbits[cur_index+1]-orbits[cur_index]) < out_lim)
        out_lim = abs(orbits[cur_index+1]-orbits[cur_index]);

    int groups = (int)ceil(expo_double(0.0, 3.0, IRREG_MOON_DECAY));
    int die, members;
    double i_min, i_max, e_min, e_max, a_min, a_max;
    int i_already[3];

    for (int j = 0; j < 3; j++)
        i_already[j] = 0;

    for (int g = 0; g < groups; g++)
    {
        do
            die = (int)ceil(rand_double(0.0,18.0));
        while (die == i_already[0] || die == i_already[1] || die == i_already[2]);

        i_already[g] = die;

        i_max = (double)die*10.0;
        i_min = i_max - 10.0;

        e_max = rand_double(0.5,1.0);
        e_min = rand_double(0.0,0.5);

        a_min = inn_lim+(abs(out_lim-inn_lim)*(i/3.0));
        a_max = inn_lim+(abs(out_lim-inn_lim)*((i+1)/3.0));

        members = (int)ceil(rand_double(0.0,4.0)) + (int)ceil(rand_double(0.0,4.0)) + (int)ceil(rand_double(0.0,4.0));

        for (int j = 0; j < members; j++)
        {
            chance = ICEMN_CHANCE;
            if (ICEMN_DECAY == 'L')
                chance = (ICEMN_CHANCE/(double)(frost_index-1))*cur_index;
            else if (ICEMN_DECAY == 'E')
                chance = exp(cur_index - frost_index - log(1.0/ICEMN_CHANCE));

            chance = 1.0-chance;

            if (rand_double(0.0,1.0) <= chance)
                gen_ice_minor(planet_ptr, &moons[i+j]);
            else
                gen_rck_minor(planet_ptr, &moons[i+j]);

            print_log("         Generated moon\n");

            moons[i+j].a = rand_double(a_min, a_max);                                                           //Semimajor axis
            moons[i+j].e = rand_double(e_min, e_max);                                                           //Eccentricity
            moons[i+j].y = 0.0588 * sqrt( pow(moons[i+j].a, 3) / ((*moons[i+j].host).mass + moons[i+j].mass)); //Year length
            moons[i+j].v = sqrt(star_ptr->mass / moons[i+j].a)*29.7827;                                      //Orbital velocity
            moons[i+j].i = rand_double(i_min, i_max);                                                           //Inclination
            moons[i+j].l = rand_double(0.0, 360.0);                                                             //LoAN
            moons[i+j].p = rand_double(0.0, 360.0);                                                             //AoP

            //The day length will have 1d2+1 digits
            die = (int)floor(rand_double(0,2));

            if (die == 0)
                moons[i+j].day_length = rand_double(DAY_MIN, 100);
            else
                moons[i+j].day_length = rand_double(100, 1000);
            moons[i+j].axial_tilt = rand_double(0.0, 360.0); //Axial tilt

            moons[i+j].type    = TYPE_SAT_MOONC;
            moons[i+j].subtype = '0'+g+1;
        }

        num_moons += members;
        i         += members;
    }

    //Transfer moons to planet's array
    planet_ptr->num_moons = num_moons;
    planet_ptr->moons     = (struct Moon*)malloc(sizeof(struct Moon)*planet_ptr->num_moons);

    sprintf(logtext, "      Number of moons: %i\n", num_moons);
    print_log(logtext);

    print_log("      Allocated new moon array\n");

    for (int j = 0; j < num_moons; j++)
        planet_ptr->moons[j] = moons[j];


    print_log("      Transfer complete\n");

    //Transfer gaps to planet's array
    planet_ptr->num_rings = (num_rings*2)+2;
    planet_ptr->rings     = (double*)malloc(sizeof(double)*planet_ptr->num_rings);

    sprintf(logtext, "      Number of ring gaps: %i\n", num_rings);
    print_log(logtext);

    planet_ptr->rings[0] = rings_min;
    planet_ptr->rings[1] = rings_max;

    planet_ptr->rings_min = rings_min;
    planet_ptr->rings_max = rings_max;

    for (int j = 0, k = 2; j < num_moons; j++)
    {
        if (gaps[j][0] != 0)
        {
            planet_ptr->rings[k++] = gaps[j][0];
            planet_ptr->rings[k++] = gaps[j][1];
        }
    }

    //Bubble sort
    for (int l = 0; l < 5; l++)
    {
        for (int j = 0; j < planet_ptr->num_rings-1; j++)
        {
            for (int k = 0; k < planet_ptr->num_rings-j-1; k++)
            {
                if (planet_ptr->rings[k] > planet_ptr->rings[k+1])
                {
                    double temp             = planet_ptr->rings[k+1];
                    planet_ptr->rings[k+1] = planet_ptr->rings[k];
                    planet_ptr->rings[k]   = temp;
                }
            }
        }
    }

    free((void*)moons);
}

//This function generates a dwarf planet satellite system.
void dwfmn_orbit(struct Star *star_ptr, struct Planet *planet_ptr, double *orbits, double orbit, int num)
{
    int i, frost_index;
    double hill_min;
    char logtext[100];

    if (orbits[0] == orbit)
    {
        planet_ptr->num_moons = 0;
        return;
    }

    //First find the frost line
    for (i = 0; i < num; i++)
        if (((star_ptr->zone_frost-1.2) <= orbits[i]) && (orbits[i] <= (star_ptr->zone_frost)+1.2))
            break;

    frost_index = i;

    //Second find the current orbit
    for (i = 0; i < num; i++)
        if (orbits[i] == orbit)
            break;

    //Generate satellites
    double chance = MOONS_CHANCE;

    //Decide major moon
    if (MOONS_DECAY == 'L')
        chance = (MOONS_CHANCE/(double)(frost_index-2))*i;
    else if (MOONS_DECAY == 'E')
        chance = exp(i - frost_index - log(1.0/MOONS_CHANCE));

    double rand   = rand_double(0.0, 2.0);
    double roll   = rand <= (chance/10.0);
    planet_ptr->num_moons = round(rand_double(0.0, 5.0));

    sprintf(logtext, "      Major moon chance: %f\n      Rand: %f\n      Roll: %f\n", chance/10.0, rand, roll);
    print_log(logtext);

    //If the planet has a moon
    if (planet_ptr->num_moons > 0)
    {
        //Create the moon array
        planet_ptr->moons = (struct Moon*)malloc(sizeof(struct Moon)*planet_ptr->num_moons);
        if (planet_ptr->moons == nullptr)
            crash(-143);
    }

    if (roll && planet_ptr->num_moons > 0)
    {
        chance = ICEMN_CHANCE;
        if (ICEMN_DECAY == 'L')
            chance = (ICEMN_CHANCE/(double)(frost_index-1))*i;
        else if (ICEMN_DECAY == 'E')
            chance = exp(i - frost_index - log(1.0/ICEMN_CHANCE));

        chance = 1.0-chance;

        if (rand_double(0.0,1.0) <= chance)
            gen_ice_major(planet_ptr, &(planet_ptr->moons[0]), planet_ptr->mass);
        else
            gen_rck_major(planet_ptr, &(planet_ptr->moons[0]), planet_ptr->mass);

        gen_ter_moon_orbit(star_ptr, &(planet_ptr->moons[0]));
        planet_ptr->moons[0].y           = sqrt( pow(planet_ptr->a, 3) / star_ptr->mass); //Year length
        planet_ptr->moons[0].v           = sqrt(star_ptr->mass / planet_ptr->a)*29.7827;  //Orbital velocity
        planet_ptr->moons[0].day_length  = planet_ptr->moons[0].y;                        //Terrestrial major moons are tidally locked
        planet_ptr->moons[0].axial_tilt += (rand_double(0.0,1.0) < RET_PROB ? 180.0 : 0.0);
    }

    //Decide minor moons
    if (MOONS_DECAY == 'L')
        chance = (MOONS_CHANCE/(double)(frost_index-2))*i;
    else if (MOONS_DECAY == 'E')
        chance = exp(i - frost_index - log(1.0/MOONS_CHANCE));

    if (rand_double(0.0, 1.0) < chance && planet_ptr->num_moons > 0)
    {
        chance = ICEMN_CHANCE;
        if (ICEMN_DECAY == 'L')
            chance = (ICEMN_CHANCE/(double)(frost_index-1))*i;
        else if (ICEMN_DECAY == 'E')
            chance = exp(i - frost_index - log(1.0/ICEMN_CHANCE));

        chance = 1.0-chance;

        double dist;
        for (int j = roll; j < planet_ptr->num_moons; j++)
        {
            if (rand_double(0.0,1.0) <= chance)
                gen_ice_minor(planet_ptr, &(planet_ptr->moons[j]));
            else
                gen_rck_minor(planet_ptr, &(planet_ptr->moons[j]));

            if (j == 0)
            {
                hill_min = 2.44 * planet_ptr->radius * pow(planet_ptr->bulk_dens/planet_ptr->moons[j].bulk_dens, (1.0/3.0));
                dist     = fudge_double(hill_min + (hill_min*FUDGE_FACTOR));
            }
            else
            {
                dist = planet_ptr->moons[j-1].a + fudge_double(planet_ptr->radius*10.0);
            }

            planet_ptr->moons[j].a = dist;                                                                                            //Semimajor axis
            planet_ptr->moons[j].e = rand_double(0.001, 0.01);                                                                        //Eccentricity
            planet_ptr->moons[j].y = 0.0588 * sqrt( pow(planet_ptr->moons[j].a, 3) / (planet_ptr->mass + planet_ptr->moons[j].mass)); //Year length
            planet_ptr->moons[j].v = sqrt(star_ptr->mass / planet_ptr->moons[j].a)*29.7827;                                           //Orbital velocity
            planet_ptr->moons[j].i = rand_double(0.0,   2.0)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0);                        //Inclination
            planet_ptr->moons[j].l = rand_double(0.0, 360.0);                                                                         //LoAN
            planet_ptr->moons[j].p = rand_double(0.0, 360.0);                                                                         //AoP

            planet_ptr->moons[j].axial_tilt  = rand_double(0.0, 180.0); //Axial tilt
            planet_ptr->moons[j].axial_tilt += (rand_double(0.0,1.0) < RET_PROB ? 180.0 : 0.0);
            planet_ptr->moons[j].day_length  = rand_double(0.0, 200.0);
        }
    }
}




//This function sets the orbital parameters for a moon of a terrestrial planet.
void gen_ter_moon_orbit(struct Star *star_ptr, struct Moon *moon_ptr)
{
    double hill_max = moon_ptr->host->a * pow(moon_ptr->host->mass / star_ptr->mass, (1.0/3.0)) * 235.0;
    double hill_min = 2.44 * moon_ptr->host->radius * pow(moon_ptr->host->bulk_dens/moon_ptr->bulk_dens, (1.0/3.0));

    moon_ptr->a = rand_double(hill_min, hill_max/2.0);                                           //Semimajor axis
    moon_ptr->e = rand_double(0.001, 0.01);                                                      //Eccentricity
    moon_ptr->y = 0.0588 * sqrt( pow(moon_ptr->a, 3) / (moon_ptr->host->mass + moon_ptr->mass)); //Year length
    moon_ptr->v = sqrt(star_ptr->mass / moon_ptr->a)*29.7827;                                    //Orbital velocity
    moon_ptr->i = rand_double(0.0,   2.0)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0);      //Inclination
    moon_ptr->l = rand_double(0.0, 360.0);                                                       //LoAN
    moon_ptr->p = rand_double(0.0, 360.0);                                                       //AoP

    moon_ptr->axial_tilt = rand_double(0.0, 90.0); //Axial tilt
    moon_ptr->day_length = moon_ptr->y;            //Terrestrial major moons are tidally locked
}

//This function sets the orbital parameters for a moon of a giant planet.
void gen_gas_moon_orbit(struct Star *star_ptr, struct Moon *moon_ptr, double a)
{
    moon_ptr->a = a;                                                                                     //Semimajor axis
    moon_ptr->e = rand_double(0.001, 0.01);                                                               //Eccentricity
    moon_ptr->y = 0.0588 * sqrt( pow(moon_ptr->a, 3) / (moon_ptr->host->mass + moon_ptr->mass)); //Year length
    moon_ptr->v = sqrt(star_ptr->mass / moon_ptr->a)*29.7827;                                        //Orbital velocity
    moon_ptr->i = rand_double(0.0,   2.0)*((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0);                //Inclination
    moon_ptr->l = rand_double(0.0, 360.0);                                                                //LoAN
    moon_ptr->p = rand_double(0.0, 360.0);                                                                //AoP

    moon_ptr->axial_tilt = rand_double(0.0, 180.0); //Axial tilt
    moon_ptr->day_length = moon_ptr->y;             //Close moons are tidally locked
}


#endif // SATELLITES_H
