### NAME
numlock - toggle the NumLock

### USAGE
    numlock [OPTIONS] [on/off]

### DESCRIPTION
This command toggles the NumLock.  Either by directly
toggling the "NumLock" XKB virtual modifier, or by
simulating the "XK_Num_Lock" keypress/release event
with the -s option.

The "on" or "off" options turns NumLock on or off.

### OPTIONS
    -h  This help.
    -v  Verbose.
    -q  Just query the NumLock state, don't toggle it.
    -s  Simulate X11 XK_Num_Lock keypress+release to toggle.

### SEE ALSO
xkbwatch, xev, xmodmap -pm

### VERSION
numlock-0.2b R.Jaksa 2024 GPLv3

