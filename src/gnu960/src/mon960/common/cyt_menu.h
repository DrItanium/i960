/* menu.h - */

/*
modification history
--------------------
01a,04mar92,bwb		written.
02a,09apr92,tga		modified to run with Nindy.  Changed WRS-style
			types (ie, VOID and ULONG) to normal (void and
			unsigned long).
*/

/*
DESCRIPTION

Include file for the table-driven menu module menu.c.  This module displays
a menu of numbered items, prompts the user for a choice, then calls an action
routine associated with the choice.  It then redisplays the menu and prompts
for another choice.  A choice of zero (0) exits the menu routine.

To use this menu module, construct a table of type MENU_ITEM, with one entry
for each menu item.  An entry consists of a string to print out with the
menu, an action routine, and an argument for the action routine.  There is
no need to have an "exit" or "quit" item, since the menu routine handles
that by assigning at the zero (0) item number.  For example, a simple menu
of tests would be constructed this way:

	IMPORT VOID doTestA ();
	IMPORT VOID doTestB ();

	#define TESTA_ARG	0
	#define TESTB_ARG	1

	LOCAL MENU_ITEM testMenu[] =
	{
	{"Test A", doTestA, (MENU_ARG)TESTA_ARG},
	{"Test B", doTestB, (MENU_ARG)TESTB_ARG},
	};

	#define	NUM_ITEMS NELEMENTS(testMenu)
	#define MENU_TITLE "Test Menu"

Then, in the test top level routine, call the routine menu(), declared as:

	STATUS menu (menuTable, numMenuItems, title, options)
	MENU_ITEM	menuTable[];
	int		numMenuItems;
	char		*title;
	ULONG		options;

For example:

	status = menu (testMenu, NUM_ITEMS, MENU_TITLE, MENU_OPT_NONE);

*/

#ifndef INCmenuh
#define INCmenuh

/*
 * The following types define an entry in the menu table.  Each entry describes
 * one menu item, including a string to describe that item, an action
 * routine to call when that item is chosen, and an argument to pass when the
 * action routine is called.
 */
typedef void (*MENU_RTN) ();
typedef volatile void *MENU_ARG;
typedef struct menuItem
{
    char	*itemName;	/* string to print with the menu */
    MENU_RTN	actionRoutine;	/* routine to call when item is chosen */
    MENU_ARG	arg;		/* argument to actionRoutine */
} MENU_ITEM;


/*
 * Menu options
 *
 * These options control the display of the menu.
 *
 * MENU_OPT_NONE - The normal behavior.  The menu is always displayed before
 * prompting the user for a choice.
 *
 * MENU_OPT_SUPPRESS_DISP - The menu is displayed before prompting the user
 * for a choice except after executing a previously valid choice.  This option
 * is useful for large menus consisting of simple commands that produce only
 * a line or two of output.  In this case, after the user executes a valid
 * choice, the redisplay of a large menu may cause output to scroll off the
 * screen.  The menu is redisplayed if the user enters an invalid choice.
 */
#define MENU_OPT_NONE		0x00
#define MENU_OPT_SUPPRESS_DISP	0x01
#define MENU_OPT_VALUE		0x02


MENU_ARG menu (MENU_ITEM	menuTable[],
	       int		numMenuItems,
	       char		*title,
	       unsigned long	options);


#endif /* INCmenuh */
