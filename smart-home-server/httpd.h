#pragma once
#include "stdafx.h"

void openhttpd(u_short port);
int get_line(int, char*, int);
void bad_request(int);
