#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Object.h"

using namespace std;

class Parser {
	public:
	static Object* parse_object(char*);
};