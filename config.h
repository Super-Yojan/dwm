/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 1;      /* 0 means no systray */
static const int showbar = 1;          /* 0 means no bar */
static const int topbar = 1;           /* 0 means bottom bar */
static const unsigned int gappih = 10; /* horiz inner gap between windows */
static const unsigned int gappiv = 10; /* vert inner gap between windows */
static const unsigned int gappoh =
    10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    10; /* vert outer gap between windows and screen edge */
static int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const char *fonts[] = {"JetBrainsMono Nerd Font Mono:size=11"};
static const char dmenufont[] = "JetBrainsMono Nerd Font Mono:size=11";
static const char col_gray1[] = "#282A30";
static const char col_gray2[] = "#32343C";
static const char col_gray3[] = "#d8dee9";
static const char col_gray4[] = "#e5e9f0";
static const char col_cyan[] = "#464954";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* colours for status bar, at least one entry is needed
 * default foreground is colours from gruvbox dark
 * default background should be the same as bg in SchemeNorm
 */
static const char *sb_colours[][2] = {
    /* fg           bg  */
    {"#E06C75", col_gray1}, {"#E5c07b", col_gray1}, {"#C678DD", col_gray1},
    {"#98c379", col_gray1}, {"#61AFEF", col_gray1}, {"#56B6C2", col_gray1},
};
/* allow colours to shift right each time drawbar() is called, useless but fun
 */
static unsigned int colour_shift = 0;
/* for rounded corner: pixels between each components */
static unsigned int r_gap = 10;
/* for rounded corner: see github comment, tldr, try 2 or 3 */
static unsigned int jagged_pixel = 3;
/* for xmonad: distance between the top edge of the underline and the bottom of
 * the sb */
static unsigned int underline_offset = 2;
/* for xmonad: height of the underline; so maximum value should be
 * underline_offset */
static unsigned int underline_height = 2;
/* for xmonad: pixels between each components*/
static unsigned int m_gap = 15;
/* for xmonad: distance between the right of the screen and the most right
 * component */
static unsigned int right_padding = 5;

/* tagging */
// static const char *tags[] = { "dev", "www", "", "4", "5", "6", "7", "dis",
// "spt" };
static const char *tags[] = {"dev", "www", "note", "4",  "VM",
                             "6",   "7",   "dis",  "spt"};
static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"firefox", NULL, NULL, 1 << 1, 0, -1},
    {"kdeconnectd", NULL, NULL, 0, 1, -1},
    {"discord", NULL, NULL, 1 << 7, 0, -1},
    {"spotify", NULL, NULL, 1 << 8, 0, -1},
    {"obsidian", NULL, NULL, 1 << 2, 0, -1}};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"[M]", monocle},
    {"[@]", spiral},
    {"[\\]", dwindle},
    {"H[]", deck},
    {"TTT", bstack},
    {"===", bstackhoriz},
    {"HHH", grid},
    {"###", nrowgrid},
    {"---", horizgrid},
    {":::", gaplessgrid},
    {"|M|", centeredmaster},
    {">M>", centeredfloatingmaster},
    {"><>", NULL}, /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",      dmenumon, "-fn",    dmenufont, "-nb",     col_gray1,
    "-nf",       col_gray3, "-sb",    col_cyan, "-sf",     col_gray4, NULL};
static const char *termcmd[] = {"alacritty", NULL};
static const char *lock[] = {"slock", NULL};

static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY | Mod4Mask | ShiftMask, XK_l, spawn, {.v = lock}},
    {MODKEY | Mod4Mask | ShiftMask, XK_x, spawn, SHCMD("flameshot gui")},
    {MODKEY | Mod4Mask | ShiftMask, XK_p, spawn,
     SHCMD("/home/drmoscovium/.config/eww/open.sh")},
    {MODKEY | Mod4Mask | ShiftMask, XK_b, spawn,
     SHCMD("/home/drmoscovium/.fehbg")},
    {MODKEY | Mod4Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_6, incrihgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_6, incrihgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_7, incrivgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_7, incrivgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_8, incrohgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_8, incrohgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_9, incrovgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_9, incrovgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_0, togglegaps, {0}},
    {MODKEY | Mod4Mask | ShiftMask, XK_0, defaultgaps, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_s, sb_scroll, {0}},
    {MODKEY | ShiftMask, XK_s, sb_shiftcolour, {0}},

    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
