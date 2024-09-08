// Math and unit constants
#define GAS_CONST             8.3144598
#define EARTH_MASS            5.9722*pow(10, 24)
#define EARTH_RADIUS       6378.0
#define EARTH_YEAR          365.2515
#define GRAVITY               9.80665
#define GRAV_CONST            6.6743015*pow(10, -11)
#define SUN_LUMIN             3.828*pow(10, 26)
#define PARSEC           206266.3
#define LIGHT_SPEED   299792458.0
#define KELVIN              273.15
#define PLANCK_CONST          6.62607015
#define BOLTZ_CONST           1.380649
#define PI                    3.14159

//Technical constants
#define true  1
#define false 0

#define nullptr ((void*)0)

#define FUDGE_FACTOR          0.05

//Orbital parameters
#define ORBIT_SPACING_MIN     1.4000
#define ORBIT_SPACING_MAX     2.0000
#define ORBIT_SATELLITE_DECAY 0.2

//Day parameters
#define DAY_MIN               8.0
#define DAY_MAX            3000.0
#define DAY_DECAY             0.015
#define RET_PROB              0.1

//Atmo parameters
#define ATMO_DECAY            0.9

//Generation parameters
#define MIGRATE_MID_CHANCE    0.0500
#define MIGRATE_HOT_CHANCE    0.0500

#define DENSE_CHANCE          0.4
#define DENSE_DECAY           'E'
#define OCEAN_CHANCE          0.6
#define OCEAN_DECAY           'L'
#define ICEGT_CHANCE          0.8
#define ICEGT_DECAY           'L'
#define GASDW_CHANCE          0.15
#define GASDW_DECAY           'E'
#define ABELT_CHANCE          0.25
#define FEATR_CHANCE          0.30
#define RINGS_CHANCE          0.25

//Stellar params
#define STAR_MASS_MIN         0.6000
#define STAR_MASS_MAX         1.4000
#define STAR_MASS_DECAY       0.5

//Params for moons
#define MOONS_CHANCE          0.95
#define MOONS_DECAY           'L'
#define ICEMN_CHANCE          0.03
#define ICEMN_DECAY           'L'

#define LAGRA_CHANCE          0.15
#define MOON_SPACE_MAX        4.0
#define IRREG_MOON_DECAY      0.5

//Params for planets
#define DWARF_MASS_MIN        0.0001
#define DWARF_MASS_MAX        0.1000
#define DWARF_RADIUS_MIN      0.0001
#define DWARF_RADIUS_MAX      0.0300

#define DENSE_MASS_MIN        0.0001
#define DENSE_MASS_MAX        1.6000
#define DENSE_RADIUS_MIN      0.0010
#define DENSE_RADIUS_MAX      1.5000

#define ROCKY_MASS_MIN        0.1000
#define ROCKY_MASS_MAX       10.0000
#define ROCKY_RADIUS_MIN      0.0300
#define ROCKY_RADIUS_MAX      2.0000

#define GREEN_MASS_MIN        0.1000
#define GREEN_MASS_MAX        3.5000
#define GREEN_RADIUS_MIN      0.5000
#define GREEN_RADIUS_MAX      1.5000

#define GAS_DWARF_MASS_MIN    1.0000
#define GAS_DWARF_MASS_MAX   10.0000
#define GAS_DWARF_RADIUS_MIN  2.0000
#define GAS_DWARF_RADIUS_MAX 10.9730

#define GAS_MASS_MIN         10.0000
#define GAS_MASS_MAX       4131.7690
#define GAS_RADIUS_LIM      635.6570
#define GAS_RADIUS_MIN        6.0000
#define GAS_RADIUS_MAX       21.2970
#define GAS_RADIUS_NORM      10.9730

//Types of planets

#define TYPE_GAS_GIANT 'J' //Gas giant
#define TYPE_GAS_PUFFY 'P' //Puffy gas giant
#define TYPE_GAS_SUPER 'S' //Super Jupiter
#define TYPE_GAS_HOT   'H' //Hot Jupiter
#define TYPE_ICE_GIANT 'I' //Ice giant
#define TYPE_ICE_DWARF 'N' //Gas dwarf
#define TYPE_RCK_DENSE 'M' //Iron-dominated mercurial world
#define TYPE_RCK_DESRT 'V' //Non-habitable rocky
#define TYPE_RCK_GREEN 'E' //Habitable rocky
#define TYPE_WTR_HYCN  'O' //Uninhabitable ocean world
#define TYPE_WTR_GREEN 'W' //Habitable ocean world

//Types of satellites

#define TYPE_SAT_MINOR 'M' //Captured asteroid satellite
#define TYPE_SAT_MAJOR 'T' //Terrestrial moon
#define TYPE_SAT_MOONA 'A' //A-type ring shepherd minor moons
#define TYPE_SAT_MOONB 'B' //B-type icy major moons of giants
#define TYPE_SAT_FOR_B 'F' //Forward lagrangian companion to a B-type moon
#define TYPE_SAT_REV_B 'R' //Reverse lagrangian companion to a B-type moon
#define TYPE_SAT_MOONC 'C' //C-type distant irregular minor moons of giants

//Types of belts and belt objects
#define TYPE_BLT_INNER 'B' //Inner-system asteroid belt
#define TYPE_BLT_KUIPR 'K' //Kuiper belt

#define TYPE_DWF_PLNTSML 'c' //Inner belt planetesimal
#define TYPE_DWF_PLUTINO '1' //2:3 resonant kuiper belt objects
#define TYPE_DWF_CUBEWNO 'C' //Classical non-resonant kuiper belt objects
#define TYPE_DWF_TWOTINO '2' //1:2 resonant kuiper belt objects
#define TYPE_DWF_SCATTER 'Q' //Extra-kuiper detached objects
#define TYPE_DWF_SEDNOID 'D' //Extremely distant extra-kuiper detached objects

//Macros
#define MIN(x,y)      (((x) < (y)) ? (x) : (y))
#define MAX(x,y)      (((x) > (y)) ? (x) : (y))

#ifndef DEFS_H
#define DEFS_H

//Structs
struct Star
{
    //Characteristics
    char  type;
    double mass;      //(Solar mass)
    double radius;    //(Solar radius)
    double surf_grav; //(Gs)
    double esc_v;     //(km/s)
    double metal;     //(dimensionless)

    //Radiance
    double lumin;     //(Solar luminosity)
    double temp;      //(K)
    double bv;        //(dimensionless)
    double abs_mag;   //(dimensionless)
    double rel_mag;   //(dimensionless)
    double *photosphere;

    //Time
    double life;      //(billion years)
    double age;       //(billion years)
    double y;         //(million years)

    //Planetary formation zone boundaries (AU)
    double zone_form_min;
    double zone_form_max;

    //Goldilocks zone boundaries (AU)
    double zone_hab_min;
    double zone_hab_max;

    //Frost line (AU)
    double zone_frost;
};

struct Planet
{
    //Characteristics
    char  type   ;
    char  subtype[2];

    double mass   ; //Mass            (Earth mass)
    double radius ; //Radius          (Earth radii)
    double height ; //For asteroid belts
    double esc_v  ; //Escape velocity (m/s)
    double albedo ; //                (%)

    //Surface
    double surf_temp; //Average surface temperature  (°C)
    double surf_grav; //Surface gravity              (Gs)
    double rck_cover; //Percentage of surface covered by dry land
    double ice_cover; //Percentage of surface covered by ices
    double wtr_cover; //Percentage of surface covered by liquid water
    double cld_cover_thn; //Percentage of sky covered by thin clouds
    double cld_cover_thk; //Percentage of sky covered by thick clouds
    char*  feature;

    //Satellites
    struct Moon *moons;
    int    num_moons;
    double rings_min; //(Earth radii)
    double rings_max; //(Earth radii)
    double *rings;    //(Earth radii)
    int    num_rings;
    int    rings_vis;
    int    trojans;

    //Composition
    double bulk_metal ; //Metallicity, typically the metal core
    double bulk_rock  ; //Rocky silicates
    double bulk_water ; //Water and water ice
    double bulk_noble ; //H-He (giants only)
    double bulk_ices  ; //Volatile gases like ammonia and methane (giants only)
    double bulk_carbon; //Carbonaceous asteroids (belts only)

    double bulk_dens ; //Density (g/m³)

    //Orbit
    double a; //Semimajor axis              (AU)
    double e; //Eccentricity                (dimensionless)
    double y; //Year length                 (Earth years)
    double v; //Orbital velocity            (km/s)
    double i; //Inclination                 (degrees)
    double l; //Longitude of Ascending Node (degrees)
    double p; //Argument of Periapsis       (degrees)

    //Axis
    double day_length; //(hours)
    double axial_tilt; //(degrees)

    //Atmosphere
    double atmo_pres;      //Equatorial sea level atmospheric pressure (arbitrary)
    double atmo_dens;      //Surface atmospheric density
    double atmo_high;      //Atmospheric scale height

    int    has_atmo;
    int    is_super;

    double *atmosphere;
    /*
     0    Hyrogen
     1    Hydrogen deuteride
     2    Helium
     3    Carbon monoxide
     4    Carbon dioxide
     5    Methylene
     6    Methane
     7    Acetylene
     8    Ethylene
     9    Ethane
    10    Propane
    11    Butadiyne
    12    Tholins
    13    Cyanogen
    14    Hydrogen cyanide
    15    Methyl cyanide
    16    Nitrogen
    17    Ammonia
    18    Azic acid
    19    Nitric oxide
    20    Nitrogen dioxide
    21    Nitrous oxide
    22    Oxygen
    23    Water
    24    Hydrogen peroxide
    25    Ozone
    26    Fluorine
    27    Hydrogen fluoride
    28    Boron trifluoride
    29    Diboron tetrafluoride
    30    Carbon tetrafluoride
    31    Silicon tetrafluoride
    32    Phosphorus trifluoride
    33    Phosphorus pentafluoride
    34    Sulfur trifluoride
    35    Sulfur hexafluoride
    36    Neon
    37    Silane
    38    Phosphine
    39    Hydrogen sulfide
    40    Sulfur dioxide
    41    Sulfur trioxide
    42    Chlorine
    43    Hydrogen chloride
    44    Boron trichloride
    45    Chloroform
    46    Carbon tetrachloride
    47    Silicon tetrachloride
    48    Phosphorus trichloride
    49    Phosphorus pentachloride
    50    Argon
    51    Bromine
    52    Hydrogen bromide
    53    Boron tribromide
    54    Silicon tetrabromide
    55    Phosphorus tribromide
    56    Arsenic tribromide
    57    Krypton
    58    Xenon
    */
};

struct Moon
{
    //Characteristics
    char   type     ; //Type
    char   subtype  ;
    double mass     ; //Mass            (Earth mass)
    double rad_a    ; //Radius          (km) (only radius for major moons)
    double rad_b    ; //Radius          (km)
    double rad_c    ; //Radius          (km)
    double esc_v    ; //Escape velocity (m/s)
    double surf_grav; //Gravity         (Gs)

    //Composition
    double bulk_metal;
    double bulk_rock ;
    double bulk_ices ;
    double bulk_dens ; //Density (g/m³)

    //Orbit
    struct Planet *host; //The planet that this moon orbits
    double a; //Semimajor axis              (Earth radii)
    double e; //Eccentricity                (dimensionless)
    double y; //Year length                 (Earth days)
    double v; //Orbital velocity            (km/s)
    double i; //Inclination                 (degrees)
    double l; //Longitude of Ascending Node (degrees)
    double p; //Argument of Periapsis       (degrees)

    //Axis
    double day_length; //(h)
    double axial_tilt; //(degrees)
};

#endif // DEFS_H
