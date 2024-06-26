#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

#include "GameObject/TextureManager.h"
#include "GameObject/Sprite2D.h"
#include "ResourceManagers.h"
#include "Map.h"
#include "PerlinNoise.h"
#include "Camera.h"
#include "Tessellation.h"

std::string GenerateChunk() {
    std::string result;
    const int totalPercentage = 100;
    const int zeroPercentage = 92;
    const int onePercentage = 8;

    srand(time(nullptr));

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        int randNum = rand() % totalPercentage;
        if (randNum < zeroPercentage) {
            result += '0';
        }
        else if (randNum < zeroPercentage + onePercentage) {
            result += '1';
        }
    }

    return result;
}

// Adjust density for river length and threashold for water percentage
std::string GenerateChunk(int width, int height, double density, double threshold) {
    PerlinNoise perlinNoise;

    std::string map;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double value = perlinNoise.noise(x / 10.0, y / 10.0); // Adjust the divisor to change the scale of the noise
            if (value < threshold - density || (value > threshold && value < threshold + density))
                map += '1'; // River
            else
                map += '0'; // Plain
        }
    }

    return map;
}

std::string GenerateObject(std::string chunkData) {
    std::string result;
    const int totalPercentage = 100;
    const int zeroPercentage = 75;
    const int onePercentage = 5;
    const int twoPercentage = 5;
    const int threePercentage = 5;
    const int fourPercentage = 2;
    const int fivePercentage = 2;
    const int sixPercentage = 1;
    const int sevenPercentage = 5;

    bool gatePlaced;

    srand(time(nullptr));
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        if (chunkData[i] == '0') {
            int randNum = rand() % totalPercentage;
            if (randNum < zeroPercentage) {
                result += '0';
            }
            else if (randNum < zeroPercentage + onePercentage) {
                result += '1';
            }
            else if (randNum < zeroPercentage + onePercentage + twoPercentage) {
                result += '2';
            }
            else if (randNum < zeroPercentage + onePercentage + twoPercentage + threePercentage) {
                result += '3';
            }
            else if (randNum < zeroPercentage + onePercentage + twoPercentage + threePercentage + fourPercentage) {
                result += '4';
            }
            else if (randNum < zeroPercentage + onePercentage + twoPercentage + threePercentage + fourPercentage + fivePercentage) {
                result += '5';
            }
            else if (randNum < zeroPercentage + onePercentage + twoPercentage + threePercentage + fourPercentage + fivePercentage + sixPercentage) {
                result += '6';
            }
            else {
                result += '7';
            } 
        }
        else {
            result += '0';
        }
    }
    //place the gate
    int randNum = rand();
    int range = CHUNK_SIZE/7;
    int place;
    
    if (randNum % totalPercentage < 50) {
        place = 0 + rand() % (range + 1);
    }
    else {
        place = (CHUNK_SIZE - range - 1) + rand() % (CHUNK_SIZE - (CHUNK_SIZE - range - 1));
    }
    while (chunkData[place] != '0') {
        place++;
    }
        result[place] = '8';
    return result;
}

MapChunk::MapChunk(MapMode mode)
{
    std::string data = GenerateChunk(CHUNK_UNITS, CHUNK_UNITS, 0.2, 0.01);
    std::string objectData;
    if (mode == MapMode::MAP_VALLILA) {
        objectData = GenerateObject(data);
    }
    else {
        MazeGenerator mazeGenerator(CHUNK_UNITS, CHUNK_UNITS);
        objectData = mazeGenerator.generateMaze();
    }

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        auto texture = ResourceManagers::GetInstance()->GetTexture(T_MAP_INVALID);
        plainTerrain = std::make_shared<GridPoint>(texture);
        plainTerrain->terrain = MTerrain::MTERRAIN_PLAIN;
        
        texture = ResourceManagers::GetInstance()->GetTexture(T_MAP_INVALID);
        riverTerrain = std::make_shared<GridPoint>(texture);
        riverTerrain->terrain = MTerrain::MTERRAIN_RIVER;

        texture = ResourceManagers::GetInstance()->GetTexture(T_MAP_INVALID);
        mObject = std::make_shared<MapObject>(texture);
        mObject->objectType = MObject::MOBJECT_INVALID;

        std::shared_ptr<MapObject> newMObject = mObject;
        std::shared_ptr<GridPoint> newGrid;
        
        if (data[i] == '0') newGrid = plainTerrain;
        else if (data[i] == '1') newGrid = riverTerrain;
      
        if (objectData[i] == '0') newMObject->objectType = MObject::MOBJECT_INVALID;
        else if (objectData[i] == '1') newMObject->objectType = MObject::MOBJECT_TREE;
        else if (objectData[i] == '2') newMObject->objectType = MObject::MOBJECT_BUSH;
        else if (objectData[i] == '3') newMObject->objectType = MObject::MOBJECT_GRASS;
        else if (objectData[i] == '4') newMObject->objectType = MObject::MOBJECT_CROP;
        else if (objectData[i] == '5') newMObject->objectType = MObject::MOBJECT_DEADBUSH;
        else if (objectData[i] == '6') newMObject->objectType = MObject::MOBJECT_CHESS;
        else if (objectData[i] == '7') newMObject->objectType = MObject::MOBJECT_ROCK;
        else if (objectData[i] == '8') newMObject->objectType = MObject::MOBJECT_GATE;
       
        newGrid->gridNumber = i;
        grids.push_back(newGrid);

        if (newMObject != NULL) {
            newMObject->gridNumber = i;
            objects.push_back(newMObject);
        }
        
    }
    for (int i = 0; i < MAX_CREATURES; i++) {
        auto texture = ResourceManagers::GetInstance()->GetTexture(T_MOB_INVALID);
        std::shared_ptr<Mob> newMob;
        newMob = std::make_shared<Mob>(texture, 1, 10, 1, 0.2f);

        mobs.push_back(newMob);
    }
}

void MapChunk::Draw(SDL_Renderer* renderer)
{
    for (auto it : grids)
    {
        it->Draw(renderer);
    }
    for (auto it : objects) 
    {
        it->Draw(renderer);
    }
    for (auto it : mobs)
    {
        
            it->Spawn(renderer);
            mobCount++;
        
    }
}

Map::Map(MapMode mode) {
    h = 3;
    v = 3;
    if (mode == MapMode::MAP_VALLILA) {
        std::shared_ptr<MapChunk> newMapChunk = std::make_shared<MapChunk>(mode);
        chunks.push_back(newMapChunk);
    }
    else {
        std::shared_ptr<MapChunk> newMapChunk = std::make_shared<MapChunk>(mode);
        chunks.push_back(newMapChunk);
    }
    
}

void Map::Init(MapMode mode) {
    std::shared_ptr<MapChunk> newMapChunk = std::make_shared<MapChunk>(mode);
    chunks.push_back(newMapChunk);
}

void Map::Draw(SDL_Renderer* renderer)
{
    for (auto it : chunks)
    {
        it->Draw(renderer);
    }
}

void Map::DisplayHitboxs(SDL_Renderer* renderer)
{
    for (auto mChunk : this->chunks) {
        for (auto mObj : mChunk->objects) {
            if (mObj->objectType != MObject::MOBJECT_INVALID) {
                float dimension = 30;
                float x1 = mObj->hitbox[0].x;
                float y1 = mObj->hitbox[0].y;
                float x2 = mObj->hitbox[1].x;
                float y2 = mObj->hitbox[1].y;
                float x3 = mObj->target.x;
                float y3 = mObj->target.y;
                auto texture = ResourceManagers::GetInstance()->GetTexture(T_HITBOX_CORNER);
                texture->Render(x1 - Camera::GetInstance()->GetPosition().x, y1 - Camera::GetInstance()->GetPosition().y, dimension, dimension, 0, SDL_FLIP_NONE);
                texture->Render(x2 - dimension - Camera::GetInstance()->GetPosition().x, y2 - dimension - Camera::GetInstance()->GetPosition().y, dimension, dimension, 180, SDL_FLIP_NONE);
                texture = ResourceManagers::GetInstance()->GetTexture(T_HITBOX_TARGET);
                texture->Render(x3 - dimension/2 - Camera::GetInstance()->GetPosition().x, y3 - dimension / 2 - Camera::GetInstance()->GetPosition().y, dimension, dimension, 0, SDL_FLIP_NONE);
            }
        }
        for (auto mMob : mChunk->mobs) {

        }
    }
}

void Map::UpdateCollies()
{
    //std::cout << collieBoxs.size() << std::endl;
    //
    std::vector<std::pair<Vector2, Vector2>> newCollies;
    for (auto mChunk : this->chunks) {
        for (auto mObj : mChunk->objects) {
            if (mObj->objectType == MObject::MOBJECT_ROCK || mObj->objectType == MObject::MOBJECT_TREE) {
                if (mObj->active && mObj->target.x != 0 && mObj->target.y != 0) {
                    std::pair <Vector2, Vector2> newPair(mObj->hitbox[0], mObj->hitbox[1]);
                    
                    //if (newCollies)
                    newCollies.push_back(newPair);
                }
            }
            else if (mObj->objectType == MObject::MOBJECT_GATE) {
                checkPoint.first = mObj->hitbox[0];
                checkPoint.second = mObj->hitbox[1];
            }
        }
        for (auto mMob : mChunk->mobs) {

        }
    }
    collieBoxs = newCollies;
}

bool Map::isOnTheCheckPoint(Vector2 playerPos)
{
    if (((playerPos.x <= checkPoint.second.x && playerPos.x >= checkPoint.first.x) ||
        (playerPos.x + CHAR_W <= checkPoint.second.x && playerPos.x + CHAR_W >= checkPoint.first.x)) &&
        ((playerPos.y <= checkPoint.second.y && playerPos.y >= checkPoint.first.y) ||
        (playerPos.y + CHAR_H <= checkPoint.second.y && playerPos.y + CHAR_H >= checkPoint.first.y)))
        return true;
    else 
    return false;
}

GridPoint::GridPoint()
{
 }

GridPoint::GridPoint(std::shared_ptr<TextureManager> texture)
{

}

void GridPoint::Draw(SDL_Renderer* renderer)
{
    float x = MAP_START_X + GRID_UNITS * (gridNumber % CHUNK_UNITS);
    float y = MAP_START_Y + GRID_UNITS * (gridNumber / CHUNK_UNITS);
    if(terrain == MTerrain::MTERRAIN_PLAIN) texture = ResourceManagers::GetInstance()->GetTexture(T_MAP_PLAIN);
    else texture = ResourceManagers::GetInstance()->GetTexture(T_MAP_WATER);
    if (texture != nullptr)
    {
        texture->Render(x - Camera::GetInstance()->GetPosition().x, y - Camera::GetInstance()->GetPosition().y, GRID_UNITS, GRID_UNITS, 0, m_flip);
    }
}
