#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "ConstantData.hpp"
#include "CustomUtility.hpp"
#include "Line.hpp"

using namespace sf;
using namespace std;

class Path
{
private:
    unsigned int circle_radius;
    vector<CircleShape> circles;
    vector<Line> lines;

public:
    Path(const char *instance);

    void UpdatePermutation(const vector<unsigned int> &permutation, const unsigned int &permutation_size);

    void Draw(RenderWindow &window);
};