/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       config.h
  ║ Defines build configuration and
  ║
  ╚══════════════════════════════════════════════════════════╝
*/
/* ╔═ Arena Size ══
 * ║ Defines the real size of an area.
 * ╚══
 */
#define CORE_N 0x1000       //real usable core

/* ╔═ Maxmimum Size ══
 * ║ Defines the size of the allocated u8-array
 * ║ for the arena. Even when it's not usable.
 * ╚══
 */
#define MAX_CORE_N 65535    //maximum 16-bit


/* ╔═ debug mode ══
 * ║ shows debug messages
 * ╚══
 */
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/* ╔═ interactive modee ══
 * ║ shows CLI VM to play locally. If not defined, just returns 0 or 1 for the game output.
 * ║ If not set, the application can be used for testing and fuzzing like afl ./vm @@ @@
 * ╚══
 */
//#define INTERACTIVE

/* ╔═ Maximum iterations ══
 * ║ Maximum executed OPCodes of all warriors per battle.
 * ╚══
 */
#define MAX_ITERATIONS 4096
