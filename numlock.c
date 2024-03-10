#include <stdio.h>			// printf
#include <stdlib.h>			// exit
#include <string.h>			// strcmp
#include <unistd.h>			// usleep
#include <X11/keysym.h>			// XK_Num_Lock
#include <X11/extensions/XTest.h>	// XTestFakeKeyEvent...
#include <X11/XKBlib.h>			// XkbGetKeyboard...

char *HELP = R"(
NAME
    numlock - toggle the NumLock

USAGE
    numlock [OPTIONS] [on/off]

DESCRIPTION
    This command toggles the NumLock.  Either by directly
    toggling the "NumLock" XKB virtual modifier, or by
    simulating the "XK_Num_Lock" keypress/release event
    with the -s option.

    The "on" or "off" options turns NumLock on or off.

OPTIONS
    -h  This help.
    -v  Verbose.
    -q  Just query the NumLock state, don't toggle it.
    -s  Simulate X11 XK_Num_Lock keypress+release to toggle.

SEE ALSO
    xkbwatch, xev, xmodmap -pm

VERSION)";

// ------------------------------------------------------------------------------

int MODE=1;	// toggle mode: 0=none 1=xkb 2=keypress_simulation
int TOGGLE=2;	// toggle type: 0=off 1=on 2=toggle
int VERBOSE=0;

// map modifier mask to its name
char *rmod(int i) {
  switch(i) {
  case   1: return "1 Shift";
  case   2: return "2 Lock";
  case   4: return "3 Control";
  case   8: return "4 Mod1";
  case  16: return "5 Mod2";
  case  32: return "6 Mod3";
  case  64: return "7 Mod4";
  case 128: return "8 Mod5"; }
  return "0"; }

// --------------------------------------------------- "NumLock" VIRTUAL MODIFIER

void toggle_modifier(Display *d) {
  XkbDescPtr k = XkbGetKeyboard(d,XkbAllComponentsMask,XkbUseCoreKbd);

  // get "NumLock" virtual modifier id
  int v=0,ok=0;
  while(v<XkbNumVirtualMods) {
    if(!strcmp(XGetAtomName(k->dpy,k->names->vmods[v]),"NumLock")) {
      ok=1; break; }}
  if(!ok) return;

  // get real modifier mask
  unsigned int r; XkbVirtualModsToReal(k,1<<v,&r);
  XkbFreeKeyboard(k,0,1);

  if(VERBOSE) { fprintf(stderr,"XKB virtual modifier: %d NumLock\n",v);
                fprintf(stderr,"XKB    real modifier: %s\n",rmod(r)); }

  // toggle
  XkbStateRec st; XkbGetState(d,XkbUseCoreKbd,&st);
  if(TOGGLE==0)			XkbLockModifiers(d,XkbUseCoreKbd,r,0);
  else if(TOGGLE==1)		XkbLockModifiers(d,XkbUseCoreKbd,r,r);
  else if(st.locked_mods & r)	XkbLockModifiers(d,XkbUseCoreKbd,r,0);
  else				XkbLockModifiers(d,XkbUseCoreKbd,r,r); }

// ------------------------------------------------------- "XK_Num_Lock" KEYPRESS

void toggle_keypress(Display *d) {
  XKeyboardState ks; XGetKeyboardControl(d,&ks);
  if(TOGGLE==1 && ks.led_mask&2) return;
  if(TOGGLE==0 && !(ks.led_mask&2)) return;

  // obtain Num_Lock keycode
  unsigned int c = XKeysymToKeycode(d,XK_Num_Lock);
  if(VERBOSE) fprintf(stderr,"X11 keycode: %d XK_Num_Lock\n",c);

  XTestFakeKeyEvent(d,c,1,CurrentTime); XFlush(d);   // press the Num_Lock
  usleep(20000);				     // wait 20 msec
  XTestFakeKeyEvent(d,c,0,CurrentTime); XFlush(d); } // release Num_Lock

// ------------------------------------------------------------------------------

int main(int argc, char *argv[]) {

  for(int i=1;i<argc;i++) {
    if(!strcmp(argv[i],"-h")) { printf("%s\n    %s\n\n",HELP,SIGN); exit(0); }
    if(!strcmp(argv[i],"-v"))  VERBOSE=1;
    if(!strcmp(argv[i],"-s"))  MODE=2;
    if(!strcmp(argv[i],"-q"))  MODE=0;
    if(!strcmp(argv[i],"on"))  TOGGLE=1;
    if(!strcmp(argv[i],"off")) TOGGLE=0; }

  // connect to X server
  Display *d = XOpenDisplay(0);

  // toggle
  if(MODE==1) { toggle_modifier(d); }
  if(MODE==2) { toggle_keypress(d); }

  // report final NumLock state
  XKeyboardState ks; XGetKeyboardControl(d,&ks);
  printf("NumLock %s\n",(ks.led_mask&2)?"On":"Off");

  XCloseDisplay(d); }

// ----------------------------------------------------------- R.Jaksa 2024 GPLv3
