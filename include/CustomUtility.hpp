#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include "ConstantData.hpp"

using namespace std;

void DecodeEUC2D(const string path, map<int, Coords> &instance);
void DecodeEUC2D(const string path, vector<Coords> &instance);