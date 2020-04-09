#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"reg.h"

void sigint_handler(int signal);
void sigterm_handler(int signal);
void sigcont_handler(int signal);

#endif /*SIGNALS_H*/
