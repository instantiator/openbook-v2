/** openbook2.h - Full header for everything included in the OpenBook project. **/

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <list>
#include <string>
#include <sstream>
using namespace std;

// library/3rd party includes
#include <GL/freeglut.h>
#include "frotz.h"
#include "zthread/Thread.h"
#include "util-bmp.h"

// individual file incudes
#include "threadexample.h"
#include "openbook-constants.h"
#include "openbook-logger.h"

// display objects
#include "openbook-umbrella.h"
#include "umbrella-line.h"
#include "umbrella-page.h"
#include "umbrella-book.h"

// interfaces
#include "frotz-interface.h"
#include "openbook-glutter.h"
#include "openbook-frotzer.h"
#include "openbook-printlogic.h"

// threads
#include "openbook-glutter-thread.h"
//#include "openbook-frotzer-thread.h"

extern bool TEST_MODE;
