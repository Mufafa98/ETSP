#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include "ConstantData.hpp"

using namespace std;

void DecodeEUC2D(const string path, map<int, Coords> &instance);
void DecodeEUC2D(const string path, vector<Coords> &instance);

double generateRandBetween(unsigned int &seed, double min_value, double max_value);
int generateIntRandBetween(unsigned int &seed, int min_value, int max_value);
bool Exists(const unsigned int &data, const vector<int> &in, const unsigned int &start, const unsigned int &end);