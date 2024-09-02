#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "utils.h"

#ifndef PROPS_H
#define PROPS_H

void gen_planet_props(struct Planet *planet_ptr, double mass_max);

void zero_fields(struct Planet *planet_ptr);
void rocky_props(struct Planet *planet_ptr);
void giant_props(struct Planet *planet_ptr);
void icy_props(struct Planet *planet_ptr);
void dwarf_props(struct Planet *planet_ptr);
void belt_props(struct Planet *planet_ptr);

//This is the top level function. It should be the only function called outside this file.
void gen_planet_props(struct Planet *planet_ptr, double mass_max)
{
    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
            planet_ptr->subtype[0] = 'M';
            planet_ptr->subtype[1] = '\0';

            //Set composition
            planet_ptr->mass       = rand_double(DENSE_MASS_MIN, DENSE_MASS_MAX);
            planet_ptr->bulk_metal = rand_double(0.6, 0.8);
            planet_ptr->bulk_rock  = 1.0-planet_ptr->bulk_metal;
            planet_ptr->bulk_water = 0.0;

            //Calculate remaining properties
            rocky_props(planet_ptr);
            return;

        case TYPE_RCK_DESRT:
            planet_ptr->subtype[0] = 'V';
            planet_ptr->subtype[1] = '\0';

            //Set composition
            planet_ptr->mass       = rand_double(ROCKY_MASS_MIN, ROCKY_MASS_MAX);
            planet_ptr->bulk_rock  = rand_double(0.6, 0.8);
            planet_ptr->bulk_metal = 1.0-planet_ptr->bulk_rock;
            planet_ptr->bulk_water = 0.0;

            //Calculate remaining properties
            rocky_props(planet_ptr);
            return;

        case TYPE_RCK_GREEN:
            planet_ptr->subtype[0] = 'E';
            planet_ptr->subtype[1] = '\0';

            //Set composition
            planet_ptr->mass       = rand_double(GREEN_MASS_MIN, GREEN_MASS_MAX);
            planet_ptr->bulk_rock  = rand_double(0.6, 0.8);
            planet_ptr->bulk_water = rand_double(0.0, 0.0015);
            planet_ptr->bulk_metal = 1.0-planet_ptr->bulk_rock-planet_ptr->bulk_water;

            //Calculate remaining properties
            rocky_props(planet_ptr);
            return;

        case TYPE_WTR_HYCN:
            planet_ptr->subtype[0] = 'H';
            planet_ptr->subtype[1] = '\0';

            //Set composition
            planet_ptr->mass = rand_double(ROCKY_MASS_MIN, ROCKY_MASS_MAX);

            //Roll a d3 to determine the type of ocean planet
            int type = (int)floor(rand_double(0,3));

            switch (type)
            {
                case 0: //The world is mostly water
                    planet_ptr->bulk_water = rand_double(0.6, 0.8);
                    planet_ptr->bulk_rock  = (1-planet_ptr->bulk_water)*fudge_double(0.75);
                    planet_ptr->bulk_metal = 1-planet_ptr->bulk_water-planet_ptr->bulk_rock;
                    planet_ptr->subtype[1] = '3';
                    break;
                case 1: //The world is equal parts water and rock
                    planet_ptr->bulk_metal = rand_double(0.01, 0.08);
                    planet_ptr->bulk_water = (1-planet_ptr->bulk_metal)*fudge_double(0.5);
                    planet_ptr->bulk_rock  = 1-planet_ptr->bulk_metal-planet_ptr->bulk_water;
                    planet_ptr->subtype[1] = '2';
                    break;
                default: //The world is mostly rock
                    planet_ptr->bulk_rock  = fudge_double(0.5);
                    planet_ptr->bulk_metal = fudge_double(0.1);
                    planet_ptr->bulk_water = 1.0-planet_ptr->bulk_rock-planet_ptr->bulk_metal;
                    planet_ptr->subtype[1] = '1';
            }

            //Calculate remaining properties
            rocky_props(planet_ptr);
            return;

        case TYPE_WTR_GREEN:
            planet_ptr->subtype[0] = 'K';
            planet_ptr->subtype[1] = '1';

            //Set composition (habitable water worlds are always mostly rock)
            planet_ptr->mass = rand_double(ROCKY_MASS_MIN, ROCKY_MASS_MAX);
            planet_ptr->bulk_rock  = fudge_double(0.5);
            planet_ptr->bulk_metal = fudge_double(0.1);
            planet_ptr->bulk_water = 1.0-planet_ptr->bulk_rock-planet_ptr->bulk_metal;

            //Calculate remaining properties
            rocky_props(planet_ptr);
            return;

        case TYPE_GAS_GIANT:
            planet_ptr->subtype[0] = 'G';
            planet_ptr->subtype[1] = '1';

            //Assign mass, radius, and type
            planet_ptr->mass   = rand_double(GAS_MASS_MIN, ((mass_max > 0.0) ? mass_max : GAS_MASS_MAX));
            planet_ptr->radius = GAS_RADIUS_NORM*fudge_double(1.0);

            if (planet_ptr->mass >= (GAS_RADIUS_LIM*(1.0+FUDGE_FACTOR)))
                planet_ptr->type  = TYPE_GAS_SUPER;
            else
                planet_ptr->type  = TYPE_GAS_GIANT;

            //Calculate properties
            giant_props(planet_ptr);
            return;

        case TYPE_GAS_PUFFY:
            planet_ptr->subtype[0] = 'G';
            planet_ptr->subtype[1] = '3';

            //Assign mass, radius, and calculate properties
            do
            {
                planet_ptr->mass       = rand_double(GAS_MASS_MIN, GAS_RADIUS_LIM);
                planet_ptr->radius     = rand_double(GAS_RADIUS_NORM, GAS_RADIUS_MAX);
                giant_props(planet_ptr);
            }
            while (planet_ptr->bulk_dens > 1.0);
            return;

        case TYPE_ICE_DWARF:
            planet_ptr->subtype[0] = 'I';
            planet_ptr->subtype[1] = '\0';

            //Assign mass, radius
            planet_ptr->mass       = rand_double(GAS_DWARF_MASS_MIN*2.0, GAS_DWARF_MASS_MAX*2.0);
            planet_ptr->radius     = rand_double(3.0, GAS_DWARF_RADIUS_MAX);

            //Calculate properties
            icy_props(planet_ptr);
            return;

        case TYPE_ICE_GIANT:
            planet_ptr->subtype[0] = 'I';
            planet_ptr->subtype[1] = '\0';

            //Assign mass, radius, and calculate properties
            do
            {
                planet_ptr->mass       = rand_double(GAS_DWARF_MASS_MIN*2.0, GAS_DWARF_MASS_MAX*2.0);
                planet_ptr->radius     = rand_double(3.0, GAS_DWARF_RADIUS_MAX);

                icy_props(planet_ptr);
            }
            while ((planet_ptr->bulk_dens < 1.0) || (planet_ptr->bulk_dens) > 3.0);
            return;

        case TYPE_BLT_INNER:
            planet_ptr->subtype[0] = '\0';
            planet_ptr->subtype[1] = '\0';

            //Assign mass, height
            planet_ptr->mass   = fudge_double(0.606 * 0.03);
            planet_ptr->height = rand_double(0.0, 17.0);

            //Set composition
            planet_ptr->bulk_rock   = fudge_double(0.17);
            planet_ptr->bulk_carbon = fudge_double(0.75);
            planet_ptr->bulk_ices   = rand_double(0.0, 0.005);
            planet_ptr->bulk_metal  = 1.0-planet_ptr->bulk_rock-planet_ptr->bulk_carbon-planet_ptr->bulk_ices;

            //Calculate properties
            belt_props(planet_ptr);
            return;

        case TYPE_BLT_KUIPR:
            planet_ptr->subtype[0] = '\0';
            planet_ptr->subtype[1] = '\0';

            //Assign mass, height
            planet_ptr->mass   = fudge_double(0.606 * 0.03) * rand_double(20.0, 200.0);
            planet_ptr->height = rand_double(0.0, 40.0);

            //Set composition
            planet_ptr->bulk_carbon = fudge_double(0.17);
            planet_ptr->bulk_ices   = fudge_double(0.75);
            planet_ptr->bulk_metal  = rand_double(0.0, 0.005);
            planet_ptr->bulk_rock   = 1.0-planet_ptr->bulk_metal-planet_ptr->bulk_carbon-planet_ptr->bulk_ices;

            //Calculate properties
            belt_props(planet_ptr);
            return;

        case TYPE_DWF_PLNTSML:
            planet_ptr->subtype[0] = '\0';
            planet_ptr->subtype[1] = '\0';

            //Assign radius
            planet_ptr->radius = rand_double(300.0, ((mass_max > 0.0) ? mass_max*2660.16 : 0.001*EARTH_RADIUS));

            //Set composition
            planet_ptr->bulk_rock  = rand_double(0.6, 0.9);
            planet_ptr->bulk_metal = (1.0-planet_ptr->bulk_rock)*rand_double(0.6, 0.9);
            planet_ptr->bulk_ices  =  1.0-planet_ptr->bulk_metal-planet_ptr->bulk_rock;

            //Calculate properties
            dwarf_props(planet_ptr);
            return;

        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
        case TYPE_DWF_SEDNOID:
            planet_ptr->subtype[0] = '\0';
            planet_ptr->subtype[1] = '\0';

            //Assign radius
            planet_ptr->radius = rand_double(300.0, ((mass_max > 0.0) ? mass_max*2660.16 : 0.001*EARTH_RADIUS));

            //Set composition
            planet_ptr->bulk_ices  = rand_double(0.7, 0.9);
            planet_ptr->bulk_rock  = (1.0-planet_ptr->bulk_ices)*rand_double(0.75, 0.99);
            planet_ptr->bulk_metal = 1.0-planet_ptr->bulk_ices-planet_ptr->bulk_rock;

            //Calculate properties
            dwarf_props(planet_ptr);
            return;
        default:
            crash(-220);
    }
}

//Zeros fields
void zero_fields(struct Planet *planet_ptr)
{
    planet_ptr->type       = '\0';
    planet_ptr->subtype[0] = '\0';
    planet_ptr->subtype[1] = '\0';

    planet_ptr->mass   = 0.0;
    planet_ptr->radius = 0.0;
    planet_ptr->esc_v  = 0.0;
    planet_ptr->albedo = 0.0;

    planet_ptr->surf_temp      = 0.0;
    planet_ptr->surf_grav      = 0.0;
    planet_ptr->rck_cover      = 0.0;
    planet_ptr->ice_cover      = 0.0;
    planet_ptr->wtr_cover      = 0.0;
    planet_ptr->cld_cover_thn  = 0.0;
    planet_ptr->cld_cover_thk  = 0.0;
    planet_ptr->feature        = nullptr;

    planet_ptr->moons       = nullptr;
    planet_ptr->num_moons   = 0;
    planet_ptr->rings       = nullptr;
    planet_ptr->num_rings   = 0.0;
    planet_ptr->rings_min   = 0.0;
    planet_ptr->rings_max   = 0.0;
    planet_ptr->rings_vis   = false;
    planet_ptr->trojans     = false;

    planet_ptr->bulk_dens  = 0.0;
    planet_ptr->bulk_ices  = 0.0;
    planet_ptr->bulk_metal = 0.0;
    planet_ptr->bulk_noble = 0.0;
    planet_ptr->bulk_rock  = 0.0;
    planet_ptr->bulk_water = 0.0;

    planet_ptr->a = 0.0;
    planet_ptr->e = 0.0;
    planet_ptr->y = 0.0;
    planet_ptr->v = 0.0;
    planet_ptr->i = 0.0;
    planet_ptr->l = 0.0;
    planet_ptr->p = 0.0;

    planet_ptr->atmo_pres  = 0.0;
    planet_ptr->atmo_dens  = 0.0;
    planet_ptr->atmo_high  = 0.0;
    planet_ptr->has_atmo   = false;
    planet_ptr->atmosphere = nullptr;
}

//Calculates the properties of a terrestrial planet given its mass and composition.
void rocky_props(struct Planet *planet_ptr)
{
    double rock_ms, rock_rad, metal_ms, metal_rad, water_ms, water_rad, true_radius;
    double components = 3.0;

    planet_ptr->bulk_noble = 0.0;
    planet_ptr->bulk_ices  = 0.0;

    rock_ms  = (planet_ptr->mass*planet_ptr->bulk_rock)/10.55;
    metal_ms = (planet_ptr->mass*planet_ptr->bulk_metal)/5.80;
    water_ms = (planet_ptr->mass*planet_ptr->bulk_water)/5.52;

    if (planet_ptr->bulk_metal > 0.0)
        metal_rad = 2.52*pow(10, -0.209450 + (1.0/3.0)*log10(metal_ms)-0.0804*pow(metal_ms,0.394));
    else
    {
        metal_rad = 0.0;
        components--;
    }

    if (planet_ptr->bulk_rock > 0.0)
    {
        if (rock_ms > 4.0)
            rock_rad = 3.9*pow(10, -0.209594 + (1.0/3.0)*log10(rock_ms)-0.0799*pow(rock_ms,0.413));
        else
            rock_rad = 3.9*pow(10, -0.209490 + (1.0/3.0)*log10(rock_ms)-0.0804*pow(rock_ms,0.394));
    }
    else
    {
        rock_rad = 0.0;
        components--;
    }

    if (planet_ptr->bulk_water > 0.0)
    {
        if (water_ms > 4.0)
            water_rad = 4.43*pow(10, -0.209396 + (1.0/3.0)*log10(water_ms)-0.0807*pow(water_ms,0.375));
        else
            water_rad = 3.9*pow(10, -0.209490 + (1.0/3.0)*log10(water_ms)-0.0804*pow(water_ms,0.394));
    }
    else
    {
        water_rad = 0.0;
        components--;
    }

    true_radius = ((1+planet_ptr->bulk_metal)*metal_rad + (1+planet_ptr->bulk_rock)*rock_rad + (1+planet_ptr->bulk_water)*water_rad)/components;

    planet_ptr->radius    = fudge_double(true_radius);
    planet_ptr->bulk_dens = (planet_ptr->bulk_metal*7.87) + (planet_ptr->bulk_rock*3.25) + (planet_ptr->bulk_water*0.93);
    planet_ptr->surf_grav = (planet_ptr->mass / pow(planet_ptr->radius,2));
    planet_ptr->esc_v     = sqrt(planet_ptr->mass / planet_ptr->radius)*1118.6;
}

//Calculates the properties and composition of a gaseous planet given its mass.
void giant_props(struct Planet *planet_ptr)
{
    //Decide albedo
    if (planet_ptr->subtype[1] == '1')
        planet_ptr->albedo = fudge_double(0.57*(2.0/3.0));
    else if (planet_ptr->subtype[1] == '2')
        planet_ptr->albedo = fudge_double(0.81);
    else if (planet_ptr->subtype[1] == '3')
        planet_ptr->albedo  = fudge_double(0.12);
    else if (planet_ptr->subtype[1] == '4')
        planet_ptr->albedo = fudge_double(0.03);
    else if (planet_ptr->subtype[1] == '5')
        planet_ptr->albedo = fudge_double(0.55);


    //Calculate composition
    double solid_mass = rand_double(0.03, 0.13); //Non-noble-gas is between 3&-13% of total mass
    double rocky_mass = rand_double(0.45, 0.7);  //Rock and metal is the majority of that mass

    //The majority of the rocky mass is silicates
    planet_ptr->bulk_rock  = fudge_double(0.75);
    planet_ptr->bulk_metal = 1-planet_ptr->bulk_rock;

    //Roll a d3
    int type = (int)floor(rand_double(0,3));
    switch (type)
    {
        case 0: //Remaining mass is mostly volatiles
            planet_ptr->bulk_ices  = (1-rocky_mass)*rand_double(0.45, 0.7);
            planet_ptr->bulk_water = (1-rocky_mass)-planet_ptr->bulk_ices;
            break;
        case 1: //Remaining mass is mostly water
            planet_ptr->bulk_water = (1-rocky_mass)*rand_double(0.45, 0.7);
            planet_ptr->bulk_ices  = (1-rocky_mass)-planet_ptr->bulk_water;
            break;
        default: //Remaining mass is equal parts water and volatiles
            planet_ptr->bulk_water = (1-rocky_mass)*rand_double(0.45, 0.7);
            planet_ptr->bulk_ices  = (1-rocky_mass)-planet_ptr->bulk_water;
    }

    //Scale to whole planet
    planet_ptr->bulk_metal *= solid_mass*rocky_mass;
    planet_ptr->bulk_rock  *= solid_mass*rocky_mass;
    planet_ptr->bulk_water *= solid_mass;
    planet_ptr->bulk_ices  *= solid_mass;
    planet_ptr->bulk_noble  = 1-planet_ptr->bulk_metal-planet_ptr->bulk_rock-planet_ptr->bulk_water-planet_ptr->bulk_ices;

    //Calculate remaining properties
    planet_ptr->bulk_dens = planet_ptr->mass/pow(planet_ptr->radius,3)*5.513;
    planet_ptr->surf_grav = planet_ptr->mass/pow(planet_ptr->radius,2);
    planet_ptr->esc_v     = sqrt(planet_ptr->mass/planet_ptr->radius)*1118.6;
}

//Calculates the properties and composition of an icy planet given its mass.
void icy_props(struct Planet *planet_ptr)
{
    //Calculate composition
    double core_mass = rand_double(0.03, 0.08); //Core is between 3&-8% of total mass

    //The majority of the rocky mass is silicates
    planet_ptr->bulk_rock  = fudge_double(0.75);
    planet_ptr->bulk_metal = 1-planet_ptr->bulk_rock;

    //Rescale
    planet_ptr->bulk_rock  *= core_mass;
    planet_ptr->bulk_metal *= core_mass;

    if (planet_ptr->type == TYPE_ICE_DWARF)
        planet_ptr->bulk_rock *= 2.0;

    //Hydrogen envelope is between 15%-20% of total mass
    planet_ptr->bulk_noble = rand_double(0.15, 0.2);

    //Roll a d3 to determine composition of remaining mass
    double remain = 1-planet_ptr->bulk_rock-planet_ptr->bulk_metal-planet_ptr->bulk_noble;

    int type = (int)floor(rand_double(0,3));
    switch (type)
    {
        case 0: //Remaining mass is mostly volatiles
            planet_ptr->bulk_ices  = remain*rand_double(0.6, 0.8);
            planet_ptr->bulk_water = remain-planet_ptr->bulk_ices;
            planet_ptr->subtype[1] = '1';
            break;
        case 1: //Remaining mass is mostly water
            planet_ptr->bulk_water = remain*rand_double(0.6, 0.8);
            planet_ptr->bulk_ices  = remain-planet_ptr->bulk_water;
            planet_ptr->subtype[1] = '3';
            break;
        default: //Remaining mass is equal parts water and volatiles
            planet_ptr->bulk_water = remain*fudge_double(0.5);
            planet_ptr->bulk_ices  = remain-planet_ptr->bulk_water;
            planet_ptr->subtype[1] = '2';
    }

    //Calculate radius
    double rock_ms, rock_rad, metal_ms, metal_rad, water_ms, water_rad, true_radius;
    double components = 5.0;

    rock_ms  = (planet_ptr->mass*planet_ptr->bulk_rock)/10.55;
    metal_ms = (planet_ptr->mass*planet_ptr->bulk_metal)/5.80;
    water_ms = (planet_ptr->mass*planet_ptr->bulk_water)/5.52;

    if (planet_ptr->bulk_metal > 0.0)
        metal_rad = 2.52*pow(10, -0.209450 + (1.0/3.0)*log10(metal_ms)-0.0804*pow(metal_ms,0.394));
    else
    {
        metal_rad = 0.0;
        components--;
    }

    if (planet_ptr->bulk_rock > 0.0)
    {
        if (rock_ms > 4.0)
            rock_rad = 3.9*pow(10, -0.209594 + (1.0/3.0)*log10(rock_ms)-0.0799*pow(rock_ms,0.413));
        else
            rock_rad = 3.9*pow(10, -0.209490 + (1.0/3.0)*log10(rock_ms)-0.0804*pow(rock_ms,0.394));
    }
    else
    {
        rock_rad = 0.0;
        components--;
    }

    if (planet_ptr->bulk_water > 0.0)
    {
        if (water_ms > 4.0)
            water_rad = 4.43*pow(10, -0.209396 + (1.0/3.0)*log10(water_ms)-0.0807*pow(water_ms,0.375));
        else
            water_rad = 3.9*pow(10, -0.209490 + (1.0/3.0)*log10(water_ms)-0.0804*pow(water_ms,0.394));
    }
    else
    {
        water_rad = 0.0;
        components--;
    }

    if (planet_ptr->bulk_noble == 0)
        components--;
    if (planet_ptr->bulk_ices == 0)
        components--;

    true_radius = ((1+planet_ptr->bulk_metal)*metal_rad +
                   (1+planet_ptr->bulk_rock )*rock_rad  +
                   (1+planet_ptr->bulk_water)*water_rad +
                   (1+planet_ptr->bulk_ices )*planet_ptr->radius +
                   (1+planet_ptr->bulk_noble)*planet_ptr->radius ) / components;

    planet_ptr->radius    = fudge_double(true_radius);
    planet_ptr->bulk_dens = planet_ptr->mass / pow(planet_ptr->radius,3)*5.513;
    planet_ptr->surf_grav = planet_ptr->mass / pow(planet_ptr->radius,2);
    planet_ptr->esc_v     = sqrt(planet_ptr->mass / planet_ptr->radius)*1118.6;
}

//Calculates the properties of an asteroid belt.
void belt_props(struct Planet *planet_ptr)
{
    planet_ptr->albedo = ((planet_ptr->bulk_carbon * rand_double(0.03, 0.1))  +
                          (planet_ptr->bulk_rock   * rand_double(0.05,0.25))  +
                          (planet_ptr->bulk_ices   * fudge_double(0.3)     )  +
                          (planet_ptr->bulk_metal  * rand_double(0.10,0.25)));

    planet_ptr->bulk_dens = ((planet_ptr->bulk_carbon * fudge_double(1.7)   )  +
                             (planet_ptr->bulk_rock   * fudge_double(3.0)   )  +
                             (planet_ptr->bulk_ices   * fudge_double(0.93)  )  +
                             (planet_ptr->bulk_metal  * rand_double(3.0,8.0)));
}

//Calculates the properties a dwarf planet given its mass.
void dwarf_props(struct Planet *planet_ptr)
{
    planet_ptr->bulk_dens = (planet_ptr->bulk_metal*7.87) + (planet_ptr->bulk_rock*3.25) + (planet_ptr->bulk_ices*0.93);
    planet_ptr->mass      = pow(planet_ptr->radius, 3.0) * (planet_ptr->bulk_dens*5.513) * pow((1.0/6378.0), 3.0);
    planet_ptr->surf_grav = (planet_ptr->mass / pow(planet_ptr->radius/6378.0, 2.0));
    planet_ptr->esc_v     = sqrt(planet_ptr->mass / planet_ptr->radius/6378.0)*1118.6;
}

#endif // PROPS_H
