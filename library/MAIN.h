/*
    This file is a part of the wiringBone library
    Copyright (C) 2015 Abhraneel Bera

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/

#ifndef MAIN_H
#define MAIN_H

#include <signal.h>
#include <pthread.h>

void sei(){} // Dummy function
void cli(){} // Dummy function

extern void setup();

extern void loop();

volatile sig_atomic_t stop;

void StopProgram(int dummy)
{
  stop = true;
}

void* execute_loop(void* dummy)
{
  setup();

  while(!stop)
  {
    loop();
  }
  return (void*)0;
}

void initialize_loop()
{
  pthread_t thread;

  int dummy=0;

  pthread_create(&thread, NULL, execute_loop, (void*)dummy);

}

#endif