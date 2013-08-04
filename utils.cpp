#include "utils.h"
#include <cmath>
#include <cassert>
#include <random>
#include <ctime>

namespace
{
    std::default_random_engine createRandomEngine()
    {
        auto seed = static_cast<unsigned long>(std::time(nullptr));
        return std::default_random_engine(seed);
    }

    auto RandomEngine = createRandomEngine();
}

/////////////////////////////

void centerOrigin(sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(Animation& animation)
{
    sf::FloatRect bounds = animation.getLocalBounds();
    animation.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}


/////////////////////////////

float length(sf::Vector2f vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
    assert(vector != sf::Vector2f(0.f,0.f));
    return (vector / length(vector));
}

/////////////////////////////

int randomInt(int exclusiveMax)
{
    std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
    return distr(RandomEngine);
}

float randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> distr(min, max);
    return distr(RandomEngine);
}
