#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <math.h>
#include "common.h"

void rotate(sf::Vector3f &coords, std::vector<sf::Vector3f> &rotor) {

    sf::Vector3f save = coords;
    coords.x = (save.x * rotor[0].x) +
                (save.y * rotor[0].y) +
                (save.z * rotor[0].z);
    coords.y = (save.x * rotor[1].x) +
                (save.y * rotor[1].y) +
                (save.z * rotor[1].z);
    coords.z = (save.x * rotor[2].x) +
                (save.y * rotor[2].y) +
                (save.z * rotor[2].z);
}

std::vector<sf::Vector3f> generateRotationMatrix(sf::Vector3f yaw_pitch_roll) {
    return {
    {std::cos(yaw_pitch_roll.y) * std::cos(yaw_pitch_roll.x), std::cos(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.y) * std::sin(yaw_pitch_roll.z) - std::cos(yaw_pitch_roll.z) * std::sin(yaw_pitch_roll.x), std::cos(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.y) * std::cos(yaw_pitch_roll.z) + std::sin(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.z)},
    {std::cos(yaw_pitch_roll.y) * std::sin(yaw_pitch_roll.x), std::sin(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.y) * std::sin(yaw_pitch_roll.z) + std::cos(yaw_pitch_roll.z) * std::cos(yaw_pitch_roll.x), std::sin(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.y) * std::cos(yaw_pitch_roll.z) - std::cos(yaw_pitch_roll.x) * std::sin(yaw_pitch_roll.z)},
    {                     -std::sin(yaw_pitch_roll.y)       ,                                        std::sin(yaw_pitch_roll.z) * std::cos(yaw_pitch_roll.y)                                                ,                                            std::cos(yaw_pitch_roll.z) * std::cos(yaw_pitch_roll.y)                                            }
    };
}

std::vector<sf::Vector3f> combine_3x3_Matrices(std::vector<sf::Vector3f> &m1, std::vector<sf::Vector3f> &m2) {
    std::vector<sf::Vector3f> result;
    result.resize(3);

    result[0].x = m1[0].x * m2[0].x + m1[0].y * m2[1].x + m1[0].z * m2[2].x;
    result[0].y = m1[0].x * m2[0].y + m1[0].y * m2[1].y + m1[0].z * m2[2].y;
    result[0].z = m1[0].x * m2[0].z + m1[0].y * m2[1].z + m1[0].z * m2[2].z;

    result[1].x = m1[1].x * m2[0].x + m1[1].y * m2[1].x + m1[1].z * m2[2].x;
    result[1].y = m1[1].x * m2[0].y + m1[1].y * m2[1].y + m1[1].z * m2[2].y;
    result[1].z = m1[1].x * m2[0].z + m1[1].y * m2[1].z + m1[1].z * m2[2].z;

    result[2].x = m1[2].x * m2[0].x + m1[2].y * m2[1].x + m1[2].z * m2[2].x;
    result[2].y = m1[2].x * m2[0].y + m1[2].y * m2[1].y + m1[2].z * m2[2].y;
    result[2].z = m1[2].x * m2[0].z + m1[2].y * m2[1].z + m1[2].z * m2[2].z;

    return result;
}

sf::Vector2f projectOnScreen(sf::Vector3f positionInSpace) {
    sf::Vector2f output;
    output.x = (focalLength * positionInSpace.x * sizeMultiplier) / (focalLength + positionInSpace.z + (WINDOW_SIZE/2)) + (WINDOW_SIZE/2);
    output.y = (focalLength * positionInSpace.y * sizeMultiplier) / (focalLength + positionInSpace.z + (WINDOW_SIZE/2)) + (WINDOW_SIZE/2);
    return output;
}

sf::Vector3f normalize(sf::Vector3f input) {
    float len = std::sqrt(std::pow(input.x, 2) + std::pow(input.y, 2) + std::pow(input.z, 2));
    input.x = input.x/len;
    input.y = input.y/len;
    input.z = input.z/len;
    return input;
}

float lerp(float a, float b, float f)
{
    return a * (1.0 - f) + (b * f);
}

void split(std::string str, std::string splitBy, std::vector<std::string>& tokens)  //stolen: stackoverflow
{
    /* Store the original string in the array, so we can loop the rest
     * of the algorithm. */
    tokens.push_back(str);

    // Store the split index in a 'size_t' (unsigned integer) type.
    size_t splitAt;
    // Store the size of what we're splicing out.
    size_t splitLen = splitBy.size();
    // Create a string for temporarily storing the fragment we're processing.
    std::string frag;
    // Loop infinitely - break is internal.
    while(true)
    {
        /* Store the last string in the vector, which is the only logical
         * candidate for processing. */
        frag = tokens.back();
        /* The index where the split is. */
        splitAt = frag.find(splitBy);
        // If we didn't find a new split point...
        if(splitAt == std::string::npos)
        {
            // Break the loop and (implicitly) return.
            break;
        }
        /* Put everything from the left side of the split where the string
         * being processed used to be. */
        tokens.back() = frag.substr(0, splitAt);
        /* Push everything from the right side of the split to the next empty
         * index in the vector. */
        tokens.push_back(frag.substr(splitAt+splitLen, frag.size()-(splitAt+splitLen)));
    }
}