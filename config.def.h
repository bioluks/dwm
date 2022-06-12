/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const int focusonwheel       = 0;        /* If this is "0", it will take advantage of the patch 'focusonclick', if "1" it will disable the patch. */
static const char *fonts[]          = { "monospace:size=10", "Noto Emoji:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const char ptagf[] = "[%s %s]";	/* format of a tag label */
static const char etagf[] = "[%s]";	/* format of an empty tag */
static const int lcaselbl = 0;		/* 1 means make tag label lowercase */	

static const unsigned int ulinepad	= 10;	/* horizontal padding between the underline and tag */ /* DEFAULT: 5 */
static const unsigned int ulinestroke	= 1;	/* thickness / height of the underline */ /* DEFAULT: 2 */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */ /* Disabled for vanitygaps patch to work, used to be "1" */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* Import XF86keysym */
#include <X11/XF86keysym.h>

/* key definitions */
#define MODKEY Mod4Mask /* The Windows (SUPER) button */
#define ALTKEY Mod1Mask /* aka the Left_ALT */
#define FNKEY XF86XK_WakeUp /* The FN Button on ThinkPads */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *shutdowncmd[]  = { "loginctl", "poweroff", "+0", NULL };
static const char *termcmd[]  = { "st", NULL };

/* static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL }; */
/* static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL }; */
/* static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL }; */
/* static const char *mutemic[] = { "/usr/bin/pactl", "set-source-mute", "0", "toggle",  NULL }; */

static const char *brightup[]       = { "xbacklight", "-inc", "5", NULL};
static const char *brightdown[]     = { "xbacklight", "-dec", "5", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ ALTKEY|MODKEY,              XK_u,      incrgaps,       {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_i,      incrigaps,      {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_o,      incrogaps,      {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_6,      incrihgaps,     {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_7,      incrivgaps,     {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_8,      incrohgaps,     {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_9,      incrovgaps,     {.i = +1 } },
	{ ALTKEY|MODKEY|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ ALTKEY|MODKEY,              XK_0,      togglegaps,     {0} },
	{ ALTKEY|MODKEY|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_minus, 		setborderpx,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_p, 		setborderpx,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_quotedbl, 	setborderpx,    {.i = 0 } },
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
	{ MODKEY|ShiftMask,             XK_Escape, spawn,          {.v = shutdowncmd} },
	{ 0,                       XF86XK_MonBrightnessUp, spawn,  {.v = brightup } },
	{ 0,                       XF86XK_MonBrightnessDown, spawn,{.v = brightdown } },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("librewolf") },
	{ 0,                       XF86XK_Explorer,      spawn,    SHCMD("pcmanfm") },
	/* { 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } }, */
	/* { 0,                       XF86XK_AudioMute, spawn,        {.v = mutevol } }, */
	/* { 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol } }, */
	/* { 0,                       XF86XK_AudioMicMute, spawn,     {.v = mutemic } }, */
	{ 0,                       XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ 0,                       XF86XK_AudioMute, spawn,        SHCMD("pamixer --toggle-mute; kill -44 $(pidof dwmblocks)") },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ 0,                       XF86XK_AudioMicMute, spawn,     SHCMD("pamixer --default-source --toggle-mute; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_gbreve, spawn,          SHCMD("slock") },
	{ MODKEY,                       XK_udiaeresis, spawn,      SHCMD("touchpadtoggle") },
	{ 0,                       XF86XK_Search, spawn,           SHCMD("rofi -show run")},
//	{ FNKEY,                   XF86XK_Display, spawn,          SHCMD("playerctl --player spotify previous") },
//	{ FNKEY,                   XF86XK_WLAN, spawn,             SHCMD("playerctl --player spotify play-pause") },
//	{ FNKEY,                   XF86XK_Tools, spawn,            SHCMD("playerctl --player spotify next") },
//	{ FNKEY|ShiftMask,         XF86XK_Display, spawn,          SHCMD("playerctl previous") },
//	{ FNKEY|ShiftMask,         XF86XK_WLAN, spawn,             SHCMD("playerctl play-pause") },
//	{ FNKEY|ShiftMask,         XF86XK_Tools, spawn,            SHCMD("playerctl next") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

