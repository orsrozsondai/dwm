/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
//#include <cstddef>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 0;        /* 0 means no systray */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode Nerd Font:size=11" };
static const char dmenufont[]       = "FiraCode Nerd Font:size=14";
static const char col_gray1[]       = "#2E3440";
static const char col_gray2[]       = "#4c566a";
static const char col_gray3[]       = "#d8dee9";
static const char col_gray4[]       = "#e5e9f0";
static const char col_cyan[]        = "#4c566a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",             NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",          NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,               NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ "Spotify",		  NULL,		NULL,			1 << 8,    0,		   0,			-1,		  -1 },
	{ "kcalc",			  NULL,		NULL,			0,		   1,		   0,			-1,		  -1 },
	{ "feh",			  NULL,		NULL,			0,		   1,		   0,			-1,		  -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-bw", "2", "-c", "-l", "10",  NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browser[] = { "brave", NULL };
static const char *private_browser[] = { "brave", "--incognito", NULL };
static const char *spotify[] = { "spotify-launcher", NULL };
#define volctl "volctl.sh"
#define mediactl "mediactl.sh"
static const char *volUp[] = { volctl, "up", NULL };
static const char *volDown[] = { volctl, "down", NULL };
static const char *muteToggle[] = { volctl, "mute", NULL };
static const char *media_play[] = { mediactl, "play", NULL };
static const char *media_next[] = { mediactl, "next", NULL };
static const char *media_prev[] = { mediactl, "prev", NULL };
static const char *media_forward[] = { mediactl, "forward", NULL };
static const char *media_back[] = { mediactl, "back", NULL };
static const char *flameshotGUI[] = { "flameshot", "gui", NULL };
static const char *flameshotFULL[] = { "flameshot", "full", NULL };
static const char *powermenu[] = { "powermenu.sh", NULL };
static const char *htop[] = { "st", "-e ", "htop", "-t", NULL };
static const char *monbrdown[] = { "brightnessctl", "s", "10%-" };
static const char *monbrup[] = { "brightnessctl", "s", "+10%" };
static const char *change_monitor[] = { "mon_change.sh", NULL };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,				XK_m,	   spawn,		   {.v = change_monitor } },
	{ 0,							XF86XK_MonBrightnessDown,  spawn, {.v = monbrdown } },
	{ 0,							XF86XK_MonBrightnessUp,    spawn, {.v = monbrup } },
	{ 0,				            XF86XK_AudioLowerVolume,   spawn, {.v = volDown } },
	{ 0,				            XF86XK_AudioRaiseVolume,   spawn, {.v = volUp } },
	{ 0,				            XF86XK_AudioMute,          spawn,   {.v = muteToggle } },
	{ 0,				            XK_Print,  spawn,          {.v = flameshotGUI } },
	{ 0,							XF86XK_AudioPlay, spawn,   {.v = media_play } },
	{ 0,							XF86XK_AudioNext, spawn,   {.v = media_next } },
	{ 0,							XF86XK_AudioPrev, spawn,   {.v = media_prev } },
	{ ControlMask,					XF86XK_AudioPrev, spawn,   {.v = media_back } },
	{ ControlMask,					XF86XK_AudioNext, spawn,   {.v = media_forward } },
	{ MODKEY,						XK_x,      spawn,          {.v = powermenu } },
	{ MODKEY,						XK_Escape,	    spawn,	   {.v = htop } },
	{ ShiftMask,			        XK_Print,  spawn,	       {.v = flameshotFULL } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			            XK_b,	   spawn,	       {.v = browser } },
	{ MODKEY|ShiftMask,	            XK_b,	   spawn,	       {.v = private_browser } },
	{ 0,				            XF86XK_HomePage,spawn,     {.v = browser } },
	{ MODKEY,			            XK_s,	   spawn,	       {.v = spotify } },
	{ MODKEY,			            XK_e,	   spawn,	       SHCMD("thunar") },
	{ 0,				            XF86XK_Explorer,   spawn,  SHCMD("thunar") },
	{ 0,				            XF86XK_Calculator, spawn,  SHCMD("kcalc") },
	{ 0,				            XF86XK_Tools,	spawn,	   SHCMD("vlc -Z ~/Music") },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

