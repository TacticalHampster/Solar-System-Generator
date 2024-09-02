#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "utils.h"
#include "gen_planet_atmos.h"

#ifndef STAR_H
#define STAR_H

void gen_star(struct Star *star_ptr);
void atmo_star(struct Star *star_ptr);
double* gen_orbits(struct Star *star_ptr, int *num);
char* pick_types(struct Star *star_ptr, int *num, double *orbits);

//This function creates a randomly generated star.
void gen_star(struct Star *star_ptr)
{
    //gen characteristics
    star_ptr->mass      = rand_double(STAR_MASS_MIN, STAR_MASS_MAX);
    star_ptr->radius    = pow(star_ptr->mass,0.74);
    star_ptr->surf_grav = (star_ptr->mass / pow(star_ptr->radius,2)) * 27.9;
    star_ptr->esc_v     = sqrt(star_ptr->mass / star_ptr->radius) * 1118.6 * 55.0;
    star_ptr->metal     = fudge_double(0.01);

    star_ptr->temp    = pow(star_ptr->mass,0.505)*5772.0;
    star_ptr->bv      = (8464 + (-2.1344*star_ptr->temp) + sqrt(71639296 + (0.98724096*star_ptr->temp*star_ptr->temp)) )/(1.6928*star_ptr->temp);
    star_ptr->lumin   = pow(star_ptr->mass,3.5);
    star_ptr->abs_mag = 4.8 - (2.5*log10(star_ptr->lumin));
    star_ptr->rel_mag = 0;

    star_ptr->life    = pow(star_ptr->mass,-2.5)*11.72;
    star_ptr->age     = rand_double((1.0/3.0)*0.98*star_ptr->life, (1.0/3.0)*1.02*star_ptr->life);
    star_ptr->y       = rand_double(200.0, 300.0);

    if (star_ptr->temp < 5300.0)
        star_ptr->type = 'K';
    else if (star_ptr->temp < 6000.0)
        star_ptr->type = 'G';
    else
        star_ptr->type = 'F';

    //Gen zones
    star_ptr->zone_form_min = star_ptr->mass*0.1;
    star_ptr->zone_form_max = star_ptr->mass*40;
    star_ptr->zone_hab_min  = sqrt(star_ptr->lumin)*0.95;
    star_ptr->zone_hab_max  = sqrt(star_ptr->lumin)*1.35;
    star_ptr->zone_frost    = sqrt(star_ptr->lumin)*4.85;

    //Gen photosphere
    atmo_star(star_ptr);
}

//Generates a photosphere
void atmo_star(struct Star *star_ptr)
{
    double *atmo = (double*)malloc(sizeof(double)*13);
    if (atmo == nullptr)
        crash(-111);

    for (int i = 0; i < 13; i++)
        atmo[i] = 0;

    double remain = 1.0;

    char logtext[100];

    //Major components
    print_log("   Creating photosphere\n");

    //Helium
    atmo[1] = rand_double(0.18,0.25);
    remain -= atmo[1];
    sprintf(logtext, "      Generated %f helium,           %f remaining\n", atmo[1], remain);
    print_log(logtext);

    //Hydrogen
    atmo[0] = remain-star_ptr->metal;
    remain -= atmo[0];
    sprintf(logtext, "      Generated %f hydrogen,         %f remaining\n", atmo[0], remain);
    print_log(logtext);

    //Oxygen
    atmo[4] = remain*fudge_double(0.5);
    remain -= atmo[4];
    sprintf(logtext, "      Generated %f oxygen,           %f remaining\n", atmo[4], remain);
    print_log(logtext);

    //Carbon
    atmo[2] = remain*fudge_double(0.55);
    remain -= atmo[2];
    sprintf(logtext, "      Generated %f carbon,           %f remaining\n", atmo[2], remain);
    print_log(logtext);

    //Iron
    atmo[12] = remain*fudge_double(0.5);
    remain -= atmo[12];
    sprintf(logtext, "      Generated %f iron,             %f remaining\n", atmo[12], remain);
    print_log(logtext);

    double comps[8] = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    double names[8] = {3, 24, 25, 26, 27, 28, 29, 2};
    round_robin(comps, &remain, 8, names);

    atmo[ 3] = comps[0]; //Nitrogen
    atmo[ 5] = comps[1]; //Neon
    atmo[ 6] = comps[2]; //Sodium
    atmo[ 7] = comps[3]; //Magnesium
    atmo[ 8] = comps[4]; //Silicon
    atmo[ 9] = comps[5]; //Phosphorous
    atmo[10] = comps[6]; //Sulfur
    atmo[11] = comps[7]; //Argon

    //Make sure all numbers are positive
    if (remain < 0.0)
    {
        free((void*)atmo);
        atmo_star(star_ptr);
    }

    //Transfer
    star_ptr->photosphere = atmo;
}

//This function creates a list of randomly generated stable orbits, given a star.
double* gen_orbits(struct Star *star_ptr, int *num)
{
    int num_of_orbits = 1;
    double cur_orbit = star_ptr->zone_frost;

    double orbit_array[20];

    //Include the frost line as the orbit for the shepherd giant
    do
    {
        orbit_array[0] = star_ptr->zone_frost + (rand_double(1.0,1.2) * ((round(rand_double(0.0,1.0))-1.0) ? 1.0 : -1.0));
    }
    while (orbit_array[0] == star_ptr->zone_frost);

    //gen orbits outside the frost line
    for (;;)
    {
        cur_orbit *= rand_double(ORBIT_SPACING_MIN, ORBIT_SPACING_MAX);

        if (cur_orbit > star_ptr->zone_form_max)
            break;

        if (cur_orbit > (orbit_array[num_of_orbits-1]+0.15))
        {
            orbit_array[num_of_orbits] = cur_orbit;
            num_of_orbits++;
        }
    }

    //Reset cur_orbit
    cur_orbit = orbit_array[0];

    //gen orbits inside the frost line
    for (;;)
    {
        cur_orbit /= rand_double(ORBIT_SPACING_MIN, ORBIT_SPACING_MAX);

        if (cur_orbit < star_ptr->zone_form_min)
            break;

        if (cur_orbit < (orbit_array[num_of_orbits-1]-0.15))
        {
            orbit_array[num_of_orbits] = cur_orbit;
            num_of_orbits++;
        }
    }

    //Update num
    *num = num_of_orbits;

    //Create final orbit array
    double *array_ptr = malloc(sizeof(double)*num_of_orbits);
    if (array_ptr == nullptr)
        crash(-120);

    //Copy orbits into new array
    for (int i = 0; i < num_of_orbits; i++)
    {
        array_ptr[i] = orbit_array[i];
    }

    //Bubble sort new array
    int swapped;
    for (int i = 0; i < num_of_orbits-1; i++)
    {
        swapped = 0;
        for (int j = 0; j < num_of_orbits-i-1; j++)
        {
            if (array_ptr[j] > array_ptr[j+1])
            {
                double temp     = array_ptr[j  ];
                array_ptr[j  ] = array_ptr[j+1];
                array_ptr[j+1] = temp;
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }

    //Make sure one of the orbits is in the habitable zone
    int is_habitable = false;
    for (int i = 0; i < num_of_orbits; i++)
    {
        if ( (star_ptr->zone_hab_min < array_ptr[i]) && (array_ptr[i] < star_ptr->zone_hab_max) )
        {
            is_habitable = true;
            break;
        }
    }

    //If not, retry
    if (!is_habitable)
    {
        free((void*)array_ptr);
        array_ptr = gen_orbits(star_ptr, num);
    }

    //Once a habitable orbit exists, return the array
    return array_ptr;
}

//This function picks planet types for each orbit.
char* pick_types(struct Star *star_ptr, int *num, double *orbits)
{
    char logtext[100];
    print_log("Choosing types of planets\n");

    //Create types array
    char *types = (char*)malloc(sizeof(char)*(*num)* 2);
    if (types == nullptr)
        crash(-121);

    int i = 0, j = 0, frost_index, hab_index, migrated = false;

    //Zero out types
    for (i = 0; i < (*num)*2; i++)
        types[i] = '\0';

    //First do the frost line orbit
    for (i = 0; i < (*num); i++)
        if (((star_ptr->zone_frost-1.2) <= orbits[i]) && (orbits[i] <= (star_ptr->zone_frost)+1.2))
            break;

    types[i]    = TYPE_GAS_GIANT;
    frost_index = i;

    sprintf(logtext, "   Assigned frost orbit (%i) as type %c\n", frost_index, types[frost_index]);
    print_log(logtext);

    //Find habitable orbit so migrating giant doesn't go there
    for (i = 0; i < (*num); i++)
        if ((star_ptr->zone_hab_min <= orbits[i]) && (orbits[i] <= star_ptr->zone_hab_max))
            break;

    hab_index = i;

    //Decide migration
    if (rand_double(0.0, 1.0) <= MIGRATE_MID_CHANCE)
    {
        print_log("   Migrated gas giant to mid-system\n");

        int mig_index;
        do
        {
            mig_index = (int)floor(expo_double(i, frost_index, MIGRATE_MID_CHANCE*2));
        }
        while (mig_index == hab_index); //Make sure the migrating gas giant doesn't take the place of the habitable planet

        types[mig_index] = TYPE_GAS_GIANT;
        types[frost_index] = '0';
        migrated = true;
    }
    else if (rand_double(0.0, 1.0) <= MIGRATE_HOT_CHANCE)
    {
        print_log("   Migrated gas giant to inner system\n");
        types[0] = TYPE_GAS_PUFFY;
        types[frost_index] = '0';
        migrated = true;
    }

    //Second do the habitable orbit

    double chance = OCEAN_CHANCE;
    if (types[i] == '\0' && types[i] != '0') //Make sure not to overwrite an existing planet
    {
        if (OCEAN_DECAY == 'L')
            chance = (OCEAN_CHANCE/(double)(frost_index-star_ptr->zone_form_min))*(double)(i-star_ptr->zone_form_min);
        else if (OCEAN_DECAY == 'E')
            chance = exp(i - frost_index - log(1.0/OCEAN_CHANCE));

        if (rand_double(0.0,1.0) < chance)
            types[i] = TYPE_WTR_GREEN;
        else
            types[i] = TYPE_RCK_GREEN;

        sprintf(logtext, "   Assigned habitable orbit (%i) as type %c\n", i, types[i]);
        print_log(logtext);
    }

    //Third do the inner orbits (inside habitable zone)
    for (i = 0; orbits[i] <= star_ptr->zone_hab_min; i++)
    {
        if (types[i] == '\0' && types[i] != '0') //Make sure not to overwrite an existing planet
        {
            double chance = DENSE_CHANCE;

            if (DENSE_DECAY == 'L')
                chance = DENSE_CHANCE - ((DENSE_CHANCE/(double)frost_index)*i);
            else if (DENSE_DECAY == 'E')
                chance = DENSE_CHANCE*exp(-i);

            if (rand_double(0.0,1.0) < chance)
                types[i] = TYPE_RCK_DENSE;
            else
                types[i] = TYPE_RCK_DESRT;

            sprintf(logtext, "   Assigned orbit %i as type %c\n", i, types[i]);
            print_log(logtext);
        }
    }

    //Fourth do the mid orbits
    for (i += 1; i < frost_index; i++)
    {
        if (types[i] == '\0' && types[i] != '0') //Make sure not to overwrite an existing planet
        {
            double chance = OCEAN_CHANCE;

            if (!migrated && i == frost_index-1)
            {
                if (rand_double(0.0, 1.0) < ABELT_CHANCE)
                {
                    sprintf(logtext, "   Created asteroid belt at orbit %i\n", i);
                    print_log(logtext);
                    types[i] = TYPE_BLT_INNER;
                    if (rand_double(0.0, 1.0) < (ABELT_CHANCE*2.0))
                    {
                        types[frost_index+1] = types[frost_index];
                        types[frost_index] = TYPE_DWF_PLNTSML;
                        i++;
                        frost_index++;
                        (*num)++;
                        sprintf(logtext, "   Created dwarf planet at index %i, shifted frostline to index %i\n", i, frost_index);
                        print_log(logtext);
                    }
                }

            }


            if (types[i] == '\0' && types[i] != '0') //If not an asteroid belt
            {
                if (OCEAN_DECAY == 'L')
                    chance = (OCEAN_CHANCE/(double)(frost_index-star_ptr->zone_form_min))*(double)(i-star_ptr->zone_form_min);
                else if (OCEAN_DECAY == 'E')
                    chance = exp(i - frost_index - log(1.0/OCEAN_CHANCE));

                if (rand_double(0.0,1.0) < chance)
                    types[i] = TYPE_WTR_HYCN;
                else
                    types[i] = TYPE_RCK_DESRT;

                sprintf(logtext, "   Assigned orbit %i as type %c\n", i, types[i]);
                print_log(logtext);
            }
        }
    }

    int giants_finished = false;

    //Finally do the outer orbits
    for (i += 0; i < (*num); i++)
    {
        if (types[i] == '\0') //Make sure not to overwrite an existing planet
        {
            double ice_chance   = ICEGT_CHANCE;
            double dwarf_chance = GASDW_CHANCE;

            if (ICEGT_DECAY == 'L')
                ice_chance = (ICEGT_CHANCE/(double)((*num)-frost_index))*(double)(i-frost_index);
            else if (OCEAN_DECAY == 'E')
                ice_chance = exp(i - (*num) - log(1.0/ICEGT_CHANCE));

            if (GASDW_DECAY == 'L')
                dwarf_chance = (GASDW_CHANCE/(double)((*num)-frost_index))*(double)(i-frost_index);
            else if (GASDW_DECAY == 'E')
                dwarf_chance = exp(i - (*num) - log(1.0/GASDW_CHANCE));

            for (;;)
            {
                double die = rand_double(0.0,1.0);

                if (die < ice_chance)
                {
                    types[i] = TYPE_ICE_GIANT;
                    giants_finished = true;
                    break;
                }
                else if (die > (1.0-dwarf_chance))
                {
                    types[i] = TYPE_ICE_DWARF;
                    giants_finished = true;
                    break;
                }
                else
                {
                    if (!giants_finished)
                    {
                        types[i] = TYPE_GAS_GIANT;
                        break;
                    }
                }
            }

            sprintf(logtext, "   Assigned orbit %i as type %c\n", i, types[i]);
            print_log(logtext);
        }
    }

    //Decide kuiper belt
    if (rand_double(0.0, 1.0) < ABELT_CHANCE)
    {
        types[i] = TYPE_BLT_KUIPR;
        i++;

        //Decide number of dwarf planets
        int plutinos  = round(rand_double(0.0, 2.0));
        int twotinos  = round(rand_double(0.0, 2.0));
        int cubewanos = round(rand_double(0.0, 4.0));
        int scattered = round(rand_double(0.0, 6.0));
        int sednoids  = round(rand_double(0.0, 3.0));

        //Update num with total
        (*num) += plutinos+twotinos+cubewanos+scattered+sednoids;

        //Add dwarfs to type array
        for (j = 0; j < plutinos+twotinos+cubewanos+scattered+sednoids; j++)
        {
            if (j < plutinos)
                types[i+j] = TYPE_DWF_PLUTINO;
            else if (j < plutinos+cubewanos)
                types[i+j] = TYPE_DWF_CUBEWNO;
            else if (j < plutinos+cubewanos+twotinos)
                types[i+j] = TYPE_DWF_TWOTINO;
            else if (j < plutinos+cubewanos+twotinos+scattered)
                types[i+j] = TYPE_DWF_SCATTER;
            else
                types[i+j] = TYPE_DWF_SEDNOID;

            sprintf(logtext, "   Created kuiper object (%c) at index %i\n", types[i+j], i+j);
            print_log(logtext);
        }
    }

    //Create new types array fitted to the actual number of planets
    char *adj_types = (char*)malloc(sizeof(char)*i);
    if (adj_types == nullptr)
        crash(-122);

    int skip = 0;
    print_log("   Transferring types\n");
    for (int k = 0; k <= i+j; k++)
    {
        if (types[k] == '0' || types[k] == '\0')
            skip++;
        else
        {
            adj_types[k-skip] = types[k];
            sprintf(logtext, "      Confirmed index %i as type %c\n", k-skip, adj_types[k-skip]);
            print_log(logtext);
        }
    }

    free((void*)types);
    (*num) = i+j-skip;

    return adj_types;
}



#endif // STAR_H
